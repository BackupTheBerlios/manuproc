/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
#include <Artikel/Einheiten.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Kunde/Kunde.h>
#include <Gtk2TeX.h>
#include <Aux/Ausgabe_neu.h>

#ifdef MABELLA_EXTENSIONS
#define TABW	"17"
#else
#define TABW	"18"
#endif

const std::string Dform_TeX(unsigned int i)
{  
#if 0 // ich glaube das gibt es schon 
 std::string str;
 char d[20];   
 char tmp1[10];
 char tmp2[10];
 char tmp3[10];
 
 snprintf(d,sizeof d,"%d",i);
    
    if (strlen(d) > 6)
       {
       strncpy(tmp1,d,3);strncpy(tmp2,d+3,3);strcpy(tmp3,d+6);
       tmp1[3]=0; tmp2[3]=0;
       snprintf(d,sizeof d,"%s\\,%s\\,%s",tmp1,tmp2,tmp3);
       }
    else if (strlen(d) > 3)
       { strncpy(tmp1,d,3);strcpy(tmp2,d+3);
       tmp1[3]=0;
       snprintf(d,sizeof d,"%s\\,%s",tmp1,tmp2);
       }
    else
       snprintf(d,sizeof d,"%s",d);

 return d;
#else 
  return FormatiereTeX(i);
#endif
}



void LR_Abstraktion::calc_all(cH_Kunde k)
{
 nettobetrag = betrag;
 entskosten = nettobetrag * (k->Entsorgung() ? 0.002 : 0.0);
 entsbetrag = nettobetrag + entskosten;
 mwstbetrag = entsbetrag * 0.16;
 bruttobetrag = entsbetrag + mwstbetrag;
 skontobetrag = bruttobetrag * ((100.0 - k->skontosatz())/100.0);
 einzugbetrag = skontobetrag * ((100.0 - k->einzugrabatt())/100.0);
}

void LR_Abstraktion::drucken_header(std::ostream &os)
{
 Gtk2TeX::HeaderFlags hf;
 
 hf.packages="pstricks,tabularx,eurosym";
 hf.ptsize=12;
 hf.pagestyle="empty";

 hf.leftmargin=2.25 * Gtk2TeX::in_cm;
 hf.topmargin=2.8 * Gtk2TeX::in_cm;
 hf.rightmargin=3.25 * Gtk2TeX::in_cm;
 hf.bottommargin=1; //inch
 // kein footer angegeben ???
 
 hf.preamble="\\newrgbcolor{mygray}{0.75 0.75 0.75}\n"
	"\\newlength{\\breite}\n"
	"\\setlength{\\topsep}{0pt}\n"
	"\\setlength{\\partopsep}{0pt}\n"
	"\\setlength{\\textheight}{27.0cm}\n";  // na, ob das sinnvoll ist?

#ifdef MABELLA_EXTENSIONS
 hf.helvetica=false;
 hf.packages+=",color,times,epsfig,graphics";

 hf.leftmargin=2 * Gtk2TeX::in_cm;
 hf.rightmargin=1.25 * Gtk2TeX::in_cm;

 hf.preamble+="\\definecolor{backg}{gray}{0.985}\n"
	      "\\definecolor{lgray}{gray}{.9}\n"
	"\\newcommand{\\shaderow}{\\raisebox{-1pt}[0pt][0pt]{\\makebox[0pt][l]"
			"{\\kern-70pt\\colorbox{backg}%%\n"
			"{\\rule{0pt}{7pt}\\rule{\\paperwidth}{0pt}}}}}\n";
#endif

 Gtk2TeX::Header(os,hf);

   os << "% output thin lines at left side\n"
	"\\vbox to 0pt{%\n"
	"\\rule{0pt}{67.5truemm}% 62.5truemm > 80.0truemm\n"
#ifndef MABELLA_EXTENSIONS	
	"\\hbox to 0pt{\\hspace*{-24truemm}\\vbox{\\hrule width 7truemm}\\hss}%\n"
#else
	"\\hbox to 0pt{\\hspace*{-24truemm}\\vbox{\\hrule width 12truemm}\\hss}%\n"
#endif	
	"\\vss}\\nointerlineskip\n"
	"\\vspace*{1.2cm}%\n";

#ifdef MABELLA_EXTENSIONS 
if(!firmenpapier)
   os << "\\raisebox{25pt}[0pt][0pt]{\\makebox[0pt][l]{\\kern+140pt\\psfig{file=/tmp/logo.eps}}}\n";

   cH_Kunde kunde_an(KdNr());
   if(kunde_an->verein().size())
     os << "\\raisebox{-120pt}[0pt][0pt]{%%\n"
         "\\makebox[0pt][s]{%%\n"
         "\\kern+170pt \\rotatebox{-30}{%%\n"
         "\\scalebox{4}{%%\n"
         "{\\color{lgray}" << string2TeX(kunde_an->verein()) << "}%%\n"
         "}}}}%%\\\\\n";
#endif

}

