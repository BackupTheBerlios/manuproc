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
   os <<    "\\usepackage{isolatin1,t1enc,german,helvetic,vmargin}\n";
   os <<    "\\usepackage{pstricks}\n";
   os <<    "\\renewcommand{\\rmdefault}{phv}\n";
   os <<    "\\setmarginsrb{2.25cm}{2.8cm}{3.25cm}{1in}{0pt}{0pt}{0pt}{0pt}\n";
   os <<    "\\DeclareFontFamily{T1}{peur}{}\n";
   os <<    "\\DeclareFontShape{T1}{peur}{m}{n}{<-> peurr}{}\n";
   os <<    "\\newcommand{\\nullpf}{--{}--}\n";
   os <<    "\\newlength{\\breite} \n";
//   os <<    "\\newrgbcolor{mygray}{0.5 1 0.75}";
   os <<    "\\newrgbcolor{mygray}{0.75 0.75 0.75}";
   os <<    "\n";
   os <<    "\\parindent0pt\n";
   os <<    "\\parskip0pt\n"  ;
   os <<    "\\setlength{\\topsep}{0pt}\n";
   os <<    "\\setlength{\\partopsep}{0pt}\n";
   os <<    "\\pagestyle{empty}\n";
   os <<    "\n";
   os <<    "\\begin{document}\n";
   os <<    "    \\vbox to 0pt{%\n";
   os <<    "     \\rule{0pt}{67.5truemm}% 62.5truemm > 80.0truemm\n";
   os <<    "     \\hbox to 0pt{\\hspace*{-24truemm}\\vbox{\\hrule width 7truemm}\\hss}%\n";
   os <<    "     \\vss}\\nointerlineskip\n";
   os <<    "\\vspace*{1.2cm}%\n";
}

void LR_Abstraktion::drucken_footer(ofstream &os)
{
  if (Was()=="Rechnung")
   {
     cH_Kunde kunde_von(1);
     os <<"\n\n\\footnotesize "<< kunde_von->getBank()<<"\\\\\n";
     os <<"Zahlung: Innerhalb von 10 Tagen 4\\% Skonto; 30 Tagen 2,5\\%; innerhalb von 60 Tagen netto\\\\\n";
     os <<"Die Lieferung erfolgt zu den Einheitsbedingungen der deutschen Texttilindustrie\\\\\n";
     os <<"Gerichtsstand ist Wuppertal\\\\\n";
   }
  os <<    "\\end{document}\n";
}


