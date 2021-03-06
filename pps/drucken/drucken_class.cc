/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001-2006 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "drucken_class.hh"
#include <Artikel/ArtikelBezeichnung.h>
#include <Kunde/Kunde.h>
#include <Gtk2TeX.h>
#include <Aux/Ausgabe_neu.h>
#include <Kunde/TelefonArt.h>
#include <pwd.h>
#include <unistd.h>
#include <Misc/multi_lang.h>
#include <Misc/relops.h>
#include <Auftrag/Auftrag.h>
#include "Configuration.h"
#include <DynamicEnums/DynamicConfig.h>

#ifdef PETIG_EXTENSIONS
#define TABW	"17.8"
#else
#define TABW	"18"
#endif

#define MWSTSATZ	0.16
#define ENTSSATZ	0.002

#define BEZEICHNUNG_SIGNIFIKANZ		-1
#define EAN_SIGNIFIKANZ			2
#define ROWS_FOR_TABLE_HEADER		6
#define OHNE_BETREUUNG			99

bool ents_flag=false;

MultiL_Dict *mld;

void LR_Abstraktion::calc_all(cH_Kunde k,bool mwst)
{
 nettobetrag = betrag;
 

// if(!k->zeilenrabatt())
//     {
     fixedpoint<2> kunden_rabatt = Rabatt();
//     fixedpoint<2> endrabatt = nettobetrag*(kunden_rabatt.as_float()/100.0);
     float rabsatz= 1 - .01 * kunden_rabatt.as_float();
     float endrabatt = nettobetrag.as_float() * (.01 * kunden_rabatt.as_float());
//     rabattiert = nettobetrag - endrabatt;
     rabattiert = nettobetrag.as_float() * rabsatz;
//     }

#ifdef MABELLA_EXTENSIONS 
 
 if(Typ()==Rechnung) ents_flag=getEntsorgung();
 else ents_flag=k->entsorgung();

 entskosten = rabattiert * (ents_flag ? ENTSSATZ : 0.0);
#endif 

 entsbetrag = rabattiert + entskosten;

 
 
 if(mwst) 
   mwstbetrag = entsbetrag * MWSTSATZ;
 else mwstbetrag=0;
 
 bruttobetrag = entsbetrag + mwstbetrag;
 fixedpoint<2> skontosatz;
 fixedpoint<2> einzugrabattsatz;
 if(Typ()==Rechnung || Typ()==Auftrag)
   {skontosatz=Skontosatz();
    einzugrabattsatz=Einzugrabatt();
   }
 else
   {skontosatz=k->skontosatz();
    einzugrabattsatz=k->einzugrabatt();
   }
 skontobetrag = bruttobetrag * ((fixedpoint<2>(100) - skontosatz)/100.0);
 einzugbetrag = skontobetrag * ((fixedpoint<2>(100) - einzugrabattsatz)/100.0);


 if(Configuration.only_check)
   {
    std::cerr << bruttobetrag <<"\n";
    exit(0);
   }
}

void LR_Abstraktion::drucken_header(std::ostream &os)
{
 Gtk2TeX::HeaderFlags hf;
 
 hf.packages="pstricks,tabularx,eurosym,ifthen";
 hf.ptsize=12;
 hf.pagestyle="empty";

 hf.leftmargin=2.25 * Gtk2TeX::in_cm;
 hf.topmargin=2.8 * Gtk2TeX::in_cm;
 hf.rightmargin=3.25 * Gtk2TeX::in_cm;
 hf.bottommargin=1; //inch
 // kein footer angegeben ???

 float mygrayvalue=0.5;
 hf.preamble="\\newrgbcolor{mygray}{"+dtos(mygrayvalue)+" "+dtos(mygrayvalue)+" "+dtos(mygrayvalue)+"}\n"
	"\\setlength{\\topsep}{0pt}\n"
	"\\setlength{\\partopsep}{0pt}\n"
	"\\setlength{\\textheight}{26.0cm}\n";  // na, ob das sinnvoll ist?

#ifdef MABELLA_EXTENSIONS
 hf.helvetica=false;
 hf.packages+=",color,times,epsfig,graphics";

 hf.leftmargin=2 * Gtk2TeX::in_cm;
 hf.rightmargin=1.25 * Gtk2TeX::in_cm;

 hf.preamble+="\\definecolor{backg}{gray}{0.985}\n"
	      "\\definecolor{lgray}{gray}{.9}\n"
	      "\\definecolor{altgray}{gray}{.6}\n"
	"\\newcommand{\\shaderow}{\\raisebox{-1pt}[0pt][0pt]{\\makebox[0pt][l]"
			"{\\kern-70pt\\colorbox{backg}%%\n"
			"{\\rule{0pt}{7pt}\\rule{\\paperwidth}{0pt}}}}}\n";
#endif

 if(Configuration.firmen_papier) os << "\\def\\kopie{1}\n";
 else if(Configuration.toTeX || Configuration.batch)
  os << "\\def\\kopie{3}\n";



 Gtk2TeX::Header(os,hf);

   os << "% output thin lines at left side\n"
	"\\vbox to 0pt{%\n"
	"\\rule{0pt}{67.5truemm}% 62.5truemm > 80.0truemm\n"
#ifndef MABELLA_EXTENSIONS	
	"\\hbox to 0pt{\\hspace*{-24truemm}\\vbox{\\hrule width 7truemm}\\hss}%\n"
#else
	"\\hbox to 0pt{\\hspace*{-24truemm}\\vbox{\\hrule width 12truemm}\\hss}%\n"
#endif	
	"\\vss}\\nointerlineskip\n";

#ifndef MABELLA_EXTENSIONS
	os << "\\vspace*{1.2cm}%\n";
#endif

  cH_Kunde kunde_an(KdNr());

#if defined MABELLA_EXTENSIONS && defined PRINT_LOGO

   os << "\\ifthenelse{\\kopie>2}"
           "{\\raisebox{0cm}[0pt][0pt]{\\makebox[0pt][l]{\\kern+140pt\\psfig{file=/usr/local/share/mabella/logo}}}}"
           "{}\n";

   if(kunde_an->verein().size())
     os << "\\raisebox{-120pt}[0pt][0pt]{%%\n"
         "\\makebox[0pt][s]{%%\n"
         "\\kern+170pt \\rotatebox{-30}{%%\n"
         "\\scalebox{4}{%%\n"
         "{\\color{lgray}" << string2TeX(kunde_an->verein()) << "}%%\n"
         "}}}}%%\\\\\n";

#endif

#ifdef HAS_ADDR_GROUP_Rechnungsadresse
 if(! kunde_an->isInGrp(KundengruppeID::Rechnungsadresse))
   kunde_an = cH_Kunde(kunde_an->Rngan());
#endif
    
 mld=new MultiL_Dict(kunde_an->Sprache());

   if(Typ()==Rechnung && storniert())
   {
     os << "\\raisebox{-300pt}[0pt][0pt]{%%\n"
         "\\makebox[0pt][s]{%%\n"
         "\\kern+70pt \\rotatebox{0}{%%\n"
         "\\scalebox{12}{%%\n"
         "{\\color{lgray} "<< mld->MLT(MultiL_Dict::TXT_STORNO) << " }%%\n"
         "}}}}%%\\\\\n";
   }

}

