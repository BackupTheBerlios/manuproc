// generated 2001/9/18 9:40:08 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to Artikeleingabe.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "Artikeleingabe.hh"
#include "Artikeleingabe_classes.hh"
#include <gtk--/main.h>
#include <gtk--/eventbox.h>
#include <libgnomeui/gnome-stock.h>
#include <gtk--/toolbar.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Gtk2TeX.h>
#include <fstream.h>
#include <cstdio>
#include <algorithm>
#include <Artikel/Einheiten.h>
#include <Auftrag/selFullAufEntry.h>
#include <unistd.h>
#include "MyMessage.h"

#define D(x) 
//cerr << x << '\n'


extern MyMessage *mess;

Artikeleingabe::Artikeleingabe(int argc, char **argv)
  : leer(cH_ArtikelBezeichnung::Default())
{D("leer->Id() " << leer->Id());
 artikelbox->show_label(true);
 progressbar->hide();
 toolbar_loeschen->hide();
 table_verschmelzen->hide(); // Nur für Administratoren!!!!
 frame_artikel->set_sensitive(false);
 frame_editieren->hide();
 kundenbox->Einschraenken_b(true);
 optionmenu_warengruppe->hide();
 set_tree_titels();
 tree->set_NewNode(&Data_Node::create);
 if (argc==2) {artikelbox->set_value(atoi(argv[1]));artikelbox_activate();}
}


void Artikeleingabe::on_checkbutton_offene_auftraege_toggled()
{
 if(checkbutton_offene_auftraege->get_active())
  {
   optionmenu_instanz->set_History(ppsInstanzID::Kundenauftraege);
   load_for_optionmenu_instanz();
  }   
}

void Artikeleingabe::optionmenu_instanz_activate()
{
 checkbutton_offene_auftraege->set_active(false);
 load_for_optionmenu_instanz();
}

void Artikeleingabe::load_for_optionmenu_instanz()
{
 vec_artbase.clear();
 artikelbox->reset();
 ppsInstanz::ID instanz = optionmenu_instanz->get_value()->Id();
 get_artikel_list(instanz);
 set_Data_from_artikelliste();
}

void Artikeleingabe::artikelbox_activate()
{
 toolbar_loeschen->hide();
 vec_artbase.clear();
 try {
    ArtikelBase AB(artikelbox->get_value());
    if (AB.Id())
    {  vec_artbase.push_back(AB.Id());
       optionmenu_instanz->set_value(ArtikelStamm(AB).BestellenBei()->Id());
    }
   } catch (SQLerror &e) {cerr << e<<'\n';}
 set_Data_from_artikelliste();

 frame_artikel->set_sensitive(true);
 fuer_artikel = artikelbox->get_value();
 Eingabe_fuer(artikelbox->get_value());
 fill_eingabebox(1);
 optionmenu_warengruppe->set_extartbezid(artikelbox->getBezSchema()->Id());
 optionmenu_warengruppe->set_value(artikelbox->getBezSchema()->Typ());
 fill_eingabebox(2);
}

void Artikeleingabe::on_optionmenu_warengruppe_activate()
{
// ArtikelTyp::ID t=optionmenu_warengruppe->get_value();
 fill_eingabebox(2);
}


void Artikeleingabe::on_button_artikel_wechsel_clicked()
{
 artikelbox->set_value(von_artikel);
 artikelbox_activate();
}

void Artikeleingabe::on_neuladen_clicked()
{
  ArtikelBaum::UnCache(fuer_artikel);
  set_Data_from_artikelliste();
  toolbar_loeschen->hide();
}


void Artikeleingabe::set_Data_from_artikelliste()
{
//cout << vec_artbase.size()<<'\n';
 progressbar->set_show_text(true);
 progressbar->show();
 tree->detach_from_clist();
 tree->clear();
 datavec_t datavec;
 instanz_spalte.clear();
 double size=vec_artbase.size();
 instanz_spalte.push_back(ppsInstanzID::None);

 if (size>10) // sinnvolle Reihenfolge vorgeben
 {
#ifdef PETIG_EXTENSIONS
    instanz_spalte.push_back(ppsInstanzID::Druckerei);
    instanz_spalte.push_back(ppsInstanzID::Faerberei);
//Was soll das sein? MAT    instanz_spalte.push_back(ppsInstanzID::Bandlager__Weberei_);
    instanz_spalte.push_back(ppsInstanzID::Bandlager);
    instanz_spalte.push_back(ppsInstanzID::Weberei);
#endif
 }

 double count=0;
 label_warnung->set_text(itos((int)size)+" Artikel");
 vec_artbase_t::const_iterator end=vec_artbase.end();
 for (vec_artbase_t::const_iterator i=vec_artbase.begin();i!=end;++i)
  {
   fill_datavec(datavec,*i);
   progressbar->set_percentage(count/size);
   while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
   ++count;
  }
 progressbar->set_percentage(1);
 tree->setDataVec(datavec);
 set_tree_titels();
 tree->Expand_recursively();
 tree->attach_to_clist();
 progressbar->hide();
 if (vec_artbase.begin()!=vec_artbase.end())
 {  tree->row(0).select();
 }
}

