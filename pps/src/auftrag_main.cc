/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001-2005 Adolf Petig GmbH & Co. KG, 
 *  written by Jacek Jakubowski
 *  Copyright (C) 2006 Christof Petig
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

#include "auftrag_main.hh"
#include "auftrag_bearbeiten.hh"
#include "auftragbase.h"
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/auftrag_status.h>
#include "auftrag_lieferschein.hh"
#include "auftrag_rechnung.hh"
#include <Gtk2TeX.h>
#include <fstream.h>
#include <gtkmm/radiomenuitem.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/Einheiten.h>
#include <algorithm>
#include <Misc/Global_Settings.h>
#include "MyMessage.h"
#include <VerfuegbarPixmap.h>
//#include <Lager/Lager_Vormerkungen.hh>
#include <unistd.h>
#include <MyWindow.hh>
#include <Misc/Trace.h>
#include <Misc/FILEstream.h>
#include <Auftrag/Verfuegbarkeit.h>
#include <sigc++/bind.h>
#include <Misc/i18n.h>
#include "lieferscheinliste.hh"
#include <DynamicEnums/DynamicConfig.h>

const UniqueValue::value_t auftrag_main::trace_instanzen
      = ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("DEBUG_INSTANZ",auftrag_main::trace_instanzen);

extern MyMessage *meldung;

bool stuecksumme;/* ob stueck summiert werden sollen */
bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
                        /* gespeichert werden soll */
extern bool load_list;

#include <Misc/Ausgabe_neu.h>
#include "aktAufEintrag.h" //wg. Lieferwoche ???
#include <cstdio>
#include "class_auftrag_tree.hh"

class MatListSort
{
   public:  
     enum esort {MENGE,ARTIKEL};
   private:
     esort es;
   public:
     MatListSort(enum esort _es):es(_es) {}
     bool operator() (const auftrag_main::artmeng &x,
                      const auftrag_main::artmeng &y) const
           { 
             switch(es) {
               case(MENGE)   : return x.second.summe() > y.second.summe() ;
               case(ARTIKEL) : return cH_ArtikelBezeichnung(x.first.art) < cH_ArtikelBezeichnung(y.first.art) ;
           }return false;}
};


bool auftrag_main::on_delete_event(GdkEventAny*)
{   
 on_beenden_activate();
 
 return true;
}


void auftrag_main::on_beenden_activate()
{   
 save_WindowSize();
 maintree_s->detach();
// tree_lager_frei->detach_from_clist();
// tree_lager_verplant->detach_from_clist();
 Gtk::Main::instance()->quit();
}

void auftrag_main::save_WindowSize()
{
  gint width,height,x,y;
  Glib::RefPtr<Gdk::Window> fenster=get_window();
  fenster->get_size(width,height);
  fenster->get_position(x,y);
  Global_Settings::create(int(getuid()),"pps","Size",itos(width)+":"+itos(height));
  Global_Settings::create(int(getuid()),"pps","Position",itos(x)+":"+itos(y));
}


void auftrag_main::on_erfassen_activate()
{   
    try
    { try 
      { AufEintragBase to_edit(instanz,searchcombo_auftragid->Content(),-1);
std::cerr << searchcombo_auftragid->Content() << '-' <<to_edit << '\n';      
        new auftrag_bearbeiten(instanz,&to_edit);
      }
      catch (...)
      { new auftrag_bearbeiten(instanz,selected_AufEintrag);
      }
    } catch (SQLerror &e)
    {  std::cerr << e << '\n';
    } 
//    selected_AufEintrag=0; (es wird doch nichts abgewählt)
}

void auftrag_main::on_neuladen_activate()
{
 fill_simple_tree();
}

void auftrag_main::neuladen()
{
 if(preload_orders->get_active() && load_list) fill_simple_tree();
 else maintree_s->clear();
}



#define TEXCMD "tex2prn -2 -t landscape"

bool begins_with(std::string const& a,std::string const& part)
{ return (a.size()>=part.size()) && (a.substr(0,part.size())==part);
}

static std::string kopfzeile(int col,const std::string &typ,
const std::string &title,gpointer user_data)
{  if (begins_with(title,_("Verarbeitung"))) return "p{3.5cm}";
   return typ;
}

#warning Umbenennung der Spalten ist hier nicht berücksichtigt ...
static std::string shorten_some(int col,const std::string &title,gpointer user_data)
{  if (begins_with(title,_("Artikel")) ||
	begins_with(title,_("Breite")) ||
	begins_with(title,_("Farbe")) ||
	begins_with(title,_("Meter")) ||
	begins_with(title,_("Aufmachung")) ||
	begins_with(title,_("Lieferwoche")) ||
	begins_with(title,_("Letzte Lief")) ||
	begins_with(title,_("offene")))
      return "\\tiny "+Gtk2TeX::string2TeX(title);
   else
      return Gtk2TeX::string2TeX(title);
}

void auftrag_main::on_main_drucken_activate()
{  
   FILE *f=popen(TEXCMD,"w");
   oFILEstream os(f);

   Gtk2TeX::HeaderFlags fl;
   fl.ptsize=10;
   static float inc=2.54;
   fl.leftmargin=1.0/inc;
   fl.topmargin=1.0/inc; 
   fl.rightmargin=1.0/inc;
   fl.bottommargin=1.0/inc;

   fl.landscape=true;

   fl.leftfoot=Gtk2TeX::string2TeX(_("offene Aufträge"));
   fl.rightfoot="\\today";
   Gtk2TeX::Header(os,fl);
   Gtk2TeX::TableFlags tf;
   tf.columntype_cb=&kopfzeile;
   tf.columntitle_cb=&shorten_some;
   tf.multicolumn=true;	
   if (maintree_s->get_selection()->count_selected_rows())
   {  
      tf.firstrow_cb=&FirstRow;
      tf.sequence=maintree_s->get_seq();
      
      maintree_s->get_selection()->get_selected_rows().assign_to(tf.selection);
      
      { Gtk::TreeModel::Path path=*(maintree_s->get_selection()->get_selected_rows().begin());
        Gtk::TreeModel::iterator i=maintree_s->getStore()->get_iter(path);
        tf.user_data=static_cast<cH_RowDataBase>((*i)[maintree_s->getStore()->m_columns.leafdata])->ref();
        tf.deep=(*i)[maintree_s->getStore()->m_columns.deep];
      }
   }
   Gtk2TeX::TreeView2Table(os,maintree_s,tf);
   if (tf.user_data) HandleContent::unref(tf.user_data);
   Gtk2TeX::Footer(os);
   os.close();
   pclose(f);
}


