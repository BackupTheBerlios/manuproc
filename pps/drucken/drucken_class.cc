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

void LR_Abstraktion::drucken_header(ostream &os)
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
	"\\newcommand{\\shaderow}{\\raisebox{-1pt}[0pt][0pt]{\\makebox[0pt][l]"
			"{\\kern-70pt\\colorbox{backg}%%\n"
			"{\\rule{0pt}{9pt}\\rule{\\paperwidth}{0pt}}}}}\n";
#endif

 Gtk2TeX::Header(os,hf);

   os << "% output thin lines at left side\n"
	"\\vbox to 0pt{%\n"
	"\\rule{0pt}{67.5truemm}% 62.5truemm > 80.0truemm\n"
	"\\hbox to 0pt{\\hspace*{-24truemm}\\vbox{\\hrule width 7truemm}\\hss}%\n"
	"\\vss}\\nointerlineskip\n"
	"\\vspace*{1.2cm}%\n";

#ifdef MABELLA_EXTENSIONS 
if(!firmenpapier)
   os << "\\raisebox{50pt}[0pt][0pt]{\\makebox[0pt][l]{\\kern+140pt\\psfig{file=/tmp/logo.eps}}}\n";
#endif

}

void LR_Abstraktion::drucken_footer(ostream &os)
{
  if (t==Rechnung)
   {
     cH_Kunde kunde_von(1);
     os <<"\n\n\\footnotesize "<< kunde_von->getBank()<<"\\\\\n";
     os << "Zahlung: "<< getZahlungsart()->Bezeichnung()<<"\\\\\n";
     os <<"Die Lieferung erfolgt zu den Einheitsbedingungen der deutschen Texttilindustrie\\\\\n";
     os <<"Gerichtsstand ist Wuppertal\\\\\n";
   }
   
  Gtk2TeX::Footer(os);
}


void LR_Abstraktion::drucken(ostream &os,bool _kopie,const cH_ppsInstanz& _instanz)
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
        
        if ((t==Rechnung || t==Lieferschein) && !!(*j).Menge())
           menge_bool=true;
        
        if (t==Rechnung && lfrsid_mem != (*j).Lfrs_Id()) 
           break; // Lieferschein wechselt

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
    {   cH_Lieferschein l(lfrsid_mem);
        cH_Kunde k(l->KdNr()); 
        string sKunde;
        if (k->Id() != kunden_id) sKunde = k->firma();
       	lieferung_an(os,lfrsid_mem,l->getDatum(),sKunde); 
       	lfrsid_drucken=false;
    }     
     drucken_table_header(os,schema_mem,preismenge_mem, einheit_mem.TeX());
     
    string einheitsize = "\\scriptsize \\,";
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
     	 string linecolor;
         if ((*k).ZusatzInfo()) linecolor = "\\mygray";
         
         cH_ArtikelBezeichnung bez(artikelbase,cH_Kunde(kunden_id)->getSchema()->Id());
         
         if (stueck_bool && (!(*k).ZusatzInfo() || (*k).Stueck()!=1))
            {  os <<linecolor ;
              	os << FormatiereTeX((*k).Stueck());
           	   if (Einheit(artikelbase).StueckEinheit().size())
              	   os <<'{'<< einheitsize <<Einheit(artikelbase).StueckEinheit() <<'}';
            }
         if (stueck_bool) os <<" & " ;         
         if (menge_bool) 
            {  if (!(*k).ZusatzInfo()) 
                    os <<linecolor<< FormatiereTeX_short((*k).Menge())<<einheitsize <<Einheit(artikelbase).MengenEinheit() ;
               os <<" & " ;
            }
         if (stueck_bool && menge_bool)
           { os <<linecolor<< FormatiereTeX_short(fixedpoint<3>((*k).Stueck() * (*k).Menge()))<<einheitsize <<Einheit(artikelbase).TeX();
             os <<" & ";
           }
           
	 for(ExtBezSchema::const_sigiterator l=schema_mem->sigbegin(signifikanz);l!=schema_mem->sigend(signifikanz);++l)
	   { if ((*k).ZusatzInfo()) os << "&";
             else os <<"{"<<linecolor<< Gtk2TeX::string2TeX((*bez)[l->bezkomptype]->getStrVal()) <<"}&";
	   }
	   
         if (preise_addieren)       
          {
            os <<linecolor<<FormatiereTeX_Preis( (*k).getPreis().Wert() )<<" & ";
            if (rabatt_bool) os<<linecolor<<FormatiereTeX((*k).Rabatt())<<" & ";
            fixedpoint<2> preis = (*k).getPreis().Gesamtpreis(getWaehrung(),(*k).Stueck(),(*k).Menge(),(*k).Rabatt());
            betrag+=preis;
            tabellenbetrag+=preis;
            os <<linecolor<< FormatiereTeX(preis);
          }
        if (t==Lieferschein) 
          {
            if ((*k).Palette()!=0) os <<linecolor<<(*k).Palette();
            os<<" & "<<linecolor << (*k).YourAuftrag()<<"\n";   
          }
        os<<"\\\\\n";
        --zeilen_passen_noch;
      }