void LR_Abstraktion::drucken_table(ofstream &os,const string& kopie)
{
 unsigned int zeilen_count = 0;
 unsigned int page_count =1;
 unsigned int signifikanz=1; // Annahme!
 unsigned int spaltenzahl=0;
 unsigned int warengruppen_count=1;
 string waehrung;
 if (Was()=="Rechnung") waehrung = getWaehrung()->TeXsymbol();
 cH_ExtBezSchema schema_mem(ExtBezSchema::default_ID);
 vector<fixedpoint<2> > preissum_warengruppe(warengruppen_count);
 vector<fixedpoint<2> > preissum_zeile;
 Kunde::ID kunden_id = KdNr();

 drucken_header(os);
 page_header(1,os,kopie);

 os << "\\begin{flushright}\n";
 os << "\\normalsize\n";

 int lfrsid_mem=-1;
 for (LR_Abstraktion::const_iterator i=begin();i!=end();)
  { LR_Abstraktion::const_iterator j=i ;//,k(Rechnung);
    ArtikelBase artikel_id  = (*i).ArtikelID();
    cH_ArtikelBezeichnung bez(artikel_id,cH_Kunde(kunden_id)->getSchema()->Id());
    cH_ExtBezSchema schema_mem = bez->getExtBezSchema();
    float preismenge_mem;
    if ((*i).Lfrs_Id()!=lfrsid_mem) 
      { class Lieferschein l((*i).Lfrs_Id());
        string sKunde;
        cH_Kunde k(l.KdNr()); 
        if (l.KdNr() != kunden_id) sKunde = k->firma();
        else sKunde = "";
        lieferung_an(os,(*i).Lfrs_Id(),l.LsDatum(),sKunde); 
      }
    if (Was()=="Rechnung")
      {
       preismenge_mem = (*i).getPreis().PreisMenge();
       lfrsid_mem=(*i).Lfrs_Id();
      } 
    bool stueck_bool=false, menge_bool=false, rabatt_bool=false;
    for (;j!=end() ;++j) 
      {
        ArtikelBase artikel_id  = (*j).ArtikelID();
        cH_ArtikelBezeichnung bez(artikel_id,cH_Kunde(kunden_id)->getSchema()->Id());
        cH_ExtBezSchema schema = bez->getExtBezSchema();
        if (Was()=="Rechnung")       
         {
           if (schema!=schema_mem  || lfrsid_mem != (*j).Lfrs_Id() ||
               (*j).getPreis().PreisMenge() != preismenge_mem) break;
           if ((*j).Stueck()!=1) stueck_bool=true;
           if ((*j).Menge()!=0) menge_bool=true;
           if ((*j).Rabatt()!=0) rabatt_bool=true;
         }
        if (Was()=="Lieferschein")       
         {
           if (schema!=schema_mem ) break;
           if ((*j).Menge()!=0) menge_bool=true;
           if ((*j).Stueck()!=1) stueck_bool=true;
         }
      }

    spaltenzahl = drucken_table_header(os,schema_mem,
         signifikanz,stueck_bool,menge_bool,rabatt_bool,preismenge_mem,waehrung,Einheit(artikel_id));
    zeilen_count+=2;
    for (LR_Abstraktion::const_iterator k=i;k!=j;++k) // Zeilen ausgeben
      {
         ArtikelBase artikel_id  = (*k).ArtikelID();
         cH_ArtikelBezeichnung bez(artikel_id,cH_Kunde(kunden_id)->getSchema()->Id());
         string einheit = Einheit(artikel_id);
         ++zeilen_count;
         if (stueck_bool) os << FormatiereTeX((*k).Stueck()) <<" & " ;
         if (menge_bool) os << FormatiereTeX((*k).Menge()) <<einheit<<" & " ;
         if (stueck_bool && menge_bool)
            os << FormatiereTeX((*k).Stueck() * (*k).Menge())<<einheit<<" & ";   
         for(ExtBezSchema::const_sigiterator l=schema_mem->sigbegin(signifikanz);l!=schema_mem->sigend(signifikanz);++l)
          os << string((*bez)[l->bezkomptype]) <<"  &  ";
         if(Was()=="Rechnung")       
          {
            os <<FormatiereTeX2( (*k).getPreis().Wert() )<<" & ";
            if (rabatt_bool) os<<FormatiereTeX2((*k).Rabatt())<<" & ";
            fixedpoint<2> preis = (*k).getPreis().Gesamtpreis(getWaehrung(),(*k).Stueck(),(*k).Menge(),(*k).Rabatt());
            preissum_zeile.push_back(  preis  ) ;
            os << FormatiereTeX2(preissum_zeile[preissum_zeile.size()-1]) <<"\\\\\n";
          }
        if (Was()=="Lieferschein") 
          {
            if ((*k).Palette()!=0) os <<(*k).Palette();
            os<<" & " << (*k).YourAuftrag()<<"\n";   
            os<<"\\\\\n";
          }
         if (zeilen_count>=ZEILEN_SEITE_1 && page_count==1
           ||zeilen_count>=ZEILEN_SEITE_N && page_count!=1 )
          {
            fixedpoint<2> vortrag;
            if (Was()=="Rechnung")
               vortrag = drucken_table_preissum_warengruppe(os,
                  preissum_zeile,preissum_warengruppe,spaltenzahl,"Übertrag");
            os << "\\end{tabular}\n";
            os << "\\newpage\n";
            ++page_count;
            page_header(page_count,os,kopie);
            spaltenzahl = drucken_table_header(os,schema_mem,signifikanz,stueck_bool,menge_bool,rabatt_bool,preismenge_mem,waehrung,einheit);
            for (unsigned int i=0;i<spaltenzahl-2;++i) os << "&";
            if (Was()=="Rechnung")
               os << "Vortrag & "<< FormatiereTeX2(vortrag) <<"\\\\";
            zeilen_count=3;
          }

      }
   if (Was()=="Rechnung")
    {
      drucken_table_preissum_warengruppe(os,
            preissum_zeile,preissum_warengruppe,spaltenzahl);
      zeilen_count+=3;
      preissum_zeile.clear();
      preissum_warengruppe.resize(++warengruppen_count);
    }
   os << "\\end{tabular}\n\n";
   i=j;
  }

  if (Was()=="Rechnung") 
   {
     os << "\\bigskip\\begin{tabular}{rrr}\n";
     fixedpoint<2> endsumme = 0.00;
     for (unsigned int i=0;i<preissum_warengruppe.size();++i) 
         endsumme+=preissum_warengruppe[i];
     os << "&Endsumme    & "<< FormatiereTeX2(endsumme)     <<"\\\\\n";
     fixedpoint<2> kunden_rabatt = Rabatt();
     if (kunden_rabatt!=0.00)
      {
        fixedpoint<2> endsumme_rabatt_ = endsumme*kunden_rabatt;
        os << "&./. "<<kunden_rabatt<<"\\%   & "<< FormatiereTeX2(endsumme_rabatt_) <<"\\\\\\cline{3-3}\n";
        endsumme-=endsumme_rabatt_;
        os <<         "&    & "<< FormatiereTeX2(endsumme)<<"\\\\\n";
      }
     fixedpoint<2> endsumme_MwSt_ = endsumme*0.16;
     os << "&+ 16\\% MwSt& "<< FormatiereTeX2(endsumme_MwSt_) <<"\\\\\\cline{1-1}\\cline{3-3}\n";
     endsumme+=endsumme_MwSt_;
     fixedpoint<2> andere_endsumme;
     cP_Waehrung andere_waehrung;
     if (getWaehrung() == cP_Waehrung(Waehrung::DM)) 
          andere_waehrung = cP_Waehrung(Waehrung::EUR);
     else andere_waehrung = cP_Waehrung(Waehrung::DM);
     andere_endsumme = endsumme*Waehrung::Umrechnung(*getWaehrung(),*andere_waehrung);
   
     os <<andere_waehrung->TeXsymbol()<<" "<< FormatiereTeX2( andere_endsumme ) <<"&"
         <<"\\hspace*{8cm} "<<waehrung<<"&" << FormatiereTeX2(endsumme)<<"\\\\";
     os <<"\\cline{1-1}\\cline{3-3}\\\\[-2.5ex]\\cline{1-1}\\cline{3-3}\\\\\n";
      
     os << "\\end{tabular}\n";
   }
  os << "\\end{flushright}\n";
  drucken_footer(os);
}