// leere Bezeichnung cachen ?

void Artikeleingabe::push_Artikel(vec_zeile_t &vec_zeile, const zeile_t &z)
{
#if 0
   vec_zeile.push_back(z);
#else
  try {
   if (vec_zeile.begin()==vec_zeile.end()) vec_zeile.push_back(z);
   else
   {  cH_ppsInstanz inst(ArtikelStamm(*(z.first)).BestellenBei());
      unsigned int i=vec_zeile.size();
      D("?" << inst->get_Name() << " @ " << i << '?' << z.first->Bezeichnung());
      for (;i<instanz_spalte.size() && instanz_spalte[i]!=inst;++i)
      	D("!" << instanz_spalte[i]->get_Name());
      if (i>=instanz_spalte.size())
      {  instanz_spalte.push_back(inst);
         assert(i<instanz_spalte.size());
         D("C" << instanz_spalte[i]->get_Name() << " @ " << i);
      }
      while(vec_zeile.size()<i) 
         vec_zeile.push_back(zeile_t(leer,0));
      vec_zeile.push_back(z);
   }
   } catch (SQLerror &e)
   { cout << e << ':' << (z.first)->Id() << '\n';
   }
#endif
}

void Artikeleingabe::fill_datavec(datavec_t& datavec,const ArtikelBase& AB)
{
try{
   vec_zeile_t vec_zeile; 
//   cH_ArtikelBezeichnung ABez(AB);
   cH_ArtikelBezeichnung ABez(AB,artikelbox->getBezSchema()->Id());
   ArtikelBaum ABaum(AB);
   push_Artikel(vec_zeile,zeile_t(ABez,0));
   
   if(ABaum.size()==0)   
       datavec.push_back(new Data_tree(vec_zeile));
   for (ArtikelBaum::const_iterator i=ABaum.begin();i!=ABaum.end();++i)
      ArtikelBaum_Pfad(ArtikelBase(i->rohartikel),i->menge,datavec,vec_zeile);

}catch(std::exception &e) {cerr<<e.what();}
}

// Ja der Vektor wird kopiert und dann verändert (Rekursion)
void Artikeleingabe::ArtikelBaum_Pfad(ArtikelBase AB,menge_t menge,
		datavec_t& datavec, vec_zeile_t vec_zeile)
{
   cH_ArtikelBezeichnung ABez(AB,artikelbox->getBezSchema()->Id());
//   cH_ArtikelBezeichnung ABez(AB);
   ArtikelBaum ABaum(AB);
   
   push_Artikel(vec_zeile,zeile_t(ABez,menge));
   if (ABaum.begin()==ABaum.end()) 
       datavec.push_back(new Data_tree(vec_zeile));
   else
    {
     for (ArtikelBaum::const_iterator i=ABaum.begin();i!=ABaum.end();++i)
      {
       ArtikelBaum_Pfad(ArtikelBase(i->rohartikel),i->menge,datavec,vec_zeile);
      }
    }
}

void Artikeleingabe::on_leaf_selected(cH_RowDataBase d)
{
//cout << "Leaf\n";
 const Data_tree *dt=dynamic_cast<const Data_tree*>(&*d);
 fuer_artikel = dt->Artikel();
 artikelbox->set_value(dt->Artikel());
// artikelbox_activate(); sollte eigentlich ...
 // hmm. What is this for? CP
 Eingabe_fuer(dt->Artikel());
 von_artikel = dt->Artikel2();
 Loeschen_von(dt->Artikel2());
}

void Artikeleingabe::on_node_selected(const TCListNode &node)
{
//cout << "Node\n";
  const Data_Node &dn=dynamic_cast<const Data_Node&>(node);
  fuer_artikel=dn.Artikel();
  von_artikel = dn.Artikel2();
  Eingabe_fuer(dn.Artikel());
  Loeschen_von(dn.Artikel2());
}