#if 0  // Tabellensumme (needs more work)
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
         os << "\\begin{tabularx}{18cm}{rXrr}\n";
         spaltenzahl=4;
         zur_preisspalte="&&";
     }
     else 
     {  drucken_betrag(os,"Summe",betrag);
     }
     
//     os << zur_preisspalte << "Endsumme & "<< FormatiereTeX(betrag) <<"\\\\""\n";
//     --zeilen_passen_noch;
     
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
      
     fixedpoint<2> betrag_MwSt_ = betrag*0.16;
     os << zur_preisspalte << "+ 16\\% MwSt& "<< FormatiereTeX(betrag_MwSt_) <<"\\\\"
     	"\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\n";
     --zeilen_passen_noch;
     betrag+=betrag_MwSt_;
     cP_Waehrung andere_waehrung;
     if (getWaehrung() == cP_Waehrung(Waehrung::DM)) 
          andere_waehrung = cP_Waehrung(Waehrung::EUR);
     else andere_waehrung = cP_Waehrung(Waehrung::DM);
     fixedpoint<2> anderer_betrag
           = betrag*Waehrung::Umrechnung(*getWaehrung(),*andere_waehrung);
   
     os <<andere_waehrung->TeXsymbol()<<"~"<< FormatiereTeX( anderer_betrag ) 
     	<< zur_preisspalte
        << getWaehrung()->TeXsymbol() << '&' << FormatiereTeX(betrag)<<"\\\\";
     os <<"\\cline{1-1}\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}"
     	   "\\\\[-2.5ex]\\cline{1-1}\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}\\\\\n";
     --zeilen_passen_noch;
     os << "\\end{tabularx}\n";
   }
//  os << "\\end{flushright}\n";
  drucken_footer(os);
}