unsigned int LR_Abstraktion::drucken_table_header(ofstream &os,
   cH_ExtBezSchema& schema,
      unsigned int signifikanz,bool stueck_bool,bool menge_bool,bool rabatt_bool,
      float preismenge, string waehrung, string einheit)
{
  string ug ="\\footnotesize\\mygray "; // Größe der Überschriften
  string sg ="\\scriptsize\\mygray "; // Größe der Überschriften
  string tabcolumn="";
  unsigned int spaltenzahl=2;
  if (stueck_bool) { tabcolumn+="r"; spaltenzahl+=1; }
  if (menge_bool) { tabcolumn+="rr"; spaltenzahl+=2; }
  for (unsigned int i=0;i<schema->size(signifikanz);++i) 
      { tabcolumn += "c" ; ++spaltenzahl ; }
  tabcolumn+="rr";
  if(Was()=="Rechnung") 
     if (rabatt_bool) {tabcolumn+="r";++spaltenzahl;}

//  os << "\\begin{tabular}{"<<tabcolumn<<"}"<<"\\\\\\hline\n";
  os << "\\begin{tabular}{"<<tabcolumn<<"}"<<"\\\\\n";
  if (stueck_bool) os <<" &";
  if (menge_bool) os << "&";
  if (stueck_bool && menge_bool) 
    os << "\\multicolumn{1}{c}{"<<sg<<" Gesamt-}  & ";
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
       os <<"&";
//  os <<" &";
  if (Was()=="Rechnung")
   {
     if (rabatt_bool)  os <<" & ";
      os << "&\\multicolumn{1}{c}{"<<sg<<" Gesamt-}";
   }
  os <<" \\\\[-1ex]\n"; 
  if (stueck_bool)
    os << "\\multicolumn{1}{c}{"<<ug<<"\\raisebox{1.0ex}[-1.5ex]{ Stück}}  &  ";
  if (menge_bool) 
    os << "\\multicolumn{1}{c}{"<<ug<<"\\raisebox{1.0ex}[-1.5ex]{ Menge}} & ";
  if (stueck_bool && menge_bool)
    os << "\\multicolumn{1}{c}{"<<sg<<" menge}  & ";
  for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz);j!=schema->sigend(signifikanz);++j)
        os <<ug<<"\\raisebox{1.0ex}[-1.5ex]{ "<<j->bezkomptext<<"}\t&\t";
  if (Was()=="Rechnung")
   {
     os <<"\\multicolumn{1}{c}{\\raisebox{1.0ex}[-1.5ex]{"<<sg<<waehrung;
     if (preismenge!=1 || einheit!="") os <<"\\,/";
     if (preismenge!=1) os << "\\,"<<preismenge;
     if (einheit !="")  os << "\\,"<<einheit;
     os  <<"}} & ";
     if (rabatt_bool) os <<"\\multicolumn{1}{c}{"<<ug<<"\\raisebox{1.0ex}[-1.5ex]{ Rabatt}} & ";
     os <<"\\multicolumn{1}{c}{"<<sg<<"preis}\\\\\\hline\n"; 
   }
  if (Was()=="Lieferschein")
   {
     os <<"\\multicolumn{1}{c}{" <<ug<<"\\raisebox{1.0ex}[-1.5ex]{ Palette}} & ";
     os <<"\\multicolumn{1}{c}{" <<ug<<"\\raisebox{1.0ex}[-1.5ex]{ Auftrag}} \\\\\\hline\n";
   }
  return spaltenzahl;
}

