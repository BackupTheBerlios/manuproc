// $Id: Artikeleingabe.cc,v 1.40 2005/11/25 15:24:46 christof Exp $
/*  Artikeleingabe: ManuProC's article management program
 *  Copyright (C) 2004-2005 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#include "config.h"
#include "Artikeleingabe.hh"
#include "Artikeleingabe_classes.hh"
#include <gtkmm/main.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/toolbar.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Gtk2TeX.h>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <Artikel/Einheiten.h>
#include <Auftrag/selFullAufEntry.h>
#include <unistd.h>
#include "MyMessage.h"
#include <Misc/FILEstream.h>
#include <gtkmm/separatortoolitem.h>
#include <glibmm/main.h>
#include <Misc/TagStream.h>
#include <Misc/TraceNV.h>

#define D(x) 
//cerr << x << '\n'

#define MAXCOLUMN 10

extern MyMessage *mess;
const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("DEBUG",trace_channel);

Artikeleingabe::Artikeleingabe(int argc, char **argv)
  : leer(cH_ArtikelBezeichnung::Default()), fire_toggles(false) 
{D("leer->Id() " << leer->Id());
 artikelbox->AnlegenCall(&Artikeleingabe::new_adaptor,this);
 artbox_verschmelzen->AnlegenCall(&Artikeleingabe::new_adaptor,this);
 artikelbox_neu->AnlegenCall(&Artikeleingabe::Anlegen,this);
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
 Artikel_Bestellen_bei->set_sensitive(true);
#endif

 table_preis->init(artikelbox);
 if (argc==2 && !strncmp(argv[1],"<?xml ",6))
 { try
   { TagStream ts(argv[1]);
     const Tag &cont=ts.getContent();
     // warengruppe und schema id ermitteln
     unsigned warengruppe=cont.getAttr<int>("warengruppe");
     unsigned schema=cont.getAttr<int>("schema");
     std::map<int,std::vector<std::string> > entries_map;
     FOR_EACH_CONST_TAG_OF(part,cont,"part")
     { std::vector<std::string> &entries=entries_map[part->getAttr<int>("signifikanz")];
       FOR_EACH_CONST_TAG_OF(i,*part,"content")
         entries.push_back(i->Value());
     }
     neuenArtikelAnlegen(warengruppe,schema,entries_map);
   }
   catch (std::exception &e)
   { std::cerr << "Exception " << e.what() << '\n';
   }
 }
 else if (argc==2) 
 {top_notebook->set_current_page(2);
  artikelbox->set_value(ArtikelBase(atoi(argv[1])));
  artikelbox_activate();
 }
 else top_notebook->set_current_page(1);
}


void Artikeleingabe::on_checkbutton_offene_auftraege_toggled()
{
 if(checkbutton_offene_auftraege->get_active())
  {
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

 Eingabe_fuer(artikelbox->get_value());
 fill_eingabebox(1);
 alias_warengruppe->set_extartbezid(artikelbox->getBezSchema()->Id());
 alias_warengruppe->set_value(artikelbox->getBezSchema()->Typ());
 fill_eingabebox(2);
 table_preis->Load();

 top_notebook->grab_focus();

 fire_toggles=true;
}


void Artikeleingabe::on_alias_warengruppe_activate()
{
 fill_eingabebox(2);
}


void Artikeleingabe::on_button_artikel_wechsel_clicked()
{
 try 
 {cH_Data_tree dt=tree->getSelectedRowDataBase_as<cH_Data_tree>();
  artikelbox->set_value(dt->Artikel());
  artikelbox_activate();
 } catch (SimpleTree::notLeafSelected &e)
 { try 
   { artikelbox->set_value(tree->getSelectedNode_as<const Data_Node>()->Artikel2());
     artikelbox_activate();
   }
   catch (...) {}
 }
 catch (...) {}
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
		datavec_t& datavec, vec_zeile_t vec_zeile, int maxrec)
{
   cH_ArtikelBezeichnung ABez(AB,artikelbox->getBezSchema()->Id());
   ArtikelBaum ABaum(AB);
   
   push_Artikel(vec_zeile,zeile_t(ABez,menge));
   if (!maxrec || ABaum.begin()==ABaum.end()) 
       datavec.push_back(new Data_tree(vec_zeile));
   else
    {
     for (ArtikelBaum::const_iterator i=ABaum.begin();i!=ABaum.end();++i)
      {
       ArtikelBaum_Pfad(ArtikelBase(i->rohartikel),i->menge,datavec,vec_zeile,maxrec-1);
      }
    }
}

void Artikeleingabe::on_leaf_selected(cH_RowDataBase d)
{
//cout << "Leaf\n";
 Handle<const Data_tree> dt=d.cast_dynamic<const Data_tree>();
 artikelbox->set_value(dt->Artikel());
// artikelbox_activate(); sollte eigentlich ...
 // zu bearbeitenden Artikel anzeigen
 Eingabe_fuer(dt->Artikel());
 Loeschen_von(dt->Artikel2());
}

void Artikeleingabe::on_node_selected(Handle<const TreeRow> node)
{
//std::cerr << "Node\n";
  Handle<const Data_Node> dn=node.cast_dynamic<const Data_Node>();
  Eingabe_fuer(dn->Artikel());
  Loeschen_von(dn->Artikel2());
}

void Artikeleingabe::Eingabe_fuer(const ArtikelBase& art)
{
 try{
 Artikel_Bestellen_bei->set_value(ArtikelStamm(art).BestellenBei());
 set_Prozess(); 
// frame_artikel->set_label("Artikel "+cH_ArtikelBezeichnung(art,artikelbox->getBezSchema()->Id())->Bezeichnung());
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

static void clear(Gtk::Container *cont)
{ while (!cont->get_children().empty())
  { cont->remove(**(cont->get_children().begin()));
  }
}

void Artikeleingabe::Loeschen_von(const ArtikelBase& art)
{
  if(!art) return;
  std::string button_text  = "Lösche\n"+cH_ArtikelBezeichnung(art,artikelbox->getBezSchema()->Id())->Bezeichnung();
  std::string button_text2 = "Artikel\n"+cH_ArtikelBezeichnung(art,artikelbox->getBezSchema()->Id())->Bezeichnung();
  button_artikel_delete->set_label(button_text);
  button_artikel_wechsel->set_label(button_text2);
  toolbar_loeschen->show();
}

void Artikeleingabe::on_unselect_row()
{
 toolbar_loeschen->hide();
}



void Artikeleingabe::on_button_artikel_delete_clicked()
{
 try
 {cH_Data_tree dt=tree->getSelectedRowDataBase_as<cH_Data_tree>();
  ArtikelBaum::delete_Artikel(dt->Artikel(),dt->Artikel2());
  on_neuladen_clicked();
 } catch (SimpleTree::notLeafSelected &e)
 { try 
   { Handle<const Data_Node> dt=tree->getSelectedNode_as<const Data_Node>();
      ArtikelBaum::delete_Artikel(dt->Artikel(),dt->Artikel2());
      on_neuladen_clicked();
   }
   catch (...) {}
 }
 catch (...) {}
}

void Artikeleingabe::on_Artikel_Bestellen_activate()
{
//cout << artikelboxb->get_Artikel().Id()<<'\t'<<artikelboxb->get_Menge()
//   <<'\t'<<artikelboxb->get_Instanz_Id()<<'\n';
  if(artikelboxb->get_Artikel().Id()==ArtikelBase::none_id )
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
 
  on_neuladen_clicked();
  Loeschen_von(artikelboxb->get_Artikel());
}

void Artikeleingabe::optionmenu_bestellen_bei_activate()
{ManuProC::Trace _t(trace_channel, __FUNCTION__, artikelbox->get_value());
 if (!(artikelbox->get_value())) return;
 ppsInstanz::ID oldInstanz=ArtikelStamm(artikelbox->get_value()).BestellenBei()->Id();
 ppsInstanz::ID newInstanz = Artikel_Bestellen_bei->get_value()->Id(); 
 ManuProC::Trace(trace_channel, "", oldInstanz,newInstanz);
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
  des = Glib::signal_timeout().connect(SigC::slot(*this,&Artikeleingabe::timeout_warnung),4000);
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
 for (unsigned i=0;i<tree->Cols();++i)
    tree->set_column_visibility(i,i<instanz_spalte.size());
}


void Artikeleingabe::on_togglebutton_edit_toggled()
{
#if 0
 if(togglebutton_edit->get_active()) frame_editieren->show();
 else frame_editieren->hide();
#endif 
}


void Artikeleingabe::eingabe_activate()
{
 try{
  save_edited_artikel();
  cH_ArtikelBezeichnung::Deregister(artikelbox->getBezSchema()->Id(),
				    artikelbox->get_value().Id());
  artikelbox->set_value(artikelbox->get_value());
  pixmap_edit->set(Gtk::StockID("gtk-stock-apply"), Gtk::IconSize(3));
  while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
  des = Glib::signal_timeout().connect(SigC::slot(*this,&Artikeleingabe::timeout_save_edited_artikel),4000);
 }catch(SQLerror &e) {mess->Show(e);}
}

void Artikeleingabe::on_einheit_activate()
{
 ArtikelStamm::setEinheit(artikelbox->get_value(),OM_Einheit->get_value());
}


gint Artikeleingabe::timeout_save_edited_artikel()
{
  pixmap_edit->set(Gtk::StockID("gtk-stock-cancel"), Gtk::IconSize(3));
  return 0;
}

void Artikeleingabe::on_kunde_activate()
{
  try{
  fill_eingabebox(2);
  alias_eingabe->grab_focus();
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
  alias_pixmap->set(Gtk::StockID("gtk-stock-apply"), Gtk::IconSize(3));
  while(Gtk::Main::events_pending()) Gtk::Main::iteration() ;
  des2 = Glib::signal_timeout().connect(SigC::slot(*this,&Artikeleingabe::timeout_save_edited_artikel2),4000);
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
  alias_pixmap->set(Gtk::StockID("gtk-stock-cancel"), Gtk::IconSize(3));
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
   Gtk2TeX::TreeView2Table(os,tree,tf);
   Gtk2TeX::Footer(os);
   os.close();
   pclose(f); 
}

cH_EntryValue Data_tree::Value(guint seqnr,gpointer gp) const
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
{  artikelbox->set_sensitive(p1==1 || p1==2 || p1==5);
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

void Artikeleingabe::save_edited_artikel()
{
  cH_ExtBezSchema schema = artikelbox->getBezSchema();
  std::string squery = "update artbez_"+itos(schema->Typ())+"_"+
         itos(schema->Id())+" set ";

  std::vector<bool> vec_all_empty;
  for(std::list<int>::const_iterator i=list_sig.begin();i!=list_sig.end();++i)
   {
     bool all_empty=true;
     for(unsigned int j=0;j<vec_eingabe.size();++j)
      {
        if( (*i) == vec_eingabe[j].signifikanz && 
            eingabebox->get_value(j) != "" ) all_empty=false ;
      }
     vec_all_empty.push_back(all_empty);
   }
  int count=0;
  int memsig=vec_eingabe[0].signifikanz;
  for (unsigned int i=0;i<vec_eingabe.size();++i)
   {
     if(memsig!=vec_eingabe[i].signifikanz)
      {
        ++count;
        memsig=vec_eingabe[i].signifikanz;
      }
     if(vec_all_empty[count]==true)
        squery += vec_eingabe[i].spalte +"=null, ";
     else
        squery += vec_eingabe[i].spalte +"='"+eingabebox->get_value(i)+"', ";
   }
  squery += " stand=now() where id='"+itos(artikelbox->get_value().Id())+"'";
  Query::Execute(squery);
  SQLerror::test(__FILELINE__);
}

bool Artikeleingabe::update_edited_artikel2()
{
  std::string squery = "update artbez_"+itos(alias_warengruppe->get_value())+"_"+
         itos(alias_schema->get_value())+" set ";

  std::vector<bool> vec_all_empty;
  for(std::list<int>::const_iterator i=list_sig2.begin();i!=list_sig2.end();++i)
   {
     bool all_empty=true;
     for(unsigned int j=0;j<vec_alias_eingabe.size();++j)
      {
        if( (*i) == vec_alias_eingabe[j].signifikanz && 
            alias_eingabe->get_value(j) != "" ) all_empty=false ;
      }
     vec_all_empty.push_back(all_empty);
   }
  int count=0;
  int memsig=vec_eingabe[0].signifikanz;
  ArgumentList args;
  for (unsigned int i=0;i<vec_alias_eingabe.size();++i)
   {
     if(memsig!=vec_alias_eingabe[i].signifikanz)
      {
        ++count;
        memsig=vec_alias_eingabe[i].signifikanz;
      }
     squery += vec_alias_eingabe[i].spalte +"=?,";
     if(vec_all_empty[count]==true) 
        args << Query::null();
     else 
        args << alias_eingabe->get_value(i);
   }
  squery += " stand=now() where id=?";
  Query q(squery);
  q << artikelbox->get_value() << args;
  return q.LinesAffected();
}


void Artikeleingabe::save_edited_artikel2()
{
//  cH_ExtBezSchema schema = artikelbox->getBezSchema();

  std::string squery = "insert into artbez_"+itos(alias_warengruppe->get_value())+"_"+
         itos(alias_schema->get_value())+" (id,";

  std::vector<bool> vec_all_empty;
  for(std::list<int>::const_iterator i=list_sig2.begin();i!=list_sig2.end();++i)
   {
     bool all_empty=true;
     
     for(unsigned int j=0;j<vec_alias_eingabe.size();++j)
      {
        if( (*i) == vec_alias_eingabe[j].signifikanz && 
            alias_eingabe->get_value(j) != "" ) all_empty=false ;
      }
     vec_all_empty.push_back(all_empty);
   }
  for (unsigned int i=0;i<vec_alias_eingabe.size();++i)
   {
     squery += vec_alias_eingabe[i].spalte +",";
   }
  ArgumentList args;
  squery += "stand) values (?,";
  args << artikelbox->get_value();
  int count=0;
  int memsig=vec_alias_eingabe[0].signifikanz;
  for (unsigned int i=0;i<vec_alias_eingabe.size();++i)
   {
     if(memsig!=vec_alias_eingabe[i].signifikanz)
      {
        ++count;
        memsig=vec_alias_eingabe[i].signifikanz;
      }
     squery+="?,";
     if(vec_all_empty[count]==true)
        args << Query::null();
     else
        args << alias_eingabe->get_value(i);
   }
  squery += "now())" ;
  Query q(squery);
  q << args;
  q.Check100();
}

#ifdef PETIG_EXTENSIONS
void Artikeleingabe::on_button_verschmelzen_clicked()
{
  ArtikelBase neu_art = artikelbox->get_value();
  ArtikelBase alt_art = artbox_verschmelzen->get_value();

  if(neu_art==ArtikelBase() || alt_art ==ArtikelBase()) return;
  Transaction tr;

  std::vector<std::string> tabellen1; // Spaltenname = id
  tabellen1.push_back("artikelstamm");
  tabellen1.push_back("artikelzusammensetzung");
  tabellen1.push_back("artbez_0_1");
  tabellen1.push_back("artbez_10_1");
  tabellen1.push_back("artbez_3_1");
  tabellen1.push_back("artbez_3_1000");
  tabellen1.push_back("artbez_3_362");
  tabellen1.push_back("artbez_3_629");
  tabellen1.push_back("artbez_3_704");
  tabellen1.push_back("artbez_4_1");
  tabellen1.push_back("artbez_4_10000");
  tabellen1.push_back("artbez_5_1");
  tabellen1.push_back("artbez_8_1");
  for (std::vector<std::string>::const_iterator i=tabellen1.begin();i!=tabellen1.end();++i)
   {
      Query("update "+*i+" set id=? where id=?") << neu_art << alt_art;
   }
   {
      Query("update artikelzusammensetzung set altartikelid=? where altartikelid=?") 
        << neu_art << alt_art;
   }
  std::vector<std::string> tabellen2; // Spaltenname = artikelid
  tabellen2.push_back("artikelpreise");
  tabellen2.push_back("lieferscheinentry");
  tabellen2.push_back("auftragentry");
  tabellen2.push_back("dispoentry");
  tabellen2.push_back("dispotabelle");
  tabellen2.push_back("extartbez");
  tabellen2.push_back("prod_changejournal");
  tabellen2.push_back("rl_hide");
  tabellen2.push_back("rohjumbo");
  tabellen2.push_back("rohjumbo_archiv");
  tabellen2.push_back("schaertabelle");
  tabellen2.push_back("schussdichte2");
  for (std::vector<std::string>::const_iterator i=tabellen2.begin();i!=tabellen2.end();++i)
   {
      Query("update "+*i+" set artikelid=? where artikelid=?") << neu_art << alt_art;
   }
  std::vector<std::string> tabellen3; // Spaltenname = material
  tabellen3.push_back("bl_inhalt");
  tabellen3.push_back("bl_log");
  tabellen3.push_back("rl_inhalt");
  tabellen3.push_back("rl_inhalt_old");
  tabellen3.push_back("rl_log");
  for (std::vector<std::string>::const_iterator i=tabellen3.begin();i!=tabellen3.end();++i)
   {
      Query("update "+*i+" set material=? where material=?") << neu_art << alt_art;
   }
  std::vector<std::string> tabellen4; // Spaltenname = artikel
  tabellen4.push_back("ketten_garn");
  for (std::vector<std::string>::const_iterator i=tabellen4.begin();i!=tabellen4.end();++i)
   {
      Query("update "+*i+" set artikel=? where artikel=?") << neu_art << alt_art;
   }
   {
      Query("update ketten_garn set garn=? where garn=?") << neu_art << alt_art;
   }
  std::vector<std::string> tabellen5; // Spaltenname = art_id
  tabellen5.push_back("maschinen_geschwindigkeit");
  for (std::vector<std::string>::const_iterator i=tabellen5.begin();i!=tabellen5.end();++i)
   {
      Query("update "+*i+" set art_id=? where art_id=?") << neu_art << alt_art;
   }
 tr.commit();
}
#endif