void auftrag_main::on_lieferscheine_activate()
{   
 new auftrag_lieferschein(instanz);
}

void auftrag_main::on_rechnung_activate()
{   
 new auftrag_rechnung(instanz);
}

// eigentlich sind diese Routine Bestandteil von Tag (in C++)
std::string auftrag_main::bool2str(bool b)
{
  if(b) return "true";
  else return "false";
}

bool auftrag_main::str2bool(std::string s,bool def)
{
  if(s=="false") return false;
  else if(s=="true") return true;
  else return def;
}


void auftrag_main::on_preload_orders_activate()
{
 if(block_callback) return;
 Global_Settings::create(int(getuid()),"pps","PreloadOrders",bool2str(preload_orders->get_active())); 
}
void auftrag_main::on_main_bezeichnung_activate()
{
 if(block_callback) return;
 interne_namen_bool = main_intbez->get_active();
 fill_simple_tree();
 Global_Settings::create(int(getuid()),"pps","InterneBezeichnung",bool2str(interne_namen_bool)); 
}
void auftrag_main::on_kunden_anr_activate()
{
  if(block_callback) return;
  kunden_anr_bool=kundenauftragsnummer->get_active();
  fill_simple_tree();
  Global_Settings::create(int(getuid()),"pps","Kundenauftragsnummer",bool2str(kunden_anr_bool)); 
}
void auftrag_main::on_zeitdarstellung_activate()
{
  if(block_callback) return;
  zeit_kw_bool=kalenderwoche->get_active();
  fill_simple_tree();
  Global_Settings::create(int(getuid()),"pps","KalenderWoche",bool2str(zeit_kw_bool)); 
}

enum { KD_NAME, KD_NAME_ORT, KD_NR };

void auftrag_main::on_kundendarstellung_activate()
{
  if(block_callback) return;
  kunden_nr_bool=kunden_nr->get_active();
  kunden_mit_ort=kunden_ort->get_active();
  fill_simple_tree();
  Global_Settings::create(int(getuid()),"pps","KundenNr",
  	itos(kunden_nr_bool?KD_NR:(kunden_mit_ort?KD_NAME_ORT:KD_NAME)));
//  bool2str(kunden_nr_bool)); 
}
void auftrag_main::on_materialbedarf_sortiert()
{
  if(block_callback) return;
  bool b=materialbedarf_sortiert_nach_artikel->get_active();
  Global_Settings::create(int(getuid()),"pps","Materialbedarf_Artikelsortiert",bool2str(b)); 
}

void auftrag_main::statusaenderung()
{
  if(block_callback) return;
  on_neuladen_activate();
  Global_Settings::create(int(getuid()),"pps","Status_shown",itos(selected_status)); 
}

void auftrag_main::on_closed_auftraege_activate()
{ if(geschlossene_auftraege->get_active())  
    { selected_status=CLOSED; statusaenderung();}}

void auftrag_main::on_offene_auftraege_activate()
{ if(offene_auftraege->get_active())
    {selected_status=OPEN; statusaenderung();}}

void auftrag_main::on_unbest_auftraege_activate()
{ if (unbestaetigte_auftraege->get_active()) 
   { selected_status=UNCOMMITED; statusaenderung();}}

void auftrag_main::on_storno_auftraege_activate()
{ if (stornierte_auftraege->get_active())    
   {selected_status=STORNO; statusaenderung();}}

void auftrag_main::on_alle_auftraege_activate()
{
 if(alle_auftraege->get_active())
   {selected_status=OPEN_AND_UNCOM; statusaenderung();}}  



void auftrag_main::auftrags_id_aenderung()
{
  AuftragBase::ID id=auftrag_main::SelectedAuftragsId();

  if(id==AuftragBase::ungeplante_id) 
    { frame_offene_mengen->set_label(_("ungeplante / fehlende Menge"));
    }
  else if(id==AuftragBase::plan_auftrag_id) 
    { frame_offene_mengen->set_label(_("geplante / bestellte / reservierte Menge"));
    }
  else if(id==AuftragBase::dispo_auftrag_id)
    { frame_offene_mengen->set_label(_("verfügbare Menge"));
    }
  else if(id==AuftragBase::none_id)
    { frame_offene_mengen->set_label(_("alle Lagermengen"));
    }

  frame_instanzen->hide() ;
  show_frame_instanzen_material();

  if(block_callback) return;
  on_neuladen_activate();
}


AuftragBase::ID auftrag_main::SelectedAuftragsId() const
{
  if     (ungeplante_menge_menu->get_active()) return AuftragBase::ungeplante_id;
  else if(plan_menge_menu->get_active()) return AuftragBase::plan_auftrag_id;
  else if(dispo_menge_menu->get_active()) return AuftragBase::dispo_auftrag_id;
  else if(alle_lagermengen->get_active()) return AuftragBase::none_id;
  else assert(!"get here");
  return AuftragBase::none_id;
}



void auftrag_main::on_auftraege_kunde_activate()
{
  if(block_callback) return;
  auftraege_mit_kunden_bool=auftraege_mit_kunde->get_active();
  Global_Settings::create(int(getuid()),"pps","Auftraege_mit_Kunde",bool2str(auftraege_mit_kunden_bool)); 
}


