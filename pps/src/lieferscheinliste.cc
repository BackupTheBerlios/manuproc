// generated 2001/9/13 14:54:32 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to lieferscheinliste.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "lieferscheinliste.hh"
#include "lieferscheinliste_classes.hh"
#include <Lieferschein/LieferscheinVoll.h>
#include <Lieferschein/Rechnung.h>
//#include <Artikel/ArtikelBezeichnung.h>
#include <gtk--/main.h>
#include <Gtk2TeX.h>
#include <cstdio>
#include <fstream.h>


void lieferscheinliste::kundenbox_activate()
{   
 kundenid = kundenbox->get_value();
 radiobutton_nur_kunde->set_active(true);
}

void lieferscheinliste::artikelbox_activate()
{   
 artbase.setID(artikelbox->get_value().Id()); 
 radiobutton_nur_artikel->set_active(true);
}

void lieferscheinliste::datum_von_activate()
{   
// datum_von = Wdatum_von->get_value();
}
void lieferscheinliste::datum_bis_activate()
{   
// datum_bis = Wdatum_bis->get_value();
}

void lieferscheinliste::on_radiobutton_zeit_toggled()
{
 if (radiobutton_alle_zeit_von->get_active())
  {
    Wdatum_von->hide();
  }
 if (radiobutton_nur_zeit_von->get_active())
  {
    Petig::Datum d(1,1,1970);
    Wdatum_von->show();
    Wdatum_von->set_value(d);
  }
}
void lieferscheinliste::on_radiobutton_zeit_2_toggled()
{
 if (radiobutton_alle_zeit_bis->get_active())
  {
    Wdatum_bis->hide();
  }
 if (radiobutton_nur_zeit_bis->get_active())
  {
    Petig::Datum d(Petig::Datum::today());
    Wdatum_bis->show();
    Wdatum_bis->set_value(d);
  }
}
void lieferscheinliste::on_radiobutton_artikel_toggled()
{
 if (radiobutton_alle_artikel->get_active())
   {  artikelbox->hide();
      artbase.setID(0);
   }
 if (radiobutton_nur_artikel->get_active())
   {  
      artikelbox->reset();
      artikelbox->grab_focus();
      artikelbox->show();
   }
}
void lieferscheinliste::on_radiobutton_kunde_toggled()
{
 if (radiobutton_alle_kunden->get_active())
   {  kundenbox->hide();
      kundenid = 0;
   }
 if (radiobutton_nur_kunde->get_active())
   {  
      kundenbox->reset();
      kundenbox->grab_focus();
      kundenbox->show();
   }
}


lieferscheinliste::lieferscheinliste(const cH_ppsInstanz& _instanz)
: instanz(_instanz)
{
  label_anzahl->hide();
  Wdatum_von->setLabel("");
  Wdatum_bis->setLabel("");
  Wdatum_von->hide();
  Wdatum_bis->hide();
  artikelbox->hide();
  kundenbox->hide();
  set_titles();
}

void lieferscheinliste::on_button_show_clicked()
{
  tree->freeze();
  try {
     if(artbase.Id() ) 
       LL = new LieferscheinList(instanz,LieferscheinList::sel_ArtikelId(artbase.Id()));
     else if (kundenid)
       LL = new LieferscheinList(instanz,LieferscheinList::sel_KundenId(kundenid));
     else 
       LL = new LieferscheinList(instanz);
   } catch (SQLerror &e) {std::cerr << e<<'\n';} 
  fill_tree();
  tree->thaw();
}

void lieferscheinliste::fill_tree()
{
  progressbar->set_show_text(true);
  tree->clear();
// tree->freeze();
  tree->detach_from_clist();
  std::vector<cH_RowDataBase> datavec;
  double size=LL->Size();
  double count=0;

  Petig::Datum datum_von = Wdatum_von->get_value();
  Petig::Datum datum_bis = Wdatum_bis->get_value();
  for (LieferscheinList::const_iterator i=LL->begin();i!=LL->end();++i)
   {
     LieferscheinVoll LV(instanz,(*i)->Id());
     Rechnung R;
     if((*i)->RngNr()) R = Rechnung((*i)->RngNr());
     cH_Lieferschein L(new Lieferschein(instanz,(*i)->Id()));
     for (LieferscheinVoll::const_iterator j=LV.begin();j!=LV.end();++j)
      {
        if (  (artbase.Id()==0 || artbase.Id()==j->ArtikelID())
            &&(radiobutton_alle_zeit_von->get_active() || datum_von < L->LsDatum())
            &&(radiobutton_alle_zeit_bis->get_active() || datum_bis > L->LsDatum())
            &&(kundenid==0 || kundenid==(*i)->KdNr() ) ) 
         {
           datavec.push_back(new Data_LListe(L,*j,R));
         }
      }
      progressbar->set_percentage(count/size);
      while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
      ++count;
   }
 label_anzahl->set_text("Insgesamt "+itos(datavec.size())+" Artikel");
 label_anzahl->show();
 tree->setDataVec(datavec);
// tree->thaw();
 tree->attach_to_clist();
 progressbar->set_percentage(1);
 progressbar->set_show_text(false);
}


void lieferscheinliste::set_titles()
{
  std::vector<std::string> t;
  t.push_back("Kunde");
  t.push_back("Artikel");
  t.push_back("Breite");
  t.push_back("Farbe");
  t.push_back("Aufm.");
//  t.push_back("Stück");
//  t.push_back("Menge");
  t.push_back("Lieferschein");
  t.push_back("Lieferdatum");
  t.push_back("geliefert am");
  t.push_back("Rechnung");
  t.push_back("Rng.Datum");
  t.push_back("Menge");
  t.push_back("Einzelmenge");
  tree->setTitles(t);
  tree->set_NewNode(&Data_ListeNode::create);
}

void lieferscheinliste::on_button_close_clicked()
{
 destroy();
}

//#define TEXCMD "tex2prn -2 -q -Phl1260 -t landscape"
#define TEXCMD "tex2prn -2 -G -Phl1260 -t landscape" // Preview

void lieferscheinliste::on_button_drucken_clicked()
{
   tree->Expand_recursively(*tree);
   FILE *f=popen(TEXCMD,"w");
   std::ofstream os(fileno(f));   

   Gtk2TeX::HeaderFlags hf;
   hf.landscape=true;
   hf.topmargin=1;
   hf.leftmargin=0;
   hf.preamble_cb=&praeambel;
   hf.rightfoot="\\today";
   Gtk2TeX::Header(os,hf);
   Gtk2TeX::TableFlags tf;
   tf.element_cb=&scale;
   Gtk2TeX::CList2Table(os,tree,tf);
   Gtk2TeX::Footer(os);


   os.close();
   pclose(f); 
}

void lieferscheinliste::praeambel(std::ostream &os,gpointer user_data)
{
   os <<    "\\usepackage{graphics}\n";
}

std::string lieferscheinliste::scale(const Gtk::CList::Row &r,int col,const std::string &text,gpointer user_data)
{
 if (col==Data_LListe::ARTIKEL)
   return Gtk2TeX::scale(text,16,"3cm");
 if (col==Data_LListe::BREITE)
   return Gtk2TeX::scale(text,8,"1.5cm");
 if (col==Data_LListe::FARBE)
   return Gtk2TeX::scale(text,8,"1.5cm");
 if (col==Data_LListe::AUFMACHUNG)
   return Gtk2TeX::scale(text,8,"1.5cm");
 return text;
}
