// $Id: DataBase_init.cc,v 1.18 2003/07/15 13:18:25 jacek Exp $
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

#include <Misc/FetchIStream.h>
#include <Misc/itos.h>
#include <Misc/Datum.h>
#include <Lager/RohwarenLager.h>
#include <Lager/JumboLager.h>
#include <Ketten/KettplanKette.h>
#include <unistd.h>
#include <Misc/Trace.h>


DataBase_init::DataBase_init() 
{
#ifdef PETIG_TEST
   ArtikelBaum_anlegen();
   RohwarenLager_initalisieren();
   JumboLager_initalisieren();
#elif defined MANU_PROC_TEST 
   Lager L(ROHWARENLAGER); 
   L.rein_ins_lager(ArtikelBase(1),200,getuid(),false); // Metall
   L.rein_ins_lager(ArtikelBase(2),50,getuid(),false); // Granulat klar
   L.rein_ins_lager(ArtikelBase(3),100,getuid(),false); // Granulat gelb
#elif defined MABELLA_TEST
   Lager L(FERTIGWLAGER); 
   L.rein_ins_lager(ARTIKEL_TRIO,4,getuid(),false); 
#endif
}

#ifdef PETIG_EXTENSIONS

void DataBase_init::ArtikelBaum_anlegen()
{
  ArtikelBaum_anlegen_execute(ARTIKEL_ROLLEREI,cH_Prozess(PROZESS_ROLLEREI_FAERBEREI),ARTIKEL_FAERBEREI);
  ArtikelBaum_anlegen_execute(ARTIKEL_ROLLEREI,PROZESS_DEFAULT,ARTIKEL_RINGE,FAKTOR_RINGE);
  ArtikelBaum_anlegen_execute(ARTIKEL_FAERBEREI,PROZESS_DEFAULT,ARTIKEL_BANDLAGER,FAKTOR_BANDLAGER);
  ArtikelBaum_anlegen_execute(ARTIKEL_BANDLAGER,PROZESS_DEFAULT,ARTIKEL_ACETAT,FAKTOR_ACETAT);
  ArtikelBaum_anlegen_execute(ARTIKEL_BANDLAGER,PROZESS_DEFAULT,ARTIKEL_KUPFER,FAKTOR_KUPFER);

  ArtikelBaum_anlegen_execute(ARTIKEL_ROLLEREIK,cH_Prozess(PROZESS_ROLLEREI_FAERBEREI),ARTIKEL_FAERBEREI);
  ArtikelBaum_anlegen_execute(ARTIKEL_ROLLEREIK,PROZESS_DEFAULT,ARTIKEL_RINGE,FAKTOR_RINGE);
}

void DataBase_init::ArtikelBaum_anlegen_execute(const ArtikelBase &art,
         const cH_Prozess &prozess,const ArtikelBase& art_von,  
         const ArtikelBaum::faktor_t& faktor)
{
  Query("delete from artikelzusammensetzung where id=? and altartikelid=?")
  	<< art.Id() << art_von.Id();
  Query("insert into artikelzusammensetzung (id,prozessid,altartikelid,menge) "
  	"values (?,?,?,?)")
  	<< art.Id() << prozess->Id() << art_von.Id() << Query::NullIf(faktor);
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
  Query("delete from rl_inhalt where material=?") << artikel.Id();
  
  RohwarenLager RL;
  std::string os;
  RohwarenLager::st_rohlager rohlager(LP,kartons,kg_pro_karton,reste,kg_reste,
                                      artikel,ManuProC::Datum().today());
#ifdef MIT_ROHWARENLAGER
  RL.RL_Einlagern(LP,rohlager,UID,os,true,false);
  SQLerror::test(__FILELINE__);
#endif
}

void DataBase_init::createJumbo(const int diffmaschine,const int menge,bool produziert)
{
#ifdef MIT_BANDLAGER
  Kette K(MASCHINE+diffmaschine,SCHAERDATUM);
  std::vector <ArtikelGang> artgang;
  artgang.push_back(ArtikelGang(GAENGE,ARTIKEL_BANDLAGER));
  KettplanKette KK=KettplanKette::create(K,artgang,menge,menge);
  std::vector<JumboRolle> JR=JumboRolle::create(KK);
  assert(JR.size()==1);
  Zeitpunkt_new zp("2002-1-1 12:00");
  class JumboLager JL;
  JL.Jumbo_Einlagern(LagerPlatzJumbo,JR.front(),JumboLager::Einlagern,UID,"testuser",&zp,produziert);
  SQLerror::test(__FILELINE__);
#endif
}


void DataBase_init::JumboLager_initalisieren()
{
  Query("delete from rohjumbo where artikelid=?") << ARTIKEL_BANDLAGER.Id();
  Query("delete from ketten where (maschine,schaerdatum)=(?,?)")
  	<< MASCHINE << SCHAERDATUM;
  createJumbo(0,2000);
}

#endif