void auftrag_main::loadEinstellungen()
{
  MyWindow::setPositionSize(*this,"pps");
  ////////////////////////////////////////////////
  block_callback=true;
  std::string bs;

  bs=Global_Settings(int(getuid()),"pps","PreloadOrders").get_Wert(); 
  preload_orders->set_active(str2bool(bs,true));

  bs=Global_Settings(int(getuid()),"pps","InterneBezeichnung").get_Wert(); 
  interne_namen_bool=str2bool(bs,true);
  main_intbez->set_active(interne_namen_bool);

  bs=Global_Settings(int(getuid()),"pps","Kundenauftragsnummer").get_Wert(); 
  kunden_anr_bool=str2bool(bs,true);
  kundenauftragsnummer->set_active(kunden_anr_bool);

  bs=Global_Settings(int(getuid()),"pps","KalenderWoche").get_Wert(); 
  zeit_kw_bool=str2bool(bs,true);
  kalenderwoche->set_active(zeit_kw_bool);

  bs=Global_Settings(int(getuid()),"pps","KundenNr").get_Wert(); 
#if 1 // backward compatible
  kunden_nr_bool=str2bool(bs,false);
  bool valid=str2bool(bs,false)==str2bool(bs,true);
  int was_denn=valid?(kunden_nr_bool?KD_NR:KD_NAME_ORT):atoi(bs.c_str());
#else
  int was_denn=atoi(bs.c_str());
#endif
  switch (was_denn)
  {  case KD_NR: kunden_nr->set_active(true);
        kunden_nr_bool=true;
        break;
     case KD_NAME_ORT: kunden_ort->set_active(true);
        kunden_nr_bool=false;
        kunden_mit_ort=true;
        break;
     case KD_NAME: kunden_name1->set_active(true);
        kunden_nr_bool=false;
        kunden_mit_ort=false;
        break;
  }

  bs=Global_Settings(int(getuid()),"pps","Materialbedarf_Artikelsortiert").get_Wert(); 
  materialbedarf_sortiert_nach_artikel->set_active(str2bool(bs,true));

  AufStatVal status;
  bs = Global_Settings(int(getuid()),"pps","Status_shown").get_Wert(); 
  if (bs=="") status=OPEN;
  else status=AufStatVal(atoi(bs.c_str()));

  if(status==OPEN)  offene_auftraege->set_active(true);
  else if(status==UNCOMMITED)  unbestaetigte_auftraege->set_active(true);
  else if(status==STORNO)  stornierte_auftraege->set_active(true);
  else if(status==CLOSED)  geschlossene_auftraege->set_active(true);
  else if(status==OPEN_AND_UNCOM) alle_auftraege->set_active(true);
  selected_status=status;

  AuftragBase::ID _id_;
  bs = Global_Settings(int(getuid()),"pps","Auftrag_ID_shown").get_Wert(); 
  if (bs=="") _id_=AuftragBase::plan_auftrag_id;
  else _id_=atoi(bs.c_str());

  if     (_id_==AuftragBase::ungeplante_id) ungeplante_menge_menu->set_active();
  else if(_id_==AuftragBase::plan_auftrag_id) plan_menge_menu->set_active();
  else if(_id_==AuftragBase::dispo_auftrag_id) dispo_menge_menu->set_active();
  else assert(!"never get here\n");

  bs=Global_Settings(int(getuid()),"pps","Auftraege_mit_Kunde").get_Wert(); 
  auftraege_mit_kunden_bool=str2bool(bs,false);
  auftraege_mit_kunde->set_active();

  ArtikelTyp at=atoi(Global_Settings(int(getuid()),"pps",
			"warengruppe maintree").
			get_Wert().c_str());
  offen_warengruppe->set_value(at);

  block_callback=false;
}


#include "bestell_plan.hh"
extern bestell_plan *BP;
void auftrag_main::on_bestellplanung_activate()
{  
 if(BP==NULL)
   BP=new bestell_plan();
}

void auftrag_main::on_mainprint_button_clicked()
{
  on_main_drucken_activate();
}


auftrag_main::auftrag_main()
  : allaufids(), instanz(ppsInstanzID::Kundenauftraege), selected_AufEintrag(),
    block_callback(), atyp(ArtikelTyp::none_id), instanz_auftrag()
{
 menu_instanz();
 loadEinstellungen();

#ifndef PETIG_EXTENSIONS    
    button_faerben->hide();
#endif     
    button_auftrag_erledigt->hide(); // nicht implementiert
    button_Kunden_erledigt->hide();  // nicht implementiert

 kunden_lieferant->setLabel(_("Lieferantennr"),_("Lieferantenname"));
#ifdef HAS_ADDR_GROUP_Lieferanten
 kunden_lieferant->EinschraenkenKdGr(KundengruppeID::Lieferanten);
#endif
 set_column_titles_of_simple_tree();
 neuer_auftrag_tree_titel_setzen();

 instanz_selected(instanz); 
 load_list=true;		// nur beim Start ggf. nicht laden
 maintree_s->set_remember("pps","maintree-"+itos(instanz->Id()));
 maintree_s->addMenuItem(_("Auftrag anzeigen"))
   .connect(sigc::bind(sigc::mem_fun(*this,&auftrag_main::on_menu_selection),M_Auftrag));
 maintree_s->addMenuItem(_("Artikel bearbeiten"))
   .connect(sigc::bind(sigc::mem_fun(*this,&auftrag_main::on_menu_selection),M_Artikel));
 maintree_s->addMenuItem(_("Kunde bearbeiten"))
   .connect(sigc::bind(sigc::mem_fun(*this,&auftrag_main::on_menu_selection),M_Kunde));
 maintree_s->addMenuItem(_("Lieferungen zeigen"))
   .connect(sigc::bind(sigc::mem_fun(*this,&auftrag_main::on_menu_selection),M_Lieferungen));
}

void auftrag_main::on_menu_selection(menu_selection m)
{ switch (m)
  { case M_Auftrag:
      new auftrag_bearbeiten(instanz,selected_AufEintrag);
      break;
    case M_Artikel: on_button_artikeleingabe_clicked();
      break;
    case M_Kunde:
      { if (selected_AufEintrag )
        {
          std::string s = "kundendaten "+itos(selected_AufEintrag->getKdNr())+" &";
          system(s.c_str());
//          Kunde::UnCache(selected_AufEintrag->getKdNr());
        }
      }
      break;
    case M_Lieferungen:
      { if (selected_AufEintrag )
        { new lieferscheinliste(instanz,selected_AufEintrag->getKdNr(),selected_AufEintrag->Artikel());
        }
      }
      break;
  }
}

