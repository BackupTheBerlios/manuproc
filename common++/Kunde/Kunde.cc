// $Id: Kunde.cc,v 1.8 2002/01/22 09:15:55 christof Exp $
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

  if(!adresse.postfach.empty())
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
  std::string adr1,adr4;

  adr4 = !adresse.lkz.empty() ? (adresse.lkz+"-") : "";

  if ((!liefer && !adresse.postfach.empty() && rng_an_postfach) || adresse.strasse.empty())
     {
      adr1 = "Postfach "+adresse.postfach;
      adr4 +=  adresse.postfachplz+" "+adresse.ort;
     }
     else
     { 
      adr1 = adresse.strasse+ " " +adresse.hsnr;
      adr4 += adresse.plz+" "+adresse.ort;
     }

#ifdef MABELLA_EXTENSIONS
  std::string s="\nFirma\\\\ ";
#else
  std::string s="{\\large Firma\\\\ ";
#endif

  s+= string2TeX(getName(),NEEDCHAR) +"\\\\";
  if (!postanwvor().empty()) s+= string2TeX(postanwvor(),NEEDCHAR) +"\\\\";
  s += string2TeX(adr1,NEEDCHAR); 
  if (!postanwnach().empty()) s+= "\\\\" + string2TeX(postanwnach(),NEEDCHAR) ;
  s += "\\\\[1ex]" + string2TeX(adr4,NEEDCHAR) +
#ifndef MABELLA_EXTENSIONS
       					"}"
#endif
					   "\\\\\n";
  return  s;
}


