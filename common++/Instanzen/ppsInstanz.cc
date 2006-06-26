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
#include <Misc/Ausgabe_neu.h> // UTF8
#if !defined(__GNUC__) || __GNUC__<3
#include <algorithm>
#endif

ppsInstanz::ppsInstanz(ID iid)
 : instid(iid),
                      lager_fuer(ppsInstanzID::None),
                      einlagern_in(ppsInstanzID::None),
                      sortierung(),
                      lieferschein(),produziert_selbst(),
                      automatisch_einlagern(),
                      produktionsdauer_tage(),
                      typ('0'),
                      externe_bestellung()
                      {get_name(); check(); }

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

cH_ppsInstanz ppsInstanz::getBestellInstanz(const ArtikelBase &artikel) 
{  return getBestellInstanz(ArtikelStamm(artikel));
}

cH_ppsInstanz ppsInstanz::getBestellInstanz(const ArtikelStamm &artikel)
{
   cH_ppsInstanz I=artikel.BestellenBei();
   if( I->ProduktionsInstanz())
    {  if (!!artikel.LagernIn()) // Lager fÃ¼r Artikel vorgegeben
       { if (artikel.LagernIn()->Id()==ppsInstanzID::None) 
           return I; // kein Lager
         return artikel.LagernIn();
       }
       if(I->EinlagernIn()!=ppsInstanzID::None) return I->EinlagernIn();
       return I;
    }
   if( I->LagerInstanz() || I->EinlagernIn()==ppsInstanzID::None ) return I;
   return cH_ppsInstanz(I->EinlagernIn());   
}

cH_ppsInstanz ppsInstanz::getProduktionsInstanz(const ArtikelBase &artikel) 
{  return getProduktionsInstanz(ArtikelStamm(artikel));
}

cH_ppsInstanz ppsInstanz::getProduktionsInstanz(const ArtikelStamm &artikel) 
{
   cH_ppsInstanz I=artikel.BestellenBei();
   if(I->ProduktionsInstanz()) return I;
   return cH_ppsInstanz(I->LagerFuer());
}

static unsigned utf8len(unsigned char x)
{ if (!(x&0x80)) return 1;
  if ((x&0xe0)==0xc0) return 2;
  if ((x&0xf0)==0xe0) return 3;
  if ((x&0xf8)==0xf0) return 4;
  if ((x&0xfc)==0xf8) return 5;
  /* if ((x&0xfe)==0xfc) */ return 6;
}

std::string ppsInstanz::shortName() const
{
  std::string s=Name();
  if (s.size()<=2) return s;
  int len=2;
  if (Ausgabe_neu::TeX_uses_UTF8)
  { len=utf8len(s[0]);
    len+=utf8len(s[len]);
  }
  s=s.substr(0,len);
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

ppsInstanz::ID ppsInstanz::NaechsteInstanz(const ArtikelStamm &art) const
{  ppsInstanz::ID next=ppsInstanzID::None;
   if (Id()==ppsInstanzID::Kundenauftraege) next=getBestellInstanz(art)->Id();
   else if (LagerInstanz()) next=getProduktionsInstanz(art)->Id();
   if (next==ppsInstanzID::Kundenauftraege) next=ppsInstanzID::None;
   // externe Bestellungen bestellen nicht weiter !
   if (next!=ppsInstanzID::None && ExterneBestellung())
      next=ppsInstanzID::None;
   return next;
}

namespace {
  struct name_compare
  { std::string comp;
    name_compare(std::string const &s) : comp(s) {}
    bool operator()(std::pair<ppsInstanz::ID,cH_ppsInstanz> const& a) const 
    { return a.second->Name()==comp; }
  };
}

cH_ppsInstanz cH_ppsInstanz::Search(std::string const& name) throw(SQLerror)
{ // speed it up by searching in-cache members first
  static bool already_all_loaded;
  if (!already_all_loaded)
  { (void)get_all_instanz();
    already_all_loaded=true;
  }
  cache_t::const_iterator i=std::find_if(cache.begin(),cache.end(),name_compare(name));
  if (i!=cache.end()) return i->second;
  throw SQLerror("cH_ppsInstanz::Search", 100,"Instanz not found");
}