void LR_Abstraktion::drucken_footer(std::ostream &os)
{
  if (t==Rechnung)
   {
#ifdef MABELLA_EXTENSIONS
    cH_Kunde kunde_an(KdNr());
    os << "\\vspace{0.5cm}\n";
    if(kunde_an->bankeinzug())
      {os << std::string("Der Rechnungsbetrag ");
       if(kunde_an->skontosatz())
      	 {os << "\\\\abzgl. "<<FormatiereTeX_short(kunde_an->skontosatz())
      	 	<<"\\% Skonto";
      	  if(kunde_an->einzugrabatt())
      	    os << " = "<<getWaehrung()->TeXsymbol()<<" "<<FormatiereTeX_Preis(skontobetrag);
      	  else
      	    os << " = {\\bf "<<getWaehrung()->TeXsymbol()<<" "<<FormatiereTeX_Preis(skontobetrag)<<"}";
      	 }
       if(kunde_an->einzugrabatt())
      	 os << "\\\\abzgl. "<<FormatiereTeX_short(kunde_an->einzugrabatt())
      	 	<<"\\% wg. Abbuchung = {\\bf "<<getWaehrung()->TeXsymbol()<<" "<<FormatiereTeX_Preis(einzugbetrag)<<"}\\\\";
       os << "wird ";
       if(u.r->getZahlziel().valid())
         os << " am "<<u.r->getZahlziel()<<"\\\\\n";
       os << "von Ihrem Konto "<<Dform_TeX(kunde_an->getKtnr());
       os << "\\\\\n bei der " <<kunde_an->getbank();
       os << "\n BLZ "<<Dform_TeX(kunde_an->getblz())<<" abgebucht\\\\\n";
      }

#else
     cH_Kunde kunde_von(1);
     os <<"\n\n\\footnotesize "<< kunde_von->getBank()<<"\\\\\n";
     os << "Zahlung: "<< getZahlungsart()->Bezeichnung()<<"\\\\\n";
     os <<"Die Lieferung erfolgt zu den Einheitsbedingungen der deutschen Texttilindustrie\\\\\n";
     os <<"Gerichtsstand ist Wuppertal\\\\\n";
#endif

   }
   
  Gtk2TeX::Footer(os);
}