void Artikeleingabe::Eingabe_fuer(const ArtikelBase& art)
{
 try{
 Artikel_Bestellen_bei->set_History(ArtikelStamm(art).BestellenBei()->Id());
 set_Prozess(); 
// frame_artikel->set_label("Artikel "+cH_ArtikelBezeichnung(art)->Bezeichnung());
 frame_artikel->set_label("Artikel "+cH_ArtikelBezeichnung(art,artikelbox->getBezSchema()->Id())->Bezeichnung());
 frame_artikel->set_sensitive(true);
 OM_Einheit->set_value(Einheit(artikelbox->get_value()));
 } catch (SQLerror &e)
 {  cout << "Artikeleingabe::Eingabe_fuer " << e << '\n';
 }
}

void Artikeleingabe::set_Prozess()
{
 try {
 cH_ppsInstanz instanz=ArtikelStamm(fuer_artikel).BestellenBei();
 Prozess::ID prozess;
 switch (instanz->Id()){
#ifdef PETIG_EXTENSIONS
   case ppsInstanzID::Faerberei: prozess = ProzessID::Faerben; break; 
   case ppsInstanzID::Druckerei: prozess = ProzessID::Drucken; break;
#endif
   default: prozess = Prozess::standard_id; break;
  }
 artikelboxb->set_Prozess(prozess);
 // assume the user want's to supply something if it's not there
// if (!ArtikelBaum(fuer_artikel).size()) artikelboxb->grab_focus_Artikel();
 } catch (SQLerror &e)
 {  cout << "Artikeleingabe::Eingabe_fuer " << e << '\n';
 }

}

