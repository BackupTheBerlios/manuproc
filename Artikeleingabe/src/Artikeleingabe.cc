#include "config.h"
#include "Artikeleingabe.hh"
#include "Artikeleingabe_classes.hh"
#include <gtkmm/main.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/toolbar.h>
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
#include <Misc/FILEstream.h>

#define D(x) 
//cerr << x << '\n'

#define MAXCOLUMN 10

extern MyMessage *mess;

Artikeleingabe::Artikeleingabe(int argc, char **argv)
  : leer(cH_ArtikelBezeichnung::Default()), fire_toggles(false) 
{D("leer->Id() " << leer->Id());
 artikelbox->show_label(true);
 progressbar->hide();
 toolbar_loeschen->hide();
 table_verschmelzen->hide(); // Nur für Administratoren!!!!
// frame_artikel->set_sensitive(false);
// frame_editieren->hide();

 // nur Kunden die ein Schema haben anzeigen
 alias_schema->Einschraenkung(" and kundennr in (select distinct extartbezid"
			" from extbezschema)");

 alias_warengruppe->hide();
 set_tree_titels();
 tree->set_NewNode(&Data_Node::create);

#ifndef MABELLA_EXTENSIONS
 artikelbox_neu->set_automatisch_anlegen(true);
#endif
#ifdef PETIG_EXTENSIONS
 no_instanz->set_active(false);
#endif

 if (argc==2) 
 {top_notebook->set_current_page(2);
  artikelbox->set_value(ArtikelBase(atoi(argv[1])));
  artikelbox_activate();}
}


void Artikeleingabe::on_checkbutton_offene_auftraege_toggled()
{
 if(checkbutton_offene_auftraege->get_active())
  {
//   optionmenu_instanz->set_History(ppsInstanzID::Kundenauftraege);
   optionmenu_instanz->set_value(cH_ppsInstanz(ppsInstanzID::Kundenauftraege));
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

 fire_toggles=false;

 try {
    ArtikelBase AB(artikelbox->get_value());
    if (!!AB)
    {  vec_artbase.push_back(AB);
       ArtikelStamm as(AB);

       show_in_prlist->set_active(as.getAktive()); 
       cH_ppsInstanz pi(as.BestellenBei());
       change_no_instanz->set_active(pi->Id()==ppsInstanzID::None);

       Artikel_Bestellen_bei->set_value(pi->Id());
       Artikel_Bestellen_bei->set_sensitive(!change_no_instanz->get_active());
       
       mindbest_check->set_active(as.getCheckBest());
       mindbestand->set_sensitive(as.getCheckBest());
       mindbestand->set_value(as.getMindBest());
    }
   } catch (SQLerror &e)   {mess->Show(e);} 
 set_Data_from_artikelliste();

// frame_artikel->set_sensitive(true);
// fuer_artikel = artikelbox->get_value();
 Eingabe_fuer(artikelbox->get_value());
 fill_eingabebox(1);
 alias_warengruppe->set_extartbezid(artikelbox->getBezSchema()->Id());
 alias_warengruppe->set_value(artikelbox->getBezSchema()->Typ());
 fill_eingabebox(2);

 top_notebook->grab_focus();

 fire_toggles=true;
}


void Artikeleingabe::on_alias_warengruppe_activate()
{
// ArtikelTyp::ID t=alias_warengruppe->get_value();
 fill_eingabebox(2);
}


void Artikeleingabe::on_button_artikel_wechsel_clicked()
{
 cH_Data_tree dt=tree->getSelectedRowDataBase_as<cH_Data_tree>();
 artikelbox->set_value(dt->Artikel());
 artikelbox_activate();
}

void Artikeleingabe::on_neuladen_clicked()
{
  ArtikelBaum::UnCache(artikelbox->get_value());
  set_Data_from_artikelliste();
  toolbar_loeschen->hide();
}


void Artikeleingabe::set_Data_from_artikelliste()
{
//cout << vec_artbase.size()<<'\n';
// progressbar->set_show_text(true);
 progressbar->show();
// tree->detach_from_clist();
 tree->clear();
 datavec_t datavec;
 instanz_spalte.clear();
 unsigned size=vec_artbase.size();
 if (size==1) instanz_spalte.push_back(ArtikelStamm(vec_artbase[0]).BestellenBei());
 else instanz_spalte.push_back(ppsInstanzID::None);

 if (size>10) // sinnvolle Reihenfolge vorgeben
 {
#ifdef PETIG_EXTENSIONS
    instanz_spalte.push_back(ppsInstanzID::Bandlager);
    instanz_spalte.push_back(ppsInstanzID::Druckerei);
    instanz_spalte.push_back(ppsInstanzID::Faerberei);
    instanz_spalte.push_back(ppsInstanzID::Bandlager);
    instanz_spalte.push_back(ppsInstanzID::Weberei);
#endif
 }

 unsigned count=0;

 label_warnung->set_text(itos((int)size)+" Artikel");
 vec_artbase_t::const_iterator end=vec_artbase.end();
 for (vec_artbase_t::const_iterator i=vec_artbase.begin();i!=end;++i)
  {
   fill_datavec(datavec,*i);
   progressbar->set_fraction(count/double(size));
   while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
   ++count;
  }
 progressbar->set_fraction(1);
 tree->setDataVec(datavec);
 set_tree_titels();
 tree->Expand_recursively();
// tree->attach_to_clist();
 progressbar->hide();
 if (vec_artbase.begin()!=vec_artbase.end())
 {  Gtk::TreeModel::Path p;
    p.push_back(0);
    tree->get_selection()->select(p);
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
      for (;i<instanz_spalte.size() && instanz_spalte[i]!=inst->Id();++i)
      	D("!" << instanz_spalte[i]->get_Name());
      if (i>=instanz_spalte.size())
      {  instanz_spalte.push_back(inst->Id());
         assert(i<instanz_spalte.size());
         D("C" << instanz_spalte[i]->get_Name() << " @ " << i);
      }
      while(vec_zeile.size()<i) 
         vec_zeile.push_back(zeile_t(leer,0));
      vec_zeile.push_back(z);
   }
   } catch (SQLerror &e)
   { std::cout << e << ':' << (z.first)->Id() << '\n';
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
   
   if(ABaum.empty())   
       datavec.push_back(new Data_tree(vec_zeile));
   for (ArtikelBaum::const_iterator i=ABaum.begin();i!=ABaum.end();++i)
      ArtikelBaum_Pfad(ArtikelBase(i->rohartikel),i->menge,datavec,vec_zeile);

}catch(std::exception &e) {std::cerr<<e.what();}
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
// fuer_artikel = dt->Artikel();
 artikelbox->set_value(dt->Artikel());
// artikelbox_activate(); sollte eigentlich ...
 // hmm. What is this for? CP
 Eingabe_fuer(dt->Artikel());
// von_artikel = dt->Artikel2();
 Loeschen_von(dt->Artikel2());
}

