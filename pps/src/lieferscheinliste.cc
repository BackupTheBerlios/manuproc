#include "config.h"
#include "lieferscheinliste.hh"
#include "lieferscheinliste_classes.hh"
#include "datum_kumul.h"
#include <Lieferschein/LieferscheinVoll.h>
#include <Lieferschein/Rechnung.h>
//#include <Artikel/ArtikelBezeichnung.h>
#include <gtkmm/main.h>
#include <Gtk2TeX.h>
#include <cstdio>
#include <fstream.h>
#include "MyMessage.h"  
#include <Misc/FILEstream.h>
#include <Misc/i18n.h>
#include <Misc/ucompose.hpp>
#include "auftrag_bearbeiten.hh"
#include "auftrag_lieferschein.hh"
#include "auftrag_rechnung.hh"

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

void lieferscheinliste::on_menu_selection(menu_selection m)
{ if (!tree->getMenuContext()) return;
  Handle<const Data_LListe> sel=tree->getMenuContext().cast_dynamic<const Data_LListe>();
  
  switch (m)
  { case M_Auftrag:
    { std::vector<LieferscheinEntry::st_AuftragMenge> vec=sel->getLEntry().getAuftragsMenge();
      for (std::vector<LieferscheinEntry::st_AuftragMenge>::const_iterator i=vec.begin();i!=vec.end();++i)
      { if (!i->ab) continue;
        // Zeilen selektieren?
        AufEintragBase aeb(i->ab,-1);
        new auftrag_bearbeiten(instanz,&aeb);
      }
    }
      break;
    case M_Lieferschein:
      new auftrag_lieferschein(sel->getLEntry());
      break;
    case M_Rechnung:
      if (sel->getRechnung().Valid())
        new auftrag_rechnung(sel->getRechnung());
      break;
  }
}


lieferscheinliste::lieferscheinliste(const cH_ppsInstanz& _instanz, Kunde::ID kid, ArtikelBase art)
: artbase(art), kundenid(kid), instanz(_instanz)
{
  Wdatum_von->set_value(ManuProC::Datum(1,1,1970));
  Wdatum_bis->set_value(ManuProC::Datum::today());
      
  label_anzahl->hide();
  Wdatum_von->setLabel("");
  Wdatum_bis->setLabel("");
  Wdatum_von->hide();
  Wdatum_bis->hide();
  set_titles();
  tree->set_remember("pps","lieferscheinliste");
  tree->addMenuItem(_("Aufträge anzeigen"))
    .connect(sigc::bind(sigc::mem_fun(*this,&lieferscheinliste::on_menu_selection),M_Auftrag));
  tree->addMenuItem(_("Lieferschein anzeigen"))
    .connect(sigc::bind(sigc::mem_fun(*this,&lieferscheinliste::on_menu_selection),M_Lieferschein));
  tree->addMenuItem(_("Rechnung anzeigen"))
    .connect(sigc::bind(sigc::mem_fun(*this,&lieferscheinliste::on_menu_selection),M_Rechnung));
  
  LL.setHoleEntries(true);
#ifdef PETIG_EXTENSIONS
  first_komponent->set_active(false);
#endif
  if (kundenid!=Kunde::none_id) 
  { radiobutton_nur_kunde->set_active(true);
    kundenbox->set_value(kid);
  }
  else kundenbox->hide();
  if (!!artbase)
  { radiobutton_nur_artikel->set_active(true);
    artikelbox->set_value(artbase);
  }
  else artikelbox->hide();
  if (kundenid!=Kunde::none_id || !!artbase)
  { on_button_show_clicked();
  }
}

void lieferscheinliste::on_button_show_clicked()
{
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
}

void lieferscheinliste::fill_tree()
{
//  progressbar->set_show_text(true);
  tree->clear();
//  tree->detach_from_clist();
  std::vector<cH_RowDataBase> datavec;
  double size=LL.Size();
 label_anzahl->set_text(String::ucompose(_("Insgesamt %1 Einträge"),LL.Size()));
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
             	KumVal(reinterpret_cast<long>(date_cumulate->get_menu()->get_active()->get_data("user_data")))
             	));
      progressbar->set_fraction(count/size);
      while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
      ++count;
   }
 tree->setDataVec(datavec);
// tree->attach_to_clist();
 progressbar->set_fraction(1);
// progressbar->set_show_text(false);
}

void lieferscheinliste::set_titles()
{
  std::vector<std::string> t(tree->MaxColumns());
  t[Data_LListe::KUNDE]=_("Kunde");
  t[Data_LListe::AUFTRAG]=_("Auftrag(ZNr)");
  t[Data_LListe::ARTIKEL]=_("Artikel");
  t[Data_LListe::BREITE]=_("Breite");
  t[Data_LListe::FARBE]=_("Farbe");
  t[Data_LListe::AUFMACHUNG]=_("Aufm.");
  t[Data_LListe::LIEFERNR]=_("Lieferschein");
  t[Data_LListe::LIEFERDATUM]=_("Lieferdatum");
  t[Data_LListe::RECHNUNG]=_("Rechnung");
  t[Data_LListe::RECHNUNGSDATUM]=_("Rng.Datum");
  t[Data_LListe::ARTIKELTYP]=_("Art");
  t[Data_LListe::SUM_MENGE]=_("Einzelmenge");
  t[Data_LListe::SUM_AMENGE]=_("Menge");
  tree->setTitles(t);
  std::vector<float> a(tree->MaxColumns());
  a[Data_LListe::SUM_AMENGE]=1;
  a[Data_LListe::SUM_MENGE]=1;
  tree->setAlignment(a);
  tree->set_NewNode(&Data_ListeNode::create);
}

void lieferscheinliste::on_button_close_clicked()
{ delete this;
}

#define TEXCMD "tex2prn -2 -tlandscape"

void lieferscheinliste::drucken_clicked()
{
 on_button_drucken_clicked();
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
   tf.element_cb=&lieferscheinliste::scale;
   tf.multicolumn=true;
   if (tree->get_selection()->count_selected_rows())
   {  tree->get_selection()->get_selected_rows().assign_to(tf.selection);
   }
   Gtk2TeX::TreeView2Table(os,tree,tf);
   Gtk2TeX::Footer(os);

   os.close();
   pclose(f); 
}

void lieferscheinliste::praeambel(std::ostream &os,gpointer user_data)
{
   os <<    "\\usepackage{graphics}\n";
}

std::string lieferscheinliste::scale(const Gtk::TreeModel::const_iterator &r,int col,const std::string &text,gpointer user_data)
{
 // geht nicht !!! könnte ja umsortiert sein (müsste über getColIdx gehen)
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
