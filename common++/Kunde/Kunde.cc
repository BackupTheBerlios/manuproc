// $Id: Kunde.cc,v 1.7 2001/12/05 07:55:59 christof Exp $
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
H_Kunde::cache_t H_Kunde::cache;

cH_Kunde::cH_Kunde(Kunde::ID id)
{  cH_Kunde *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   {  *this=cH_Kunde(new Kunde(id));
      cache.Register(id,*this);
   }
}

H_Kunde::H_Kunde(Kunde::ID id)
{  H_Kunde *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   {  *this=H_Kunde(new Kunde(id));
      cache.Register(id,*this);
   }
}


const std::string Kunde::LaTeX_von() const
{
  std::string s="\\underline{\\scriptsize ";
  s+= string2TeX(getName()+", ");

  if(adresse.postfach.size())
    s+= string2TeX(adresse.postfach +", Postfach "+adresse.postfach+", "+
		adresse.lkz+"-"+adresse.postfachplz+" "+adresse.ort); 
  else
    s+= string2TeX(adresse.strasse +" "+adresse.hsnr+", "+
		adresse.lkz+"-"+adresse.plz+" "+adresse.ort);
 
  s+= "}\\\\[3mm]\n";
  return  s;
}

const std::string Kunde::LaTeX_an(bool liefer=false) const
{
  std::string adr0,adr1,adr2,adr3;

  adr2 = adresse.lkz.size() ? (adresse.lkz+"-") : "";

  if (!liefer && adresse.postfach.size() && rng_an_postfach)
     {
      adr0 = postanwvor();
      adr1 = "Postfach "+adresse.postfach;
      adr2 +=  adresse.postfachplz+" "+adresse.ort;
      adr3 = postanwnach();
     }
     else
     { 
      adr0 = postanwvor();
      adr1 = adresse.strasse+ " " +adresse.hsnr;
      adr2 += adresse.plz+" "+adresse.ort;
      adr3 = postanwnach();
     }

#ifdef MABELLA_EXTENSIONS
  std::string s="\nFirma\\\\ ";
#else
  std::string s="{\\large Firma\\\\ ";
#endif

  s+= string2TeX(getName()) +"\\\\";
  if (postanwvor().size()) s+= string2TeX(adr0) +"\\\\";
  s += string2TeX(adr1); 
  if (postanwnach().size()) s+= "\\\\" + string2TeX(adr3) ;
  s += "\\\\[1ex]" + string2TeX(adr2) +"\\\\\n";
  return  s;
}

