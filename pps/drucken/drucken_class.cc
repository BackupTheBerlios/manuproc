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

void LR_Abstraktion::drucken_header(ofstream &os)
{

 os << "\\documentclass[12pt,a4paper]{article}\n";
#ifdef MABELLA_EXTENSIONS
 os << 	"\\usepackage{isolatin1,t1enc,german,times,vmargin,color}\n"
	"\\usepackage{epsfig}\n"
	"\\usepackage{graphics}\n"
	"\\setmarginsrb{2.00cm}{2.8cm}{1.25cm}{1in}{0pt}{0pt}{0pt}{0pt}\n"
	"\\definecolor{backg}{gray}{0.985}\n"
	"\\newcommand{\\shaderow}{\\raisebox{-1pt}[0pt][0pt]{\\makebox[0pt][l]{\\kern-70pt\\colorbox{backg}%%\n"
		"{\\rule{0pt}{9pt}\\rule{\\paperwidth}{0pt}}}}}\n";
#else
 os << "\\usepackage{isolatin1,t1enc,german,helvetic,vmargin}\n";
 os << "\\setmarginsrb{2.25cm}{2.8cm}{3.25cm}{1in}{0pt}{0pt}{0pt}{0pt}\n";
#endif

   os <<    "\\usepackage{pstricks}\n";
   os <<    "\\usepackage{tabularx}\n";
   os <<    "\\usepackage{eurosym}\n";
   os <<    "\\renewcommand{\\rmdefault}{phv}\n";
   os <<    "\\newrgbcolor{mygray}{0.75 0.75 0.75}\n";
   os <<    "\\newlength{\\breite}\n";
   os <<    "\n";
   os <<    "\\parindent0pt\n";
   os <<    "\\parskip0pt\n"  ;
   os <<    "\\setlength{\\topsep}{0pt}\n";
   os <<    "\\setlength{\\partopsep}{0pt}\n";
   os <<    "\\setlength{\\textheight}{27.0cm}";
   os <<    "\\pagestyle{empty}\n";
   os <<    "\n";
   os <<    "\\begin{document}\n";
   os <<    "    \\vbox to 0pt{%\n";
   os <<    "     \\rule{0pt}{67.5truemm}% 62.5truemm > 80.0truemm\n";
   os <<    "     \\hbox to 0pt{\\hspace*{-24truemm}\\vbox{\\hrule width 7truemm}\\hss}%\n";
   os <<    "     \\vss}\\nointerlineskip\n";
   os <<    "\\vspace*{1.2cm}%\n";

#ifdef MABELLA_EXTENSIONS 
if(!firmenpapier)
  os << "\\raisebox{120pt}[0pt][0pt]{\\makebox[0pt][l]{\\kern+140pt\\psfig{file=/tmp/logo.eps}}}\n";
#endif

}

void LR_Abstraktion::drucken_footer(ofstream &os)
{
  if (t==Rechnung)
   {
     cH_Kunde kunde_von(1);
     os <<"\n\n\\footnotesize "<< kunde_von->getBank()<<"\\\\\n";
     os <<"Zahlung: Innerhalb von 10 Tagen 4\\% Skonto; 30 Tagen 2,5\\%; innerhalb von 60 Tagen netto\\\\\n";
     os <<"Die Lieferung erfolgt zu den Einheitsbedingungen der deutschen Texttilindustrie\\\\\n";
     os <<"Gerichtsstand ist Wuppertal\\\\\n";
   }
  os <<    "\\end{document}\n";
}