void LR_Abstraktion::drucken(std::ostream &os,bool _kopie,const cH_ppsInstanz& _instanz)
{instanz=_instanz;
 kopie=_kopie;

 if (t==Rechnung || (t==Auftrag && instanz==ppsInstanz::INST_KNDAUF)) 
 	preise_addieren=true;
 else preise_addieren=false;

 betrag=0;

 Kunde::ID kunden_id = KdNr();

 drucken_header(os);
 page_counter = 1;
 page_header(os);

// os << "\\begin{flushright}\n";
 os << "\\normalsize\n";

 int lfrsid_mem=-1;
 bool lfrsid_drucken=false;
#ifdef MABELLA_EXTENSIONS 
 int aufid_mem=-1;
 bool aufid_drucken=false;
#endif

 for (LR_Abstraktion::const_iterator i=begin();i!=end();)
  { //********* je Durchlauf eine Tabelle ausgeben ********************
  
     ArtikelBase artikel_id  = (*i).ArtikelID();
     cH_ArtikelBezeichnung bez(artikel_id,cH_Kunde(kunden_id)->getSchema()->Id());
    cH_ExtBezSchema schema_mem = bez->getExtBezSchema();
    Preis::preismenge_t preismenge_mem;
    Einheit einheit_mem = Einheit(artikel_id); 

// falls Lieferschein sich geändert hat, neue Nummer unten ausgeben
// merken welche Werte in der 1. Zeile stehen
    if (t==Rechnung && (*i).Lfrs_Id()!=lfrsid_mem)
    {  lfrsid_drucken=true;
       lfrsid_mem=(*i).Lfrs_Id();
    }
    
#ifdef MABELLA_EXTENSIONS
// falls Auftrag sich geändert hat, neue Nummer unten ausgeben
// merken welche Werte in der 1. Zeile stehen
    if (t==Lieferschein && (*i).AufId()!=aufid_mem)
    {  aufid_drucken=true;
       aufid_mem=(*i).AufId();
    }    
#endif    
    
    if (preise_addieren) preismenge_mem = (*i).getPreis().PreisMenge();

// welche Spalten sollen vorkommen
    stueck_bool=false;
    menge_bool=false;
    rabatt_bool=false;
//******** wieviele Zeilen passen denn maximal in diese Tabelle ************
    LR_Abstraktion::const_iterator j=i ;
    for (;j!=end() ;++j) 
      {
        ArtikelBase artikelbase  = (*j).ArtikelID();
        cH_ArtikelBezeichnung bez(artikelbase,cH_Kunde(kunden_id)->getSchema()->Id());
        cH_ExtBezSchema schema = bez->getExtBezSchema();
        
        if (schema!=schema_mem) break; // Schema hat gewechselt
        
        if (Einheit(artikelbase) != einheit_mem ) break;  // Einheit wechselt
        
        if ((*j).Stueck()!=1 || !(*j).Menge()) stueck_bool=true;

        						// was soll das? (!!)
        if ((t==Rechnung || t==Lieferschein) && !!(*j).Menge())
           menge_bool=true;
        
        if (t==Rechnung && lfrsid_mem != (*j).Lfrs_Id()) 
           break; // Lieferschein wechselt

#ifdef MABELLA_EXTENSIONS           
        if (t==Lieferschein && aufid_mem != (*j).AufId())
          break; // Auftrag wechselt
#endif

        if (preise_addieren)
        {
           if ((*j).getPreis().PreisMenge() != preismenge_mem) 
              break; // Preismenge wechselt
              
           if (!!(*j).Rabatt()) rabatt_bool=true;
        }
      }
//******** die Tabelle die wir ausgeben können endet bei j *****************

     bool neue_seite=false;
//----------------- Seitenumbruch ? -------------------------------
     // 3+1+1 Mindestzeilen bei langer Tabelle, 1 Zeile f. Header angenommen
     if (zeilen_passen_noch<5 && 
     	zeilen_passen_noch<(j-i)
     	+1 // für Kopfzeile
     	+(preise_addieren?1:0) // für Übertrag
     	+(lfrsid_drucken?2:0)) // für Lieferschein X am Y an Z
     {   // Tabelle beenden, preis ausgeben
         if (preise_addieren)
         {  drucken_betrag(os,"Übertrag",betrag);
	    os << "\\end{tabularx}\n";
         }
         os << "\\newpage\n";
         ++page_counter;
         page_header(os);
         neue_seite=true;
     }
     else if (preise_addieren && i!=begin()) // da ist noch was offen ...
        os << "\\end{tabularx}\n";

//------------------------- Kopf -------------------------
    if (lfrsid_drucken)
    {   cH_Lieferschein l(instanz,lfrsid_mem);
        cH_Kunde k(l->KdNr()); 
        std::string sKunde;
        if (k->Id() != kunden_id) sKunde = k->firma();
       	lieferung_an(os,lfrsid_mem,l->getDatum(),sKunde); 
       	lfrsid_drucken=false;
    }     

#ifdef MABELLA_EXTENSIONS    
    if(aufid_drucken)
    {   AuftragBase ab(ppsInstanz::Kundenauftraege, aufid_mem);
    	class Auftrag a(ab);
       	auftrag_von(os,a);
       	aufid_drucken=false;
    }         
#endif
    
     drucken_table_header(os,schema_mem,preismenge_mem, einheit_mem.TeX());
     
    std::string einheitsize = "\\scriptsize \\,";
    if (neue_seite && preise_addieren)
    {  os << zur_preisspalte << "Vortrag & "<< FormatiereTeX(betrag) <<"\\\\";
       --zeilen_passen_noch;
    }
    for (LR_Abstraktion::const_iterator k=i;k!=j;++k) // Zeilen ausgeben
      {

//---------------------- Seitenumbruch innerhalb einer Tabelle ----------      
        if (zeilen_passen_noch<=1)
          {
            if (preise_addieren)
            {  drucken_betrag(os,"Übertrag",betrag);
            }
   	    os << "\\end{tabularx}\n"
            	 "\\newpage\n";
            ++page_counter;
            page_header(os);
            drucken_table_header(os,schema_mem,preismenge_mem,einheit_mem.TeX());
            if (preise_addieren)
            {  os << zur_preisspalte << "Vortrag & "<< FormatiereTeX(betrag) <<"\\\\";
               --zeilen_passen_noch;
            }
          }

//************************* eine Zeile ausgeben ***************************
         ArtikelBase artikelbase  = (*k).ArtikelID();
     	 std::string linecolor;

// für die erste Spalte braucht man kein "&", 
// nach der letzten darf keines stehen ... daher dies
     	 bool erste_spalte=true;
#define NEUE_SPALTE ({ if (!erste_spalte) os << " & "; else erste_spalte=false; })
     	 
#ifdef MABELLA_EXTENSIONS
	if(zeilen_passen_noch%2) os << "\\shaderow";
#endif
         if ((*k).ZusatzInfo()) linecolor = "\\mygray";
         
         cH_ArtikelBezeichnung bez(artikelbase,cH_Kunde(kunden_id)->getSchema()->Id());
         
         if (stueck_bool)
         {  NEUE_SPALTE;
            if ((!(*k).ZusatzInfo() || (*k).Stueck()!=1))
            {  os <<linecolor ;
               os << FormatiereTeX((*k).Stueck());
               if (Einheit(artikelbase).StueckEinheit().size())
                  os <<'{'<< einheitsize <<Einheit(artikelbase).StueckEinheit() <<'}';
            }
         }
         if (menge_bool) 
            {  NEUE_SPALTE;
               if (!(*k).ZusatzInfo()) 
                    os <<linecolor<< FormatiereTeX_short((*k).Menge())<<einheitsize <<Einheit(artikelbase).MengenEinheit() ;
            }
         if (stueck_bool && menge_bool)
           { NEUE_SPALTE;
             os <<linecolor<< FormatiereTeX_short(fixedpoint<3>((*k).Stueck() * (*k).Menge()))<<einheitsize <<Einheit(artikelbase).TeX();
           }
           
	 for(ExtBezSchema::const_sigiterator l=schema_mem->sigbegin(signifikanz);l!=schema_mem->sigend(signifikanz);++l)
	   { NEUE_SPALTE;
	     if (!(*k).ZusatzInfo())
	        os <<"{"<<linecolor<< Gtk2TeX::string2TeX((*bez)[l->bezkomptype]->getStrVal()) <<"}";
	   }
#ifdef MABELLA_EXTENSIONS
	   NEUE_SPALTE;
	   os << bez->Bezeichnung(3);
#endif 
	   
         if (preise_addieren)       
          { NEUE_SPALTE;
            os <<linecolor<<FormatiereTeX_Preis( (*k).getPreis().Wert() );
            if (rabatt_bool) 
            {  NEUE_SPALTE; os << linecolor<<FormatiereTeX((*k).Rabatt()); }
            fixedpoint<2> preis = (*k).getPreis().Gesamtpreis(getWaehrung(),(*k).Stueck(),(*k).Menge(),(*k).Rabatt());
            betrag+=preis;
            tabellenbetrag+=preis;
            NEUE_SPALTE;
            os <<linecolor<< FormatiereTeX(preis);
          }

#ifdef PETIG_EXTENSIONS
        if (t==Lieferschein) 
          { NEUE_SPALTE;
            if ((*k).Palette()!=0) os << linecolor<<(*k).Palette();
            NEUE_SPALTE;
            os << linecolor << (*k).YourAuftrag()<<"\n";   
          }
#endif

        os<<"\\\\\n";
#undef NEUE_SPALTE        
        --zeilen_passen_noch;
      }
#if 0  // Tabellensumme (needs more work, but I don't want to delete it)
   if (preise_addieren)
      drucken_betrag(os,"",betrag); // oder Tabellenbetrag
#endif    
   if (!preise_addieren) os << "\\end{tabularx}\n\n";
   i=j;
  }

  if (preise_addieren) 
  //*********************** Summe bilden *********************************
   { 
#warning Schätzwert: Zeilen fuer Summe benötigt   
     if (zeilen_passen_noch<10)
     {   // Tabelle beenden, preis ausgeben
         drucken_betrag(os,"Summe",betrag);
	 os << "\\end{tabularx}\n";
         os << "\\newpage\n";
         ++page_counter;
         page_header(os);
         os << "\\begin{tabularx}{" << TABW << "cm}{rXrr}\n";
         spaltenzahl=4;
         zur_preisspalte="&&";
     }
     else 
     {  drucken_betrag(os,"Summe",betrag);
     }
     
#ifdef MABELLA_EXTENSIONS
	calc_all(cH_Kunde(KdNr()));     
#endif
     
//     os << zur_preisspalte << "Endsumme & "<< FormatiereTeX(betrag) <<"\\\\""\n";
//     --zeilen_passen_noch;
     

#ifdef PETIG_EXTENSIONS
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
#endif
      
#ifdef MABELLA_EXTENSIONS
 cH_Kunde kunde_an(KdNr());
 if(kunde_an->Entsorgung())
   {
    os << "&\\multicolumn{" << spaltenzahl-2 << 
   	"}{r}{Gesetzlicher Entsorgungskosten-Anteil VfW Reflora von 0,20\\%}"
   	"& " << FormatiereTeX(entskosten) << "\\\\\n";
    --zeilen_passen_noch;
    os << "\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\n";
    os << zur_preisspalte << "&"+FormatiereTeX(entsbetrag)+"\\\\\n";
    --zeilen_passen_noch;
   }
   
   os << zur_preisspalte << "+ 16\\% MwSt& "<< FormatiereTeX(mwstbetrag) <<"\\\\"
     	"\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\n";  
   os << zur_preisspalte << getWaehrung()->TeXsymbol() <<"&"<< FormatiereTeX(bruttobetrag) <<"\\\\"
     	"\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\\\\[-12pt]\n"  
     	"\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\n";  
   os << "\\end{tabularx}\n\n";
#else      
     fixedpoint<2> betrag_MwSt_ = betrag*0.16;
     os << zur_preisspalte << "+ 16\\% MwSt& "<< FormatiereTeX(betrag_MwSt_) <<"\\\\"
     	"\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\n";
     --zeilen_passen_noch;
     betrag+=betrag_MwSt_;
     cP_Waehrung andere_waehrung=getWaehrung();
     if (andere_waehrung != cP_Waehrung(Waehrung::EUR)) 
     {  andere_waehrung = cP_Waehrung(Waehrung::EUR);
        fixedpoint<2> anderer_betrag
              = betrag*Waehrung::Umrechnung(*getWaehrung(),*andere_waehrung);
      
        os <<andere_waehrung->TeXsymbol()<<"~"<< FormatiereTeX( anderer_betrag );
     }
     os << zur_preisspalte
        << getWaehrung()->TeXsymbol() << '&' << FormatiereTeX(betrag)<<"\\\\";
     os <<"\\cline{1-1}\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}"
     	   "\\\\[-2.5ex]\\cline{1-1}\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\\\\\n";
     --zeilen_passen_noch;
     os << "\\end{tabularx}\n";
#endif     

   }
//  os << "\\end{flushright}\n";


  drucken_footer(os);
}


