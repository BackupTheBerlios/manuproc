// $Id: Preis.cc,v 1.1 2001/04/23 08:11:58 christof Exp $
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

Preis::pfennig_cent_t Preis::Wert_fr(cP_Waehrung tp,float stueckgr) const throw()
{  float result=pfennig_cent;
   if (tp!=waehrung) result*=Waehrung::Umrechnung(*waehrung,*tp);
   if (stueckgr>0 && preismenge!=stueckgr) result*=stueckgr/preismenge;
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

#if 0
Preis::Preis(float dm_euro, const char *_waehrung, const char *einheit,float stueckgr):
	pfennig_cent(dm_euro), stueckgroesse(stueckgr), waehrung(0), 
	stueckpreis(false), short_shl(false)
{  if (!strcmp(_waehrung,"DM")) waehrung=DM;
   else if (!strcmp(_waehrung,"EUR")) waehrung=EURO;
   else cerr << "Preis::Preis: unbekannte Währung '" << waehrung << "'\n";
   if (!strcmp(einheit,"stk")) stueckpreis=true;
   else if (!strcmp(einheit,"100m")) 
   {  stueckpreis=false; stueckgroesse=100; }
   else if (!strcmp(einheit,"Tstk"))
   {  stueckpreis=true; stueckgroesse=1000; }
   else cerr << "Preis::Preis: unbekannte Einheit '" << einheit << "'\n";
}
#endif

#if 0
void Preis::write(float &dm_euro, char *waehr, int size_w, char *einheit, int size_e) const
{  dm_euro=pfennig_cent;
   if (waehrung==DM) strncpy0(waehr,"DM",size_w);
   else strncpy0(waehr,"EUR",size_w);
   if (!stueckpreis) strncpy0(einheit,"100m",size_e);
   else if (stueckpreis && stueckgroesse!=1000) strncpy0(einheit,"stk",size_e);
   else strncpy0(einheit,"Tstk",size_e);
}
#endif

#include <iomanip.h>

ostream &operator<<(ostream &o,const Preis &p)
{  o << (p.pfennig_cent);
   if (!p.short_shl) o << p.Typtext();
   return o;
}

const string Preis::Typtext() const
{  return waehrung->Kurzbezeichnung()+'/'+itos(preismenge);
}

Preis::pfennig_cent_t Preis::Gesamtpreis(cP_Waehrung w,int anzahl,float menge,const fixedpoint<2> &rabatt) const
{  return Gesamtpreis(anzahl,menge,rabatt).In(w).Wert();
}

const Preis Preis::Gesamtpreis(int anzahl,float menge,const fixedpoint<2> &rabatt) const
{  if (menge) return In(waehrung,menge*anzahl)*(1-0.01*double(rabatt));
   else return (*this)*(anzahl*(1-0.01*double(rabatt)));
}
