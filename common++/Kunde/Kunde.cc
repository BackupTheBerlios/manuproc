// $Id: Kunde.cc,v 1.19 2002/06/20 06:29:53 christof Exp $
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
#include <Kunde/Telefon.h>

cH_Kunde::cache_t cH_Kunde::cache;
H_Kunde::cache_t H_Kunde::cache;

cH_Kunde::cH_Kunde(Kunde::ID id)
{  cH_Kunde *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   { 
    *this=cH_Kunde(new Kunde(id));
      cache.Register(id,*this);
   }
}

H_Kunde::H_Kunde(Kunde::ID id)
{  H_Kunde *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   {
     *this=H_Kunde(new Kunde(id));
      cache.Register(id,*this);
   }
}


const std::string Kunde::LaTeX_von() const
{
  std::string s="\\underline{\\scriptsize ";
  s+= string2TeX(getName()+", ");
  
  std::string lkz=adresse.land->LKZ();
  if (!lkz.empty()) lkz+='-';

  if(!adresse.postfach.empty())
    s+= string2TeX("Postfach "+adresse.postfach+", "+
		lkz+adresse.postfachplz+" "+adresse.ort); 
  else
    s+= string2TeX(adresse.strasse +" "+adresse.hsnr+", "+
		lkz+adresse.plz+" "+adresse.ort);
//  if(kundennummer_von!=none_id) s+="\\qquad KdNr: "+Kunde(kundennummer_von).UnsereKundenNr(); 
  s+= "}\\\\[3mm]\n";
  return  s;
}

const std::string Kunde::LaTeX_von_gross(const ID kid, const string width) const
{
  std::string s=string("\\parbox[t]{")+width+"}{\\footnotesize\\raggedleft ";
  s+= string2TeX(getName())+"\\\\\n";
  s+= string2TeX(strasse()+" "+hausnr())+"\\\\\n";
  s+= string2TeX(plz()+" "+ort())+"\\\\\n";
  s+= string2TeX("Tel. "+get_first_telefon(TEL_TEL))+"\\\\\n";
  s+= string2TeX("Fax. "+get_first_telefon(TEL_FAX))+"\\\\[1ex]\n";
  s+= "\\normalsize\n";
  s+= string2TeX("Kd.-Nr. "+cH_Kunde(kid)->UnsereKundenNr())+"\\\\\n";
  s+= "}\\\\[6mm]\n";
  return  s;
}

std::string Kunde::get_first_telefon(const TelArt& art) const
{
 std::list<cH_Telefon> L=getTelefon();
 for(std::list<cH_Telefon>::const_iterator i=L.begin();i!=L.end();++i)
  {
    if((*i)->TelefonArt()==art)
      switch(art) {
         case TEL_TEL:
         case TEL_FAX: return (*i)->NummerStr(); break;
         default : return (*i)->Text();
         }
  }
 return "";
}



const std::string Kunde::LaTeX_an(bool liefer,TelArt telart,
				const string width,
				const string telwidth) const
{
  std::string strasse_postfach;
  std::string lkz_plz_ort;

#ifdef MABELLA_EXTENSIONS  // meines Wissens nicht mehr �blich, CP
  if (!adresse.land->LKZ().empty()) lkz_plz_ort += adresse.land->LKZ()+" ";
#endif  

  if ((!liefer && !adresse.postfach.empty() && rng_an_postfach) || adresse.strasse.empty())
     {
      strasse_postfach = "Postfach "+adresse.postfach;
      lkz_plz_ort +=  adresse.postfachplz+" "+adresse.ort;
     }
     else
     { 
      strasse_postfach = adresse.strasse+ " " +adresse.hsnr;
      lkz_plz_ort += adresse.plz+" "+adresse.ort;
     }

  std::string s=string("\\parbox[t]{")+width+"}{\n";
#ifndef MABELLA_EXTENSIONS
  s+="\\large ";
#endif
  s+="Firma \\\\";
  s+= string2TeX(getName(),NEEDCHAR) +"\\\\";
  if (!postanwvor().empty()) s+= string2TeX(postanwvor(),NEEDCHAR) +"\\\\";
  s += string2TeX(strasse_postfach,NEEDCHAR); 
  if (!postanwnach().empty()) s+= "\\\\" + string2TeX(postanwnach(),NEEDCHAR) ;
  s += "\\\\[1ex]" + string2TeX(lkz_plz_ort,NEEDCHAR);
  if (Auslaender()) s+="\\\\[1ex]"+string2TeX(adresse.land->Name());
  s += "}\n";
  if(telart==TEL_NONE)
    return s;


  std::string s2;
  std::list<cH_Telefon> T=getTelefon();
  s2+="\\parbox[t]{"+telwidth+"}{\\scriptsize~\\\\\\vfill\n";
  for(std::list<cH_Telefon>::const_iterator i=T.begin();i!=T.end();++i)
     {s2+= (*i)->ArtString() +" "+(*i)->NummerStr()+"\\\\\n"; }
  s2 +="}";

  return  string("\\parbox[t]{")+telwidth+"}{"+s+s2+"}\n";
}



const PreisListe::ID Kunde::preisliste() const
{
 if(!prlist_valid) Preislisten();
 
 return preislisten.empty() ? PreisListe::none_id : preislisten.front().second;
}