void auftrag_main::set_column_titles_of_simple_tree()
{
 const int signif=1;
 std::vector<std::string> ct;
 ct=std::vector<std::string>(maintree_s->MaxColumns());

 if(instanz->ExterneBestellung())
   ct[KUNDE]=_("Lieferant");
 else
   ct[KUNDE]=_("Kunde");

 int artcol=0;
 try
 {
 cH_ExtBezSchema ebz(ExtBezSchema::default_id,offen_warengruppe->get_value());
 ExtBezSchema::const_sigiterator bezend=ebz->sigend(signif);
 for(ExtBezSchema::const_sigiterator bezit=ebz->sigbegin(signif); 
	bezit!=bezend && (artcol<4); ++bezit,artcol++) 
    ct[artcol+A1]=_(bezit->bezkomptext.c_str());
 }
 catch (SQLerror &e)
 {  if (e.Code()!=100) 
    {  std::cerr << e << '\n'; 
       throw;
    }
 }
// for(int j=i; j<4; j++) // auffüllen bis 4
//   ct.push_back("");

 ct[LIEFERDATUM]=_("Lieferwoche");
 if(instanz->ExterneBestellung())
   ct[AUFTRAG]=_("Bestellung");
 else
   ct[AUFTRAG]=_("Auftrag");

 ct[LETZTEPLANINSTANZ]=_("L.P.");
 ct[VERARBEITUNG]=_("Ver.");
 ct[LETZTELIEFERUNG]=_("Letzte Lief.");
 ct[INSTANZEN]=_("Abteilungen");
 ct[METER]=_("offene Menge/Stk. (Rohware)");
 ct[STUECK]=_("offene Menge/Stk.");
#ifdef PETIG_EXTENSIONS
 // Petig verwendet andere Spalten ...
 ct[METER]=_("offene Meter");
 ct[STUECK]=_("offene Stück");
#endif

 maintree_s->setTitles(ct);
 maintree_s->setResizeable(true);
 maintree_s->set_NewNode(&Data_Node::create);
 int cols=maintree_s->MaxColumns();
 std::vector<gfloat> al(cols);
 al[cols-1]=1.0;
 al[cols-2]=1.0;
 maintree_s->setAlignment(al);
}

void auftrag_main::fill_simple_tree()
{
#ifndef MABELLA_EXTENSIONS
 stop_idle();
#endif

  if(allaufids) { delete(allaufids); allaufids=0; }
//  if(instanz->LagerInstanz())
//      lager_zeigen();   
//  else
   {
    selected_AufEintrag=0;
    if(!allaufids) 
      { 
        SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(
			instanz->Id(),
			selected_status,
			SelectedAuftragsId(),
			offen_warengruppe->get_value());
        allaufids = new SelectedFullAufList(psel);
      }
    std::vector<cH_RowDataBase> datavec;
    for(SelectedFullAufList::const_iterator i = allaufids->begin();i!=allaufids->end(); ++i)
     {
//      if(instanz->Id()!=ppsInstanzID::Kundenauftraege && i->Id()!=0) continue;
      datavec.push_back(new Data_auftrag(*i,this));
     }
    maintree_s->setDataVec(datavec);
   }
#ifndef MABELLA_EXTENSIONS
//   start_idle();
#endif
}

void auftrag_main::start_idle()
{ idle_iter.clear();
  idle_iter.push_back(maintree_s->get_model()->children().begin());
  idle_con=Glib::signal_idle().connect(SigC::slot(*this,&auftrag_main::idle_fill));
}

void auftrag_main::stop_idle()
{ idle_con.disconnect();
}

bool auftrag_main::idle_fill()
{ if (!idle_iter.empty())
  {  // std::cerr << idle_iter.size() << '\n';
//     const TreeRow *tlr=reinterpret_cast<const TreeRow *>(idle_iter.back()->get_user_data());
     if (!guint((*idle_iter.back())[maintree_s->getStore()->m_columns.childrens_deep]) 
             && maintree_s->ColumnVisible(INSTANZEN))
     {  Verfuegbarkeit::map_det_t res;
        Handle<Data_auftrag> hda=cH_RowDataBase((*idle_iter.back())[maintree_s->getStore()->m_columns.leafdata])
          .cast_dynamic<const Data_auftrag>()
          .cast_const<Data_auftrag>();
          
        Verfuegbarkeit::benoetigt(hda->get_AufEintrag(),res);
        std::string departments;
        for (Verfuegbarkeit::map_det_t::const_iterator i=res.begin();
                i!=res.end();++i)
        {  if (!i->first.Instanz()->overview.empty() 
                && departments.find(i->first.Instanz()->overview)==std::string::npos)
             departments+=i->first.Instanz()->overview;
        }
        if (!departments.empty())
        {  hda->departments=departments;
           maintree_s->redisplay(idle_iter.back(),INSTANZEN);
        }
     }
     else if (idle_iter.back()->children().begin()!=idle_iter.back()->children().end())
     { idle_iter.push_back(idle_iter.back()->children().begin());
       return true;
     }
    reiterate:
     ++idle_iter.back();
     if (idle_iter.begin()+1==idle_iter.end()) // oberste Ebene
     { if (idle_iter.back()==maintree_s->get_model()->children().end())
       { idle_iter.clear();
         return false;
       }
       return true;
     }
     else
     { // Ende des Elter erreicht?
       if (idle_iter.back()==(*(idle_iter.end()-2))->children().end())
       { idle_iter.pop_back();
         goto reiterate;
       }
       return true;
     }
  }
  return false;
}

void auftrag_main::on_node_selected(Handle<const TreeRow> node)
{
  Verfuegbarkeit::map_t map_allart;
  Gtk::TreeModel::Path path=*(maintree_s->get_selection()->get_selected_rows().begin());
  Gtk::TreeModel::iterator i=maintree_s->getStore()->get_iter(path);
  getAufEintrag_fromNode(i->children().begin(),i->children().end(),map_allart);
  instanz_menge(map_allart);
  // Überschriften setzen 
  schema_select(cH_ArtikelBezeichnung(static_cast<cH_RowDataBase>
      ((*i)[maintree_s->getStore()->m_columns.leafdata])
      .cast_dynamic<const Data_auftrag>()->get_Artikel())
  	->getExtBezSchema());
//  erfassen_button->set_sensitive(false);
}

