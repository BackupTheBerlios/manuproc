// $Id: Preis.cc,v 1.19 2003/11/18 12:59:28 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#include "Preis.h"
#include<iostream>
#include <Misc/string0.h>
#include <Misc/itos.h>
#include <iomanip.h>
#include <Misc/Ausgabe_neu.h>
#include <utility>
#include<ManuProCConfig.h>

Preis::geldbetrag_t Preis::Wert_fr(const cP_Waehrung w,preismenge_t stueckgr) const throw()
{  if (w==waehrung && (!stueckgr || stueckgr==preismenge)) 
	return pfennig_cent;

   float result=pfennig_cent.as_float();
   preismenge_t preism=preismenge;
   if (!preism) preism=1;
// Währung und Stückgröße werden zusammen umgerechnet und dann erst gerundet
   if (w!=waehrung) result*=Waehrung::Umrechnung(*waehrung,*w);
   if (stueckgr>0 && preism!=stueckgr) result*=stueckgr.as_float()/preism.as_float();
   return result; // erst hier wird wieder gerundet!
}

Preis Preis::operator+(const Preis &b) const
{  if (pfennig_cent.Scaled()!=0)
   {  Preis res(*this);
      res.pfennig_cent+=b.Wert_fr(waehrung,preismenge);
      return res;
   }
   else return b;
}

Preis Preis::operator-(const Preis &b) const
{  Preis res(*this);
   res.pfennig_cent-=b.Wert_fr(waehrung,preismenge);
   return res;
}

bool Preis::operator!=(const Preis &b) const
{  return pfennig_cent!=b.Wert_fr(waehrung,preismenge);
}

bool Preis::operator<(const Preis &b) const
{  return pfennig_cent<b.Wert_fr(waehrung,preismenge);
}

std::ostream &operator<<(std::ostream &o,const Preis &p)
{  o << (p.pfennig_cent);
   if (!p.short_shl) o << p.Typtext();
   return o;
}

const std::string Preis::Typtext() const
{  return waehrung->Kurzbezeichnung() + '/'+ Formatiere_short(preismenge);
}

Preis::geldbetrag_t Preis::Gesamtpreis(const cP_Waehrung w,int anzahl,preismenge_t menge,const rabatt_t &rabatt) const
{  if (!*this) return 0;
   if (!menge) menge=1;
   Preis result=*this;

#ifdef MABELLA_EXTENSIONS
   bool rescale(result.Wert().Scale()>2 && !(result.Wert().Scaled()%10));
#endif

   // Währung umrechnen?
   if (w!=waehrung) result=result.In(w,result.preismenge);
   // Rabattieren?
   if (!!rabatt) result.pfennig_cent=result.pfennig_cent*(1-0.01*rabatt.as_float());
   // mit Menge multiplizieren

#ifdef MABELLA_EXTENSIONS
   if(rescale)
     {fixedpoint<2> retpr(result.Wert());
      Preis ret(retpr,result.waehrung,menge*anzahl);
      return fixedpoint<2>(ret.In(ret.waehrung,menge*anzahl).Wert());
     }
   else
#endif
     return result.In(result.waehrung,menge*anzahl).Wert();
}

// braucht man diese Routine wirklich? ich würde den anderen Gesamtpreis empfehlen CP
const Preis Preis::Gesamtpreis(int anzahl,float menge,const rabatt_t &rabatt) const
{  return Preis(Gesamtpreis(waehrung,anzahl,menge,rabatt),waehrung,1);
}


const Preis operator*(fixedpoint<5> f, const Preis &p)
{  if (f==fixedpoint<5>(1.0)) return p;
   return Preis(p.Wert()*f.as_float(),p.getWaehrung(),p.BezugsMenge());
}