void LR_Abstraktion::drucken_table_header(std::ostream &os,
   	const cH_ExtBezSchema &schema, 
   	float preismenge, const std::string &preiseinheit)
{
#ifdef MABELLA_EXTENSIONS
  std::string ug ="\\scriptsize{}"; // Größe der Überschriften
  std::string sg ="\\scriptsize{}"; // Größe der Überschriften
#else
  std::string ug ="\\footnotesize\\mygray{}"; // Größe der Überschriften
  std::string sg ="\\scriptsize\\mygray{}"; // Größe der Überschriften
#endif

  std::string tabcolumn;
  spaltenzahl=0;
   if (stueck_bool) { tabcolumn+="r"; spaltenzahl++; }
   if (menge_bool) { tabcolumn+="r"; spaltenzahl++; }
  if (menge_bool && stueck_bool) { tabcolumn+="r"; spaltenzahl++; }
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
      { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; }

#ifndef MABELLA_EXTENSIONS
  if (t==Lieferschein)  // Palette + Auftrag
  { tabcolumn+="rr"; spaltenzahl+=2; }
#else
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(3);j!=schema->sigend(3);++j)
      { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; }
#endif

  if (preise_addieren && rabatt_bool)  // Rabatt
  {  tabcolumn+="r"; ++spaltenzahl;}
  if (preise_addieren)  // Einzelpreis, Gesamtpreis
  { tabcolumn+="rr"; spaltenzahl+=2; }
  else if (t==Auftrag) 
  { tabcolumn+="X"; spaltenzahl+=1; }

  os << "\\settowidth{\\breite}{"<<ug<<" Bezeichnung}%\n";
