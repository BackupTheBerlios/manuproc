// $Id: DataBase_init.cc,v 1.9 2002/11/28 15:59:28 thoma Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "DataBase_init.hh"

#include <Aux/FetchIStream.h>
#include <Aux/itos.h>
#include <Aux/Datum.h>
#include <Lager/RohwarenLager.h>
#include <Lager/JumboLager.h>
#include <Ketten/KettplanKette.h>
#include <unistd.h>
#include <Misc/Trace.h>


DataBase_init::DataBase_init() 
{
#ifdef PETIG_TEST
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
   ArtikelBaum_anlegen();
   RohwarenLager_initalisieren();
   JumboLager_initalisieren();
#elif defined MANU_PROC_TEST 
   Lager L(ROHWARENLAGER); 
   L.rein_ins_lager(1,200,getuid()); // Metall
   L.rein_ins_lager(2,50,getuid()); // Granulat klar
   L.rein_ins_lager(3,100,getuid()); // Granulat gelb
#elif defined MABELLA_TEST
   Lager L(FERTIGWARENLAGER); 
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
   L.rein_ins_lager(ARTIKEL_TRIO,4,getuid()); 
//exit(1);
#endif
}

#ifdef  PETIG_TEST

void DataBase_init::ArtikelBaum_anlegen()
{
  ArtikelBaum_anlegen_execute(ARTIKEL_ROLLEREI,cH_Prozess(PROZESS_ROLLEREI_FAERBEREI),ARTIKEL_FAERBEREI);
  ArtikelBaum_anlegen_execute(ARTIKEL_ROLLEREI,PROZESS_DEFAULT,ARTIKEL_RINGE,FAKTOR_RINGE);
  ArtikelBaum_anlegen_execute(ARTIKEL_FAERBEREI,PROZESS_DEFAULT,ARTIKEL_BANDLAGER,FAKTOR_BANDLAGER);
  ArtikelBaum_anlegen_execute(ARTIKEL_BANDLAGER,PROZESS_DEFAULT,ARTIKEL_ACETAT,FAKTOR_ACETAT);
  ArtikelBaum_anlegen_execute(ARTIKEL_BANDLAGER,PROZESS_DEFAULT,ARTIKEL_KUPFER,FAKTOR_KUPFER);
}

void DataBase_init::ArtikelBaum_anlegen_execute(const ArtikelBase &art,
         const cH_Prozess &prozess,const ArtikelBase& art_von,  
         const ArtikelBaum::faktor_t& faktor)
{
  std::string s1="delete from artikelzusammensetzung where id="
                +itos(art.Id())+" and altartikelid="+itos(art_von.Id());
  Query::Execute(s1);
  SQLerror::test(__FILELINE__,100);

  std::string s2="insert into artikelzusammensetzung (id,prozessid,altartikelid";
  if(faktor!=ArtikelBaum::faktor_t(0)) s2 +=",menge";
  s2 +=") values ("+itos(art.Id())+","+itos(prozess->Id())+","+itos(art_von.Id());
  if(faktor!=ArtikelBaum::faktor_t(0)) s2 +=","+faktor.String();
  s2+=")"; 
  Query::Execute(s2);
  SQLerror::test(__FILELINE__);
}

void DataBase_init::RohwarenLager_initalisieren()
{
  LagerPlatz LP2=LagerPlatz(ppsInstanzID::Rohwarenlager,KUPFER_LAGERPLATZ);
  RohwarenLager_initalisieren_execute(ARTIKEL_KUPFER,LP2,KUPFER_KARTONS,KUPFER_KG_PRO_KARTON,KUPFER_RESTE,KUPFER_RESTE_KG);
  LagerPlatz LP(ppsInstanzID::Rohwarenlager,ACETAT_LAGERPLATZ);
  RohwarenLager_initalisieren_execute(ARTIKEL_ACETAT,LP,ACETAT_KARTONS,ACETAT_KG_PRO_KARTON,ACETAT_RESTE,ACETAT_RESTE_KG);

}

void DataBase_init::RohwarenLager_initalisieren_execute(const ArtikelBase &artikel,
      const LagerPlatz& LP,int kartons,int kg_pro_karton,int reste,int kg_reste)
{
  std::string s1="delete from rl_inhalt where material="+itos(artikel.Id());
  Query::Execute(s1);
  SQLerror::test(__FILELINE__,100);
  
  RohwarenLager RL;
  std::string os;
  RohwarenLager::st_rohlager rohlager(LP,kartons,kg_pro_karton,reste,kg_reste,
                                      artikel,ManuProC::Datum().today());
#ifdef MIT_ROHWARENLAGER
  RL.RL_Einlagern(LP,rohlager,UID,os,true);
  SQLerror::test(__FILELINE__);
#endif
}

void DataBase_init::createJumbo(const int diffmaschine,const int menge)
{
#ifdef MIT_BANDLAGER
  Kette K(MASCHIENE+diffmaschine,SCHAERDATUM);
  std::vector <ArtikelGang> artgang;
  artgang.push_back(ArtikelGang(GAENGE,ARTIKEL_BANDLAGER));
  KettplanKette KK=KettplanKette::create(K,artgang,menge,menge);
  std::vector<JumboRolle> JR=JumboRolle::create(KK);
  assert(JR.size()==1);
  Zeitpunkt_new zp("2002-1-1 12:00");
  class JumboLager JL;
  JL.Jumbo_Einlagern(LagerPlatzJumbo,JR.front(),JumboLager::Einlagern,UID,"testuser",&zp);
  SQLerror::test(__FILELINE__);
#endif
}


void DataBase_init::JumboLager_initalisieren()
{
  std::string s1="delete from rohjumbo where artikelid="+itos(ARTIKEL_BANDLAGER.Id());
  Query::Execute(s1);
  SQLerror::test(__FILELINE__,100);
  std::string s2="delete from ketten where (maschine,schaerdatum)=("
                  +itos(MASCHIENE)+",'"+SCHAERDATUM.to_iso()+"')";
  Query::Execute(s2);
  SQLerror::test(__FILELINE__,100);
  createJumbo(0,2000);
}

#endif