void LR_Abstraktion::drucken_footer(std::ostream &os)
{
  cH_Kunde kunde_an(KdNr());
  cH_Kunde kunde_von(Kunde::eigene_id);

#ifdef HAS_ADDR_GROUP_Rechnungsadresse
  if(! kunde_an->isInGrp(KundengruppeID::Rechnungsadresse))
    kunde_an = cH_Kunde(kunde_an->Rngan());
#endif
    
  if(((signed int)zeilen_passen_noch)<0) zeilen_passen_noch=0;

  if (Typ()==Rechnung && !gutschrift() && !storniert())
   {
#ifdef MABELLA_EXTENSIONS
    os << "\\vspace{0.5cm}\n";

    int passende_zeilen=8;
    // Ja, ein Hack :-)
    if(kunde_an->land()->Auslaender())
      if(!kunde_an->land()->EU())
        passende_zeilen=10; 

    if(zeilen_passen_noch<passende_zeilen) {  
	os << "\\newpage\n";
	++page_counter; 
	page_header(os);
	os << "\\bigskip\n";
// ich dachte das sollte nur f�r Schweiz rein, warum steht das hier?
if(0) {
	os << "\\begin{flushright}\n" 
	    // auf Wunsch von Frau Will für die Schweiz
	    // Ok von Gudrun (auch wenn ich es für überflüssig halte ...)
	   << mld->MLT(MultiL_Dict::TXT_UEBERTRAG) << " " << FormatiereTeX(betrag) << "\\\\\n";
        os << mld->MLT(MultiL_Dict::TXT_SUMME) << " " <<  getWaehrung()->TeXsymbol() << " "
            << FormatiereTeX(betrag) << "\\\\\n";
        os << "\\end{flushright}\n";
}

	}
    os << mld->MLT(MultiL_Dict::TXT_ZAHLUNG) << ": ";
    getZahlungsart()->TeX_out(os,getWaehrung(),
	skontobetrag,einzugbetrag,u.r->getZahlziel(),getDatum(),kunde_an,*mld);
    zeilen_passen_noch-=passende_zeilen; // CP: korrekt?

    if(kunde_an->land()->Auslaender())
         {if(zeilen_passen_noch<(passende_zeilen)) 
	  {  os << "\\newpage\n";++page_counter; page_header(os);
	// dann eben nur f�r die Schweiz
	   if(kunde_an->land()->LKZ()=="CH" ||
              kunde_an->land()->LKZ()=="N") {
	     os << "\\begin{flushright}\n" 
     	    // auf Wunsch von Frau Will für die Schweiz
     	    // Ok von Gudrun (auch wenn ich es für überflüssig halte ...)
     	        << mld->MLT(MultiL_Dict::TXT_UEBERTRAG) << " " << FormatiereTeX(betrag) << "\\\\\n";
             os << mld->MLT(MultiL_Dict::TXT_SUMME) << " " <<  getWaehrung()->TeXsymbol() << " "
                 << FormatiereTeX(betrag) << "\\\\\n";
             os << "\\end{flushright}\n";
	     }
	    
          }

	  if(!getZahlungsart()->getBankeinzug())
	    {
	     os << "~\\\\~\\\\"<<mld->MLT(MultiL_Dict::TXT_BANKVERB) <<": ";
	     os << mld->MLT(MultiL_Dict::TXT_KONTO) << " " << ulltos(kunde_von->getKtnr()) << ", ";
	     os << "BLZ " << itos(kunde_von->getblz()) << ", ";	  
	     os << mld->MLT(MultiL_Dict::TXT_BANK) << " " << kunde_von->getbank();
//	     os << "~\\\\S.W.I.F.T.: WELA DE D1 VEL - IBANDE61334500000000240044\\\\\n";
	     os << "~\\\\S.W.I.F.T.: WUPS DE 33 - IBANDE91330500000000423111\\\\\n";
	    }
	  else
	    os << "~\\\\~\\\\\n";

	  os << mld->MLT(MultiL_Dict::TXT_BTN)<<": 58063210\n";
      os << "~\\\\\\footnotesize - "<<mld->MLT(MultiL_Dict::TXT_WARE_ZOLL)<<"\\\\\\\\\n";
      os << "\\bigskip "<< mld->MLT(MultiL_Dict::TXT_MADEINGER) <<"\n";

   try{u.r->setGewicht();}

catch(SQLerror &e) { std::cout << e; return; }

   if(u.r->Pakete())
     os << "\\\\\\normalsize " << u.r->Pakete() << " "<< 
		mld->MLT(MultiL_Dict::TXT_PACKSPALLET)<<"\\hfill\n";
   if(u.r->BruttoGew().as_float())
     os << mld->MLT(MultiL_Dict::TXT_GEWBRUTTO)<<": " <<FormatiereTeX(u.r->BruttoGew()) << " kg\\hfill\n";
   if(u.r->NettoGew().as_float())
     os << mld->MLT(MultiL_Dict::TXT_GEWNETTO)<<": " <<FormatiereTeX(u.r->NettoGew()) << " kg\\\\\n";
   if (mld->getSprId()!=DEFAULT_SPRACHE) os << mld->MLT(MultiL_Dict::TXT_BTN);
   else os << "Zoll-Tarif-Nr.";
   os << ": 58063210\\\\\n";
   } // Ausland



#else
//     cH_Kunde kunde_von(Kunde::default_id);

     os <<"\n\n\\footnotesize "<< kunde_von->getBank()<<"\\\\\n";

     os << "Zahlung: "<< string2TeX(getZahlungsart()->Bezeichnung())<<"\\\\\n";
#endif

   }
 else if(Typ()==Auftrag)
   {
    if(!Rueckstand())
      {
       os << "\\bigskip\n";
       if(zeilen_passen_noch<8) {  
            os << "\\newpage\n";
            ++page_counter; 
            page_header(os);
            os << "\\bigskip\n";
            }
    #ifdef PETIG_EXTENSIONS
        os << "\\small Zahlung: "<< string2TeX(getZahlungsart()->getBezeichnung())<<"\\\\\n";
    #else
        os << mld->MLT(MultiL_Dict::TXT_ZAHLUNG) << ": "; 
        getZahlungsart()->TeX_out(os,u.a->Zahlziel(),kunde_an,skontobetrag,*mld);
    #endif
        zeilen_passen_noch-=8;
        os << "\\bigskip\n";
        if(kunde_an->get_lieferung_frei_haus()) 
          os << "\\\\" << mld->MLT(MultiL_Dict::TXT_LIEF_FREI) <<"\\\\\n";
      }
    else os << "\\bigskip\n";  
#ifndef PETIG_EXTENSIONS      
    os << "~\\\\"<<mld->MLT(MultiL_Dict::TXT_LIEFERWOCHE)<<": "<<min_KWStr<<"\\\\\n";
#endif    

   }
 else if(Typ()==Extern )
   {
    os << "\\bigskip\n";
    if(kunde_an->get_lieferung_frei_haus()) 
      os << mld->MLT(MultiL_Dict::TXT_LIEF_FREI) <<"\\\\\n";
//     os << "Mit freundlichen Gr��en\\\\[6ex]\n";
//     if(kunde_an->getBetreuer()!=Person::none_id)
//	{cH_Person betr(kunde_an->getBetreuer());
//         os<< betr->Vorname()<<" "<<betr->Name()<<"\\\\\n";
//	}
//     else
//        {std::string name =  getpwuid(getuid())->pw_gecos;
//	 os <<name<<"\\\\\n";
//	}
    os << "~\\\\Wir bitten um Zusendung einer Auftragsbest�tigung\\\\"
      "Bitte geben Sie auch bei Ihren Lieferungen und Rechnungen unsere Auftragsnummern an.\n";

   }
 else if(Typ()==Lieferschein)
  {
#ifdef MABELLA_EXTENSIONS  
   if(u.l->Pakete() || u.l->Paeckchen())
   {os << "\\vspace{.5cm}";
    char buf[500];
    snprintf(buf,sizeof buf,
		mld->MLT(MultiL_Dict::PRINTF_LIEFBESTEHT1).c_str(),
    			u.l->Paeckchen());
    os << buf;
    if(u.l->Pakete())
      {std::string endE,endM;
       if(mld->getSprId() == 4)
	 {endE="o";endM="i";}
       else if(mld->getSprId() == 2)
	 {endE="";endM="s";}
	else if(mld->getSprId() == 3)
	 {endE="colis";endM="palettes";}
	else if(mld->getSprId() == 1)
	 {endE="";endM="en";}
	else
	 {endE="";endM="";}
 
       snprintf(buf,sizeof buf,
		mld->MLT(MultiL_Dict::PRINTF_LIEFBESTEHT2).c_str(),
    			u.l->Pakete(),(u.l->Pakete()==1 ? 
				(endE+std::string(".\n")).c_str():
				(endM+std::string(".\n")).c_str() ));
       os << ( (mld->getSprId()==2 || 
		mld->getSprId()==3)   ? "" : " ") << buf;
      }
    else os << ".\n";	
	
   }
#endif	
  }
   
    if(Typ()==Auftrag || Typ()==Rechnung || Typ()==Lieferschein ||
	Typ()==Extern || Typ()==Wareneingang)
      if(!Notiz().empty())
        {
	 Gtk2TeX::StringFlags sf;
	 sf.bar_means_newline=true;
	 sf.at_least_one_non_space=false;
	 sf.convert_space_to_tilde=true;
	os << "~" << string2TeX(Notiz(),sf)<<"~" << "\\\\\n";
	}

#ifdef PETIG_EXTENSIONS
 if (Typ()==Rechnung || Typ()==Auftrag || Typ()==Lieferschein)
 {  // ohne das ~ würde no line here to end im Lieferschein erscheinen ...
     os <<"~\\\\\nDie Lieferung erfolgt zu den Einheitsbedingungen der deutschen"
       " Textilindustrie unter verl�ngertem Eigentumsvorbehalt. Gerichtsstand ist Wuppertal.\\\\\n";
 }
 if (Typ()==Auftrag)
 {  os << "{\\footnotesize Minimale Farbabweichungen sind technisch bedingt und m�ssen wir uns vorbehalten}\\\\\n";
 }
#endif 
 if(Typ()==Rechnung && !gutschrift() && !storniert()
     && kunde_an->land()->Auslaender()
     && !kunde_an->land()->EU())
        {os << "~\\\\";
	if(kunde_an->land()->LKZ()=="IL")
	  os << mld->MLT(MultiL_Dict::TXT_EU_WARE_ISRAEL);
	else
	  os << mld->MLT(MultiL_Dict::TXT_EU_PREFERENZ);
	os << "\\\\\n";
	os << "~\\\\\n"<<kunde_von->ort()<<", "<<mld->MLT(MultiL_Dict::TXT_DEN);
	}
  Gtk2TeX::Footer(os);
}