//  os << "\\vspace{-2ex}\n";
  os << "\\begin{tabularx}{" << TABW << "cm}{"<<tabcolumn<<"}"<<"\\\\\n";

// für die erste Spalte braucht man kein "&", 
// nach der letzten darf keines stehen ... daher dies
  bool erste_spalte=true;
#define NEUE_SPALTE ({ if (!erste_spalte) os << " & "; else erste_spalte=false; })
  if (stueck_bool && menge_bool) 
  {  NEUE_SPALTE;
     os << "\\multicolumn{2}{c}{"<<ug<<"Menge}  "; 
     NEUE_SPALTE;
     os << "\\multicolumn{1}{c}{"<<sg<<"Gesamtmenge}  ";
  }
  else if (stueck_bool || menge_bool) 
  {  NEUE_SPALTE;
     os << "\\multicolumn{1}{c}{"<<ug<<"Menge}  ";
  }
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
  {  NEUE_SPALTE;
     os << "\\mbox{"<<ug<<j->bezkomptext<<"}";
  }
 

#ifndef MABELLA_EXTENSIONS
  if (t==Lieferschein)
   { NEUE_SPALTE;
     os <<"\\multicolumn{1}{c}{" <<ug<<"Palette}";
     NEUE_SPALTE;
     os <<"\\multicolumn{1}{c}{" <<ug<<"Auftrag}";
   }
