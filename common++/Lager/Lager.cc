// $Id: Lager.cc,v 1.15 2002/10/24 14:06:50 thoma Exp $
/*  pps: ManuProC's production planning system
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

#include "Lager.h"
#ifdef PETIG_EXTENSIONS
#include "JumboLager.h"
#include "RohwarenLager.h"
#endif
#include "Lager_Vormerkungen.h"
#include <Auftrag/AufEintrag.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/selFullAufEntry.h>
#include <Instanzen/ppsInstanz.h>
#include <Aux/Transaction.h>
#include <Artikel/ArtikelStamm.h>
#include <algorithm>
#include <unistd.h>
#include <Aux/AdminProblems.h>
//#include <Instanzen/Produziert.h>
#include <Instanzen/ppsInstanzProduziert.h>
#include <Misc/relops.h>
#include <Misc/Trace.h>

H_Lager::H_Lager(const ArtikelBase& artikel) 
{
// *this=H_Lager(ArtikelStamm(artikel).BestellenBei());
 *this=H_Lager(ppsInstanz::getBestellInstanz(artikel));
}

H_Lager::H_Lager(const cH_ppsInstanz& instanz) 
{
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
 switch (instanz->Id())
   {
      case ppsInstanzID::Bandlager : *this= new JumboLager() ; break;
      case ppsInstanzID::Rohwarenlager  : *this= new RohwarenLager(); break;
      default: *this= new Lager(instanz->Id()); 
//      default : assert (!"Ungültiges Lager");
   }
#else
  assert(!"Keine Lagerklasse definiert");
#endif

}


void Lager::rein_ins_lager(ArtikelBase artikel,AuftragBase::mengen_t menge,int uid)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
     "Lager="+cH_ppsInstanz(instanz)->Name(),
     "Artikel="+cH_ArtikelBezeichnung(artikel)->Bezeichnung(),
     "Menge="+dtos(menge));
  assert(menge>=0);
  try{
     Transaction tr;
     dispo_auftrag_aendern(artikel,menge);

     Lager_Vormerkungen::freigegeben_menge_neu_verplanen(instanz,artikel,menge,uid,AufEintragBase::r_Produziert);

//     cH_ppsInstanz I(instanz);
     ManuProC::st_produziert sp(artikel,menge,uid);
//cout << "Lager: "<<ppsInstanz::getProduktionsInstanz(artikel)->Name()<<'\n';
     if(!ppsInstanz::getProduktionsInstanz(artikel)->ProduziertSelbst())
           ppsInstanz::getProduktionsInstanz(artikel)->Produziert(sp);

     tr.commit();
   } catch(SQLerror &e)
     { std::cout << e <<'\n';}
}

void Lager::raus_aus_lager(ArtikelBase artikel,AuftragBase::mengen_t menge,int uid)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
     "Artikel="+cH_ArtikelBezeichnung(artikel)->Bezeichnung(),
     "Menge="+dtos(menge));
  assert(menge>=0);
  try{
    assert(cH_ppsInstanz(instanz)->ProduziertSelbst());

     ManuProC::st_produziert sp(artikel,menge,uid);
     cH_ppsInstanz(instanz)->Lager_abschreiben(sp);

   } catch(SQLerror &e) { std::cout << e <<'\n';}
}


bool Lager::dispo_auftrag_aendern(ArtikelBase artikel,AuftragBase::mengen_t menge)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
     "Artikel="+cH_ArtikelBezeichnung(artikel)->Bezeichnung(),
     "Menge="+dtos(menge));
   AuftragBase da(instanz,AuftragBase::dispo_auftrag_id);
   int znr=-1,newznr=-1;
   AuftragBase::mengen_t oldmenge;
   bool alt=da.existEntry(artikel,Lager::Lagerdatum(),znr,newznr,oldmenge,OPEN);
   if(alt)
     {
      AuftragBase::mengen_t mt=AufEintragBase(da,znr).updateStkDiffBase__(getuid(),menge);
      assert(mt==menge);
     }
   else
     {
      Auftrag A(da);
      A.push_back(menge,Lager::Lagerdatum(),artikel,OPEN,getuid(),false);
     }
  return alt;
}

Lager::Lager(ppsInstanz::ID _instanz)
: instanz(_instanz)
{
  if(cH_ppsInstanz(instanz)->LagerInstanz()==ppsInstanzID::None)
     assert(!"Lager::check_lager_is_valid: Kein gültiges Lager\n");
}


std::vector<class LagerInhalt> Lager::LagerInhalt()  const
{
  return LagerInhalt_(ArtikelBase());
} 
  
class LagerInhalt Lager::LagerInhalt(const ArtikelBase& artikel) const
{
  std::vector<class LagerInhalt> L=LagerInhalt_(artikel);
  if(L.empty()) return class LagerInhalt(artikel,0,0,0,0);
  return *(L.begin());
}
        
void Lager::LagerInhaltSum(std::vector<class LagerInhalt>& LI) 
{
  std::sort(LI.begin(),LI.end());  
  std::vector<class LagerInhalt>::iterator i,j,k;
  for(i=LI.begin();i!=LI.end();)
   {
     j=i; ++j;
     if(*i!=*j) {++i; continue;}
     k=j;
     for(;k!=LI.end() && *i==*k ;++k)  *i+=*k ;
     i=LI.erase(j,k);     
   }
}

        