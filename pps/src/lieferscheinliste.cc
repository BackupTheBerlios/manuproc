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
#include "MyMessage.h"  
#include <Misc/FILEstream.h>

extern MyMessage *meldung;

void lieferscheinliste::kundenbox_activate()
{   
 kundenid = kundenbox->get_value();
 radiobutton_nur_kunde->set_active(true);
}

void lieferscheinliste::kundengruppe_activate()
{   // @@@
 radiobutton_kundengruppe->set_active(true);
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
    ManuProC::Datum d(1,1,1970);
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
    ManuProC::Datum d(ManuProC::Datum::today());
    Wdatum_bis->show();
    Wdatum_bis->set_value(d);
  }
}
void lieferscheinliste::on_radiobutton_artikel_toggled()
{
 if (radiobutton_alle_artikel->get_active())
   {  artikelbox->hide();
      artbase=ArtikelBase();
   }
 if (radiobutton_nur_artikel->get_active())
   {  
      artikelbox->reset();
      artikelbox->show();      
      artikelbox->grab_focus();
   }
}
void lieferscheinliste::on_radiobutton_kunde_toggled()
{
 if (radiobutton_alle_kunden->get_active())
   {  kundenid = ManuProcEntity<>::none_id;
   }
 if (radiobutton_nur_kunde->get_active())
   {  kundenbox->reset();
      kundenbox->grab_focus();
      kundenbox->show();
   }
 else kundenbox->hide();

 if (radiobutton_kundengruppe->get_active())
   {  kundengruppe->set_value(KundengruppeID::None);
      kundengruppe->grab_focus();
      kundengruppe->show();
   }
 else kundengruppe->hide();
}


lieferscheinliste::lieferscheinliste(const cH_ppsInstanz& _instanz)
: instanz(_instanz)
{
  Wdatum_von->set_value(ManuProC::Datum(1,1,1970));
  Wdatum_bis->set_value(ManuProC::Datum::today());
      
  label_anzahl->hide();
  Wdatum_von->setLabel("");
  Wdatum_bis->setLabel("");
  Wdatum_von->hide();
  Wdatum_bis->hide();
  artikelbox->hide();
  kundenbox->hide();
  set_titles();
  tree->set_remember("pps","lieferscheinliste");
  kundenid=ManuProcEntity<>::none_id;
  
  LL.setHoleEntries(true);
}

void lieferscheinliste::on_button_show_clicked()
{
  tree->freeze();
  LL.reset();
  try {
       LL.setForArtikel(artbase.Id());
       LL.setFirstKompOnly(first_komponent->get_active());
       if (radiobutton_kundengruppe->get_active()) 
          LL.setKundenGruppe(kundengruppe->get_value());
       else LL.setForKunde(kundenid);
       LL.setForInstanz(instanz);
       
       if(radiobutton_alle_zeit_von->get_active())
         LL.setFromDate(ManuProC::Datum(1,1,1970));
       else LL.setFromDate(Wdatum_von->get_value());
       
       if(radiobutton_alle_zeit_bis->get_active())
         LL.setToDate(ManuProC::Datum::today());
       else LL.setToDate(Wdatum_bis->get_value());
       
       LL.build_list();
   } catch (SQLerror &e) 
   	   {meldung->Show(e); return;}
  fill_tree();
  tree->thaw();
}

void lieferscheinliste::fill_tree()
{
  progressbar->set_show_text(true);
  tree->clear();
  tree->detach_from_clist();
  std::vector<cH_RowDataBase> datavec;
  double size=LL.Size();
 label_anzahl->set_text("Insgesamt "+itos(LL.Size())+" Einträge");
 label_anzahl->show();
  double count=0;

  for (LieferscheinList::const_iterator i=LL.begin();i!=LL.end();++i)
   {
     Rechnung R;
     if((*i)->RngNr()!=ManuProcEntity<>::none_id && (*i)->RngNr()>0)
        R = Rechnung((*i)->RngNr());
     for (std::vector<LieferscheinEntryBase>::const_iterator j=LL.begin((*i)->Id());
     		j!=LL.end((*i)->Id()); ++j)
        datavec.push_back(new Data_LListe(*i,LieferscheinEntry(*j),R,
             	Data_LListe::KumVal(reinterpret_cast<int>(date_cumulate->get_menu()->get_active()->get_user_data()))
             	));
      progressbar->set_percentage(count/size);
      while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
      ++count;
   }
 tree->setDataVec(datavec);
 tree->attach_to_clist();
 progressbar->set_percentage(1);
 progressbar->set_show_text(false);
}

void lieferscheinliste::set_titles()
{
  std::vector<std::string> t(tree->Cols());
  t[Data_LListe::KUNDE]="Kunde";
  t[Data_LListe::AUFTRAG]="Auftrag(ZNr)";
  t[Data_LListe::ARTIKEL]="Artikel";
  t[Data_LListe::BREITE]="Breite";
  t[Data_LListe::FARBE]="Farbe";
  t[Data_LListe::AUFMACHUNG]="Aufm.";
  t[Data_LListe::LIEFERNR]="Lieferschein";
  t[Data_LListe::LIEFERDATUM]="Lieferdatum";
  t[Data_LListe::RECHNUNG]="Rechnung";
  t[Data_LListe::RECHNUNGSDATUM]="Rng.Datum";
  t[Data_LListe::ARTIKELTYP]="Art";
  t[Data_LListe::SUM_MENGE]="Einzelmenge";  
  t[Data_LListe::SUM_AMENGE]="Menge";

  tree->setTitles(t);
  tree->set_NewNode(&Data_ListeNode::create);
  tree->set_column_justification(Data_LListe::SUM_AMENGE, GTK_JUSTIFY_RIGHT);
  tree->set_column_justification(Data_LListe::SUM_MENGE, GTK_JUSTIFY_RIGHT);  
}

void lieferscheinliste::on_button_close_clicked()
{
 destroy();
}

//#define TEXCMD "tex2prn -2 -q -Phl1260 -t landscape"
#define TEXCMD "tex2prn -2 -G -Phl1260 -t landscape" // Preview


gint lieferscheinliste::on_button_drucken_button_release_event(GdkEventButton *event)
{
 if (event->button==3)  tree->Expand_recursively(*tree); 
 on_button_drucken_clicked();
 return false;
}


void lieferscheinliste::on_button_drucken_clicked()
{
   FILE *f=popen(TEXCMD,"w");
   oFILEstream os(f);   

   Gtk2TeX::HeaderFlags hf;
   hf.landscape=true;
   hf.topmargin=1;
   hf.leftmargin=0;
   hf.preamble_cb=&praeambel;
   hf.rightfoot="\\today";
   Gtk2TeX::Header(os,hf);
   Gtk2TeX::TableFlags tf;
   tf.element_cb=&scale;
   tf.multicolumn=true;
   if (tree->selection().begin()!=tree->selection().end())
   {  // yes, this hackery is not amusing! We need an API change ...
      // Try to detect size of the selected tree
      TCListRow *tclapi=(TCListRow*)(tree->selection().begin()->get_data());
      
      TCListRow *last=tclapi;
      while (last->begin()!=last->end()) last=&*(--last->end());
      tf.first_line=tclapi->get_lineno();
      tf.last_line=last->get_lineno();
   }
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