void Artikeleingabe::on_node_selected(const TreeRow &node)
{
//cout << "Node\n";
  const Data_Node &dn=dynamic_cast<const Data_Node&>(node);
//  fuer_artikel=dn.Artikel();
//  von_artikel = dn.Artikel2();
  Eingabe_fuer(dn.Artikel());
  Loeschen_von(dn.Artikel2());
}

void Artikeleingabe::Eingabe_fuer(const ArtikelBase& art)
{
 try{
// Artikel_Bestellen_bei->set_History(ArtikelStamm(art).BestellenBei()->Id());
 Artikel_Bestellen_bei->set_value(ArtikelStamm(art).BestellenBei());
 set_Prozess(); 
// frame_artikel->set_label("Artikel "+cH_ArtikelBezeichnung(art)->Bezeichnung());
// frame_artikel->set_label("Artikel "+cH_ArtikelBezeichnung(art,artikelbox->getBezSchema()->Id())->Bezeichnung());
// frame_artikel->set_sensitive(true);
 OM_Einheit->set_value(Einheit(artikelbox->get_value()));
 } catch (SQLerror &e)
 {  std::cout << "Artikeleingabe::Eingabe_fuer " << e << '\n';
 }
}

void Artikeleingabe::set_Prozess()
{
 try {
 cH_ppsInstanz instanz=ArtikelStamm(artikelbox->get_value()).BestellenBei();
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
 {  std::cout << "Artikeleingabe::Eingabe_fuer " << e << '\n';
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
   (Gtk::TOOLBAR(toolbar_loeschen->gobj()), Gtk::TOOLBAR_CHILD_BUTTON, 0, 
    button_text.c_str(),0, 0, Gtk::WIDGET(gnome_stock_pixmap_widget
      (Gtk::WIDGET(toolbar_loeschen->gobj()), GNOME_STOCK_PIXMAP_NOT)), 0, 0));
  toolbar_loeschen->tools().push_back(Gtk::Toolbar_Helpers::Space());
  button_artikel_wechsel= Gtk::wrap((GtkButton*)gtk_toolbar_append_element
   (Gtk::TOOLBAR(toolbar_loeschen->gobj()), Gtk::TOOLBAR_CHILD_BUTTON, 0, 
    button_text2.c_str(),0, 0, Gtk::WIDGET(gnome_stock_pixmap_widget
      (Gtk::WIDGET(toolbar_loeschen->gobj()), GNOME_STOCK_PIXMAP_FORWARD)), 0, 0));
  button_artikel_delete ->signal_clicked().connect(SigC::slot(*static_cast<class  Artikeleingabe*>(this), &Artikeleingabe::on_button_artikel_delete_clicked));
  button_artikel_wechsel->signal_clicked().connect(SigC::slot(*static_cast<class  Artikeleingabe*>(this), &Artikeleingabe::on_button_artikel_wechsel_clicked));
  toolbar_loeschen->show();
}

void Artikeleingabe::on_unselect_row(gint row, gint column, GdkEvent *event)
{
 toolbar_loeschen->hide();
// frame_artikel->set_sensitive(false);
}



void Artikeleingabe::on_button_artikel_delete_clicked()
{
  cH_Data_tree dt=tree->getSelectedRowDataBase_as<cH_Data_tree>();
  ArtikelBaum::delete_Artikel(dt->Artikel(),dt->Artikel2());
  on_neuladen_clicked();
}

void Artikeleingabe::on_Artikel_Bestellen_activate()
{
//cout << artikelboxb->get_Artikel().Id()<<'\t'<<artikelboxb->get_Menge()
//   <<'\t'<<artikelboxb->get_Instanz_Id()<<'\n';
  if(artikelboxb->get_Artikel().Id()==ArtikelBase::none_id )
//     artikelboxb->get_Menge()==0 )
   {
    mess->Show("Artikel muß eingegeben werden");
    return;
   }
  label_warnung->set_text("");
  ArtikelBaum::RohArtikel ra;
  ra.rohartikel = artikelboxb->get_Artikel().Id();
  ra.menge = artikelboxb->get_Menge();
  ra.erzeugung=artikelboxb->get_Prozess(); 
  ArtikelBaum::new_Artikel(artikelbox->get_value(),ra);
 

// wozu das?  
//  cH_Data_tree dt=tree->getSelectedRowDataBase_as<cH_Data_tree>();
//  artikelbox->set_value(dt->Artikel2());


  on_neuladen_clicked();

//  von_artikel = artikelboxb->get_Artikel();
  Loeschen_von(artikelboxb->get_Artikel());
}

void Artikeleingabe::optionmenu_bestellen_bei_activate()
{
 if (!(artikelbox->get_value())) return;
 ppsInstanz::ID oldInstanz=ArtikelStamm(artikelbox->get_value()).BestellenBei()->Id();
 ppsInstanz::ID newInstanz = Artikel_Bestellen_bei->get_value()->Id(); 
 if(oldInstanz==newInstanz) return;
 ArtikelStamm::set_BestellenBei(artikelbox->get_value(),newInstanz);
 
 for (vec_artbase_t::iterator i=vec_artbase.begin();i!=vec_artbase.end();++i)
   if((*i)==artikelbox->get_value()) { vec_artbase.erase(i); break;}
 set_Prozess();
}

// missbrauch von label_warnung ...
void Artikeleingabe::warnung(std::string s)
{
  label_warnung->set_text(s);
  des = Gtk::Main::signal_timeout().connect(slot(this,&Artikeleingabe::timeout_warnung),4000);
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
 std::vector<std::string> t;
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
#if 0
 if(togglebutton_edit->get_active()) frame_editieren->show();
 else frame_editieren->hide();
#endif 
}


#include "../src/stock_button_apply.xpm"
#include "../src/stock_button_cancel.xpm"

void Artikeleingabe::eingabe_activate()
{
//  ArtikelBase artikel=fuer_artikel;
  
 try{
  save_edited_artikel();
  cH_ArtikelBezeichnung::Deregister(artikelbox->getBezSchema()->Id(),
				    artikelbox->get_value().Id());
  artikelbox->set_value(artikelbox->get_value());
  pixmap_edit->set(stock_button_apply_xpm);
  while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
  des = Gtk::Main::signal_timeout().connect(slot(this,&Artikeleingabe::timeout_save_edited_artikel),4000);
 }catch(SQLerror &e) {mess->Show(e);}
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
  alias_eingabe->grab_focus();
  // diese Heuristik ist etwas fraglich CP
// ich würde es ganz rausnehmen JJ
//  if(alias_schema->get_value()==1) 
//    alias_warengruppe->show();
//  else 
    alias_warengruppe->hide();

  }catch(SQLerror &e) {std::cerr << e<<'\n';}
 fill_eingabebox(2);
}