void auftrag_main::getAufEintrag_fromNode(Gtk::TreeModel::iterator b,
         Gtk::TreeModel::iterator e, Verfuegbarkeit::map_t& M)
{  
 for (Gtk::TreeModel::iterator i=b;i!=e;++i)
  {  if(!guint((*i)[maintree_s->getStore()->m_columns.childrens_deep]))
      {
        Handle<const Data_auftrag> dt=cH_RowDataBase
            ((*i)[maintree_s->getStore()->m_columns.leafdata])
            .cast_dynamic<const Data_auftrag>();
        if (togglebutton_material->get_active()) 
          Verfuegbarkeit::verfuegbar(dt->get_AufEintrag(),M);
        else if(togglebutton_auftraege->get_active())
          Verfuegbarkeit::wozu_benoetigt(dt->get_AufEintrag(),M);
      }
    else if (i->children().begin()!=i->children().end()) 
      getAufEintrag_fromNode(i->children().begin(),i->children().end(),M);
  }
}

void auftrag_main::on_button_faerben_clicked()
{
#ifdef PETIG_EXTENSIONS
  try{
      cH_Data_auftrag dt=maintree_s->getSelectedRowDataBase_as<cH_Data_auftrag>();
      AufEintragZu::list_t kinder=AufEintragZu(dt->get_AufEintrag())
          .get_Referenz_listFull(AufEintragZu::list_kinder,false);
      bool done=false;
      for (AufEintragZu::list_t::reverse_iterator i=kinder.rbegin();
                i!=kinder.rend();++i)
      { if (i->AEB.Instanz()->Id()==ppsInstanzID::Faerberei
            || i->AEB.Instanz()->Id()==ppsInstanzID::Druckerei)
        { if (i->AEB.Id()!=Auftrag::ungeplante_id) continue;
          AufEintrag ae(i->AEB);
          if (!ae.getRestStk()) continue;
          Auftrag a(make_value(Auftrag::PassenderPlanungsAuftrag(i->AEB.Instanz()->Id())));
          ae.Planen(ae.getRestStk(),a,ae.getLieferdatum());
          done=true;
        }
      }
      if (!done)
      {
        cH_ppsInstanz I(ppsInstanzID::Kundenauftraege);
        dt->get_AufEintrag().setLetztePlanungFuer(I);
      }
      dt->get_AufEintrag().reload();
      dt->redisplayLetzePlanInstanz(maintree_s);
  } catch (SimpleTree::SelectionError &e) {std::cerr << e.what()<<'\n';}
#endif  
}



void auftrag_main::on_leaf_selected(cH_RowDataBase d)
{
 try{
 const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*d);
 selected_AufEintrag = &dt->get_AufEintrag();
//cout << "SE="<< selected_AufEintrag<<'\t'<<selected_AufEintrag->Instanz()->Name()<<' '<<selected_AufEintrag->Id()<<' '<<selected_AufEintrag->ZNr()<<'\n';;
//cout << dt->get_AufEintrag()<<'\t'<<dt->get_AufEintrag().editierbar()<<'\n';
// if(dt->get_AufEintrag().editierbar())
      erfassen_button->set_sensitive(true);
// else erfassen_button->set_sensitive(false);
  // Überschriften setzen 
  schema_select(cH_ArtikelBezeichnung(dt->get_Artikel())->getExtBezSchema());
 show_something_for(*selected_AufEintrag);
 instanz_auftrag_anlegen(*selected_AufEintrag); 
 }catch(std::exception &e) {std::cerr<<e.what();}
}


void auftrag_main::show_something_for(AufEintrag& selAufEintrag)
{
 if(instanz->Id() != ppsInstanzID::Kundenauftraege 
      && !instanz->LagerInstanz()
      && !togglebutton_material->get_active()
      && !togglebutton_auftraege->get_active()) 
;//YYY     instanz_leaf_auftrag(selAufEintrag);
 else
  {
 // Zum Anzeigen des benötigten Materials
    Verfuegbarkeit::map_t map_allart;
    if(togglebutton_material->get_active()) 
       Verfuegbarkeit::verfuegbar(selAufEintrag,map_allart);
    else if(togglebutton_auftraege->get_active())
       Verfuegbarkeit::wozu_benoetigt(selAufEintrag,map_allart);
    instanz_menge(map_allart); 
  }
}

void auftrag_main::on_unselect_row()
{
   erfassen_button->set_sensitive(true);
//  show_frame_instanzen_material();
}



void auftrag_main::show_selected_line()
{
/*  if(lager)
   {
    try{
       cH_RowDataBase dt(tree_lager_verplant->getSelectedRowDataBase());
       on_lager_leaf_selected(dt);
      } catch(std::exception &e) {}
    try{
       TreeRow dt(tree_lager_verplant->getSelectedNode());
       on_lager_node_selected(dt);
      } catch(std::exception &e) {}
   }
  else
*/
   {
    try{
       cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
       on_leaf_selected(dt);
      } catch(std::exception &e) {}
    try{
       Handle<const TreeRow> dt(maintree_s->getSelectedNode());
       on_node_selected(dt);
      } catch(std::exception &e) {}
   }
 show_frame_instanzen_material();   
}