void LR_Abstraktion::drucken_table(ofstream &os,const string& kopie,const cH_ppsInstanz& instanz)
{
 unsigned int zeilen_count = 0;
 unsigned int page_count =1;
 unsigned int signifikanz=1; // Annahme!
 unsigned int spaltenzahl=0;
 unsigned int warengruppen_count=1;
 string waehrung;
 if (t==Rechnung) waehrung = getWaehrung()->TeXsymbol();
 cH_ExtBezSchema schema_mem(ExtBezSchema::default_ID);
 vector<fixedpoint<2> > preissum_warengruppe(warengruppen_count);
 vector<fixedpoint<2> > preissum_zeile;
 Kunde::ID kunden_id = KdNr();

 drucken_header(os);
 page_header(1,os,kopie,instanz);

 os << "\\begin{flushright}\n";
 os << "\\normalsize\n";

 int lfrsid_mem=-1;
//cout << "\n\nSIZE = "<<size()<<"\n\n\n";
 for (LR_Abstraktion::const_iterator i=begin();i!=end();)
  { LR_Abstraktion::const_iterator j=i ;
    ArtikelBase artikel_id  = (*i).ArtikelID();
    cH_ArtikelBezeichnung bez(artikel_id,cH_Kunde(kunden_id)->getSchema()->Id());
    cH_ExtBezSchema schema_mem = bez->getExtBezSchema();
    Preis::preismenge_t preismenge_mem;
    Einheit einheit_mem = Einheit(artikel_id); 
    unsigned int tabellen_zeile=0;

    if (t==Auftrag && (*i).Lfrs_Id()!=lfrsid_mem ) 
      { class Lieferschein l((*i).Lfrs_Id());
        string sKunde;
        cH_Kunde k(l.KdNr()); 
        if (l.KdNr() != kunden_id) sKunde = k->firma();
        else sKunde = "";
        if (t==Rechnung)
       	   lieferung_an(os,(*i).Lfrs_Id(),l.getDatum(),sKunde); 
        ++zeilen_count;    
        ++tabellen_zeile;
      }
    if (t==Rechnung)
      {
       preismenge_mem = (*i).getPreis().PreisMenge();
       lfrsid_mem=(*i).Lfrs_Id();
      } 
    if (t==Auftrag)
      {
       preismenge_mem = (*i).getPreis().PreisMenge();
      } 
    bool stueck_bool=false, menge_bool=false, rabatt_bool=false;
    for (;j!=end() ;++j) 
      {
        ArtikelBase artikel_id  = (*j).ArtikelID();
        cH_ArtikelBezeichnung bez(artikel_id,cH_Kunde(kunden_id)->getSchema()->Id());
        cH_ExtBezSchema schema = bez->getExtBezSchema();
        if (t==Rechnung)       
         {
           if (schema!=schema_mem  || lfrsid_mem != (*j).Lfrs_Id() ||
               (*j).getPreis().PreisMenge() != preismenge_mem ||
               Einheit(artikel_id) != einheit_mem ) break;
           if ((*j).Stueck()!=1) stueck_bool=true;
           if ((*j).Menge()!=fixedpoint<2>(0)) menge_bool=true;
           if ((*j).Rabatt()!=fixedpoint<2>(0)) rabatt_bool=true;
         }
        else if (t==Auftrag)       
         {
           if (schema!=schema_mem  || 
               (*j).getPreis().PreisMenge() != preismenge_mem ||
               Einheit(artikel_id) != einheit_mem ) break;
           if ((*j).Stueck()!=1) stueck_bool=true;
//           if ((*j).Menge()!=fixedpoint<2>(0)) menge_bool=true;
           if ((*j).Rabatt()!=fixedpoint<2>(0)) rabatt_bool=true;
         }
        else if (t==Lieferschein)       
         {
           if (schema!=schema_mem || Einheit(artikel_id) != einheit_mem ) break;
           if ((*j).Menge()!=fixedpoint<2>(0)) menge_bool=true;
           if ((*j).Stueck()!=1) stueck_bool=true;
         }
        else abort();
      }
     spaltenzahl = drucken_table_header(os,schema_mem,
       signifikanz,stueck_bool,menge_bool,rabatt_bool,preismenge_mem,waehrung,Einheit(artikel_id).TeX(),instanz);
    zeilen_count+=1;
    ++tabellen_zeile;
    string einheitsize = "\\scriptsize \\,";
    for (LR_Abstraktion::const_iterator k=i;k!=j;++k) // Zeilen ausgeben
      {
        if (zeilen_count-1>=ZEILEN_SEITE_1 && page_count==1
           ||zeilen_count-1>=ZEILEN_SEITE_N && page_count!=1 )
          {
            fixedpoint<2> vortrag;
            if (t==Rechnung && tabellen_zeile)
               vortrag = drucken_table_preissum_warengruppe(os,
                  preissum_zeile,preissum_warengruppe,spaltenzahl,"Übertrag");
            os << "\\end{tabularx}\n";
            os << "\\newpage\n";
            ++page_count;
            page_header(page_count,os,kopie,instanz);
            spaltenzahl = drucken_table_header(os,schema_mem,signifikanz,stueck_bool,menge_bool,rabatt_bool,preismenge_mem,waehrung,Einheit(artikel_id).TeX(),instanz);
            tabellen_zeile=0;
            ++zeilen_count;
            if (t==Rechnung)
	        {	
   	       for (unsigned int i=0;i<spaltenzahl-2;++i) os << "&";
                  os << "Vortrag & "<< FormatiereTeX(vortrag) <<"\\\\";
               // Preissumme wg Übertrag zurücksetzen
             preissum_warengruppe[preissum_warengruppe.size()-1] = 0.;
   	     }
            zeilen_count=0; // Zurücksetzen für neue Seite 
          }

         ArtikelBase artikel_id  = (*k).ArtikelID();
     	   string linecolor = "";
         if ((*k).ZusatzInfo()) 
	      linecolor = "\\mygray";
         cH_ArtikelBezeichnung bez(artikel_id,cH_Kunde(kunden_id)->getSchema()->Id());
//         string einheit = Einheit(artikel_id);
         ++zeilen_count;
         ++tabellen_zeile;
//os << zeilen_count<<"  ";
        if(t!=Auftrag)
          {if (stueck_bool && (!(*k).ZusatzInfo() || (*k).Stueck()!=1))
            {  os <<linecolor ;
             	// if ((*k).ZusatzInfo()) os << '(';
              	os << FormatiereTeX((*k).Stueck());
           	   if (Einheit(artikel_id).StueckEinheit().size())
              	   os <<'{'<< einheitsize <<Einheit(artikel_id).StueckEinheit() <<'}';
           	//   if ((*k).ZusatzInfo()) os << ')';
            }
            if (stueck_bool) os <<" & " ;         
            if (menge_bool) 
               { if (!(*k).ZusatzInfo()) os <<linecolor<< FormatiereTeX_short((*k).Menge())<<einheitsize <<Einheit(artikel_id).MengenEinheit() ;
                 os <<" & " ;
               }
           }
         if (stueck_bool && menge_bool)
           { if (t==Auftrag) os << linecolor << FormatiereTeX_short(fixedpoint<3>((*k).Stueck() ))<<einheitsize <<Einheit(artikel_id).TeX();
             else os <<linecolor<< FormatiereTeX_short(fixedpoint<3>((*k).Stueck() * (*k).Menge()))<<einheitsize <<Einheit(artikel_id).TeX();
             os <<" & ";
           }
	 for(ExtBezSchema::const_sigiterator l=schema_mem->sigbegin(signifikanz);l!=schema_mem->sigend(signifikanz);++l)
	   { if ((*k).ZusatzInfo()) os << "&";
             else os <<"{"<<linecolor<< Gtk2TeX::string2TeX((*bez)[l->bezkomptype]->getStrVal()) <<"}&";
	   }
         if(t==Rechnung)       
          {
            os <<linecolor<<FormatiereTeX_Preis( (*k).getPreis().Wert() )<<" & ";
            if (rabatt_bool) os<<linecolor<<FormatiereTeX((*k).Rabatt())<<" & ";
            fixedpoint<2> preis = (*k).getPreis().Gesamtpreis(getWaehrung(),(*k).Stueck(),(*k).Menge(),(*k).Rabatt());
            preissum_zeile.push_back(  preis  ) ;
            os <<linecolor<< FormatiereTeX(preissum_zeile[preissum_zeile.size()-1]);
          }
         else if(t==Auftrag && instanz->Id()==ppsInstanz::INST_KNDAUF)       
          {
            os <<linecolor<<FormatiereTeX_Preis( (*k).getPreis().Wert() )<<" & ";
            if (rabatt_bool) os<<linecolor<<FormatiereTeX((*k).Rabatt())<<" & ";
            fixedpoint<2> preis = (*k).getPreis().Gesamtpreis(getWaehrung(),(*k).Stueck(),(*k).Menge(),(*k).Rabatt());
            preissum_zeile.push_back(  preis  ) ;
            os <<linecolor<< FormatiereTeX(preissum_zeile[preissum_zeile.size()-1]);
          }
        else if (t==Lieferschein) 
          {
            if ((*k).Palette()!=0) os <<linecolor<<(*k).Palette();
            os<<" & "<<linecolor << (*k).YourAuftrag()<<"\n";   
          }
        os<<"\\\\\n";
      }
   if (t==Rechnung)
    {
      drucken_table_preissum_warengruppe(os,
            preissum_zeile,preissum_warengruppe,spaltenzahl);
      zeilen_count+=2;
      preissum_zeile.clear();
//cout << "preissum_warengruppe = "<< preissum_warengruppe[preissum_warengruppe.size()-1]<<"\n";
      preissum_warengruppe.resize(++warengruppen_count);
    }
   os << "\\end{tabularx}\n\n";
   i=j;
  }

  if (t==Rechnung) 
   {
     os << "\\bigskip\\begin{tabularx}{18cm}{rrr}\n";
//     os << "\\begin{tabularx}{18cm}{rrr}\n";
     fixedpoint<2> endsumme = 0.00;
     for (unsigned int i=0;i<preissum_warengruppe.size();++i) 
         endsumme+=preissum_warengruppe[i];
     os << "&Endsumme    & "<< FormatiereTeX(endsumme)     <<"\\\\\n";
     fixedpoint<2> kunden_rabatt = Rabatt();
     if (kunden_rabatt>0.0)
      {
        fixedpoint<2> endsumme_rabatt_ = endsumme*kunden_rabatt/100.0;
        os << "&./. "<<FormatiereTeX_short(kunden_rabatt)<<"\\%   & "<< FormatiereTeX(endsumme_rabatt_) <<"\\\\\\cline{3-3}\n";
        endsumme-=endsumme_rabatt_;
        os <<         "&    & "<< FormatiereTeX(endsumme)<<"\\\\\n";
      }
     else if (kunden_rabatt<0.0)
      {
        fixedpoint<2> endsumme_aufschlag_ = endsumme*(-kunden_rabatt)/100.0;
        os << "&Aufschlag "<<FormatiereTeX_short(-kunden_rabatt)<<"\\%   & "<< FormatiereTeX(endsumme_aufschlag_) <<"\\\\\\cline{3-3}\n";
        endsumme+=endsumme_aufschlag_;
        os <<         "&    & "<< FormatiereTeX(endsumme)<<"\\\\\n";
      }
     fixedpoint<2> endsumme_MwSt_ = endsumme*0.16;
     os << "&+ 16\\% MwSt& "<< FormatiereTeX(endsumme_MwSt_) <<"\\\\\\cline{1-1}\\cline{3-3}\n";
     endsumme+=endsumme_MwSt_;
     fixedpoint<2> andere_endsumme;
     cP_Waehrung andere_waehrung;
     if (getWaehrung() == cP_Waehrung(Waehrung::DM)) 
          andere_waehrung = cP_Waehrung(Waehrung::EUR);
     else andere_waehrung = cP_Waehrung(Waehrung::DM);
     andere_endsumme = endsumme*Waehrung::Umrechnung(*getWaehrung(),*andere_waehrung);
   
     os <<andere_waehrung->TeXsymbol()<<" "<< FormatiereTeX( andere_endsumme ) <<"&"
         <<"\\hspace*{8cm} "<<waehrung<<"&" << FormatiereTeX(endsumme)<<"\\\\";
     os <<"\\cline{1-1}\\cline{3-3}\\\\[-2.5ex]\\cline{1-1}\\cline{3-3}\\\\\n";
      
     os << "\\end{tabularx}\n";
   }
  os << "\\end{flushright}\n";
  drucken_footer(os);
}