void LR_Abstraktion::drucken(std::ostream &os,const cH_ppsInstanz& _instanz)
{instanz=_instanz;

 if (Typ()==Rechnung || Typ()==Auftrag || Typ()==Extern ||
     Typ()==Wareneingang) 
 	preise_addieren=true;
 else preise_addieren=false;
#ifdef KEIN_PREIS_IM_EXTERNEN_AUFTRAG
  if(Typ()==Extern) preise_addieren=false;
#endif

 betrag=0;
 zwischensumme_betrag=0;

 Kunde::ID kunden_id = KdNr();
 
 cH_Kunde chk(kunden_id);
#ifdef HAS_ADDR_GROUP_RechnungMitAuftrag
 if (Typ()==Rechnung && chk->isInGrp(KundengruppeID::RechnungMitAuftrag)) 
   order_on_bill=true;
#  ifdef HAS_ADDR_GROUP_RechnungAuftragSchmal
 if (Typ()==Rechnung && chk->isInGrp(KundengruppeID::RechnungAuftragSchmal)) 
   order_on_bill_fixed=true;
#  endif
#endif
#ifdef HAS_ADDR_GROUP_Kombiniert
 if (Typ()==Rechnung && chk->isInGrp(KundengruppeID::Kombiniert)) 
   Configuration.combine=true;
#endif

 drucken_header(os);
 page_counter = 1;
 page_header(os);

// os << "\\begin{flushright}\n";
 os << "\\normalsize\n";

 LieferscheinBase::ID lfrsid_mem=LieferscheinBase::none_id;
 bool lfrsid_drucken=false;
 bool zwischensumme_drucken=false;
#ifdef MABELLA_EXTENSIONS 
 int aufid_mem=AuftragBase::none_id;
 bool aufid_drucken=false;
#endif

 for (LR_Abstraktion::const_iterator i=begin();i!=end();)
  { //********* je Durchlauf eine Tabelle ausgeben ********************
  
     ArtikelBase artikel  = (*i).Artikel();
#ifdef MABELLA_EXTENSIONS     
     cH_ArtikelBezeichnung bez(artikel,ExtBezSchema::default_id);
#else     
     cH_ArtikelBezeichnung bez(artikel,cH_Kunde(kunden_id)->getSchemaId());
#endif     
    schema_mem = bez->getExtBezSchema();
#ifdef MABELLA_EXTENSIONS
    try{
    schema_own = cH_ExtBezSchema(cH_Kunde(kunden_id)->getSchemaId(),ArtikelTyp(artikel));
    }
    catch(SQLerror &e)
      {if(e.Code()!=100) {std::cout << e; exit(1);}
       schema_own = schema_mem;
//	std::cout << "\nNo owner scheme; set to default scheme\n";
    }
#endif    
    preismenge_mem;
    einheit_mem = Einheit(artikel); 

// falls Lieferschein sich ge�ndert hat, neue Nummer unten ausgeben
// merken welche Werte in der 1. Zeile stehen
    if (Typ()==Rechnung && (*i).Lfrs().Id()!=lfrsid_mem)
    {  lfrsid_drucken=true;
       cH_Kunde kunde_an(KdNr());
       zwischensumme_drucken=
		((lfrsid_mem!=LieferscheinBase::none_id) && 
#ifdef HAS_ADDR_GROUP_Zwischensumme
		 (kunde_an->isInGrp(KundengruppeID::Zwischensumme))
#else
                 false
#endif
		 ); 
       lfrsid_mem=(*i).Lfrs().Id();
    }
    
#ifdef MABELLA_EXTENSIONS
// falls Auftrag sich ge�ndert hat, neue Nummer unten ausgeben
// merken welche Werte in der 1. Zeile stehen
    if ((Typ()==Lieferschein || Typ()==Wareneingang || Typ()==Rechnung) && (*i).AufId()!=aufid_mem)
    {  if(!gutschrift())
	 aufid_drucken=true;
       aufid_mem=(*i).AufId();
    }    
#endif    
    
    if (preise_addieren) preismenge_mem = (*i).getPreis().PreisMenge();

// welche Spalten sollen vorkommen
    stueck_bool=false;
    menge_bool=false;
    rabatt_bool=false;
    palette_bool=false;
    notice_column_bool=false;
    
    bool notice_column_possible=true;
//******** wieviele Zeilen passen denn maximal in diese Tabelle ************
    LR_Abstraktion::const_iterator j=i ;
    for (;j!=end() ;++j) 
      {
        ArtikelBase artikelbase  = (*j).Artikel();
	ExtBezSchema::ID schema_id=cH_Kunde(kunden_id)->getSchemaId();
        cH_ArtikelBezeichnung bez(artikelbase,schema_id);
        cH_ExtBezSchema schema = bez->getExtBezSchema();

#ifdef MABELLA_EXTENSIONS        
	// Schema hat gewechselt; nicht jedoch wenn im neuen Schema nicht gefunden
	// unde deswegen auf default Schema umgesprungen
        if (schema!=schema_own && schema_id==schema->Id() && !Configuration.combine) break; 
#else
        if (schema!=schema_mem && !Configuration.combine) break; // Schema hat gewechselt
#endif        
        
        if (Einheit(artikelbase) != einheit_mem && !Configuration.combine) break;  // Einheit wechselt
        

        if(Typ()==Rechnung || Typ()==Lieferschein || Typ()==Wareneingang)
          {
            if ((*j).Stueck()!=1 || !(*j).Menge()) stueck_bool=true;
            if ((*j).Menge()!=0)                   menge_bool=true;
          }
        else  stueck_bool=true  ;
        
        if (Typ()==Rechnung && lfrsid_mem != (*j).Lfrs().Id())
            break; // Lieferschein wechselt;

#ifdef MABELLA_EXTENSIONS           
        if ((Typ()==Lieferschein || Typ()==Wareneingang || Typ()==Rechnung) && aufid_mem != (*j).AufId())
          break; // Auftrag wechselt
#endif

        if (preise_addieren)
        {
           if ((*j).getPreis().PreisMenge() != preismenge_mem && !Configuration.combine)
              break; // Preismenge wechselt
	   
#ifdef MABELLA_EXTENSIONS
	   {cH_Kunde chk(cH_Kunde(kunden_id)->Rngan());
 	    if(chk->land()->LKZ() == "I" || chk->land()->LKZ()=="IL")
             if (!!(*j).Rabatt()) rabatt_bool=true;

#warning provisory HACK, must be made soon
	     if(chk->Id()==22954 || chk->Id()==23219)
               rabatt_bool=true;
	   }
#else
           if (!!(*j).Rabatt()) rabatt_bool=true;
#endif
        }
        if ((Typ()==Lieferschein || Typ()==Wareneingang) && notice_column_possible)
        { if (!(*j).Text().empty())
          {
#ifdef MABELLA_EXTENSIONS // immer in einer eigenen Zeile Zeigen
	    notice_column_bool=notice_column_possible=false; 
#else
            if ((*j).Text().size()>10) 
            { notice_column_bool=notice_column_possible=false; 
            }
            else notice_column_bool=true;
#endif
          }
        }
        if ((Typ()==Lieferschein || Typ()==Wareneingang) && !palette_bool && (*j).Palette())
          palette_bool=true;
      }
      assert(i!=j);
//******** die Tabelle die wir ausgeben k�nnen endet bei j *****************

     bool neue_seite=false;
//----------------- Seitenumbruch ? -------------------------------
     // 3+1+1 Mindestzeilen bei langer Tabelle, 1 Zeile f. Header angenommen
     if (zeilen_passen_noch<ROWS_FOR_TABLE_HEADER && 
     	zeilen_passen_noch<(j-i)
     	+1 // f�r Kopfzeile
     	+(preise_addieren?1:0) // f�r �bertrag
     	+(lfrsid_drucken?2:0) // f�r Lieferschein X am Y an Z
#ifdef MABELLA_EXTENSIONS
	+(Typ()==Rechnung ? 2:0) // f�r Unsere Auftragsnr.....
#endif
	)
     {   // Tabelle beenden, preis ausgeben
         if (preise_addieren)
         {  drucken_betrag(os,mld->MLT(MultiL_Dict::TXT_UEBERTRAG),betrag);
             os << "\\end{tabularx}\n";
         }
         os << "\\newpage\n";
         ++page_counter;
         page_header(os);
         neue_seite=true;
     }
     else if (preise_addieren && i!=begin()) // da ist noch was offen ...
        {		// bug (?) in tabularx (zu gro�er Abstand) beheben
         if(zwischensumme_drucken)
          {
	   fixedpoint<2> zwb=betrag - zwischensumme_betrag;
	   drucken_betrag(os,mld->MLT(MultiL_Dict::TXT_ZWISCHENSUMME),
					zwb,preisspalte-2); 
                      // Zwischensumme zum Lieferschein
	   zwischensumme_betrag=betrag;
           zwischensumme_drucken=false;
          }
         os << "\\end{tabularx}~\\\\[-1ex]\n";
        }
        

//std::cout << "table ends\n";

//------------------------- Kopf -------------------------
    if (lfrsid_drucken)
    {   cH_Lieferschein l(instanz,lfrsid_mem);
        cH_Kunde k(l->KdNr()); 
        std::string sKunde;


#ifdef MABELLA_EXTENSIONS
	if(!gutschrift())
       	  lieferung_an(os,lfrsid_mem,l->LsDatum(),""); 
#else
        if (k->Id() != kunden_id) sKunde = k->firma();
       	lieferung_an(os,lfrsid_mem,l->getDatum(),sKunde);
#endif
       	lfrsid_drucken=false;
    }
    else 
    os << "~\\\\[-1ex]\n";

#ifdef MABELLA_EXTENSIONS    
    if(aufid_drucken && aufid_mem!=AuftragBase::none_id)
    { //  AuftragBase ab(ppsInstanzID::Kundenauftraege, aufid_mem);
        AuftragBase ab(instanz->Id(), aufid_mem);
    	class Auftrag a(ab);
       	auftrag_von(os,a);
       	aufid_drucken=false;
    }         
#endif
    
     drucken_table_header(os,schema_mem,preismenge_mem, einheit_mem.TeX(*mld));
     
    std::string einheitsize = "\\scriptsize \\,";
    if (neue_seite && preise_addieren)
    {  
     os << "\\multicolumn{" << preisspalte-1 << 
   	"}{r}{"<<mld->MLT(MultiL_Dict::TXT_VORTRAG)<<"} & "<< FormatiereTeX(betrag) <<"\\\\";
       --zeilen_passen_noch;
    }

    for (LR_Abstraktion::const_iterator k=i;k!=j;++k) // Zeilen ausgeben
      {
        AufEintragBase AEB;
        if(Typ()==Intern || Typ()==Extern) AEB=(*k).getAEB();
        std::string YourAuftrag;
        std::vector<LieferscheinEntry::st_AuftragMenge> auftragmenge=(*k).getAuftragsMenge();
        if (auftragmenge.size()==1)
           YourAuftrag=Auftrag::getYourAufNr(auftragmenge[0].ab);
        if (!(*k).getRefOrder().empty())
           YourAuftrag=(*k).getRefOrder();
        std::string orders;
        // collect the orders fulfilled by this delivery
        if (Typ()==Rechnung && order_on_bill)
        { LieferscheinEntry lse((*k).Lfrs());
          std::vector<LieferscheinEntry::st_AuftragMenge> orderlist=lse.getAuftragsMenge();
          for (std::vector<LieferscheinEntry::st_AuftragMenge>::const_iterator l=orderlist.begin();
                l!=orderlist.end();++l)
          { if (!orders.empty()) 
            { orders+=",";
              if (order_on_bill_fixed) orders+=" ";
            }
            if (!l->ab) orders+="*";
            else
            { class Auftrag auftr(l->ab);
              if (!auftr.getYourAufNr().empty()) 
                orders+=auftr.getYourAufNr();
              else orders+=auftr.getAuftragidToStr();
            }
          }
        }
        Zeile_Ausgeben(os,preismenge_mem,einheit_mem,einheitsize,
            (*k).Rest(),(*k).Artikel(),false,(*k).Stueck(),
            (*k).Menge(),(*k).getPreis(true),(*k).getPreis(false),
            (*k).Rabatt(),(*k).getLieferdatum(),(*k).Palette(),YourAuftrag,
	    (*k).getPreisliste(),
            AEB,(*k).Text(),orders);
        if (!(*k).Text().empty() && !notice_column_bool)
        { os << "\\multicolumn{" << spaltenzahl << "}{l}"
             "{\\hspace*{1cm}\\begin{minipage}{16.5cm}" << string2TeX((*k).Text()) 
             << "\\end{minipage}}\\\\\n";
          zeilen_passen_noch-=(*k).Text().size()/60+1;
        }
        if(auftragmenge.size()>1) 
         { 
#if 0 // nicer layout (?)
           std::string auftraege;
           for(std::vector<LieferscheinEntry::st_AuftragMenge>::const_iterator 
           	l=auftragmenge.begin();l!=auftragmenge.end();++l)
           { if (!auftraege.empty()) auftraege+=" und ";
             auftraege+=FormatiereTeX_short(l->menge)+Einheit((*k).Artikel()).TeX(*mld);
             auftraege+="~in ";
             if (!l->ab) auftraege+="keinen Auftrag";
             else auftraege+=string2TeX(Auftrag::getYourAufNr(l->ab));
           }
           os << "\\multicolumn{"<<(spaltenzahl)<<"}{r}"
             "{\\begin{minipage}{16.5cm}\\begin{flushright}hiervon fallen "
             << auftraege << "\\end{flushright}\\end{minipage}}\\\\\n";
           zeilen_passen_noch-=auftraege.size()/60+1;
#else         
           for(std::vector<LieferscheinEntry::st_AuftragMenge>::const_iterator 
           	l=auftragmenge.begin();l!=auftragmenge.end();++l)
            {
              Zeile_Ausgeben(os,preismenge_mem,einheit_mem,einheitsize,
               0,(*k).Artikel(),true,einheit_mem.hatMenge()?1:l->menge.as_int(),
               einheit_mem.hatMenge()?l->menge:0,Preis(),Preis(),
               0,ManuProC::Datum(),0,Auftrag::getYourAufNr(l->ab),
		(*k).getPreisliste(), AEB,std::string(),std::string());
            }
#endif
         }
      }

#if 0  // Tabellensumme (needs more work, but I don't want to delete it)
   if (preise_addieren)
      drucken_betrag(os,"",betrag); // oder Tabellenbetrag
#endif    
   if (!preise_addieren) os << "\\end{tabularx}\n\n";
   i=j;
  }


#ifdef MABELLA_EXTENSIONS
  if(preisspalte==0 && Typ()==Rechnung) // Mabella mu� auch NULL Rechnung drucken
  	{Einheit e(Einheit::default_id);
  	 stueck_bool=true;
         drucken_table_header(os,schema_own, 1, e.TeX(*mld));
	}
#endif	

  cH_Kunde kunde_rng(KdNr());
#if defined(MABELLA_EXTENSIONS) && defined(HAS_ADDR_GROUP_Rechnungsadresse)
  if(! kunde_rng->isInGrp(KundengruppeID::Rechnungsadresse))
    kunde_rng = cH_Kunde(kunde_rng->Rngan());
#endif


  if (preise_addieren) 
  //*********************** Summe bilden *********************************
   {
    
#warning Sch�tzwert: Zeilen fuer Summe ben�tigt   

   if((Typ() == Rechnung) && 
#ifdef HAS_ADDR_GROUP_Zwischensumme
      (kunde_rng->isInGrp(KundengruppeID::Zwischensumme))
#else
      false
#endif
      )
     {fixedpoint<2> zwb=betrag - zwischensumme_betrag;
      drucken_betrag(os,mld->MLT(MultiL_Dict::TXT_ZWISCHENSUMME),
					zwb,preisspalte-2);  
     }

#ifdef MABELLA_EXTENSIONS
     if (zeilen_passen_noch<4)
#else
     if (zeilen_passen_noch<8)
#endif     
     {   // Tabelle beenden, preis ausgeben
         drucken_betrag(os,mld->MLT(MultiL_Dict::TXT_SUMME),betrag);
	 os << "\\end{tabularx}\n";
         os << "\\newpage\n";
         ++page_counter;
         page_header(os);
         os << "\\begin{tabularx}{" << TABW << "cm}{rXrr}\n";
         spaltenzahl=4;
         preisspalte=spaltenzahl;
         zur_preisspalte="&&";
         os << zur_preisspalte << mld->MLT(MultiL_Dict::TXT_SUMME)<<" & "<< FormatiereTeX(betrag) <<"\\\\";
         --zeilen_passen_noch;
     }
     else 
     {  
       drucken_betrag(os,mld->MLT(MultiL_Dict::TXT_SUMME),betrag);
     }
     
  calc_all(kunde_rng,cH_Kunde(KdNr())->MwSt());     
     
//     os << zur_preisspalte << "Endsumme & "<< FormatiereTeX(betrag) <<"\\\\""\n";
//     --zeilen_passen_noch;
     

#ifdef MABELLA_EXTENSIONS

//     if(!kunde_rng->zeilenrabatt())
     if(Rabatt()!=0.0)
     {
#endif
     fixedpoint<2> kunden_rabatt = Rabatt();
     if (kunden_rabatt>0.0)
      {
        fixedpoint<2> betrag_rabatt_ = betrag*kunden_rabatt/100.0;
        os << zur_preisspalte << "./. "
        	<< FormatiereTeX_short(kunden_rabatt)<<"\\% & "
        	<< FormatiereTeX(betrag_rabatt_) <<"\\\\";
        --zeilen_passen_noch;
        betrag-=betrag_rabatt_;
        drucken_betrag(os,"",betrag);
      }
     else if (kunden_rabatt<0.0)
      {
        fixedpoint<2> betrag_aufschlag_ = betrag*(-kunden_rabatt)/100.0;
        os << zur_preisspalte << "Aufschlag "
        	<< FormatiereTeX_short(-kunden_rabatt)<<"\\% & "
        	<< FormatiereTeX(betrag_aufschlag_) <<"\\\\";
        betrag+=betrag_aufschlag_;
        --zeilen_passen_noch;
        drucken_betrag(os,"",betrag);
      }

#ifdef MABELLA_EXTENSIONS
	}
#endif

   }
      
#ifdef MABELLA_EXTENSIONS
  if(Typ() == Rechnung || Typ()==Auftrag)
  {
  if(ents_flag)
   {os << "\\multicolumn{" << preisspalte-1 << 
   	"}{r}{Gesetzlicher Entsorgungskosten-Anteil VfW Reflora von "
	<< FormatiereTeX(fixedpoint<1>(ENTSSATZ*100.0)) << "\\%}"
   	"& " << FormatiereTeX(entskosten) << "\\\\\n";
    --zeilen_passen_noch;
    os << "\\cline{"<<preisspalte<<"-"<<preisspalte<<"}\n";
    os << zur_preisspalte << "&"+FormatiereTeX(entsbetrag)+"\\\\\n";
    --zeilen_passen_noch;
   }
  }
#endif   
   
 if (preise_addieren)
 { cH_Kunde kunde_an(KdNr());
//   if(kunde_rng->MwSt())   
   if(kunde_an->MwSt())   
   { os << zur_preisspalte.substr(0,zur_preisspalte.size()-1) << "\\multicolumn{2}{r}{+ "<<MWSTPROZ<<"\\% "
		<< mld->MLT(MultiL_Dict::MWST) <<"}& "<< FormatiereTeX(mwstbetrag) <<"\\\\"
     	"\\cline{"<<preisspalte<<"-"<<preisspalte<<"}\n";  
     --zeilen_passen_noch;
     os << zur_preisspalte
           << getWaehrung()->TeXsymbol() << '&' << FormatiereTeX(bruttobetrag)<<"\\\\";
   }
     os << "\\cline{"<<preisspalte<<"-"<<preisspalte<<"}\\\\[-2.5ex]";
     os << "\\cline{"<<preisspalte<<"-"<<preisspalte<<"}\\\\\n";
     --zeilen_passen_noch;

     cP_Waehrung andere_waehrung=getWaehrung();
#ifdef MABELLA_EXTENSIONS
     bool umrechnen=false;
#else
     bool umrechnen=andere_waehrung != cP_Waehrung(WaehrungID::EUR);
#endif

     if (umrechnen) 
     {  andere_waehrung = cP_Waehrung(WaehrungID::EUR);
        fixedpoint<2> anderer_betrag
              = betrag*Waehrung::Umrechnung(*getWaehrung(),*andere_waehrung);
      
        os << andere_waehrung->TeXsymbol()<<"~"<< FormatiereTeX( anderer_betrag );
     	  os << "& \\multicolumn{"<<preisspalte-1<<"}{l}{for information purposes only}";
        os << "\\\\\\cline{1-1}\\\\[-2.5ex]";
        os << "\\cline{1-1}\n";
     }
   }
//  os << "\\end{flushright}\n";

//if(Typ()!=Lieferschein && Typ()!=Intern)
  if(preise_addieren)
     os << "\\end{tabularx}\n\n";
  drucken_footer(os);
}




