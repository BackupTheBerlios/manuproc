/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include "ppsInstanz.h"
#include <Artikel/ArtikelStamm.h>

std::ostream &operator<<(std::ostream &o,const cH_ppsInstanz &i)
{return o<<i->Name()<<"("<<i->Id()<<")";}

cH_ppsInstanz::cache_t cH_ppsInstanz::cache;

cH_ppsInstanz::cH_ppsInstanz(ppsInstanz::ID iid) 
{ 
 cH_ppsInstanz *cached(cache.lookup(iid));
 if (cached) *this=*cached;
 else 
  { *this=cH_ppsInstanz(new ppsInstanz(iid));
    cache.Register(iid,*this);
  }
}

void  ppsInstanz::check() const
{
   if(instid==ppsInstanzID::None) return;
   if(AutomatischEinlagern())  assert(LagerInstanz());
   if(Lieferschein())          assert(ProduziertSelbst());
   if(ExterneBestellung())     assert(instid!=ppsInstanzID::Kundenauftraege);
   if(PlanungsInstanz())      {assert(!ProduziertSelbst()); 
                               assert(!Lieferschein());
                               }
//   if(EinlagernIn()!=ppsInstanzID::None) assert(ProduziertSelbst());
}     




cH_Prozess ppsInstanz::get_Prozess() const
{
  switch(instid) {
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
	case ppsInstanzID::Faerberei: return cH_Prozess(ProzessID::Faerben);break;
	case ppsInstanzID::Druckerei: return cH_Prozess(ProzessID::Drucken);break;
	case ppsInstanzID::Weberei: return cH_Prozess(ProzessID::Weben);break;
	case ppsInstanzID::Schaererei: return cH_Prozess(ProzessID::Schaeren);break;
	case ppsInstanzID::Spritzgiesserei: return cH_Prozess(ProzessID::Giessen);break;
#endif	
   default : return cH_Prozess(ManuProC::DefaultValues::Prozess); break;
	}
}

bool ppsInstanz::LagerInstanz() const
{
  if (LagerFuer()!=ppsInstanzID::None) return true;
  if(typ=='L' || typ=='l') return true;
  return false;
}

bool ppsInstanz::EigeneLagerKlasseImplementiert() const
{
  if(typ=='L') return true;
  return false;
}


bool ppsInstanz::ProduktionsInstanz() const
{
  if(typ=='P' || typ=='E') return true;
  return false;
}

bool ppsInstanz::PlanungsInstanz() const
{
  if(typ=='E') return true;
  return false;
}

cH_ppsInstanz ppsInstanz::getBestellInstanz(const ArtikelBase &artikel)
{
   cH_ppsInstanz I=ArtikelStamm(artikel).BestellenBei();
   if( I->ProduktionsInstanz())
    {  if(I->EinlagernIn()!=ppsInstanzID::None) return I->EinlagernIn();
       return I;
    }
   if( I->LagerInstanz() || I->EinlagernIn()==ppsInstanzID::None ) return I;
   return cH_ppsInstanz(I->EinlagernIn());   
}

cH_ppsInstanz ppsInstanz::getProduktionsInstanz(const ArtikelBase &artikel) 
{
   cH_ppsInstanz I=ArtikelStamm(artikel).BestellenBei();
   if(I->ProduktionsInstanz()) return I;
   return cH_ppsInstanz(I->LagerFuer());
}



std::string ppsInstanz::shortName() const
{
  std::string s=Name();
  s=s.substr(0,2);
  return s;
}

/* Uralter Code bestimmt irgendwann mal nützlich ...
int ProdLager::Lieferzeit_in_Tagen()
{
  double dauer;
  if (instanz==ppsInstanz::INST_WEB)  
   {
     Maschinen_Geschwindigkeit MG(artikel);
     dauer = menge_bestellt/MG.Speed_m_Tag();
   }     
  return dauer;
}
*/