void LR_Abstraktion::drucken_table_header(ostream &os,
   	const cH_ExtBezSchema &schema, 
   	float preismenge, const string &preiseinheit)
{
  string ug ="\\footnotesize\\mygray "; // Größe der Überschriften
  string sg ="\\scriptsize\\mygray "; // Größe der Überschriften

  string tabcolumn;
  spaltenzahl=0;
   if (stueck_bool) { tabcolumn+="r"; spaltenzahl++; }
   if (menge_bool) { tabcolumn+="r"; spaltenzahl++; }
  if (menge_bool && stueck_bool) { tabcolumn+="r"; spaltenzahl++; }
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
      { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; }
  if (t==Lieferschein)  // Palette + Auftrag
  { tabcolumn+="rr"; spaltenzahl+=2; }
  if (preise_addieren && rabatt_bool)  // Rabatt
  {  tabcolumn+="r"; ++spaltenzahl;}
  if (preise_addieren)  // Einzelpreis, Gesamtpreis
  { tabcolumn+="rr"; spaltenzahl+=2; }
  else if (t==Auftrag) 
  { tabcolumn+="X"; spaltenzahl+=1; }

  os << "\\settowidth{\\breite}{"<<ug<<" Bezeichnung}%\n";
//  os << "\\vspace{-2ex}\n";
  os << "\\begin{tabularx}{18cm}{"<<tabcolumn<<"}"<<"\\\\\n";

  if (stueck_bool && menge_bool) os << "\\multicolumn{2}{c}{"<<ug<<"Menge} & ";
  else if (stueck_bool || menge_bool) os << "\\multicolumn{1}{c}{"<<ug<<"Menge} & ";
  if (stueck_bool && menge_bool)
    os << "\\multicolumn{1}{c}{"<<sg<<" {Gesamtmenge}}  & ";
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
    os << "\\mbox{"<<ug<<j->bezkomptext<<"}&";
  if (t==Lieferschein)
   {
     os <<"\\multicolumn{1}{c}{" <<ug<<"Palette} & ";
     os <<"\\multicolumn{1}{c}{" <<ug<<"Auftrag} ";
   }
  if (preise_addieren)
   { // Preis ausgeben
     os <<"\\multicolumn{1}{c}{"<<sg<<getWaehrung()->TeXsymbol();
     if (preismenge!=1 || !preiseinheit.empty()) os <<"\\,/";
     if (preismenge!=1) os << "\\,"<<preismenge;
     if (!preiseinheit.empty())  os << "\\,"<<preiseinheit;
     os  <<"} & ";
     if (rabatt_bool) os <<"\\multicolumn{1}{c}{"<<ug<<"Rabatt} & ";
     os <<"\\multicolumn{1}{c}{"<<sg<<"{Gesamtpreis}} "; 
   }
  os << "\\\\" "\\hline\n";
  --zeilen_passen_noch;
  zur_preisspalte="";
  for (unsigned int i=0;i<spaltenzahl-2;++i)  zur_preisspalte+='&';  
  tabellenbetrag=0;
}

void LR_Abstraktion::drucken_betrag(ostream &os,const string &text, fixedpoint<2> betrag)
{
  os << "\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}"<<"\n";
  os << zur_preisspalte << text << '&' << FormatiereTeX(betrag)<<"\\\\\n";
  --zeilen_passen_noch;
}


void LR_Abstraktion::page_header(ostream &os)
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
     os << kunde_von->LaTeX_von()<<"\n\n\\bigskip";
     os << kunde_an->LaTeX_an(typString())<<"\n\n\\bigskip";
     zeilen_passen_noch=ZEILEN_SEITE_1;
    }
   else 
    {
     os <<"\n\n\\vspace{1cm}\n";
     os <<"\\large Firma\n\n"<< kunde_an->getName()<<"\n\n\\bigskip";
     zeilen_passen_noch=ZEILEN_SEITE_N;
    }
   os << "\\LARGE "<<typString()<<' '<<RngNr()<<"\\hfill\\normalsize "
        <<(kopie?"Kopie, ":"")<<"Seite "<<page_counter<<"\\hfill "<< getDatum() << "\\\\\n\n";
   if (page_counter==1 && t==Lieferschein)
    { 
     os << "Wir sandten Ihnen auf Ihre Rechung und Gefahr\n";
     --zeilen_passen_noch; // immer besser
    }
   os <<"\\end{flushleft}\n";
  }
}


void LR_Abstraktion::lieferung_an(ostream &os, unsigned int lfrs_id, const Petig::Datum& datum,const string& sKunde)
{ --zeilen_passen_noch;
//  os << "\\begin{flushleft}\n";
  os << "\\normalsize\n";
  os << "~\\\\[2ex]""\n";  // ohne ~ gibt's  Error: There's no line here to end.
  --zeilen_passen_noch;
  os << "Lieferschein "<<lfrs_id;
  os << " lieferten wir am "<<datum;
  if(!sKunde.empty())  os << " an "<<sKunde ;
  os << "\\\\[-2ex]\n";
//  os << "\\end{flushleft}\n";
}