void LR_Abstraktion::check_page_break(std::ostream &os)
{
//---------------------- Seitenumbruch innerhalb einer Tabelle ----------      
        if (zeilen_passen_noch<=1)
          {
            if (preise_addieren)
            {  drucken_betrag(os,mld->MLT(MultiL_Dict::TXT_UEBERTRAG),betrag);
            }
   	    os << "\\end{tabularx}\n"
            	 "\\newpage\n";
            ++page_counter;
            page_header(os);
	    drucken_table_header(os,schema_mem,preismenge_mem,einheit_mem.TeX(*mld));
            if (preise_addieren)
            { os << "\\multicolumn{" << preisspalte-1 << 
   			"}{r}{"<<mld->MLT(MultiL_Dict::TXT_VORTRAG)<<"} & "<< FormatiereTeX(betrag) <<"\\\\";
//            os << zur_preisspalte << mld->MLT(MultiL_Dict::TXT_VORTRAG)<<" & "<< FormatiereTeX(betrag) <<"\\\\";
               --zeilen_passen_noch;
            }
          }
}


// CP: TODO: Diese ganzen Argumente sind idiotisch, das sollte neu
// ueberarbeitet werden!

//////////////////////////////////////////////////////////////////////////////
void LR_Abstraktion::Zeile_Ausgeben(std::ostream &os,
   const Preis::preismenge_t &preismenge_mem,
   const Einheit &einheit_mem,const std::string &einheitsize,
   const AuftragBase::mengen_t rest,const ArtikelBase &artikelbase,
   const bool zusatzinfo,const int stueck, const LieferscheinBase::mengen_t &menge,
   const Preis &BruttoPreis, const Preis &NettoPreis,
   const AuftragBase::rabatt_t &rabatt, const ManuProC::Datum &lieferdatum,
   const int palette, const std::string &your_auftrag,
   const cH_PreisListe pl,
   const AufEintragBase AEB, const std::string &notice,
   const std::string &orders)
{
#ifdef MABELLA_EXTENSIONS // gelieferte Zeilen nicht anzeigen beim R�ckstand     
       if(Rueckstand() && rest==0) return;
#endif

check_page_break(os);

//************************* eine Zeile ausgeben ***************************
     	 std::string linecolor;

     	 bool erste_spalte=true;
     	 
#ifdef MABELLA_EXTENSIONS
	if(zeilen_passen_noch%2) os << "\\shaderow{}";
#endif
#ifndef PETIG_EXTENSIONS
         if (zusatzinfo) linecolor = "\\mygray{}";
#endif
         
         if (stueck_bool)
         {  neue_spalte(erste_spalte,os);
            if ((!zusatzinfo || !menge_bool))
            {  os <<linecolor ;

		         if(Rueckstand())
              		os << FormatiereTeX(rest);
         		else
              		os << FormatiereTeX((unsigned long)stueck);

               std::string einheit;

//             if(Typ()==Rechnung || Typ()==Lieferschein) einheit=Einheit(artikelbase).StueckEinheit_TeX();
//               else 

	       if(menge_bool)
	          einheit=Einheit(artikelbase).StueckEinheit_TeX();
	  	else
	          einheit=Einheit(artikelbase).TeX(*mld);

               if (einheit.size()) 
                  os <<'{'<< einheitsize <<einheit <<'}';
            }
         }
         if (menge_bool) 
            {  neue_spalte(erste_spalte,os);
               if (!zusatzinfo || !stueck_bool) 
                    os <<linecolor<< FormatiereTeX_short(menge)<<einheitsize <<Einheit(artikelbase).MengenEinheit_TeX() ;
            }
         if (stueck_bool && menge_bool)
           { neue_spalte(erste_spalte,os);
             // diese Hartcodierung von 3 sieht nicht gut aus CP
//             os <<linecolor<< FormatiereTeX_short(fixedpoint<3>(stueck * menge))<<einheitsize <<Einheit(artikelbase).TeX();
             os <<linecolor<< FormatiereTeX_short(stueck * menge)<<
                  einheitsize << 
#ifdef MABELLA_EXTENSIONS // ich glaube da an einen Bug ... CP             
			Einheit(artikelbase).MengenEinheit_TeX();
#else
                        Einheit(artikelbase).TeX(*mld);
#endif
           }

// NEW COLUMNS; ab hier irgendwo einf�gen um Tabellenspalten auszugeben

         cH_ArtikelBezeichnung bez(artikelbase,schema_mem->Id());
         drucken_artikel(os,bez,zusatzinfo,linecolor,erste_spalte,schema_mem,0,Typ()==Intern);
         
#ifdef MABELLA_EXTENSIONS
	if(schema_own->Id() != schema_mem->Id())
	  {
	   cH_ArtikelBezeichnung own_bez(artikelbase,schema_own->Id());
	   if((!ean_code && !rabatt_bool) || Typ()==Lieferschein || Typ()==Wareneingang)
	     drucken_artikel(os,own_bez,false,linecolor,erste_spalte,schema_own);
	   else // nur Auff�llen; die Daten kommen in die zweite Spalte
	     {for(int signc=0;signc<schema_own->size(1);signc++)
	       os << "&"; 
	     }
	  }
#endif         
         
         
         if(Typ()==Intern)
           {  
             AufEintragZu::list_t L;
             for(int run=0;run<2;++run)
              {   
               //Rohware
               if(run==0)  L=AufEintragZu::get_Referenz_list(AEB,AufEintragZu::list_kinder,AufEintragZu::list_Artikel);
               //_for_geplant(true); 
               //geplant f�r
               if(run==1) L =AufEintragZu::get_Referenz_list(AEB,AufEintragZu::list_eltern,false); //AufEintragZu::list_Artikel);
               //_for_geplant(false);
               neue_spalte(erste_spalte,os);
               if(L.size()>1) os << "$\\left\\{\\mbox{" ;
               os << "\\begin{tabular}{ll}  ";
#ifdef PETIG_EXTENSIONS
               for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();)
                {
                   ArtikelBase artbase(AufEintrag(i->AEB).ArtId());
                  cH_ArtikelBezeichnung bez2(artbase);
                  bool btmp=true; // in dieser Tabelle ist es IMMER die 'erste_spalte'
                  AuftragBase::mengen_t menge=0;
                  if(run==1 && L.size()>1) menge=i->Menge;
                  drucken_artikel(os,bez2,zusatzinfo,linecolor,btmp,schema_mem,menge,true);
                  ++i;
                  if(i!=L.end()) os << "\\\\ ";
                }
#endif
               os << "\\end{tabular}\n";
               if(L.size()>1) os << "}\\right\\}$" ;
              }
            }
           
        if (Typ()==Rechnung && order_on_bill)
        { neue_spalte(erste_spalte,os);
          os << linecolor << orders;
        }
         if (preise_addieren)       
          { 
           neue_spalte(erste_spalte,os);
#ifdef MABELLA_EXTENSIONS // Anzeigen, dass der Preis manuell eingegeben wurde
	    if(Typ()==Auftrag && Configuration.preview_only)
	      if(pl->Id() == PreisListe::none_id)
	        os << "{\\color{altgray}(M) }";
#endif
            if (rabatt_bool) 
              {os <<linecolor<<FormatiereTeX_Preis((BruttoPreis.Wert()) );
               neue_spalte(erste_spalte,os); os << linecolor <<
			FormatiereTeX(rabatt); 
               neue_spalte(erste_spalte,os); os << linecolor<<
			FormatiereTeX_Preis((NettoPreis.Wert())); 
	      }
	    else if (!!NettoPreis) // nur wenn != 0
	     {
	      os <<linecolor<<FormatiereTeX_Preis(NettoPreis.Wert());
	      if (Configuration.combine)
	      { os << einheitsize << getWaehrung()->TeXsymbol() << "/";
                if (NettoPreis.PreisMenge()!=1) os << Formatiere_short(NettoPreis.PreisMenge()) << "\\,";
                std::string einheit=Einheit(artikelbase).TeX(*mld);
                if (!einheit.empty()) os << einheit;
	      }
	     }

	    Preis::geldbetrag_out preis;
	    if(Rueckstand())
//            	preis = BruttoPreis.Gesamtpreis(getWaehrung(),rest.as_int(),menge,rabatt);
            	preis = NettoPreis.Gesamtpreis(getWaehrung(),
				rest.as_int(),menge);
	    else
//            	preis = BruttoPreis.Gesamtpreis(getWaehrung(),stueck,menge,rabatt);
            	preis = NettoPreis.Gesamtpreis(getWaehrung(),
				stueck,menge);

            betrag+=preis;
            tabellenbetrag+=preis;
            neue_spalte(erste_spalte,os);
            if (!!preis) os <<linecolor<< FormatiereTeX(preis);
          }
          

	if((Typ() == Auftrag) || (Typ()==Extern))
	  {
	    Kalenderwoche kw(lieferdatum.KW());
	    char jahr[3];
	    snprintf(jahr,3,"%02d",kw.Jahr()%100);
	    std::string kws = kw.valid() ? itos(kw.Woche())+"'"+jahr : "-";
#ifdef HAS_ADDR_GROUP_Datum_im_Auftrag
            if (cH_Kunde(KdNr())->isInGrp(KundengruppeID::Datum_im_Auftrag))
              kws=lieferdatum.write_euro();
#endif

	    if(Typ()==Extern
#ifdef PETIG_EXTENSIONS
                                || Typ() == Auftrag
#endif	    
                  	                                          )
            {
	      neue_spalte(erste_spalte,os);
	      os << linecolor << kws;
            }

	    if(!min_liefdatum.valid()) 
		{min_liefdatum=lieferdatum;
		 min_KWStr=kws;
		}
	    else
	      if(min_liefdatum>lieferdatum)
		{min_liefdatum=lieferdatum;
		 min_KWStr=kws;	
		}
	  }