void auftrag_main::instanz_menge(const Verfuegbarkeit::map_t& map_allart)
{ ManuProC::Trace _t(trace_instanzen, __FUNCTION__);
  if(!togglebutton_material->get_active() &&
     !togglebutton_auftraege->get_active()) 
    return;
  // 1. Schritt: Wieviel Instanzen gibt es?
  //             Und Artikel nach Instanzen sortiert in Listen schreiben
  std::map<cH_ppsInstanz,std::list<artmeng> > LM;
  for (Verfuegbarkeit::map_t::const_iterator i=map_allart.begin();i!=map_allart.end();++i)
    {
      LM[i->first.inst].push_back(artmeng(i->first,i->second));
      ManuProC::Trace(trace_instanzen, __FILELINE__,i->first.inst,i->first.art,
      		i->second.vorraetig,i->second.geplant,i->second.ungeplant);
    }
  // 2. Schritt Überschriften (Instanzen) in die Tabelle schreiben
  //            UND Listen sortieren UND in Tabelle einfügen
  Gtk::Table *table_materialbedarf = manage(new Gtk::Table(0,0,false));
  table_materialbedarf->set_col_spacings(5);
  for(std::map<cH_ppsInstanz,std::list<artmeng> >::iterator i=LM.begin();i!=LM.end();++i)
    {
      // Überschriften
      Gtk::Label *l=manage (new Gtk::Label(" "+i->first->get_Name()+" "));
      int col = i->first->Sortierung();
      std::string pat;
      for (guint u=0;u<i->first->get_Name().size()+2;++u) pat+="_";
      l->set_pattern(pat);
      l->set_alignment(0, 0.5);
      l->set_padding(0, 0);
      l->show();
      table_materialbedarf->attach(*l,col,col+1,0,1,Gtk::AttachOptions(),Gtk::AttachOptions(),0,0);

     // Spalten sortieren
     if(materialbedarf_sortiert_nach_artikel->get_active())
        i->second.sort(MatListSort(MatListSort::ARTIKEL)); 
     else if(materialbedarf_sortiert_nach_menge->get_active())
        i->second.sort(MatListSort(MatListSort::MENGE));

     int row=1;
     Gtk::Table *tab=manage(new Gtk::Table(0,0,false));
     for(std::list<artmeng>::const_iterator j=i->second.begin();j!=i->second.end();/*siehe unten*/)
      {
        ++row;
        {  
           Glib::RefPtr<Gdk::Pixbuf> pixbuf=ManuProC::VerfuegbarPixmap(j->second);
           Gtk::Image *pixmapG=manage(new Gtk::Image(pixbuf));
           tab->attach(*pixmapG,0,1,row,row+1,Gtk::FILL,Gtk::AttachOptions(),0,0);
           pixmapG->show();
        }
        int col2=1;
        if(/*auftraege_mit_kunden_bool &&*/ j->first.kunde!=Kunde::default_id)
         {
           Gtk::Label *lk=manage (new Gtk::Label(cH_Kunde(j->first.kunde)->sortname()+": "));
          lk->set_justify(Gtk::JUSTIFY_LEFT);
          lk->set_alignment(1.0, 0.5);
          lk->set_padding(0, 0);
          tab->attach(*lk,col2,col2+1,row,row+1,Gtk::FILL,Gtk::AttachOptions(),0,0);
          lk->show();
         }
        ++col2;
         
        Gtk::Label *la=manage (new Gtk::Label(cH_ArtikelBezeichnung(j->first.art)->Bezeichnung()+" "));
        Einheit E(j->first.art);
        std::string menge = Formatiere_short(j->second.summe());
        std::string einheit = E.MengenEinheit();
        Gtk::Label *lm=manage (new Gtk::Label(menge+einheit+" "));
        la->set_justify(Gtk::JUSTIFY_LEFT);
        la->set_alignment(0, 0.5);
        la->set_padding(0, 0);
        lm->set_justify(Gtk::JUSTIFY_RIGHT);
        lm->set_alignment(1.0, 0.5);
        lm->set_padding(0, 0);
        if(materialbedarf_sortiert_nach_artikel->get_active())
          { tab->attach(*la,col2  ,col2+1,row,row+1,Gtk::FILL,Gtk::AttachOptions(),0,0);
            tab->attach(*lm,col2+1,col2+2,row,row+1,Gtk::FILL,Gtk::AttachOptions(),0,0); }
        else 
          { tab->attach(*lm,col2  ,col2+1,row,row+1,Gtk::FILL,Gtk::AttachOptions(),0,0);
            tab->attach(*la,col2+1,col2+2,row,row+1,Gtk::FILL,Gtk::AttachOptions(),0,0); }
        lm->show();
        la->show();
        ++j;
      }
     table_materialbedarf->attach(*tab,col,col+1,1,2,Gtk::FILL,Gtk::EXPAND|Gtk::FILL,0,0);
     tab->show();
   }
 viewport_materialbedarf->remove();
 viewport_materialbedarf->add(*table_materialbedarf);
 table_materialbedarf->show();
// table_materialbedarf->show_all();
 show_frame_instanzen_material();
}

void auftrag_main::on_button_auftrag_erledigt_clicked()
{
  try{
  try{
  try{
     cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
     dynamic_cast<const Data_auftrag*>(&*dt)->get_AufEintrag().setStatus(CLOSED,int(getuid()));
     dynamic_cast<Data_auftrag&>(const_cast<RowDataBase&>(*dt)).redisplayMenge(maintree_s);
  }catch(SQLerror &e) {meldung->Show(e);}
  }catch(SimpleTree::notLeafSelected &x) {std::cerr << x.what()<<'\n';}
  }catch(SimpleTree::noRowSelected &x) {std::cerr << x.what()<<'\n';}
 selected_AufEintrag=0;
}


void auftrag_main::on_button_artikeleingabe_clicked()
{
 if (selected_AufEintrag )//sonst geht das nur bei Kundenaufträgen->  && selected_AufEintrag->ArtId())
  {
    std::string s = "artikeleingabe "+itos(selected_AufEintrag->Artikel().Id())+" &";
    system(s.c_str());
    ArtikelBaum::UnCache(selected_AufEintrag->Artikel());
  }
}


void auftrag_main::menu_instanz()
{
 try{
 Gtk::Menu *instanz_menu = manage(new class Gtk::Menu());
 Gtk::MenuItem *instanz = manage(new class Gtk::MenuItem("Instanz"));
 instanz->set_submenu(*instanz_menu);

 std::vector<cH_ppsInstanz> vec_instanz = cH_ppsInstanz::get_all_instanz();
 Gtk::RadioMenuItem::Group _RadioMIGroup_;
 for (std::vector<cH_ppsInstanz>::const_iterator i=vec_instanz.begin();i!=vec_instanz.end();++i)
  {
    Gtk::RadioMenuItem *rm = manage(new class Gtk::RadioMenuItem(_RadioMIGroup_,(*i)->Name()));
    instanz_menu->append(*rm);
    rm->show();    
    rm->signal_activate().connect(SigC::bind(SigC::slot(*this,&auftrag_main::radio_instanz_selected),rm,*i));
  }
 main_menubar->append(*instanz);
 instanz->show();
 }catch(SQLerror &e) {meldung->Show(e);}
}

#if 0
// Diese struct kann weg, wenn uns kein anderes Vergleichskriterium einfällt
struct show_maching_compare
{  
   AufEintrag AE;
   public:
     show_maching_compare(const AufEintrag &a)
       : AE(a) {} 
};
  
bool operator==(cH_RowDataBase rdb, const show_maching_compare &comp)
{  
  try{
      const Data_auftrag &dt=dynamic_cast<const Data_auftrag&>(*rdb);
      if(dt.get_AufEintrag().getFirstKundenAuftrag()==comp.AE)  return true ;
   }catch(...){std::cerr << "Fehler\n";}
  return false;
}
#endif