unsigned int LR_Abstraktion::drucken_table_header(ofstream &os,
   cH_ExtBezSchema& schema,
      unsigned int signifikanz,bool stueck_bool,bool menge_bool,bool rabatt_bool,
      float preismenge, string waehrung, string einheit,const cH_ppsInstanz& instanz)
{
  string ug ="\\footnotesize\\mygray "; // Größe der Überschriften
  string sg ="\\scriptsize\\mygray "; // Größe der Überschriften
  string tabcolumn="";
  unsigned int spaltenzahl=2;
  if(t!=Auftrag)
   { if (stueck_bool) { tabcolumn+="r"; spaltenzahl+=1; }
     if (menge_bool) { tabcolumn+="r"; spaltenzahl+=1; }
   }
  if (menge_bool && stueck_bool) { tabcolumn+="r"; spaltenzahl+=1; }
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
//  for (unsigned int i=0;i<schema->size(signifikanz);++i) 
      { tabcolumn += j->TeXtabformat ; ++spaltenzahl ; }
//      { tabcolumn += "X" ; ++spaltenzahl ; }
  tabcolumn+="rr";
  if(t==Rechnung || t==Auftrag) 
     if (rabatt_bool) {tabcolumn+="X";++spaltenzahl;}

  os << "\\settowidth{\\breite}{"<<ug<<" Bezeichnung}";
  os << "\n\\vspace{-2ex}\n";
  os << "\\begin{tabularx}{18cm}{"<<tabcolumn<<"}"<<"\\\\\n";

  if(t!=Auftrag)
   { if (stueck_bool)  os << "\\multicolumn{1}{c}{"<<ug<<"Stück}  &  ";
     if (menge_bool)   os << "\\multicolumn{1}{c}{"<<ug<<"Menge} & ";
   }
  if (stueck_bool && menge_bool)
    os << "\\multicolumn{1}{c}{"<<sg<<" {Gesamtmenge}}  & ";
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
        os << "\\mbox{"<<ug<<j->bezkomptext<<"}&";
  if (t==Rechnung|| (t==Auftrag && instanz->Id()==ppsInstanz::INST_KNDAUF))
   {
     os <<"\\multicolumn{1}{c}{"<<sg<<waehrung;
     if (preismenge!=1 || einheit!="") os <<"\\,/";
     if (preismenge!=1) os << "\\,"<<preismenge;
     if (einheit !="")  os << "\\,"<<einheit;
     os  <<"} & ";
     if (rabatt_bool) os <<"\\multicolumn{1}{c}{"<<ug<<"Rabatt} & ";
     os <<"\\multicolumn{1}{c}{"<<sg<<"{Gesamtpreis}} "; 
   }
  if (t==Lieferschein)
   {
     os <<"\\multicolumn{1}{c}{" <<ug<<"Palette} & ";
     os <<"\\multicolumn{1}{c}{" <<ug<<"Auftrag} ";
   }
  os << "\\\\\\hline\n";
  return spaltenzahl;
}