#ifdef HAS_ADDR_GROUP_Kundenauftrag_bei_Lieferanten
        if (Typ()==Extern && cH_Kunde(KdNr())->isInGrp(KundengruppeID::Kundenauftrag_bei_Lieferanten))
        { neue_spalte(erste_spalte,os);
std::cerr << "AEB " << AEB << '\n';
          AufEintragZu::list_t L=AufEintragZu(AEB).get_Referenz_listFull(AufEintragZu::list_eltern);
          std::set<AufEintragBase> L2;
          for (AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
          { L2.insert(i->AEB);
std::cerr << i->AEB << '\n';
          }
          std::set<std::string> auftraege;
          for (std::set<AufEintragBase>::const_iterator i=L2.begin();i!=L2.end();++i)
          { if (i->Id()>=AuftragBase::handplan_id)
            { class Auftrag a=*i;
              // intern
              if (a.getKundennr()==Kunde::default_id) continue;
              std::string text=cH_Kunde(a.getKundennr())->getSortName()+":"
                  +a.getYourAufNr()+" ";
              // AufEintrag ae(AEB);
              AufEintrag ae2(*i);
              text+=cH_ArtikelBezeichnung(ae2.Artikel())->Bezeichnung()+" ";
              auftraege.insert(text);
            }
          }
          for (std::set<std::string>::const_iterator i=auftraege.begin();i!=auftraege.end();++i)
            os << (*i) << ' ';
        }
#endif

#ifdef PETIG_EXTENSIONS
        if (Typ()==Lieferschein || Typ()==Wareneingang) 
          { if (palette_bool)
            { neue_spalte(erste_spalte,os);
              if (palette!=0) os << linecolor<<palette;
            }
            neue_spalte(erste_spalte,os);
            os << linecolor << your_auftrag;
            if (notice_column_bool)
            { neue_spalte(erste_spalte,os);
              os << linecolor<< notice;
            }
          }
#endif

        os<<"\\\\\n";
        --zeilen_passen_noch;

#ifdef MABELLA_EXTENSIONS
	if(schema_own->Id() != schema_mem->Id() && 
	    (ean_code || rabatt_bool) && Typ()!=Lieferschein && Typ()!=Wareneingang)
	  {
	   cH_ArtikelBezeichnung own_bez(artikelbase,schema_own->Id());
	   os << "&&&&";
           if(schema_own->Id()==own_bez->getExtBezSchema()->Id())	     
	     os << "\\multicolumn{4}{l}{"<<string2TeX(own_bez->Bezeichnung())<<"}";
           else
	     os << "\\multicolumn{4}{l}{ }";
	   os << "\\\\\n";
           --zeilen_passen_noch;
	  }
#endif         

if(Configuration.show_article_tree)
  {ArtikelBaum ab(artikelbase,true);
   ArtikelTyp at(artikelbase);
   int tiefe=0;
   if(!ab.empty())
     if(at.Id() != ArtikelTyp::default_ID)
       Zeige_RohArtikel(os,tiefe,ab,pl);
  }


//std::cout << "noch passende Zeilen" << zeilen_passen_noch <<"\n";
}