#else
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(3);j!=schema->sigend(3);++j)
  {  NEUE_SPALTE;
     os << "\\mbox{"<<ug<<j->bezkomptext<<"}";
  }
#endif

  if (preise_addieren)
   { // Preis ausgeben
     NEUE_SPALTE;
     os <<"\\multicolumn{1}{c}{"<<sg<<getWaehrung()->TeXsymbol();
     if (preismenge!=1 || !preiseinheit.empty()) os <<"\\,/";
     if (preismenge!=1) os << "\\,"<<preismenge;
     if (!preiseinheit.empty())  os << "\\,"<<preiseinheit;
     os  <<"} ";
     if (rabatt_bool) { NEUE_SPALTE; os <<"\\multicolumn{1}{c}{"<<ug<<"Rabatt}"; }
#ifdef MABELLA_EXTENSIONS
     NEUE_SPALTE;
     os <<"\\multicolumn{1}{c}{"<<sg<<"{"<<getWaehrung()->TeXsymbol()<<" Wert}}"; 
#else
     NEUE_SPALTE;
     os <<"\\multicolumn{1}{c}{"<<sg<<"{Gesamtpreis}}"; 
#endif
   }
  os << "\\\\" "\\hline\n";
#undef NEUE_SPALTE
  --zeilen_passen_noch;

  zur_preisspalte="";
  for (unsigned int i=0;i<spaltenzahl-2;++i)  zur_preisspalte+='&';  

  tabellenbetrag=0;
}

void LR_Abstraktion::drucken_betrag(std::ostream &os,const std::string &text, fixedpoint<2> betrag)
{
  os << "\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}"<<"\n";
  os << zur_preisspalte << text << '&' << FormatiereTeX(betrag)<<"\\\\\n";
  --zeilen_passen_noch;
}