fixedpoint<2> LR_Abstraktion::drucken_table_preissum_warengruppe(ofstream &os,
   vector<fixedpoint<2> >& preissum_zeile, vector<fixedpoint<2> >& preissum_warengruppe,
   unsigned int spaltenzahl,string text)
{
  for (unsigned int i=0;i<preissum_zeile.size();++i)
{
      preissum_warengruppe[preissum_warengruppe.size()-1]+=preissum_zeile[i];

//cout << preissum_zeile[i] <<"\t"<<preissum_warengruppe[preissum_warengruppe.size()-1]<<"\n";
}
//cout <<"\n";

  os << "\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}"<<"\n";
  for (unsigned int i=0;i<spaltenzahl-2;++i) os << "&";
  if (text!="") os << text;
  os <<"&"<<FormatiereTeX(preissum_warengruppe[preissum_warengruppe.size()-1])<<"\\\\\n";
  return preissum_warengruppe[preissum_warengruppe.size()-1];
}


void LR_Abstraktion::page_header(int page,ofstream &os,const string& kopie,const cH_ppsInstanz& instanz)
{
 if(instanz->Id()!=ppsInstanz::INST_KNDAUF)
  {
   os <<"\\begin{flushleft}\n";
   os <<"Auftrag an "+instanz->get_Name()<<"\\\\\n";
   os <<"\\end{flushleft}\n";
  }
 else
  {
   cH_Kunde kunde_an(KdNr());
   os <<"\\begin{flushleft}\n";
   if (page==1)
    {
     cH_Kunde kunde_von(1);
     os << kunde_von->LaTeX_von()<<"\n\n\\bigskip";
     os << kunde_an->LaTeX_an(typString())<<"\n\n\\bigskip";
    }
   else 
    {
     os <<"\n\n\\vspace{1cm}\n";
     os <<"\\large Firma\n\n"<< kunde_an->getName()<<"\n\n\\bigskip";
    }
   os << "\\LARGE "<<t<<": "<<RngNr()<<"\\hfill\\normalsize "
        <<kopie<<" Seite "<<page<<"\\hfill "<< getDatum() << "\\\\[1ex]\n\n";
   if (page==1 && t!=Auftrag)
    {
     os << "\\scriptsize Wir sandten Ihnen auf Ihre Rechung und Gefahr\n";
    }
   os <<"\\end{flushleft}\n";
  }
}


void LR_Abstraktion::lieferung_an(ofstream &os, unsigned int lfrs_id, const Petig::Datum& datum,const string& sKunde)
{
  os << "\\begin{flushleft}\n\\normalsize\n";
  os << "Lieferschein "<<lfrs_id;
  os << " lieferten wir am "<<datum;
  if(sKunde!="")  os << " an "<<sKunde;
  os << "\\end{flushleft}\n";
}