void LR_Abstraktion::Zeige_RohArtikel(std::ostream &os,int tiefe,
ArtikelBaum &ab, const cH_PreisListe pl)
{

 ArtikelBaum::const_iterator i;
 for(i=ab.begin(); i!=ab.end(); ++i)
   {
    ArtikelBaum chd((*i).rohartikel,true);
    ArtikelTyp at(chd);
    std::string gray="\\mygray{} ";
    if(!chd.empty() && (at.Id() != ArtikelTyp::default_ID)) 
      gray=" ";

    check_page_break(os);

    static int multi_col=3;

    cH_ArtikelBezeichnung bez((*i).rohartikel,schema_mem->Id());

    cH_ExtBezSchema chd_sch = bez->getExtBezSchema();

    bool first_col=false;

    std::string einheitsize = "\\scriptsize \\,";
    os << gray << " \\small \\`{a} "<<(*i).menge<<"{\\scriptsize " <<
	Einheit((*i).rohartikel).TeX(*mld) <<"}";
    neue_spalte(first_col,os);
    os << "\\multicolumn{"<<multi_col<<"}{l}{"<<gray<<" \\small "<<string2TeX(bez->Bezeichnung())<<"}";

   neue_spalte(first_col,os);
   os << "\\mbox{" << gray <<" {\\small ";
#ifdef HAS_WG_ATTRIB_mit_ean
   if(ArtikelTyp::hasAttribute(chd_sch->Typ(),ArtikelTypAttr::mit_ean))
		os << bez->Bezeichnung(EAN_SIGNIFIKANZ)<<", ";
#endif
#ifdef HAS_WG_ATTRIB_mit_bezeichnung
   if(ArtikelTyp::hasAttribute(chd_sch->Typ(),ArtikelTypAttr::mit_bezeichnung))
	os << string2TeX(bez->Bezeichnung(BEZEICHNUNG_SIGNIFIKANZ));
#endif
   os << "}}";

   neue_spalte(first_col,os);

   if(pl->Id()!=PreisListe::none_id)
     {Artikelpreis ap(pl,(*i).rohartikel,(*i).menge.as_int());
      Preis::geldbetrag_out preis=ap.Wert();
      os<<"\\mbox{"<<gray<<" \\small "<<FormatiereTeX(preis)<<"}";
     }
   else os << "&";

   os << "&&&" << "\\\\\n";

    --zeilen_passen_noch;

    if(!chd.empty())
      if(at.Id() != ArtikelTyp::default_ID)
        Zeige_RohArtikel(os,++tiefe,chd,pl);
   }
}




void LR_Abstraktion::drucken_artikel(std::ostream &os,cH_ArtikelBezeichnung bez,
            bool zusatzinfo,std::string linecolor,bool &erste_spalte,
            cH_ExtBezSchema s, AuftragBase::mengen_t menge, bool combine)
{

   if (combine || Configuration.combine)
   {  neue_spalte( erste_spalte, os);
      if (!zusatzinfo)
        os <<"{"<<linecolor<< Gtk2TeX::string2TeX(bez->Bezeichnung()) ;
      if(menge!=0) os << " }&{"<<linecolor<<"("<<menge<<")";
      os <<"}";
   }
   else
	 for(ExtBezSchema::const_sigiterator l=s->sigbegin(signifikanz);l!=s->sigend(signifikanz);++l)
	   { neue_spalte( erste_spalte, os);
	     if (!zusatzinfo)
		{
#ifdef MABELLA_EXTENSIONS
		// Wenn im fremden Schema die Bezeichnung nicht existiert,
		// dann nichts ausgeben, damit es gleich auff�llt.
		if(s->Id()!=bez->getExtBezSchema()->Id())	     
	          os << " ";
	        else
#endif
	          os <<"{"<<linecolor<< Gtk2TeX::string2TeX((*bez)[l->bezkomptype]->getStrVal()) <<"}";
		}

#ifdef PETIG_EXTENSIONS // lieber zuwenige Zeilen als Umbruch riskieren
	     if ((*bez)[l->bezkomptype]->getStrVal().size()>31) --zeilen_passen_noch;
#endif	        
	   }

#ifdef MABELLA_EXTENSIONS
	if(s->Id()==ExtBezSchema::default_id && ean_code)
     	  { neue_spalte( erste_spalte, os);
	    if(ArtikelTyp::hasAttribute(s->Typ(),ArtikelTypAttr::mit_ean))
	        os <<bez->Bezeichnung(EAN_SIGNIFIKANZ);
     	  }
	if(s->Id()==ExtBezSchema::default_id)
     	   {
	    if(ArtikelTyp::hasAttribute(s->Typ(),
				ArtikelTypAttr::mit_bezeichnung))
	      { neue_spalte( erste_spalte, os);
		  os << string2TeX(bez->Bezeichnung(BEZEICHNUNG_SIGNIFIKANZ));
	      }
	   }
#endif 
	   
}



void LR_Abstraktion::drucken_table_header(std::ostream &os,
   	const cH_ExtBezSchema &schema, 
   	fixedpoint<2> preismenge, const std::string &preiseinheit)
{
#ifdef MABELLA_EXTENSIONS
  std::string ug ="\\scriptsize{}"; // Gr��e der �berschriften
  std::string sg = ug;
#else
  std::string ug ="\\footnotesize{}"; // Gr��e der �berschriften
  std::string sg ="\\scriptsize{}"; // Gr��e der �berschriften
  
  if(Typ()!=Extern) { ug += "\\mygray{}"; sg += "\\mygray{}"; }
#endif

  std::string ueberschriften;

  std::string tabcolumn;
  spaltenzahl=0;
  preisspalte=0;

// NEW COLUMNS; ab hier irgendwo einf�gen um spaltenzahl zu setzten

   if (stueck_bool || menge_bool)
   {

    if(Rueckstand())
	ueberschriften="\\multicolumn{1}{c}{"+ug+mld->MLT(MultiL_Dict::TXT_OFFEN)+"}";
    else ueberschriften="\\multicolumn{1}{c}{"+ug+mld->MLT(MultiL_Dict::TXT_MENGE)+"}";
    tabcolumn+="r"; spaltenzahl++;

   }   
   if (menge_bool && stueck_bool) 
   { tabcolumn+="r"; spaltenzahl++;
     ueberschriften="\\multicolumn{2}{c}{"+ug+mld->MLT(MultiL_Dict::TXT_MENGE)+"}";
   
     tabcolumn+="r"; spaltenzahl++; 
     ueberschriften+="&\\multicolumn{1}{c}{"+sg+"Gesamtmenge}";
   }
  
  if(Typ()==Intern)
     { tabcolumn += "ccc"; spaltenzahl+=3;
       ueberschriften+= "&\\mbox{"+ug+"Artikel}";
       ueberschriften+= "&\\mbox{"+ug+"Rohware}";
       ueberschriften+= "&\\mbox{"+ug+"geplant f�r}";
     }
  else if (Configuration.combine) // || Typ()==Extern)
     { tabcolumn += "X"; spaltenzahl++; 
       ueberschriften+= "&\\mbox{"+ug+"Artikel}";
     }
  else
#ifdef MABELLA_EXTENSIONS
     {
     for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
         { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; 
           ueberschriften += "&\\mbox{"+ug +mld->MLT(j->textid)+"}";
         }
     }
#else
     for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
         { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; 
           ueberschriften += "&\\mbox{"+ug + j->bezkomptext+"}";
         }
#endif


#ifdef MABELLA_EXTENSIONS
  if(ean_code)
   {
    ExtBezSchema::const_sigiterator j=schema_mem->sigbegin(EAN_SIGNIFIKANZ);
    if(j!=schema_mem->sigend(EAN_SIGNIFIKANZ))
     {for(;j!=schema_mem->sigend(EAN_SIGNIFIKANZ);++j)
       { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; 
        ueberschriften += "&\\mbox{"+ug + j->bezkomptext+"}";
       }}
    else
       { tabcolumn += "l" ; ++spaltenzahl ; 
        ueberschriften += "&\\mbox{"+ug +"}";
       }
   }
  for(ExtBezSchema::const_sigiterator j=schema_mem->sigbegin(BEZEICHNUNG_SIGNIFIKANZ);
  			j!=schema_mem->sigend(BEZEICHNUNG_SIGNIFIKANZ);++j)
      { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; 
        ueberschriften += "&\\mbox{"+ug +mld->MLT(MultiL_Dict::TXT_BEZEICHNUNG)+"}";
      }

	// Wenn eigene Bezeichnung aber kein EAN und kein Rabatt dann nur in einer Zeile
	// auch beim Lieferschein nur eine Zeile
  if(schema_mem->Id() != schema_own->Id())
    {
     for(ExtBezSchema::const_sigiterator j=schema_own->sigbegin(1);
     				j!=schema_own->sigend(1);++j)
      { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; 
        if((!ean_code && !rabatt_bool) || Typ()==Lieferschein || Typ()==Wareneingang)
//          ueberschriften += "&\\mbox{"+ug + j->bezkomptext+"}";
	 {if(j->textid==72)
           ueberschriften += "&\\mbox{"+ug+"Votre Barcode}";       
          else
           ueberschriften += "&\\mbox{"+ug+mld->MLT(MultiL_Dict::IHR_ARTIKEL)+"}";
	 }
	else
          ueberschriften += "&";
      }    

   }
      
#else
  if (Typ()==Lieferschein || Typ()==Wareneingang)
  { if (palette_bool)
    { tabcolumn+="r"; spaltenzahl++; 
      ueberschriften += "&\\multicolumn{1}{c}{"+sg+"Palette}";
    }
    tabcolumn+="r"; spaltenzahl++; 
    ueberschriften += "&\\multicolumn{1}{c}{"+ug+"Auftrag}";
    if (notice_column_bool)
    { tabcolumn+="l"; spaltenzahl++; 
      ueberschriften += "&\\multicolumn{1}{c}{"+ug+"Anmerkung}";
    }
  }
#endif
  if (order_on_bill)
  { if (!order_on_bill_fixed) tabcolumn+="l"; 
    else tabcolumn+="p{4cm}";
    spaltenzahl++; 
    ueberschriften += "&\\multicolumn{1}{l}{"+ug+"Auftrag}";
  }

  if (preise_addieren)  // Einzelpreis, Gesamtpreis
  { 
    tabcolumn+="rr"; spaltenzahl+=2; preisspalte=spaltenzahl;
    if (!Configuration.combine)
    { ueberschriften += "&\\multicolumn{1}{c}{"+sg+getWaehrung()->TeXsymbol();
      if(rabatt_bool) ueberschriften +=" "+mld->MLT(MultiL_Dict::TXT_BRUTTO);
      if (preismenge!=1 || !preiseinheit.empty()) ueberschriften += "\\,/";
      if (preismenge!=1) ueberschriften += "\\,"+ Formatiere_short(preismenge);
      if (!preiseinheit.empty())  ueberschriften += "\\," + preiseinheit;
      ueberschriften +="} ";   	// Einzelpreis brutto
    }
    else ueberschriften += "&"+sg+"Preis";
     
    if (rabatt_bool)  
      {  tabcolumn+="r"; ++spaltenzahl; // Rabatt
         ueberschriften += "&\\multicolumn{1}{c}{"+ug+"\\%\\,"+mld->MLT(MultiL_Dict::TXT_RABATT)+"}";
         tabcolumn+="r"; ++spaltenzahl;  // Einzelpreis netto
         ueberschriften += "&\\multicolumn{1}{c}{"+
		ug+getWaehrung()->TeXsymbol()+" "+mld->MLT(MultiL_Dict::TXT_NETTO);
	 if (!preiseinheit.empty())  ueberschriften += "\\,/\\," + preiseinheit;
	ueberschriften+="}";
      }     
     
#ifndef MABELLA_EXTENSIONS
     ueberschriften += "&\\multicolumn{1}{c}{"+ug+"{Gesamtpreis}}";
#else     
     ueberschriften += "&\\multicolumn{1}{c}{"+sg+"{"+ 
		getWaehrung()->TeXsymbol()+" "+mld->MLT(MultiL_Dict::TXT_WERT)+"}}";
#endif
  }

#ifndef MABELLA_EXTENSIONS
  if((Typ()==Auftrag) || (Typ()==Extern))
#else
  if((Typ()==Extern))
#endif
  { tabcolumn+="r"; spaltenzahl+=1; ueberschriften +=  "&{"+sg
			+mld->MLT(MultiL_Dict::TXT_LIEFERKW)+"}"; }

#ifdef HAS_ADDR_GROUP_Kundenauftrag_bei_Lieferanten
   if (Typ()==Extern && cH_Kunde(KdNr())->isInGrp(KundengruppeID::Kundenauftrag_bei_Lieferanten))
   { tabcolumn+="p{4cm}"; spaltenzahl++; 
     ueberschriften += "&\\multicolumn{1}{c}{"+sg+"f�r Auftrag}";
   }
#endif
  
//  os << "\\settowidth{\\breite}{"<<ug<<" "<<mld->MLT(MultiL_Dict::TXT_BEZEICHNUNG)<<"}%\n";
#ifdef MABELLA_EXTENSIONS
  os << "\\setlength{\\tabcolsep}{1.5mm}";
#endif
  os << "\\begin{tabularx}{" << TABW << "cm}{"<<tabcolumn<<"}"<<"\\\\\n";

  os << ueberschriften;
  --zeilen_passen_noch;

  if(preise_addieren && preisspalte>=2)
    {zur_preisspalte="";
     if(rabatt_bool) preisspalte+=2;
     for (unsigned int i=0;i<preisspalte-2;++i)  zur_preisspalte+='&';
    }

// ggf. Zweite Kopfzeile erzeugen
  if(schema_mem->Id() != schema_own->Id() && 
	(ean_code || rabatt_bool) && Typ()!=Lieferschein)
    {
     os << "\\\\[-1ex]&\\multicolumn{3}{l}{\\scriptsize{}}";
     os << "&\\multicolumn{4}{l}{\\scriptsize{";
     for(ExtBezSchema::const_sigiterator j=schema_own->sigbegin(1);
     				j!=schema_own->sigend(1);++j)
//       os << " " << j->bezkomptext << " " << j->separator;
       os << " " << mld->MLT(MultiL_Dict::IHR_ARTIKEL) << " " << j->separator;
     os << "}}\\\\\n";
     --zeilen_passen_noch;
    }
  else os << "\\\\\n";

  os << "\\hline\n";


  tabellenbetrag=0;
}