void LR_Abstraktion::page_header(std::ostream &os)
{
 if(instanz->Id()!=ppsInstanz::INST_KNDAUF)
  {
   os <<"\\begin{flushleft}\n";
//   os <<"Auftrag an "+instanz->get_Name()<<"\\\\\n";
   os <<"Auftrag an "+instanz->get_Name()<<"\\hfill "<<
         getDatum()<<"\\\\\n";
   os <<"\\end{flushleft}\n";
   zeilen_passen_noch=ZEILEN_SEITE_N;
  }
 else
  {
   cH_Kunde kunde_an(KdNr());
   os <<"\\begin{flushleft}\n";
   if (page_counter==1) // 1. Seite
    {
     cH_Kunde kunde_von(1);

#ifdef MABELLA_EXTENSIONS
	os << "\\begin{minipage}[b]{0.6\\linewidth}\n"
	   << "\\begin{flushleft}\n";
#endif

     os << kunde_von->LaTeX_von()<<"\n\n";
     os << kunde_an->LaTeX_an(t==Lieferschein)<<"\n\n";

#ifdef MABELLA_EXTENSIONS
	os << "\\end{flushleft}\\end{minipage}\\hfill\\begin{minipage}[b]{0.35\\linewidth}\n"
	      "\\small\n"
	      "\\sloppy\n"
	      "\\begin{flushleft}\n";
	if(!kunde_an->isLieferadresse() && t!=Lieferschein)
		{
		os << "Lieferung an\\\\\n"
		   << string2TeX(kunde_an->firma()+"\\\\\n")
		   << string2TeX(kunde_an->strasse()+"\\\\\n")
		   << string2TeX(kunde_an->plz()+" "+kunde_an->ort()+"\\\\\n");
		}
	os << "Bei Rückfragen\\\\\n"
		<< "Telefon: 0202 6489250\\\\\n"
		<< "Telefax: 0202 6489255\\\\\n"
		<< "E-Mail: mabella@wtal.de\\\\\n" 
	      	<< "\\end{flushleft}\\fussy\\normalsize\\end{minipage}\\\\[15mm]\n";
#endif

	os << "\\bigskip";

     zeilen_passen_noch=ZEILEN_SEITE_1;
    }
   else 
    {
     os <<"\n\n\\vspace{1cm}\n";
     os <<"\\large Firma\n\n"<< kunde_an->getName()<<"\n\n\\bigskip";
     zeilen_passen_noch=ZEILEN_SEITE_N;
    }

#ifdef MABELLA_EXTENSIONS
   os << "{\\Large "<<typString()<<" }";
   os.width(6);os.fill('0');
   os <<RngNr()<<" vom "<<getDatum() << "\\\\\n\n";
   os << "Ihre Kundennummer: ";
   os.width(5);os.fill('0');
   os << kunde_an->Id() << "\\\\\n";


#else
   os << "\\LARGE "<<typString()<<' '<<RngNr()<<"\\hfill\\normalsize "
        <<(kopie?"Kopie, ":"")<<"Seite "<<page_counter<<"\\hfill "<< getDatum() << "\\\\\n\n";
#endif


#ifdef MABELLA_EXTENSIONS
   if (page_counter==1 && (t==Lieferschein || t==Rechnung))
    {os << "\\scriptsize{Sie erhielten zu unseren allgemeinen "
	"Geschäftsbedingungen}\\\\\n";
     --zeilen_passen_noch; // immer besser
    }
#else
   if (page_counter==1 && t==Lieferschein)
    {os << "Wir sandten Ihnen auf Ihre Rechung und Gefahr\n";
     --zeilen_passen_noch; // immer besser
    }
#endif


   os <<"\\end{flushleft}\n";
  }
}


void LR_Abstraktion::lieferung_an(std::ostream &os, unsigned int lfrs_id, const Petig::Datum& datum,const std::string& sKunde)
{ --zeilen_passen_noch;
//  os << "\\begin{flushleft}\n";
  os << "~\\\\[2ex]""\n";  // ohne ~ gibt's  Error: There's no line here to end.
  --zeilen_passen_noch;
  os << "Unsere Lieferung "<<lfrs_id;
  os << " am "<<datum;
  if(!sKunde.empty())  os << " an "<<sKunde ;
  os << "\\\\[-2ex]\n";
//  os << "\\end{flushleft}\n";
}

#ifdef MABELLA_EXTENSIONS
void LR_Abstraktion::auftrag_von(std::ostream &os, const class Auftrag &a)
{ --zeilen_passen_noch;
  os << "~\\\\Ihr Auftrag ";
  os.width(6);os.fill('0'); os<< a.Id();
  os << " vom "<< a.getDatum();
  if(a.getYourAufNr() != a.getBemerkung())
    {os <<" \\scriptsize{(";
      if(a.getYourAufNr()!="") os << "Ihre Nr. "<<a.getYourAufNr();
      if(a.getBemerkung()!="") os << "  "<<a.getBemerkung()<<")}";
    }
  os << "\\normalsize\\\\[-2ex]\n";
  
}
#endif


