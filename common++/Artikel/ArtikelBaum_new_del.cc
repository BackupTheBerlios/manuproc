// $Id: ArtikelBaum_new_del.cc,v 1.4 2003/03/20 15:32:01 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, 
 *  written by Malte Thoma and Christof Petig
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

#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/AufEintragZu.h>
#include <Misc/Transaction.h>

void ArtikelBaum::new_Artikel(int uid,ArtikelBase fuer_artikel,const RohArtikel& RA)
{assert(RA.rohartikel.Id()!=0);
 Transaction tr;

 Query("insert into artikelzusammensetzung "
 	"(id,prozessid,altartikelid,menge,stand,uid) "
 	"values (?,?,?,?,now(),?)").lvalue()
 	<< fuer_artikel.Id() << RA.erzeugung->Id()
 	<< RA.rohartikel.Id() << Query::NullIf<faktor_t>(RA.menge,0)
 	<< uid;
 SQLerror::test(__FILELINE__);

// create_in_zuordnung(uid,fuer_artikel,RA.rohartikel,RA.menge);
 tr.commit();
}

#if 0
void ArtikelBaum::create_in_zuordnung(int uid,ArtikelBase alt_artikel,ArtikelBase kind_artikel,faktor_t RohMenge)
{
  std::list<AufEintragBase> elternliste = AufEintragZu::get_AufEintragList_from_Artikel(alt_artikel,ArtikelStamm(alt_artikel).BestellenBei()->Id(),NOSTAT);
//std::cout << "Create :"<<elternliste.size()<<"\n\n\n\n";
  for(std::list<AufEintragBase>::const_iterator e=elternliste.begin();e!=elternliste.end();++e)
   {
     AufEintrag AE(*e);
     AufEintrag::ArtikelInternNachbestellen(
     	ppsInstanz::getBestellInstanz(kind_artikel),AE.getStueck()*RohMenge,
     	AE.getLieferdatum()-AE.Instanz()->ProduktionsDauer(),
     	kind_artikel,uid,AE);
   }  
}
#endif

void ArtikelBaum::delete_Artikel(int uid,ArtikelBase fuer_artikel,ArtikelBase von_artikel)
{
 Transaction tr;
 Query("delete from artikelzusammensetzung "
   "where id=? and altartikelid=?")
   << fuer_artikel.Id() << von_artikel.Id();
 SQLerror::test(__FILELINE__,100);

// delete_from_zuordnung(uid,fuer_artikel,von_artikel);
 tr.commit();
}

#if 0
namespace {
struct st_aam
{	AufEintrag AEB;
	AufEintrag oldAEB;
	AufEintragBase::mengen_t menge;

	st_aam() : menge(0) {}
#if 0	
        st_aam(const AufEintrag &_AEB,const AufEintrag &_oldAEB,AufEintragBase::mengen_t _menge)
          :AEB(_AEB),oldAEB(_oldAEB),menge(_menge) {} 
#endif          
};
}

static FetchIStream &operator>>(FetchIStream &is, st_aam &b)
{  return is >> b.oldAEB >> b.AEB >> b.menge;
}

void ArtikelBaum::delete_from_zuordnung(int uid,ArtikelBase alt_artikel,ArtikelBase kind_artikel)
{  // Mengen holen und
   // AufEntrys um die Menge reduzieren
     std::vector<st_aam> V;
     (Query("select altinstanz,altauftragid,altzeilennr,"
         	"neuinstanz,neuauftragid,neuzeilennr,"
         	"menge "
         	"from auftragsentryzuordnung where "
         	"(altinstanz,altauftragid,altzeilennr) in "
               	"(select instanz,auftragid,zeilennr from auftragentry "
               		"where artikelid=?) and "   
         	"(neuinstanz,neuauftragid,neuzeilennr) in "
            	"(select instanz,auftragid,zeilennr from auftragentry "
       		"where artikelid=?)") 
       	<< alt_artikel.Id() << kind_artikel.Id()).FetchArray(V);
     
     for(std::vector<st_aam>::const_iterator i=V.begin();i!=V.end();++i)
      {
        reduceChildren(uid,i->AEB,i->oldAEB,i->menge);
        Query("delete from auftragsentryzuordnung "
        	"where (altinstanz,altauftragid,altzeilennr)=(?,?,?) "
        	"and (neuinstanz,neuauftragid,neuzeilennr)=(?,?,?) "
        	/*"and menge=0"*/).lvalue()
           << i->oldAEB << i->AEB;
        SQLerror::test(__FILELINE__,100);
      }
}

#warning dies sollte nach AufEintrag wandern ... wenn es noch so stimmt

void ArtikelBaum::reduceChildren(int uid,const AufEintrag& AEB,
                                 const AufEintrag& oldAEB,
                                 AufEintragBase::mengen_t menge) 
{
  ManuProC::Trace _t(AuftragBase::trace_channel,__FUNCTION__,AEB,oldAEB,menge);

  const_cast<AufEintrag&>(AEB).MengeAendern(uid,-menge,false,oldAEB,ManuProC::Auftrag::r_Anlegen);

  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(AEB,AufEintragZu::list_kinder);
  for(AufEintragZu::list_t::iterator i=L.begin();i!=L.end();++i)
     reduceChildren(uid,AufEintrag(i->AEB),AEB,i->Menge);
}        

#endif