fixedpoint<2> LR_Abstraktion::drucken_table_preissum_warengruppe(ofstream &os,
   vector<fixedpoint<2> >& preissum_zeile, vector<fixedpoint<2> >& preissum_warengruppe,
   unsigned int spaltenzahl,string text)
{
  for (unsigned int i=0;i<preissum_zeile.size();++i)
      preissum_warengruppe[preissum_warengruppe.size()-1]+=preissum_zeile[i];
  os << "\\cline{"<<spaltenzahl<<"-"<<spaltenzahl<<"}"<<"\n";
  for (unsigned int i=0;i<spaltenzahl-2;++i) os << "&";
  if (text!="") os << text;
  os <<"&"<<FormatiereTeX2(preissum_warengruppe[preissum_warengruppe.size()-1])<<"\\\\\n";
  return preissum_warengruppe[preissum_warengruppe.size()-1];
}


void LR_Abstraktion::page_header(int page,ofstream &os,const string& kopie)
{
 cH_Kunde kunde_an(KdNr());
 os <<"\\begin{flushleft}\n";
 if (page==1)
  {
   cH_Kunde kunde_von(1);
   os << kunde_von->LaTeX_von()<<"\n\n\\bigskip";
   os << kunde_an->LaTeX_an(Was())<<"\n\n\\bigskip";
  }
 else 
  {
   os <<"\n\n\\vspace{1cm}\n";
   os <<"\\large Firma\n\n"<< kunde_an->getName()<<"\n\n\\bigskip";
  }
 os << "\\LARGE "<<Was()<<": "<<RngNr()<<"\\hfill\\normalsize "
      <<kopie<<"\\hfill \\today\\\\[1ex]\n\n";
 if (page==1)
  {
   os << "\\scriptsize Wir sandten Ihnen auf Ihre Rechung und Gefahr\n";
  }
 os <<"\\end{flushleft}\n";
}


void LR_Abstraktion::lieferung_an(ofstream &os, unsigned int lfrs_id, const Petig::Datum& datum,const string& sKunde)
{
  os << "\\begin{flushleft}\n\\normalsize\n";
  os << "Lieferschein "<<lfrs_id;
  os << " lieferten wir am "<<datum;
  if(sKunde!="")  os << " an "<<sKunde;
  os << "\\end{flushleft}\n";
}
