// $Id: Preis.cc,v 1.10 2002/05/06 13:41:22 christof Exp $
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
#include <Aux/string0.h>
#include <Aux/itos.h>
#include <iomanip.h>
#include <Aux/Ausgabe_neu.h>

Preis::geldbetrag_t Preis::Wert_fr(cP_Waehrung tp,preismenge_t stueckgr) const throw()
{  float result=pfennig_cent;
   preismenge_t preism=preismenge;
   if (!preism) preism=1;
   if (tp!=waehrung) result*=Waehrung::Umrechnung(*waehrung,*tp);
   if (stueckgr>0 && preism!=stueckgr) result*=stueckgr/preism;
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
   res.pfennig_cent-=b.Wert(waehrung,preismenge);
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

Preis::geldbetrag_t Preis::Gesamtpreis(cP_Waehrung w,int anzahl,float menge,const rabatt_t &rabatt) const
{  if (!*this) return 0;
   if (!menge) menge=1;
   return (In(w,menge*anzahl)*(1-0.01*double(rabatt))).Wert();
}

const Preis Preis::Gesamtpreis(int anzahl,float menge,const rabatt_t &rabatt) const
{  if (!*this) return *this;
   if (!menge) menge=1;
   Preis result=In(waehrung,menge*anzahl)*(1-0.01*double(rabatt));
   result.preismenge=1;
   return result;
}

const Preis operator*(fixedpoint<5> f, const Preis &p)
{  if (f==fixedpoint<5>(1.0)) return p;
   return Preis(p.Wert()*f.as_float(),p.getWaehrung(),p.BezugsMenge());
}
