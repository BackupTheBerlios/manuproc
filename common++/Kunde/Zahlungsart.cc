// $Id: Zahlungsart.cc,v 1.37 2004/03/03 16:02:15 jacek Exp $
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
#include <Misc/Ausgabe_neu.h>

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

const ManuProC::Datum Zahlungsart::getZahlungstermin
(const ManuProC::Datum rgdatum) const
{
 if(!rgdatum.valid()) return ManuProC::Datum::today();
 
 ManuProC::Datum termin(rgdatum+zahlungsfrist);
 
 if(!zahlungsfrist && !bankeinzug) termin=rgdatum+getSkonto(1).skontofrist;

 // so viele Monate vorrücken
 for(int i=0; i<frist_monate; i++)
   {termin=termin+(termin.Tage_in_Monat() - termin.Tag());
    termin++;
   }

 
 if(monatsende)
   {
   termin=termin+(termin.Tage_in_Monat() - termin.Tag());
   }
 


 return termin;
}


void Zahlungsart::TeX_out(std::ostream &os,
	cP_Waehrung w, const fixedpoint<2> skontobetrag, 
		const fixedpoint<2> einzugrabattbetrag, 
		const ManuProC::Datum &zahlziel,
		const ManuProC::Datum &rgdatum,
		const cH_Kunde k,
		MultiL_Dict &mld) const
{
 char strbuf[1000];
 int BSIZE=sizeof strbuf;
 
 typedef MultiL_Dict TID;

 if(bankeinzug)
  {
  if(verfahren==LCR)
    {
     char tmpbuf[100];
//     os << "par traite Lettre de change relev\\'e (LCR)";
     if(zahlungsfrist && monatsende)
       {
	snprintf(tmpbuf,sizeof tmpbuf,mld.MLT(TID::PRINTF_ZAHLUNG6).c_str(),
			zahlungsfrist);
	os << tmpbuf << ".\\\\\n";
//os << " \\`a "<<itos(zahlungsfrist)<<" jours";
       }
     else
       os << "par traite LCR.\\\\\n";       
    }
  else if(verfahren==RIBA)  
    {
     std::string zz=zahlziel.valid() ? std::string("; valuta: ")+zahlziel.c_str() : "";

	snprintf(strbuf,BSIZE,mld.MLT(
		(monatsende ? TID::PRINTF_ZAHLUNG3 : TID::PRINTF_ZAHLUNG4)).c_str(),
	     	zahlungsfrist,zz.c_str());
	os << strbuf;
    }
  else
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
     os << "von Ihrem Konto "<< ulltos(k->getKtnr());
     os << "\\\\\n bei der " <<k->getbank();
     os << "\n BLZ "<<FormatiereTeX((unsigned long)k->getblz())<<" abgebucht\\\\\n";
    }    
      
  }
 else
  {


   if(zahlziel.valid())
     {char tmpbuf[100];
     
      if(vec_skonto.size()>=1)
       {os << "Bei Zahlung bis zum "<<zahlziel<<" "
          << vec_skonto[0].skontosatz<<"\\.\\% Skonto"
          << " = {\\bf "<<w->TeXsymbol()<<" "<<
                     FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";
	if(zahlungsfrist)
	  {
	   snprintf(tmpbuf,sizeof tmpbuf,
		mld.MLT(TID::PRINTF_ZAHLUNG9).c_str(),
		(zahlziel+zahlungsfrist-vec_skonto[0].skontofrist).c_str());
	   os << tmpbuf<<".\\\\\n";
	  }
	}
      else     
       {
        snprintf(tmpbuf,sizeof tmpbuf,
			mld.MLT(TID::PRINTF_ZAHLUNG9).c_str(),zahlziel.c_str());
        os << tmpbuf << ".\\\\\n";
       }
     }
   else
     {
      if(vec_skonto.size()>=1)
        {if(vec_skonto[0].skontofrist)
	   {
	    char tmpbuf[100];
	    snprintf(tmpbuf,sizeof tmpbuf,mld.MLT(TID::PRINTF_ZAHLUNG10).c_str(),
	    	(rgdatum+vec_skonto[0].skontofrist).c_str(),
	    	FormatiereTeX_short(vec_skonto[0].skontosatz).c_str(),
	    	w->TeXsymbol().c_str(),
	    	FormatiereTeX_Preis(skontobetrag).c_str());
	    os << tmpbuf << "\\\\\n";	
	    if(zahlungsfrist)
	      {
	       snprintf(tmpbuf,sizeof tmpbuf,mld.MLT(TID::PRINTF_ZAHLUNG9).c_str(),
	         (rgdatum+zahlungsfrist).c_str());
	       os << tmpbuf << "\\\\\n";		      
	      }
/*	      
	    os << "Bei Zahlung bis zum "
           	<<rgdatum+vec_skonto[0].skontofrist<<" "
           	<< vec_skonto[0].skontosatz<<"\\.\\% Skonto"
           	<< " = {\\bf "<<w->TeXsymbol()<<" "
	   	<<FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";
	    if(zahlungsfrist)
	      os << "Bei Zahlung bis zum "<<rgdatum+zahlungsfrist<<" netto.\\\\\n";
*/	      
	   }
	 else
	   {
	    char tmpbuf[100];
	    if(!!vec_skonto[0].skontosatz)
	      {TID::LangTXT txt;
	       if(frist_monate)
		 txt=TID::PRINTF_ZAHLUNG8;
	       else 
		 {if(entityid==29)
		    txt=TID::PRINTF_ZAHLUNG11;
	          else
		    txt=TID::PRINTF_ZAHLUNG7;
		 }

		snprintf(tmpbuf,sizeof tmpbuf,
			mld.MLT(txt).c_str(),
			vec_skonto[0].skontosatz.String(false,0,"",",").c_str());
           	os << tmpbuf
	     	<< " = {\\bf "<<k->getWaehrung()->TeXsymbol()<<" "
	   	<<FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";
	      }
	    else
	   	os<<"\\\\\n";
	   }
	}
      else
        if(zahlungsfrist)
	     {snprintf(strbuf,BSIZE,mld.MLT(
	     	(monatsende ? TID::PRINTF_ZAHLUNG2 : TID::PRINTF_ZAHLUNG1)).c_str(),
	     	zahlungsfrist);
             os << strbuf;
	     }
         else
             os << "Zahlung sofort netto\\\\\n";
     }
  }
}