void auftrag_main::radio_instanz_selected(const Gtk::RadioMenuItem *rm,const cH_ppsInstanz _instanz)
{ 
  // do select action
  if(rm==NULL) return;

  if(rm->get_active()) instanz_selected(_instanz);
}

// cH_RowDataBase == AufEintragBase wird so oft verschieden verwendet, 
// dass ich es lieber explizit mache !!!
namespace { struct auftrag_main_compare : AufEintragBase {
  auftrag_main_compare(AufEintragBase const& a) : AufEintragBase(a) {}
};}

bool operator==(cH_RowDataBase const& a,auftrag_main_compare const& aeb)
{ Handle<const Data_auftrag> ah=a.cast_dynamic<const Data_auftrag>();
//  if (ah->get_AufEintrag()==aeb) std::cout << "found\n";
  return ah->get_AufEintrag()==aeb;
}

void auftrag_main::instanz_selected(const cH_ppsInstanz _instanz)
{ AufEintragBase newselection;
  if (selected_AufEintrag)
  { cH_Data_auftrag dt=maintree_s->getSelectedRowDataBase_as<cH_Data_auftrag>();
      AufEintragZu::list_t kinder=AufEintragZu(dt->get_AufEintrag())
          .get_Referenz_listFull(AufEintragZu::list_kinder,false);
      for (AufEintragZu::list_t::const_iterator i=kinder.begin();
                i!=kinder.end();++i)
      { if (i->AEB.Instanz()==_instanz)
          if (!newselection || !(newselection<i->AEB)) 
            newselection=i->AEB;
      }
    if (!newselection) // in andere Richtung suchen
    { AufEintragZu::list_t eltern=AufEintragZu(dt->get_AufEintrag())
          .get_Referenz_listFull(AufEintragZu::list_eltern,false);
      for (AufEintragZu::list_t::const_iterator i=eltern.begin();
                i!=eltern.end();++i)
      { if (i->AEB.Instanz()==_instanz)
          if (!newselection || !(newselection<i->AEB)) 
            newselection=i->AEB;
      }
    }
//if (!!newselection) std::cout << newselection << '\n';
  }
  bool unbestaetigt=_instanz->KundenInstanz() || _instanz->ExterneBestellung();

  unbestaetigte_auftraege->set_sensitive(!_instanz->LagerInstanz() && unbestaetigt);
  offene_auftraege->set_sensitive(!_instanz->LagerInstanz());
  alle_auftraege->set_sensitive(!_instanz->LagerInstanz());
  stornierte_auftraege->set_sensitive(!_instanz->LagerInstanz());
  geschlossene_auftraege->set_sensitive(!_instanz->LagerInstanz());

  plan_menge_menu->set_sensitive(!_instanz->KundenInstanz());
  ungeplante_menge_menu->set_sensitive(!_instanz->KundenInstanz());
  dispo_menge_menu->set_sensitive(_instanz->LagerInstanz());
  alle_lagermengen->set_sensitive(_instanz->LagerInstanz());

  instanz=_instanz;
  set_title(instanz->get_Name());
  akt_instanz->set_value(_instanz);
  instanz_auftrag=0;

  block_callback=true;
  togglebutton_material->set_active(false);
  togglebutton_auftraege->set_active(false);
  togglebutton_bestellen->set_active(false);

  if(instanz->KundenInstanz())
       plan_menge_menu->set_active(true);
  else if (!newselection || !newselection.Id())
    ungeplante_menge_menu->set_active(true);
  else
    plan_menge_menu->set_active(true);


  frame_instanzen->hide() ;
  // für spätere Erweiterung (falls doch noch Planungsinstanzen verwendet werden)
  info_label_instanzen->hide();
  if (instanz->ExterneBestellung()) 
  { togglebutton_bestellen->set_label(_("Bestellen"));
    frame_instanzen->set_label(_("Bestellung"));
  }
  else 
  { togglebutton_bestellen->set_label(_("Prod.planen"));
    frame_instanzen->set_label(_("Produktionsauftrag")+std::string(" ")+instanz->get_Name());
  }
//  if (instanz->getTyp()=='?') OptMen_Instanz_Bestellen->show();
//  else
     OptMen_Instanz_Bestellen->hide();
  searchcombo_auftragid->Instanz(instanz->Id());
  show_frame_instanzen_material();   
  show_main_menu();
  block_callback=false;

  set_column_titles_of_simple_tree();    

  if (!newselection) neuladen();
  else
  { fill_simple_tree();
    maintree_s->selectFirstMatchingLine(auftrag_main_compare(newselection));
    maintree_s->ScrollToSelection();
  }
//  guint psize=Gtk::WIDGET(vpaned_an_mat->gobj())->allocation.height;
//  vpaned_an_mat->set_position(5*psize);
}

