// $Id: Zahlungsart.cc,v 1.12 2002/11/22 15:31:05 christof Exp $
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

#include <Kunde/Zahlungsart.h>
#include <Kunde/Kunde.h>
#include <Aux/Ausgabe_neu.h>

cH_Zahlungsart::cache_t cH_Zahlungsart::cache;

cH_Zahlungsart::cH_Zahlungsart(Zahlungsart::ID id)
{  cH_Zahlungsart *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   { 
    *this=cH_Zahlungsart(new Zahlungsart(id));
      cache.Register(id,*this);
   }
}

const Zahlungsart::st_skonto Zahlungsart::getSkonto(unsigned int i) const
{
  if(i>vec_skonto.size()) return st_skonto(0,0);
  return vec_skonto[--i];
}



void Zahlungsart::TeX_out(std::ostream &os,
	cP_Waehrung w, const fixedpoint<2> skontobetrag, 
		const fixedpoint<2> einzugrabattbetrag, 
		const ManuProC::Datum &zahlziel,
		const ManuProC::Datum &rgdatum,
		const cH_Kunde k) const
{
 if(bankeinzug)
  {
   os << std::string("Der Rechnungsbetrag ");
   if(vec_skonto.size()>=1)
     {os << "\\\\abzgl. "<<FormatiereTeX_short(vec_skonto[0].skontosatz)
         <<"\\% Skonto";
      if(!!einzugrabatt)
        os << " = "<<w->TeXsymbol()<<" "<<FormatiereTeX_Preis(skontobetrag);
      else
        os << " = {\\bf "<<w->TeXsymbol()<<" "<<FormatiereTeX_Preis(skontobetrag)<<"}";
     }
  if(!!einzugrabatt)
    os << "\\\\abzgl. "<<FormatiereTeX_short(einzugrabatt)
       <<"\\% wg. Abbuchung = {\\bf "<<w->TeXsymbol()
       <<" "<<FormatiereTeX_Preis(einzugrabattbetrag)<<"}\\\\";
  os << "wird ";
  if(zahlziel.valid())
     os << " am "<<zahlziel<<"\\\\\n";
  else if(zahlungsfrist)   
     os << itos(zahlungsfrist) << " Tage nach Rechnungsstellung\\\\\n";     
   os << "von Ihrem Konto "<<FormatiereTeX(k->getKtnr());
   os << "\\\\\n bei der " <<k->getbank();
   os << "\n BLZ "<<FormatiereTeX(k->getblz())<<" abgebucht\\\\\n";
  }
 else
  {
   if(zahlziel.valid())
     {if(vec_skonto.size()>=1)
       {os << "Bei Zahlung bis zum "<<zahlziel<<" "
          << vec_skonto[0].skontosatz<<"\\.\\% Skonto"
          << " = {\\bf "<<w->TeXsymbol()<<" "<<
                     FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";
	if(zahlungsfrist)
	os << "Bei Zahlung bis zum "<<zahlziel+zahlungsfrist-
			vec_skonto[0].skontofrist<<" netto.\\\\\n";
	}
      else     
       os << "Zahlung bis zum "<<zahlziel<<" netto\\\\\n";
     }
   else
     {
      if(vec_skonto.size()>=1)
        {if(vec_skonto[0].skontofrist)
	   {os << "Bei Zahlung bis zum "
           	<<rgdatum+vec_skonto[0].skontofrist<<" "
           	<< vec_skonto[0].skontosatz<<"\\.\\% Skonto"
           	<< " = {\\bf "<<w->TeXsymbol()<<" "
	   	<<FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";
	    if(zahlungsfrist)
	      os << "Bei Zahlung bis zum "<<rgdatum+zahlungsfrist-
				vec_skonto[0].skontofrist<<" netto.\\\\\n";
	   }
	 else
	   {os << "Zahlung Vorauskasse";
	    if(!!vec_skonto[0].skontosatz)
           	os << " mit "<<vec_skonto[0].skontosatz<<"\\.\\% Skonto"
           	<< " = {\\bf "<<w->TeXsymbol()<<" "
	   	<<FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";
	    else
	   	os<<"}\\\\\n";
	   }
	}
      else
        if(zahlungsfrist)
	     {if(Id()==3)
                os << "Zahlung innerhalb " << zahlungsfrist << " (dann bis Monatsende) Tage netto.";
	      else
                os << "Zahlung innerhalb " << zahlungsfrist << " Tage netto.";
	     }
         else
             os << "Zahlung sofort netto\\\\\n";
     }
  }
}


void Zahlungsart::TeX_out(std::ostream &os,
		const ManuProC::Datum &zahlziel,
		const cH_Kunde k,
		const fixedpoint<2> skontobetrag) const
{
 if(bankeinzug)
  {
   os << std::string("Der Rechnungsbetrag ");
   if(vec_skonto.size()>=1)
     {os << "\\\\abzgl. "<<FormatiereTeX_short(vec_skonto[0].skontosatz)
         <<"\\% Skonto\\\\\n";
     }
  if(!!einzugrabatt)
    os << "\\\\abzgl. "<<FormatiereTeX_short(einzugrabatt)
       <<"\\% wg. Abbuchung\\\\\n";
  os << "wird ";
  if(zahlziel.valid())
     os << " am "<<zahlziel<<"\\\\\n";
  else if(zahlungsfrist)   
     os << itos(zahlungsfrist) << " Tage nach Rechnungsstellung\\\\\n";
   os << "von Ihrem Konto "<<FormatiereTeX(k->getKtnr());
   os << "\\\\\n bei der " <<k->getbank();
   os << "\n BLZ "<<FormatiereTeX(k->getblz())<<" abgebucht\\\\\n";
  }
 else
  {
   if(zahlziel.valid())
     {if(vec_skonto.size()>=1)
       {os << "Bei Zahlung bis zum "<<zahlziel<<" "
          << vec_skonto[0].skontosatz<<"\\.\\% Skonto\\\\\n";
	    }
      else     
       os << "Zahlung bis zum "<<zahlziel<<" netto\\\\\n";
     }
   else
     {
      if(vec_skonto.size()>=1)
        {if(vec_skonto[0].skontofrist)
	 {
	 os << "Bei Zahlung innerhalb "
            << vec_skonto[0].skontofrist<<" Tage "
            << vec_skonto[0].skontosatz<<"\\.\\% Skonto\\\\\n";
	 if(zahlungsfrist)
   	   os << "Bei Zahlung innerhalb "<<zahlungsfrist<<" Tage netto\\\\\n";
	 }
	 else
	   {os << "Zahlung Vorauskasse";
	    if(!!vec_skonto[0].skontosatz)
           	os << " mit "<<vec_skonto[0].skontosatz<<"\\.\\% Skonto"
	     	<< " = {\\bf "<<k->getWaehrung()->TeXsymbol()<<" "
	   	<<FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";
	    else
	   	os<<"\\\\\n";
	   }
	}
      else        
        if(zahlungsfrist)
	     {if(Id()==3)
            os << "Zahlung innerhalb " << zahlungsfrist << " (dann bis Monatsende) Tage netto.";
	      else
            os << "Zahlung innerhalb " << zahlungsfrist << " Tage netto.";
	     }
         else
            os << "Zahlung sofort netto\\\\\n";
     }
  }
}