void Zahlungsart::TeX_out(std::ostream &os,
		const ManuProC::Datum &zahlziel,
		const cH_Kunde k,
		const fixedpoint<2> skontobetrag,
		MultiL_Dict &mld) const
{
 char strbuf[1000];
 int BSIZE=sizeof strbuf;
 
  typedef MultiL_Dict TID;

 if(bankeinzug)
  {
  if(verfahren==LCR)
    {
     char tmpbuf[100];
     if(zahlungsfrist && monatsende)
       {
	snprintf(tmpbuf,sizeof tmpbuf,mld.MLT(TID::PRINTF_ZAHLUNG6).c_str(),
			zahlungsfrist);
	os << tmpbuf << ".\\\\\n";
       }
     else
       os << "par traite LCR.\\\\\n";       
    }
  else if(verfahren==RIBA)  
    {
     std::string zz=zahlziel.valid() ? std::string("; valuta: ")+zahlziel.c_str() : "";

	snprintf(strbuf,BSIZE,mld.MLT(
		(monatsende ? TID::PRINTF_ZAHLUNG3 : TID::PRINTF_ZAHLUNG4)).c_str(),
	     	zahlungsfrist,zz.c_str());
	os << strbuf;  
    }
  else  
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
     os << "von Ihrem Konto "<< ulltos(k->getKtnr());
     os << "\\\\\n bei der " <<k->getbank();
     os << "\n BLZ "<<FormatiereTeX((unsigned long)k->getblz())<<" abgebucht\\\\\n";
   }
  }
 else
  {
   if(zahlziel.valid())
     {if(vec_skonto.size()>=1)
       {os << "Bei Zahlung bis zum "<<zahlziel<<" "
          << vec_skonto[0].skontosatz<<"\\.\\% Skonto\\\\\n";
	    }
      else     
	{
	   snprintf(strbuf,BSIZE,
		mld.MLT(TID::PRINTF_ZAHLUNG9).c_str(),zahlziel.c_str());
	   os << strbuf<<"\\\\\n";
	}
     }
   else
     {
      if(vec_skonto.size()>=1)
        {if(vec_skonto[0].skontofrist)
	 {
	 char tmpbuf[100];
	 snprintf(tmpbuf,sizeof
		tmpbuf,mld.MLT(TID::PRINTF_ZAHLUNG5).c_str(),	
            		vec_skonto[0].skontofrist,
		vec_skonto[0].skontosatz.String(false,0,"",",").c_str());
//<<"\\.\\% Skonto\\\\\n";
	 os << tmpbuf;
	 if(zahlungsfrist)
	   {
	    snprintf(tmpbuf,sizeof
		tmpbuf,mld.MLT(TID::PRINTF_ZAHLUNG1).c_str(),	
            		zahlungsfrist);
   	    os << "\\\\\n"<<tmpbuf<<"\\\\\n";
	  }
	 }
	 else
	   {char tmpbuf[100];
	    if(!!vec_skonto[0].skontosatz)
	      {
		TID::LangTXT txt;
	       if(frist_monate)
		 txt=TID::PRINTF_ZAHLUNG8;
	       else 
		 {if(entityid==29)
		    txt=TID::PRINTF_ZAHLUNG11;
	          else
		    txt=TID::PRINTF_ZAHLUNG7;
		  }

		snprintf(tmpbuf,sizeof tmpbuf,
			mld.MLT(txt).c_str(),
			vec_skonto[0].skontosatz.String(false,0,"",",").c_str());
           	os << tmpbuf
	     	<< " = {\\bf "<<k->getWaehrung()->TeXsymbol()<<" "
	   	<<FormatiereTeX_Preis(skontobetrag)<<"}\\\\\n";

	      }
	    else
	   	os<<"\\\\\n";
	   }
	}
      else        
        if(zahlungsfrist)
	     {snprintf(strbuf,BSIZE,mld.MLT(
	     	(monatsende ? TID::PRINTF_ZAHLUNG2 : TID::PRINTF_ZAHLUNG1)).c_str(),
	     	zahlungsfrist);
             os << strbuf;
	     }        
         else
            os << "Zahlung sofort netto\\\\\n";
     }
  }
}


void Zahlungsart::setVerfahren(const std::string v)
{
 if(v=="DTAUS")
   verfahren=DTAUS;
 else if(v=="RIBA") 
   verfahren=RIBA;
 else if(v=="LCR") 
   verfahren=LCR;   
 else verfahren=NONE;   
 
}

std::string Zahlungsart::VerfahrenStr(Verfahren v)
{
 switch(v)
   { case DTAUS : return "DTAUS"; break;
     case RIBA : return "RIBA"; break;
     case LCR : return "LCR"; break;
     default : return "";
   }
 return "";
}