// wird von instanz_selected aufgerufen
void auftrag_main::show_frame_instanzen_material()
{
 button_Kunden_erledigt->hide();  // noch nicht implementiert !
 tree_neuer_auftrag->clear();
 Datum_instanz->setLabel("");
 searchcombo_auftragid->reset();

/*  if(instanz->LagerInstanz())
   { 
     hpaned_tree_lager->show();
     vpaned_an_mat->hide();
     togglebutton_auftraege->hide();
     togglebutton_material->hide();
     togglebutton_bestellen->hide();
   }
  else
*/
   { 
     vpaned_an_mat->show();
//     hpaned_tree_lager->hide();
     togglebutton_auftraege->show();
     togglebutton_material->show();
     if(SelectedAuftragsId()==AuftragBase::ungeplante_id) 
          togglebutton_bestellen->show();
     else {togglebutton_bestellen->hide(); togglebutton_bestellen->set_active(false);}
   }

  if(instanz->Lieferschein())  lieferschein_button->show();
  else                         lieferschein_button->hide();

  if(instanz == ppsInstanzID::Kundenauftraege)
   {
    togglebutton_bestellen->hide(); 
//    mainprint_button->show();
    rechnung_button->show();
    togglebutton_auftraege->hide();
    button_faerben->show();    
   }
  else
   {
//    mainprint_button->hide();
    rechnung_button->hide();
    button_faerben->hide();    
   }

  if(instanz->ExterneBestellung() || instanz == ppsInstanzID::Kundenauftraege)  
       { kunden_lieferant->show();
//         erfassen_button->show();
       }
  else { kunden_lieferant->hide(); 
//         erfassen_button->hide();
       }
/*
  if((instanz->ExterneBestellung() &&  SelectedAuftragsId()==AuftragBase::plan_auftrag_id)
      || instanz == ppsInstanzID::Kundenauftraege)  
       {erfassen_button->show();}
  else {erfassen_button->hide();}
*/

#if 0 // geht so nicht, da set_column_visibility zu clist gehört !!!
// Spalten im Baum
 if (instanz->Id() == ppsInstanzID::Kundenauftraege)
   {
    maintree_s->set_column_visibility(KUNDE,true);
    maintree_s->set_column_visibility(AUFTRAG,true);
    maintree_s->set_column_visibility(LETZEPLANINSTANZ,true);
   }
  else 
   { 
    maintree_s->set_column_visibility(KUNDE,false); 
    maintree_s->set_column_visibility(AUFTRAG,false); 
    maintree_s->set_column_visibility(LETZEPLANINSTANZ,false);
   }
#endif

  // frame_materialbedarf
  guint psize=GTK_WIDGET(vpaned_an_mat->gobj())->allocation.height;
  if(  maintree_s->get_selection()->count_selected_rows()==0 ||
//&&     // keine Zeile gewählt
//        tree_lager_verplant->selection().size()==0) 
     (!togglebutton_material->get_active() &&  // oder
      !togglebutton_auftraege->get_active()))  // kein Button aktiv
   {
    // *5 weil beim Programmstart psize kleiner ist als das Fenster,
    // kleine Ahnung warum.
     vpaned_an_mat->set_position(5*psize);
   }
  else 
   {
     vpaned_an_mat->set_position((int)(2*psize/3.));
     maintree_s->ScrollToSelection();
//     else if(tree_lager_verplant->selection().size()!=0)
//        tree_lager_verplant->moveto(tree_lager_verplant->selection().begin()->get_row_num(),0);
   }
}


void auftrag_main::show_main_menu()
{
  if(instanz == ppsInstanzID::Kundenauftraege 
#ifdef MABELLA_EXTENSIONS  
      || instanz == ppsInstanzID::Einkauf
#endif      
    )
   {
     kundenauftragsnummer->show();
     interne_auftragsnummer->show();
   }
  else
   {
     kundenauftragsnummer->hide();
     interne_auftragsnummer->hide();
   }
}

void auftrag_main::on_togglebutton_bestellen_toggled()
{
  if(togglebutton_bestellen->get_active()) 
   { 
     frame_instanzen->show() ;
     maintree_s->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
   }
  else 
   { 
     frame_instanzen->hide() ;
     maintree_s->get_selection()->set_mode(Gtk::SELECTION_SINGLE);
     guint psize=GTK_WIDGET(vpaned_an_mat->gobj())->allocation.height;
     vpaned_an_mat->set_position(5*psize);
   }
}

void auftrag_main::on_togglebutton_material_toggled()
{
  if(togglebutton_material->get_active()) handle_togglebutton('M');
  else frame_materialbedarf->hide();
}

void auftrag_main::on_togglebutton_auftraege_toggled()
{
  if(togglebutton_auftraege->get_active()) handle_togglebutton('A');
  else frame_materialbedarf->hide();
}

void auftrag_main::handle_togglebutton(char c)
{
  if(block_callback) return;
  block_callback=true;
  frame_materialbedarf->show();
  switch (c)
   {
     case 'A': 
      {
        togglebutton_material->set_active(false);
        togglebutton_bestellen->set_active(false);
        frame_materialbedarf->set_label(_("Auftragszusammensetzung"));
        break;
      }
     case 'M': 
      {
        togglebutton_auftraege->set_active(false);
        togglebutton_bestellen->set_active(false);
        frame_materialbedarf->set_label(_("Materialbedarf"));
        break;
      }
//     case 'B': 
//      {
//        show_frame_instanzen_material(); // nur wg return sonst macht das 'show_selected_line()'
//        block_callback=false;
//        return; // bislang nix besonders tun
//        break;
//      }
     default: assert(!"Falscher charakter\n");
   }
  if(!togglebutton_material->get_active() && !togglebutton_auftraege->get_active())
      maintree_s->get_selection()->unselect_all();
//  if(instanz->LagerInstanz())  show_selected_line(true);
//  else  
  show_selected_line();
  block_callback=false;
}

std::ostream &operator<<(std::ostream &o, const ArtikelMenge &am)
{  return o << cH_ArtikelBezeichnung(am.Artikel())->Bezeichnung() << ":\t"
        << am.Menge() << "\t| " << am.abgeleiteteMenge();
}
        
std::ostream &operator<<(std::ostream &o, const ArtikelMengeSumme &ams)
{  return o << ams.Menge() << "\t| " << ams.abgeleiteteMenge();
}

std::string auftrag_main::FirstRow(gpointer user_data, int deep,
        std::deque<guint> seq)
{
 std::string ret;

 Handle<const Data_auftrag> prd=static_cast<const Data_auftrag*>(user_data);

 for(int i=0;i<deep;i++)
   ret+=prd->Value(seq[i],user_data)->getStrVal()+"&";
 ret+="\\\\\n";

 return ret;
}

void auftrag_main::schema_select(const cH_ExtBezSchema &ebz)
{
 const int signif=1;
 int bezidx=A1;

 ExtBezSchema::const_sigiterator bezend=ebz->sigend(signif);
 for(ExtBezSchema::const_sigiterator bezit=ebz->sigbegin(signif); 
	bezit!=bezend && bezidx<=A4; ++bezit,bezidx++) 
    maintree_s->setTitleAt(bezidx,bezit->bezkomptext);
 for(; bezidx<=A4; bezidx++) // auffüllen bis A4
	maintree_s->setTitleAt(bezidx,"");
}

void auftrag_main::on_offwarengrp_activate()
{
 Global_Settings::create(int(getuid()),"pps","warengruppe maintree",
	itos(offen_warengruppe->get_value()));

 try{
    schema_select(cH_ExtBezSchema(ExtBezSchema::default_id,
    			offen_warengruppe->get_value()));
 }
 catch (SQLerror &e) {
     for(int bezidx=A1; bezidx<=A4; bezidx++) maintree_s->setTitleAt(bezidx,"");
 }
}
