/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Artikel/ArtikelMengeSumme.h>
#include <Aux/Ausgabe_neu.h>

void ArtikelMenge::summiere(const ArtikelBaum &baum, mengen_t menge, const Einheit &e)
{  bool same=false;
   lvalue(e); // damit dieser Eintrag vorher auftaucht, vorab einfügen.
   for (ArtikelBaum::const_iterator i=baum.begin();i!=baum.end();++i)
   {  Einheit e2=ArtikelBase(i->rohartikel);
      if (e2==e) same=true;
      summiere(ArtikelBase(i->rohartikel),menge*i->menge,e2);
   }
   if (!same) lvalue(e)+=menge;
}

ArtikelMenge::ArtikelMenge(const ArtikelBase &_ab, int Stueck, mengen_t Menge)
	: ab(_ab)
{  mengen_t menge;
   if (!Menge) menge=Stueck;
   else if (!Stueck) menge=Menge;
   else menge=Stueck*Menge;
   
   summiere(ab,menge,Einheit(ab));
}

std::string ArtikelMenge::abgeleiteteMenge() const
{  if (emenge.empty()) return "";
   vector_t::const_iterator b=emenge.begin(),e=emenge.end();
   ++b;
   std::string result;
   for (;b!=e;++b)
      result+=ArtikelMenge::Formatiere(*b)+' ';
   return result;
}

std::string ArtikelMenge::Menge() const
{  if (emenge.empty()) return "";
   return ArtikelMenge::Formatiere(emenge[0]);
}

bool operator==(const ArtikelMenge::pair_t &a, const Einheit &b)
{  return a.first==b; }

bool operator==(const ArtikelMenge::pair_l_t &a, const Einheit &b)
{  return a.first==b; }

ArtikelMenge::mengen_t ArtikelMenge::getMenge(const Einheit &e) const
{  vector_t::const_iterator i= find(emenge.begin(),emenge.end(),e);
   if (i==end()) return mengen_t(0);
   return i->second;
}

ArtikelMengeSumme::mengen_t ArtikelMengeSumme::Summe(const Einheit &e) const
{  vector_t::const_iterator i= find(emenge.begin(),emenge.end(),e);
   if (i==emenge.end()) return mengen_t(0);
   return i->second;
}

ArtikelMengeSumme::mengen_t &ArtikelMengeSumme::lvalue(const Einheit &e)
{  vector_t::iterator i= find(emenge.begin(),emenge.end(),e);
   if (i==emenge.end()) 
   { emenge.push_back(pair_t(e,0));
     return emenge.back().second;
   }
   return i->second;
}

ArtikelMenge::mengen_t &ArtikelMenge::lvalue(const Einheit &e)
{  vector_t::iterator i= find(emenge.begin(),emenge.end(),e);
   if (i==end()) 
   { emenge.push_back(pair_t(e,0));
     return emenge.back().second;
   }
   return i->second;
}

void ArtikelMengeSumme::cumulate(const ArtikelMenge &am)
{  if (am.empty()) return;
   if (empty())
   {  for (ArtikelMenge::const_iterator i=am.begin();i!=am.end();++i)
         emenge.push_back(pair_t(i->first,i->second));
      return;
   }
   if (einfach && !(emenge[0]==am.getEinheit()))
      einfach=false;
   for (ArtikelMenge::const_iterator i=am.begin();i!=am.end();++i)
      lvalue(i->first)+=i->second;
}

std::string ArtikelMengeSumme::abgeleiteteMenge() const
{  if (empty()) return "";
   vector_t::const_iterator b=emenge.begin(),e=emenge.end();
   if (ist_einfach()) ++b;
   std::string result;
   for (;b!=e;++b)
      result+=ArtikelMenge::Formatiere(*b)+' ';
   return result;
}

std::string ArtikelMengeSumme::Menge() const
{  if (empty() || !ist_einfach()) return "";
   return ArtikelMenge::Formatiere(emenge[0]);
}

std::string ArtikelMenge::Formatiere(const pair_t &p)
{  return ::Formatiere_short(p.second) + std::string(p.first);
}

std::string ArtikelMenge::Formatiere(const pair_l_t &p)
{  return ::Formatiere_short(p.second) + std::string(p.first);
}

std::string ArtikelMengeSumme::Menge(const Einheit &e) const
{  mengen_t m=Summe(e);
   if (!m) return "";
   return ArtikelMenge::Formatiere(pair_t(e,m));
}

std::string ArtikelMenge::Menge(const Einheit &e) const
{  mengen_t m=getMenge(e);
   if (!m) return "";
   return Formatiere(pair_t(e,m));
}