void LR_Abstraktion::neue_spalte(bool& erste_spalte, std::ostream &os)
{
// f�r die erste Spalte braucht man kein "&", 
// nach der letzten darf keines stehen ... daher dies
  if (!erste_spalte)  os << " & ";
  else erste_spalte = false;
}

void LR_Abstraktion::drucken_betrag(std::ostream &os,const std::string &text, 
			fixedpoint<2> betrag, int preiscolumn)
{
int prcl = preiscolumn == 0 ? preisspalte : preiscolumn;

  os << "\\cline{"<<prcl<<"-"<< (prcl<preisspalte ? preisspalte : prcl) <<"}"<<"\n";
  os << "\\multicolumn{" << prcl-1 << 
   	"}{r}{"<<text<<" }";  
//  os << zur_preisspalte << text << " " ;
#ifndef MABELLA_EXTENSIONS // also ich brauch das nicht ...  
  if (text.empty()) 
#endif
     os << getWaehrung()->TeXsymbol() ;
  os << '&' << FormatiereTeX(betrag)<<"\\\\\n";
  --zeilen_passen_noch;
}


void LR_Abstraktion::page_header(std::ostream &os)
{
 cH_Kunde kunde_von(Kunde::default_id);

 if(Typ()==LR_Base::Intern)
  {
   os <<"\\begin{flushleft}\n";
   os <<"Auftrag " << RngNr() << " an "<<instanz->get_Name()<<"\\hfill "<<
         getDatum()<<"\\\\\n";
   os <<"\\end{flushleft}\n";
   zeilen_passen_noch=ZEILEN_SEITE_N;
  }
// else if(Typ()==LR_Base::Extern)
 else if(false)
  {
   cH_Kunde kunde_an(KdNr());

   if (page_counter==1) // 1. Seite
    {
      os << kunde_an->LaTeX_an(Typ()==Lieferschein,TEL_FAX,"1\\textwidth");
      os << "\\hfill "<<kunde_von->LaTeX_von_gross(KdNr())<<"\n\n"  ;
      zeilen_passen_noch=ZEILEN_SEITE_1;
    }
   os << "\\LARGE "<<typString()<<' '<<RngNr()<<"\\hfill\\normalsize "
       "\\ifthenelse{\\kopie>1}{Kopie, }{}"
       "\\hfill "<< getDatum() << "\\\\\n\n";
   os <<string2TeX(getBemerkung())<<"\n\n";
  }
 else
  {
   cH_Kunde kunde_an(KdNr());
   cH_Kunde kunde_rng(kunde_an->Rngan());

   os <<"\\begin{flushleft}\n";
   if (page_counter==1) // 1. Seite
    {
     cH_Kunde kunde_von(Kunde::default_id);

#ifdef MABELLA_EXTENSIONS
	os << "\\raisebox{-1.5cm}[0pt][5cm]{\\begin{minipage}[t]{0.6\\linewidth}\n"
	   << "\\begin{flushleft}\n";
#endif
     os << kunde_von->LaTeX_von()<<"\n\n"  ;
     TelArt telart=TEL_NONE;

#ifdef PETIG_EXTENSIONS
     if(Typ()==Lieferschein || Typ()==Auftrag || Typ()==Extern) 
         telart=TEL_FAX; // Im Moment werden alle Telefonnummern angezeigt
     os << kunde_an->LaTeX_an(Typ()==Lieferschein,telart)<<"\n\n";
#elif MABELLA_EXTENSIONS
     if( (Typ()==Rechnung && !kunde_an->isInGrp(KundengruppeID::Rechnungsadresse)) ||
         (Typ()==Auftrag && kunde_an->AB_an_rngadresse())
	)
       os << kunde_rng->LaTeX_an(Typ()==Lieferschein,telart,"1\\textwidth")<<"\n\n";
     else if(Typ()==Lieferschein || Typ()==Wareneingang)
       {
	cH_Kunde kunde_lief(kunde_an->Lfran());        
	os << kunde_lief->LaTeX_an(Typ()==Lieferschein,telart,"1\\textwidth")<<"\n\n";
       }
     else
	os << kunde_an->LaTeX_an(Typ()==Lieferschein,telart,"1\\textwidth")<<"\n\n";       

#endif     

#ifdef MABELLA_EXTENSIONS
	os << "\\end{flushleft}\\end{minipage}}\\hfill"
		"\\raisebox{0pt}[0pt][0pt]{\\begin{minipage}[t]{0.38\\linewidth}\n"
	      "\\footnotesize\n"
	      "\\sloppy\n"
	      "\\begin{flushleft}\n";

        
	if(!kunde_an->isInGrp(KundengruppeID::Rechnungsadresse) 
		&& Typ()!=Rechnung
		&& Typ()!=Extern
		&& !kunde_an->AB_an_rngadresse())
		{
		os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_RNGADRESSE)<<":\\rm\\\\\n"
		   << string2TeX(kunde_rng->firma())+"\\\\\n";
		   if(kunde_rng->Rng_an_postfach())
			{os << "Postfach "<<kunde_rng->postfach()+"\\\\\n"
			 << string2TeX(kunde_rng->postfachplz())+" "
			 << string2TeX(kunde_rng->ort())<<"\\\\\n\n";
			}
		   else
			{			
		   	  os << string2TeX(kunde_rng->strasse())+" "
		   	  << string2TeX(kunde_rng->hausnr())+"\\\\\n"		   
		   	  << string2TeX(kunde_rng->plz()+" "+kunde_rng->ort())+"\\\\\n\n";
			}
		os << "\\smallskip\n";   
		}		

	if(!kunde_an->isInGrp(KundengruppeID::Rechnungsadresse) && Typ()==Rechnung)
		{
		os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_LIEFADRESSE)<<":\\rm\\\\\n"
		   << string2TeX(kunde_an->firma())+"\\\\\n";
		   if(kunde_an->name2()!="")
		     os << string2TeX(kunde_an->name2())+"\\\\\n";
		   os << string2TeX(kunde_an->strasse())+" "
		   << string2TeX(kunde_an->hausnr())+"\\\\\n"		   
		   << string2TeX(kunde_an->plz()+" "+kunde_an->ort())+"\\\\\n\n";
		os << "\\smallskip\n";   
		}
	else	
	if(!kunde_an->isInGrp(KundengruppeID::Rechnungsadresse) && Typ()==Auftrag
		&& kunde_an->AB_an_rngadresse())
		{
		os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_LIEFADRESSE)<<":\\rm\\\\\n"
		   << string2TeX(kunde_an->firma())+"\\\\\n";
		   if(kunde_an->name2()!="")
		     os << string2TeX(kunde_an->name2())+"\\\\\n";
		   os << string2TeX(kunde_an->strasse())+" "
		   << string2TeX(kunde_an->hausnr())+"\\\\\n"		   
		   << string2TeX(kunde_an->plz()+" "+kunde_an->ort())+"\\\\\n\n";
		os << "\\smallskip\n";   
		}
	else
	if(!kunde_an->isInGrp(KundengruppeID::Lieferadresse) && 
		(Typ()==Auftrag || Typ()==Rechnung))
		{
		cH_Kunde kunde_lfr(kunde_an->Lfran());
		os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_LIEFADRESSE)<<":\\rm\\\\\n"
		   << string2TeX(kunde_lfr->firma())+"\\\\\n";
		   if(kunde_lfr->name2()!="")
		     os << string2TeX(kunde_lfr->name2())+"\\\\\n";
		   if(!kunde_lfr->strasse().empty())
		     {os << string2TeX(kunde_lfr->strasse())+" "
		      << string2TeX(kunde_lfr->hausnr())+"\\\\\n";
		     }
		os << string2TeX(kunde_lfr->plz()+" "+kunde_lfr->ort())+"\\\\\n\n";
		os << "\\smallskip\n";   
		}
	else
	if(Typ()==Extern)
		{
		os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_LIEFADRESSE)<<":\\rm\\\\\n"
		   << string2TeX(kunde_von->firma())+"\\\\\n";
		   if(kunde_von->name2()!="")
		     os << string2TeX(kunde_von->name2())+"\\\\\n";
		   if(!kunde_von->strasse().empty())
		     {os << string2TeX(kunde_von->strasse())+" "
		      << string2TeX(kunde_von->hausnr())+"\\\\\n";
		     }
		os << string2TeX(kunde_von->plz()+" "+kunde_von->ort())+"\\\\\n\n";
		os << "\\smallskip\n";   		
		}	
		
	if(kunde_rng->getBetreuer()!=Kunde::none_id)
	  {cH_Kunde betr(kunde_rng->getBetreuer());
	   if(Typ()==Extern)
             os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_ANSPRECHPARTNER)<<":\\rm \\\\\n";
	   else
             os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_BETREUUNG)<<":\\rm \\\\\n";

           os << betr->Anrede()->getAnrede(*mld) <<" "
           	<< betr->getName()<<" "<<betr->getName2()<<"\\\\\n"
        	<< mld->MLT(MultiL_Dict::TXT_TELEFON)<<": "<<betr->Kontakt(TEL_TEL,kunde_von->Id())<<"\\\\\n"
        	<< mld->MLT(MultiL_Dict::TXT_TELEFAX)<<": "<<betr->Kontakt(TEL_FAX,kunde_von->Id())<<"\\\\\n"
        	<< "E-Mail: "<<betr->Kontakt(TEL_E_MAIL,kunde_von->Id())<<"\\\\\n";
          }
       else
	{   	
	os << "\\bf "<<mld->MLT(MultiL_Dict::TXT_ANSPRECHPARTNER)<<":\\rm \\\\\n"
		<< mld->MLT(MultiL_Dict::TXT_TELEFON)<<": "<<kunde_von->get_first_telefon(TEL_TEL)<<"\\\\\n"
		<< mld->MLT(MultiL_Dict::TXT_TELEFAX)<<": "<<kunde_von->get_first_telefon(TEL_FAX)<<"\\\\\n"
        	<< "E-Mail: "<<kunde_von->get_first_telefon(TEL_E_MAIL)<<"\\\\\n";
        }

	if(kunde_an->VerkNr()!=Kunde::none_id && Typ()!=Extern &&
	   kunde_an->VerkNr()!=OHNE_BETREUUNG
	  )
	  {
	   std::string fullname;
	   cH_Kunde verk(kunde_an->VerkNr());

	   if(verk->Aktiv())
 	     {
               if(verk->Rngan()==verk->Id())
                  fullname=verk->getFullName();
               else
                {cH_Kunde v(verk->Rngan());
                 fullname=v->getFullName();
                }
                 os << "\\vspace*{2mm}\\bf "<<mld->MLT(MultiL_Dict::TXT_BESUCHTSIE)<<":\\rm \\\\\n"
                    << string2TeX(fullname) <<"~\\\\\n";
	     }
          }
          
        os << "\\end{flushleft}\\fussy\\normalsize\\end{minipage}}\\\\\n";          
        os << "\\vspace{1cm}";