void Artikeleingabe::Loeschen_von(const ArtikelBase& art)
{
  if(!art) return;
//  std::string button_text  = "Lösche\n"+cH_ArtikelBezeichnung(art)->Bezeichnung();
//  std::string button_text2 = "Artikel\n"+cH_ArtikelBezeichnung(art)->Bezeichnung();
  std::string button_text  = "Lösche\n"+cH_ArtikelBezeichnung(art,artikelbox->getBezSchema()->Id())->Bezeichnung();
  std::string button_text2 = "Artikel\n"+cH_ArtikelBezeichnung(art,artikelbox->getBezSchema()->Id())->Bezeichnung();
  toolbar_loeschen->tools().clear();
  button_artikel_delete = Gtk::wrap((GtkButton*)gtk_toolbar_append_element
   (GTK_TOOLBAR(toolbar_loeschen->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, 
    button_text.c_str(),0, 0, GTK_WIDGET(gnome_stock_pixmap_widget
      (GTK_WIDGET(toolbar_loeschen->gtkobj()), GNOME_STOCK_PIXMAP_NOT)), 0, 0));
  toolbar_loeschen->tools().push_back(Gtk::Toolbar_Helpers::Space());
  button_artikel_wechsel= Gtk::wrap((GtkButton*)gtk_toolbar_append_element
   (GTK_TOOLBAR(toolbar_loeschen->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, 
    button_text2.c_str(),0, 0, GTK_WIDGET(gnome_stock_pixmap_widget
      (GTK_WIDGET(toolbar_loeschen->gtkobj()), GNOME_STOCK_PIXMAP_FORWARD)), 0, 0));
  button_artikel_delete ->clicked.connect(SigC::slot(static_cast<class  Artikeleingabe*>(this), &Artikeleingabe::on_button_artikel_delete_clicked));
  button_artikel_wechsel->clicked.connect(SigC::slot(static_cast<class  Artikeleingabe*>(this), &Artikeleingabe::on_button_artikel_wechsel_clicked));
  toolbar_loeschen->show();
}

void Artikeleingabe::on_unselect_row(gint row, gint column, GdkEvent *event)
{
 toolbar_loeschen->hide();
 frame_artikel->set_sensitive(false);
}



void Artikeleingabe::on_button_artikel_delete_clicked()
{
  ArtikelBaum::delete_Artikel(getuid(),fuer_artikel,von_artikel);
  on_neuladen_clicked();
}

void Artikeleingabe::on_Artikel_Bestellen_activate()
{
//cout << artikelboxb->get_Artikel().Id()<<'\t'<<artikelboxb->get_Menge()
//   <<'\t'<<artikelboxb->get_Instanz_Id()<<'\n';
  if(artikelboxb->get_Artikel().Id()==0 || artikelboxb->get_Menge()==0 )
   {
    label_warnung->set_text("WARNUNG: Eingabe unvollständig");
    return;
   }
  label_warnung->set_text("");
  ArtikelBaum::RohArtikel ra;
  ra.rohartikel = artikelboxb->get_Artikel().Id();
  ra.menge = artikelboxb->get_Menge();
  ra.erzeugung=artikelboxb->get_Prozess(); 
  ArtikelBaum::new_Artikel(getuid(),fuer_artikel,ra);
  
  artikelbox->set_value(von_artikel);
  on_neuladen_clicked();
}

void Artikeleingabe::optionmenu_bestellen_bei_activate()
{
  Transaction tr;
  if (!fuer_artikel) return;
  ppsInstanz::ID oldInstanz=ArtikelStamm(fuer_artikel).BestellenBei()->Id();
  ppsInstanz::ID newInstanz = Artikel_Bestellen_bei->get_Instanz_Id(); 
  if(oldInstanz==newInstanz) return;
  ArtikelStamm::set_BestellenBei(fuer_artikel,newInstanz);
  
  for (vec_artbase_t::iterator i=vec_artbase.begin();i!=vec_artbase.end();++i)
    if((*i)==fuer_artikel) { vec_artbase.erase(i); break;}
  set_Prozess();

  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Artikel(oldInstanz,fuer_artikel);
  SelectedFullAufList allaufids=SelectedFullAufList(psel);
  try{
  for(SelectedFullAufList::iterator i=allaufids.begin();i!=allaufids.end();++i)
   {
     if(i->Instanz()->Id()==ppsInstanzID::Kundenauftraege) continue;
     AuftragBase A(newInstanz,0);
     i->moveInstanz(getuid(),A);
   }
 }catch(SQLerror &e) {warnung("Fehlgeschlagen") ;}
 tr.commit();
}

void Artikeleingabe::warnung(std::string s)
{
  label_warnung->set_text(s);
  des = Gtk::Main::timeout.connect(slot(this,&Artikeleingabe::timeout_warnung),4000);
}

gint Artikeleingabe::timeout_warnung()
{
  label_warnung->set_text("");
  return 0;
}
    

void Artikeleingabe::on_button_close_clicked()
{   
  des.disconnect();
  des2.disconnect();
  des3.disconnect();
  Gtk::Main::instance()->quit();
}

void Artikeleingabe::set_tree_titels()
{
 vector<std::string> t;
 for (instanz_spalte_t::const_iterator i=instanz_spalte.begin();
 		i!=instanz_spalte.end();++i)
    t.push_back((*i)->get_Name());
 tree->setTitles(t);
 for (unsigned int i=0;i<instanz_spalte.size();++i)
    tree->column(i).set_visiblity(true);
 for (unsigned int i=instanz_spalte.size();i<tree->Cols();++i)
    tree->column(i).set_visiblity(false);
// tree->show_titles(false);
}


void Artikeleingabe::on_togglebutton_edit_toggled()
{
 if(togglebutton_edit->get_active()) frame_editieren->show();
 else frame_editieren->hide();
}


#include "../src/stock_button_apply.xpm"
#include "../src/stock_button_cancel.xpm"

void Artikeleingabe::eingabe_activate()
{
  ArtikelBase artikel=fuer_artikel;
  
  save_edited_artikel();
  cH_ArtikelBezeichnung::Deregister(artikelbox->getBezSchema()->Id(),artikel.Id());
  artikelbox->set_value(artikel);
  pixmap_edit->set(stock_button_apply_xpm);
  while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
  des = Gtk::Main::timeout.connect(slot(this,&Artikeleingabe::timeout_save_edited_artikel),4000);
}

void Artikeleingabe::on_einheit_activate()
{
//cout << "ACTIVATE  ";
//cout <<  OM_Einheit->get_value().Bezeichnung()<<'\n';;
 ArtikelStamm::setEinheit(artikelbox->get_value(),OM_Einheit->get_value());
}


gint Artikeleingabe::timeout_save_edited_artikel()
{
  pixmap_edit->set(stock_button_cancel_xpm);
  return 0;
}

void Artikeleingabe::on_kunde_activate()
{
  try{
  fill_eingabebox(2);
  eingabe2->grab_focus();
  if(kundenbox->get_value()==1) 
    optionmenu_warengruppe->show();
  else 
    optionmenu_warengruppe->hide();
  }catch(SQLerror &e) {cerr << e<<'\n';}
 fill_eingabebox(2);
}

void Artikeleingabe::on_eingabe2_activate()
{
  try{
  if(!update_edited_artikel2())
    save_edited_artikel2();
  cH_ArtikelBezeichnung::Deregister(kundenbox->get_value(),fuer_artikel.Id());  
  pixmap_edit2->set(stock_button_apply_xpm);
  while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
  des2 = Gtk::Main::timeout.connect(slot(this,&Artikeleingabe::timeout_save_edited_artikel2),4000);
  }
  catch(SQLerror &e)
    {cerr<<e<<'\n';
      if(e.Code()==-400)
     	{string msg("Die Bezeichnung ");
     	 msg=msg+eingabe2->get_value(0)+"... existiert schon";
     	 mess->Show(msg);
     	}
     else
        mess->Show(e);
    }
}

gint Artikeleingabe::timeout_save_edited_artikel2()
{
  pixmap_edit2->set(stock_button_cancel_xpm);
  return 0;
}





//#define TEXCMD "tex2prn -2 -q -Phl1260 -t landscape"
//#define TEXCMD "tex2prn -2 -q -Pps -t landscape"
#define TEXCMD "tex2prn -2 -G -Phl1260 -t landscape" // Preview

void Artikeleingabe::on_button_drucken_clicked()
{
   tree->Expand_recursively();
   FILE *f=popen(TEXCMD,"w");
   ofstream os(fileno(f));   

   Gtk2TeX::HeaderFlags hf;
   hf.landscape=true;
   hf.topmargin=1;   
   hf.leftmargin=-1; 
 //  hf.preamble_cb=&praeambel;
   hf.rightfoot="\\today";   
   Gtk2TeX::Header(os,hf);   
   Gtk2TeX::TableFlags tf;   
//   tf.element_cb=&scale;     
   Gtk2TeX::CList2Table(os,tree,tf);
   Gtk2TeX::Footer(os);
   os.close();
   pclose(f); 
}

const cH_EntryValue Data_tree::Value(guint seqnr,gpointer gp) const
{
try{
   if (vec_zeile.size()>seqnr && seqnr <MAXCOLUMN) 
   {  if (!(vec_zeile[seqnr].first->Id()))
         return cH_EntryValueIntString("");
      else if (!vec_zeile[seqnr].second)
         return cH_EntryValueIntString(vec_zeile[seqnr].first->Bezeichnung());
      else
      {  string menge;
         Einheit e(*(vec_zeile[seqnr].first)),e0(Einheit::Stueck);
         
         for (int i=seqnr-1;i>=0;--i)
         {  if (vec_zeile[seqnr-1].first->Id()) 
            {  e0=Einheit(*(vec_zeile[seqnr-1].first));
               break;
            }
         }

         if (e!=e0)
         {  menge=Formatiere_short(vec_zeile[seqnr].second)+std::string(e)+'/'+std::string(e0)+' ';
         }
         else if (vec_zeile[seqnr].second!=menge_t(1))
         {  menge=Formatiere_short(vec_zeile[seqnr].second)+"x ";
         }
         return cH_EntryValueIntString(menge+vec_zeile[seqnr].first->Bezeichnung());
      }
   }
} catch (SQLerror &e) { cout << "Data_tree::Value " << e << '\n'; }
   return cH_EntryValueIntString("");
}

ArtikelBase Data_tree::Artikel() const
{  for (vec_zeile_t::const_iterator i=vec_zeile.begin();i!=vec_zeile.end();++i)
   {  if (!!(*i).first && (*i).first->Id()) return *((*i).first);
   }
   return ArtikelBase();
}

ArtikelBase Data_tree::Artikel2() const
{  vec_zeile_t::const_iterator i=vec_zeile.begin();
   for (;i!=vec_zeile.end();++i)
   {  if (!!(*i).first && (*i).first->Id()) { ++i; break; }
   }
   for (;i!=vec_zeile.end();++i)
   {  if (!!(*i).first && (*i).first->Id()) return *((*i).first);
   }
   return ArtikelBase();
}

void Artikeleingabe::on_kunde_activate();        

void Artikeleingabe::on_togglebutton_neue_toggled()
{
  if(togglebutton_neue_artikel->get_active()) 
   {  
     artikelbox->set_automatisch_anlegen(true);
     artikelbox->grab_focus();
     on_standard_einheit_activate();
     on_optionmenu_standardinstanz_activate();
     frame_neue_artikel->show();
   }
  else
   {
     artikelbox->set_automatisch_anlegen(false);
     artikelbox->set_Default_Instanz(ppsInstanzID::None);
     artikelbox->set_Default_Einheit(Einheit::default_id);  
     frame_neue_artikel->hide();
   }
}


void Artikeleingabe::on_standard_einheit_activate()
{
  artikelbox->set_Default_Einheit(standard_einheit->get_value());  
}


void Artikeleingabe::on_optionmenu_standardinstanz_activate()
{
  artikelbox->set_Default_Instanz(standard_instanz->get_value()->Id());
}
                
