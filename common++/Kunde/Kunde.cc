// $Id: Kunde.cc,v 1.1 2001/04/23 08:11:59 christof Exp $
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

#include "Kunde/Kunde.h"
#include <Aux/Ausgabe_neu.h>
#include <Aux/Transaction.h>

cH_Kunde::cache_t cH_Kunde::cache;

cH_Kunde::cH_Kunde(Kunde::ID id)
{  cH_Kunde *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   {  *this=cH_Kunde(new Kunde(id));
      cache.Register(id,*this);
   }
}

const string Kunde::LaTeX_von() const
{
  string s="\\underline{\\scriptsize ";
  s+= string2TeX(getName() +", Postfach "+adresse.postfach+", "+
				adresse.postfachplz+" "+adresse.ort); 
  s+= "}\\\\[3mm]\n";
  return  s;
}

const string Kunde::LaTeX_an(const string& wo) const
{
  string adr1,adr2;
  if (wo=="Lieferschein" || !rng_an_postfach || !adresse.postfach.size() )
   {
     adr1 = adresse.strasse+ " " +adresse.hsnr;
     adr2 =  adresse.plz+" "+adresse.ort;
   }
  else 
   {
     adr1 = "Postfach "+adresse.postfach;
     adr2 =  adresse.postfachplz+" "+adresse.ort;
   }

  string s="{\\large Firma\\\\ ";
  s+= string2TeX(getName()) +"\\\\"+ string2TeX(adr1) 
      + "\\\\[1ex]" + string2TeX(adr2) +"}\\\\\n";
  return  s;
}