void Artikeleingabe::on_alias_eingabe_activate()
{
  try{
  if(!update_edited_artikel2())
    save_edited_artikel2();
  cH_ArtikelBezeichnung::Deregister(alias_schema->get_value(),
				    artikelbox->get_value().Id());  
  alias_pixmap->set(stock_button_apply_xpm);
  while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
  des2 = Gtk::Main::signal_timeout().connect(slot(this,&Artikeleingabe::timeout_save_edited_artikel2),4000);
  }
  catch(SQLerror &e)
    {std::cerr<<e<<'\n';
     if(e.Code()==-400)
       {std::string msg("Die Bezeichnung ");
        msg=msg+alias_eingabe->get_value(0)+"... existiert schon";
        mess->Show(msg);
        }
     else
        mess->Show(e); 
    }
}

gint Artikeleingabe::timeout_save_edited_artikel2()
{
  alias_pixmap->set(stock_button_cancel_xpm);
  return 0;
}

#define TEXCMD "tex2prn -2 -G -tlandscape" // Preview

void Artikeleingabe::on_button_drucken_clicked()
{
   tree->Expand_recursively();
   FILE *f=popen(TEXCMD,"w");
   oFILEstream os(f);   

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
      {  std::string menge;
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
} catch (SQLerror &e) { std::cout << "Data_tree::Value " << e << '\n'; }
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

void Artikeleingabe::on_notebook1_switch_page(GtkNotebookPage *p0, guint p1)
{  artikelbox->set_sensitive(p1==1 || p1==2);
}

void Artikeleingabe::on_no_instanz_toggled()
{
 if(!fire_toggles) return;

 if(no_instanz->get_active())
   {standard_instanz->set_sensitive(false);
   aktuelle_gruppe.bestellen_bei=ppsInstanzID::None;
   }
 else
   {standard_instanz->set_sensitive(true);   
    aktuelle_gruppe.bestellen_bei=standard_instanz->get_value()->Id();
   }
}


void Artikeleingabe::on_show_in_prlist_toggled()
{
 if(!fire_toggles) return;

 if(!artikelbox->get_value()) return;
 
 try{
 bool alle_farben=true;
 ArtikelStamm::setAktive(artikelbox->get_value(),
		show_in_prlist->get_active(),alle_farben); 
 }
 catch(SQLerror &e)
  {mess->Show(e); 
  }
}


void Artikeleingabe::on_change_no_instanz_toggled()
{ 
 if(!fire_toggles) return;

 ppsInstanz::ID pid=change_no_instanz->get_active() ?
		 ppsInstanzID::None : Artikel_Bestellen_bei->get_value()->Id();
 try{
   if(!!artikelbox->get_value())
     {ArtikelStamm::set_BestellenBei(artikelbox->get_value(),pid);
     }
 }
 catch(SQLerror &e)
  {
   Artikel_Bestellen_bei->set_sensitive(!change_no_instanz->get_active());   
   mess->Show(e); 
  } 
 Artikel_Bestellen_bei->set_sensitive(!change_no_instanz->get_active());   
}



void Artikeleingabe::on_mindbest_check_toggled()
{
 if(!fire_toggles) return;

 mindbestand->set_sensitive(mindbest_check->get_active());

 if(!artikelbox->get_value()) return;

 if(mindbest_check->get_active())
   {mindbestand->set_value(0);  
    on_mindbestand_activate();
   }
 else
   {
    ArtikelStamm as(artikelbox->get_value());
    as.setMindBest(-1);  // deactivate midbest check at all
    mindbestand->set_value(0);      
   }
}

void Artikeleingabe::on_mindbestand_activate()
{  
 ArtikelStamm as(artikelbox->get_value());
 mindbestand->update();
 as.setMindBest(mindbestand->get_value_as_int());
 top_notebook->grab_focus(); 
}