#else
	os << "\\bigskip\n";
#endif

	
     zeilen_passen_noch=ZEILEN_SEITE_1;
    }
   else 
    {
#if defined MABELLA_EXTENSIONS && defined PRINT_LOGO
   os << "\\ifthenelse{\\kopie>2}"
        "{\\raisebox{0cm}[0pt][0pt]{\\makebox[0pt][l]{\\kern+140pt\\psfig{file=/usr/local/share/mabella/logo}}}\\\\}"
           "{}\n";
#endif

#ifdef MABELLA_EXTENSIONS
     os << "\\normalsize";        
#else
     os <<"\\large";
#endif     

#ifdef MABELLA_EXTENSIONS
     if(!kunde_an->isInGrp(KundengruppeID::Rechnungsadresse) && Typ()==Rechnung)
       os <<"\n "<< string2TeX(kunde_rng->getFullName())<<"\\\\\\bigskip";
     else
       os <<"\n  "<< string2TeX(kunde_an->getFullName())<<"\\\\\\bigskip";
       
     zeilen_passen_noch=ZEILEN_SEITE_N;
    }
#else
#ifdef HAS_ADDR_GROUP_Rechnungsadresse
     if(!kunde_an->isInGrp(KundengruppeID::Rechnungsadresse) && Typ()==Rechnung)
       os <<"\n "<<mld->MLT(MultiL_Dict::TXT_FIRMA)<<" "<< string2TeX(kunde_rng->getName())<<"\\\\\\bigskip";
     else
#endif
       os <<"\n "<<mld->MLT(MultiL_Dict::TXT_FIRMA)<<" "<< string2TeX(kunde_an->getName())<<"\\\\\\bigskip";
       
     zeilen_passen_noch=ZEILEN_SEITE_N;
    }
#endif

#ifdef MABELLA_EXTENSIONS
   os <<"\\hfill " << mld->MLT(MultiL_Dict::TXT_SEITE) << " \\thepage\\\\\n";
   os << "\\large "<<typString()<<" ";
   os.width(6);os.fill('0');
   os <<RngNr()<<"\\normalsize ~" << mld->MLT(MultiL_Dict::TXT_VOM)
		<<" " <<getDatum()<<". ";
   if(Typ()==Extern)
     os << "\n~\\\\\\small Die Bestellnummer bitte immer auf Auftragsbest�tigung,"
	   " Lieferschein und Rechnung angeben \\normalsize\\\\[.5ex]\n";


   if(Typ()==Auftrag  && page_counter==1 && !Rueckstand())
     os <<mld->MLT(MultiL_Dict::TXT_DANKE_AUFTR)<<" \\\\\n";
   else if(Typ()!=Extern) os << "~\\\\\n";

//   if(!Rueckstand())
   if(Typ()==Auftrag) 
		{auftrag_von(os,GCC295(class )NOTGCC295(::)Auftrag(AuftragBase(
   				instanz->Id(),u.a->Id())),true);
   		}

   
   if(Typ()==Extern)
     os << "Ihre Lieferantennummer: ";
   else
     os << "~\\\\\n"<<mld->MLT(MultiL_Dict::TXT_IHREKDNR)<<": ";
   os.width(5);os.fill('0');
   os << kunde_an->Id();
   if(Typ()==Extern)
     {if(!kunde_an->UnsereKundenNr().empty())
	os << ", Unsere Kundennummer: "<<kunde_an->UnsereKundenNr();
     }
   else if(Typ()==Auftrag || Typ()==Rechnung)
     {if(!kunde_an->UnsereKundenNr().empty())
	os << ", Unsere Lieferanten-Nr.: "<<kunde_an->UnsereKundenNr();
     }

   os << "\\hfill "<<
   	(kunde_an->idnr().empty() ? "" : mld->MLT(MultiL_Dict::TXT_USTID)
			+std::string(": ")+kunde_an->idnr()) << "\\\\\n";


#else
   os << "\\LARGE ";
   if (Typ()==Auftrag) os << "Auftragsbest�tigung";
   else os << typString();
   os <<' '<<RngNr()<<"\\hfill\\normalsize "
       "\\ifthenelse{\\kopie>1}{Kopie, }{}";
   os <<"Seite "<<page_counter;
   os <<"\\hfill "<< getDatum();
   if (Typ()==Auftrag && !YourAuftrag().empty()) 
   {  os << "\\\\f�r Ihren Auftrag " << YourAuftrag();
   }
   os << "\\\\\n\n";
   if (!kunde_an->idnr().empty())
      os << "\\hfill "<< mld->MLT(MultiL_Dict::TXT_USTID) << ": " 
            << kunde_an->idnr() << "\\\\\n";
#endif


#ifdef MABELLA_EXTENSIONS
   if (page_counter==1 && (Typ()==Lieferschein || Typ()==Rechnung))
    {os << "\\scriptsize{"<<mld->MLT(MultiL_Dict::TXT_SIEERHIELTEN)<<"}";
     if(Typ()==Rechnung)
       os <<  "\\hfill \\scriptsize{"<<mld->MLT(MultiL_Dict::TXT_UNSERESTNR)
		 <<": "<< kunde_von->idnr() << "}\\\\\n"; //5131/5923/0168}
     else os << "\\\\\n";
     --zeilen_passen_noch; // immer besser
    }
#else
   if (page_counter==1 && Typ()==Lieferschein)
    {os << "Wir sandten Ihnen auf Ihre Rechnung und Gefahr\n";
     --zeilen_passen_noch; // immer besser
    }
#endif
   os <<"\\end{flushleft}\n";
  }
}


void LR_Abstraktion::lieferung_an(std::ostream &os, unsigned int lfrs_id, 
			const ManuProC::Datum& datum,const std::string& sKunde)
{ --zeilen_passen_noch;

  os << "~\\\\[2ex]""\n";  

    --zeilen_passen_noch;  

    os << mld->MLT(MultiL_Dict::TXT_UNSERELIEF)<<" "<<lfrs_id;
    os << " "<<mld->MLT(MultiL_Dict::TXT_VOM)<<" "<<datum;
    if(!sKunde.empty())  os << " an "<< string2TeX(sKunde);
    os << "\\\\[-2ex]\n";

}

#ifdef MABELLA_EXTENSIONS
void LR_Abstraktion::auftrag_von(std::ostream &os, const class Auftrag &a,
				bool only_braces)
{ 
  if(! only_braces)
    {--zeilen_passen_noch;
       os << "~\\\\"<<mld->MLT(MultiL_Dict::TXT_UNSEREAUFNR)<<" ";
     os.width(6);os.fill('0'); os<< a.Id();
     os << " "<< mld->MLT(MultiL_Dict::TXT_VOM)<<" "<< a.getDatum();
     --zeilen_passen_noch;
     os <<". "<<mld->MLT(MultiL_Dict::TXT_DANKE_AUFTR)<<" \\\\\n";
    }
    
  if(a.getYourAufNr() != a.getBemerkung())
    {os <<" \\small{";
      if(!a.getYourAufNr().empty()) 
	{os << mld->MLT(MultiL_Dict::TXT_IHRAUFTRAG);	
	 os <<" "<<string2TeX(a.getYourAufNr());
	}
      if(!a.getBemerkung().empty()) os << " "<<string2TeX(a.getBemerkung());
     os << "}";
    }
  os << "\\normalsize";
  if(! only_braces) os << "\\\\[-3ex]\n";
  
}
#endif


