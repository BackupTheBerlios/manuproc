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
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/selFullAufEntry.h>
#include <Aux/ppsInstanz.h>
#include <Artikel/ArtikelStamm.h>
#include <algorithm>

H_Lager::H_Lager(const ArtikelBase& artikel) 
{
 *this=H_Lager(ArtikelStamm(artikel).BestellenBei());
}

H_Lager::H_Lager(const cH_ppsInstanz& instanz) 
{
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
 switch (instanz->Id())
   {
      case ppsInstanzID::Bandlager : *this= new JumboLager() ; break;
      case ppsInstanzID::Rohwarenlager  : *this= new RohwarenLager(); break;
      default : assert (!"Ungültiges Lager");
   }
#else
  assert(!"Keine Lagerklasse definiert");
#endif

}



void Lager::rein_ins_lager(ArtikelBase artikel,AuftragBase::mengen_t menge)
{
  bewegung(false,artikel,menge);
}

void Lager::raus_aus_lager(ArtikelBase artikel,AuftragBase::mengen_t menge)
{
  bewegung(true,artikel,menge);
}

void Lager::bewegung(bool raus,ArtikelBase artikel,AuftragBase::mengen_t menge)
{
  assert(menge>=0);
  try{
    ppsInstanz::ID abschreib_instanz=instanz;
    SQLFullAuftragSelector AEBSEL(SQLFullAuftragSelector::sel_Artikel_Planung
                                 (abschreib_instanz,artikel,true));
    if(!raus) 
         { instanz=cH_ppsInstanz(instanz)->LagerFuer();
           AEBSEL = SQLFullAuftragSelector::sel_Artikel_Planung
                                 (abschreib_instanz,artikel,false);
           
         }
    SelectedFullAufList AEBabschreiben(AEBSEL);
    for(SelectedFullAufList::iterator i=AEBabschreiben.begin();i!=AEBabschreiben.end();++i)
      {
//cout << "\nAufEintrag: "<<i->Instanz()->Name()<<' '<<i->Id()<<' '<<i->ZNr()<<'\n'
//   <<"Benötigt werden am "<<i->getLieferdatum().c_str()<<" noch "
//   <<i->getRestStk()<<"   Menge:"<<menge<<'\n';

       AuftragBase::mengen_t abschreibmenge;       
       if(i->getRestStk() >= menge) abschreibmenge = menge;
       else                         abschreibmenge = i->getRestStk();

       if(raus) i->abschreiben(abschreibmenge);
       else     Lager_Vormerkungen(*i).artikel_vormerken(abschreibmenge);

       menge-=abschreibmenge;
       if(!menge) break;
      }
   } catch(SQLerror &e)
     { std::cout << e <<'\n';}
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
  else return *(L.begin());
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

        