/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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
#include"auftragbase.h"
#include<Auftrag/AufEintragBase.h>
#include<Auftrag/AuftragsBaum.h>
#include<Auftrag/auftrag_status.h>
#include"auftrag_lieferschein.hh"
#include"auftrag_rechnung.hh"
#include <Gtk2TeX.h>
#include <fstream.h>
#include <gtk--/radiomenuitem.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/Einheiten.h>
#include <algorithm>
#include <Aux/Global_Settings.h>
#include "MyMessage.h"
#include <PPixmap.hh>
//#include <Lager/Lager_Vormerkungen.hh>
#include <unistd.h>
#include <MyWindow.hh>
#include <Misc/Trace.h>
#include <Misc/FILEstream.h>

const UniqueValue::value_t auftrag_main::trace_instanzen
      = ManuProC::Tracer::channels.get();

extern MyMessage *meldung;

bool stuecksumme;/* ob stueck summiert werden sollen */
bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
                        /* gespeichert werden soll */
extern bool load_list;

#include <Aux/Ausgabe_neu.h>
#include "aktAufEintrag.h" //wg. Lieferwoche ???
#include <cstdio>
#include "class_auftrag_tree.hh"

gint auftrag_main::on_delete_event(GdkEventAny*)
{   
 on_beenden_activate();
 
 return true;
}


void auftrag_main::on_beenden_activate()
{   
 save_WindowSize();
 maintree_s->detach_from_clist();
 tree_lager_frei->detach_from_clist();
 tree_lager_verplant->detach_from_clist();
 Gtk::Main::instance()->quit();
}

void auftrag_main::save_WindowSize()
{
  gint width,height,x,y;
  Gdk_Window fenster=get_window();
  fenster.get_size(width,height);
  fenster.get_position(x,y);
  Global_Settings::create(int(getuid()),"pps","Size",itos(width)+":"+itos(height));
  Global_Settings::create(int(getuid()),"pps","Position",itos(x)+":"+itos(y));
}


void auftrag_main::on_erfassen_activate()
{   
    hide();
    try
    { if (selected_AufEintrag) manage(new auftrag_bearbeiten(instanz,selected_AufEintrag));
//      else  manage(new auftrag_bearbeiten(instanz,AufEintragBase(instanz->Id())));
      else  manage(new auftrag_bearbeiten(instanz,0));
    } catch (SQLerror &e)
    {  std::cerr << e << '\n';
       show();
    } 
    selected_AufEintrag=0;
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



#define TEXCMD "tex2prn -2 -q -Phl1260"

#define TEXCMDVIEW "tex2prn -2 -G -Phl1260" // Preview

static std::string print_cmd;

static std::string kopfzeile(int col,const std::string &typ,const std::string &title,gpointer user_data)
{  if (!strncmp(title.c_str(),"Verarbeitung",12)) return "p{3.5cm}";
   return typ;
}

static std::string shorten_some(int col,const std::string &title,gpointer user_data)
{  if (!strncmp(title.c_str(),"Artikel",7) ||
	!strncmp(title.c_str(),"Breite",6) ||
	!strncmp(title.c_str(),"Farbe",5) ||
	!strncmp(title.c_str(),"Aufmachung",10) ||
	!strncmp(title.c_str(),"Lieferwoche",11) ||
	!strncmp(title.c_str(),"offen ",6))
      return "\\tiny "+Gtk2TeX::string2TeX(title);
   else
      return Gtk2TeX::string2TeX(title);
}

void auftrag_main::on_main_drucken_activate()
{  
   FILE *f=popen(print_cmd.c_str(),"w");
   oFILEstream os(f);

   Gtk2TeX::HeaderFlags fl;
   fl.ptsize=10;
   fl.leftmargin=0.5;
   fl.leftfoot=Gtk2TeX::string2TeX("offene Aufträge");
   fl.rightfoot="\\today";
   Gtk2TeX::Header(os,fl);
   Gtk2TeX::TableFlags tf;
   tf.columntype_cb=&kopfzeile;
   tf.columntitle_cb=&shorten_some;
   tf.multicolumn=true;
   if (maintree_s->selection().begin()!=maintree_s->selection().end())
   {  // yes, this hackery is not amusing! We need an API change ...
      // Try to detect size of the selected tree
      TCListRow *tclapi=(TCListRow*)(maintree_s->selection().begin()->get_data());
      
      TCListRow *last=tclapi;
      int last_visible=-1;
      while (last->begin()!=last->end())
        {
         last=&*(--last->end());
         if(last->get_lineno()!=-1)
           last_visible=last->get_lineno();
        }
#if PETIG_EXTENSIONS      
      maintree_s->Expand_recursively(*maintree_s);
#endif
      tf.first_line=tclapi->get_lineno();
      tf.last_line=last_visible;

      tf.firstrow_cb=&FirstRow;
      tf.sequence=maintree_s->get_seq();

      try{
      tf.user_data=(gpointer)(&*(maintree_s->getSelectedRowDataBase_as<Handle<const Data_auftrag> >()));
#warning how to get deep from a selected leaf ?
      tf.deep=0; 
      }
      catch(TreeBase::notLeafSelected &ns)
        {
         tf.user_data=(gpointer)dynamic_cast<const Data_auftrag*>(&*(maintree_s->getSelectedNode().LeafData()));
         tf.deep=maintree_s->getSelectedNode().Deep();
        }


   }
   else
   {
#if PETIG_EXTENSIONS      
      maintree_s->Expand_recursively(*maintree_s);
#endif
   }
   Gtk2TeX::CList2Table(os,maintree_s,tf);
   Gtk2TeX::Footer(os);
   os.close();
   pclose(f);
}


void auftrag_main::on_lieferscheine_activate()
{   
 manage (new auftrag_lieferschein(instanz));
}

void auftrag_main::on_rechnung_activate()
{   
 manage (new auftrag_rechnung(instanz));
}

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
void auftrag_main::on_kundendarstellung_activate()
{
  if(block_callback) return;
  kunden_nr_bool=kunden_nr->get_active();
  fill_simple_tree();
  Global_Settings::create(int(getuid()),"pps","KundenNr",bool2str(kunden_nr_bool)); 
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

void auftrag_main::auftrags_id_aenderung()
{
  AuftragBase::ID id=auftrag_main::SelectedAuftragsId();
  if(id==AuftragBase::ungeplante_id) 
    { frame_offene_mengen->set_label("Ungeplante Menge");
    }
  else if(id==AuftragBase::plan_auftrag_id) 
    { frame_offene_mengen->set_label("Bestellte (offene) Mengen");
    }
  else if(id==AuftragBase::dispo_auftrag_id)
    { frame_offene_mengen->set_label("Frei verfügbare Menge");
    }
  frame_instanzen->hide() ;
  show_frame_instanzen_material();

  if(block_callback) return;
  on_neuladen_activate();
// Sinnlos das zu speichern, da der defaultwert von den Instanzen abhängt
//  Global_Settings::create(int(getuid()),"pps","Auftrag_ID_shown",itos(SelectedAuftragsId())); 
}

void auftrag_main::on_plan_menge_menu_activate()
{ if(plan_menge_menu->get_active()) auftrags_id_aenderung();
}
void auftrag_main::on_ungeplante_menge_menu_activate()
{ if(ungeplante_menge_menu->get_active()) auftrags_id_aenderung();
}
void auftrag_main::on_dispo_menge_menu_activate()
{ if(dispo_menge_menu->get_active()) auftrags_id_aenderung();
}

AuftragBase::ID auftrag_main::SelectedAuftragsId() const
{
  if     (ungeplante_menge_menu->get_active()) return AuftragBase::ungeplante_id;
  else if(plan_menge_menu->get_active()) return AuftragBase::plan_auftrag_id;
  else if(dispo_menge_menu->get_active()) return AuftragBase::dispo_auftrag_id;
  else assert(!"never get here");
  abort();
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
  kunden_nr_bool=str2bool(bs,false);
  kunden_nr->set_active(kunden_nr_bool);

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

  block_callback=false;
}



gint auftrag_main::on_mainprint_button_clicked(GdkEventButton *ev)
{
  if(ev->button==1) print_cmd=TEXCMD;
  else if(ev->button==3) print_cmd=TEXCMDVIEW;
  else return false;
  
  on_main_drucken_activate();
  
  return false;
}


auftrag_main::auftrag_main()
  : allaufids(0), instanz(ppsInstanzID::Kundenauftraege), selected_AufEintrag(0),
    block_callback(false),instanz_auftrag(0)
{
 menu_instanz();
 loadEinstellungen();


#ifdef MABELLA_EXTENSIONS    
    button_faerben->hide();
#endif     
    button_auftrag_erledigt->hide(); // nicht implementiert
    button_Kunden_erledigt->hide();  // nicht implementiert

 kunden_lieferant->setLabel("Lieferantennr","Lieferantenname");
 kunden_lieferant->EinschraenkenKdGr(KundengruppeID::Lieferanten);
 set_column_titles_of_simple_tree();
 neuer_auftrag_tree_titel_setzen();

 instanz_selected(instanz); 
 load_list=true;		// nur beim Start ggf. nicht laden
 maintree_s->set_remember("pps","maintree-"+itos(instanz->Id()));
}

void auftrag_main::set_column_titles_of_simple_tree()
{
 std::vector<std::string> ct;
 ct.push_back("Kunde");
 ct.push_back("Artikel");
 ct.push_back("Breite");
 ct.push_back("Farbe");
 ct.push_back("Aufmachung");
 ct.push_back("Lieferwoche");
 ct.push_back("Auftrag");
 ct.push_back("L.P.");
 ct.push_back("Ver.");
 ct.push_back("Letze Lieferung");
 ct.push_back("offene Meter");
 ct.push_back("offene Stück");
 maintree_s->setTitles(ct);
 maintree_s->set_NewNode(&Data_Node::create);
 int cols=maintree_s->columns().size();
 maintree_s->set_column_justification(cols-1, GTK_JUSTIFY_RIGHT);
 maintree_s->set_column_justification(cols-2, GTK_JUSTIFY_RIGHT);

}

void auftrag_main::fill_simple_tree()
{
  if(allaufids) { delete(allaufids); allaufids=0; }
  if(instanz->LagerInstanz())
      lager_zeigen();   
  else
   {
    selected_AufEintrag=0;
    if(!allaufids) 
      { 
        SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),selected_status,SelectedAuftragsId());
        allaufids = new SelectedFullAufList(psel);
      }
    std::vector<cH_RowDataBase> datavec;
    for(SelectedFullAufList::const_iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
     {
//      if(instanz->Id()!=ppsInstanzID::Kundenauftraege && i->Id()!=0) continue;
      datavec.push_back(new Data_auftrag(*i,this));
     }
    maintree_s->setDataVec(datavec);
   }
}


void auftrag_main::on_node_selected(const TreeRow &node)
{
  std::map<st_index,st_mengen> map_allart;
  getAufEintrag_fromNode(node.begin(),node.end(),map_allart);
  instanz_menge(map_allart);
  erfassen_button->set_sensitive(false);
}


void auftrag_main::getAufEintrag_fromNode(TCListRow_API::const_iterator b,
         TCListRow_API::const_iterator e, std::map<st_index,st_mengen>& M)
{  
 for (TCListRow_API::const_iterator i=b;i!=e;++i)
  {  const TreeRow *tlr=reinterpret_cast<const TreeRow *>((*i).get_user_data());
     if(tlr->Leaf())
      {
        const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*(tlr->LeafData()));
        if(togglebutton_material->get_active()) 
          get_ArtikelZusammensetzung(dt->get_AufEintrag(),M);
        else if(togglebutton_auftraege->get_active())
;//          get_ArtikelHerkunft(ArtikelBase(dt->get_Artikel_ID()),
//                                        dt->get_AufEintrag(),M);
      }
    if ((*i).begin()!=(*i).end()) getAufEintrag_fromNode((*i).begin(),(*i).end(),M);
  }
}



void auftrag_main::on_button_faerben_clicked()
{
#ifdef PETIG_EXTENSIONS
  try{
      cH_Data_auftrag dt=maintree_s->getSelectedRowDataBase_as<cH_Data_auftrag>();
      cH_ppsInstanz I(ppsInstanzID::Faerberei);
      dt->get_AufEintrag().setLetztePlanungFuer(I);
      dt->redisplayLetzePlanInstanz(maintree_s);
  } catch (TreeBase::SelectionError &e) {std::cerr << e.what()<<'\n';}
#endif  
}



void auftrag_main::on_leaf_selected(cH_RowDataBase d)
{
 try{
 const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*d);
 selected_AufEintrag = &dt->get_AufEintrag();
//cout << "SE="<< selected_AufEintrag<<'\t'<<selected_AufEintrag->Instanz()->Name()<<' '<<selected_AufEintrag->Id()<<' '<<selected_AufEintrag->ZNr()<<'\n';;
//cout << dt->get_AufEintrag()<<'\t'<<dt->get_AufEintrag().editierbar()<<'\n';
 if(dt->get_AufEintrag().editierbar())
      erfassen_button->set_sensitive(true);
 else erfassen_button->set_sensitive(false);
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
    std::map<st_index,st_mengen> map_allart;
    if(togglebutton_material->get_active()) 
      get_ArtikelZusammensetzung(selAufEintrag,map_allart);
    else if(togglebutton_auftraege->get_active())
      get_ArtikelHerkunft(selAufEintrag,map_allart);
    instanz_menge(map_allart); 
  }
}


void auftrag_main::get_ArtikelZusammensetzung(const AufEintrag& AE_Kunde,
                                   std::map<st_index,st_mengen>& map_allart)
{ ManuProC::Trace _t(trace_instanzen, __FUNCTION__,AufEintragBase(AE_Kunde),AE_Kunde.Artikel());
  std::list<AufEintragZu::st_reflist> artikelreflist=AufEintragZu(AE_Kunde).get_Referenz_listFull(true,false);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=artikelreflist.begin();i!=artikelreflist.end();++i)
   {
     AufEintrag AE(i->AEB);
     AuftragBase::mengen_t planMenge=0,geliefertMenge=0;
     AuftragBase::mengen_t bestellMenge=AE.getStueck();
     ArtikelBaum::faktor_t faktor = ArtikelBaum(AE_Kunde.Artikel()).Faktor(i->Art);
     AuftragBase::mengen_t sollMenge=AE_Kunde.getStueck()*faktor;

     if(i->AEB.Id()!=0) // nur Mengen der geplante Aufträge berücksichtigen
      {
        planMenge=AE.getStueck();
        geliefertMenge=AE.getGeliefert();
      }
     map_allart[st_index(i->AEB.Instanz(),i->Art)] += 
         st_mengen(sollMenge,planMenge,bestellMenge,geliefertMenge);
//     ManuProC::Trace(trace_instanzen, __FILELINE__,i->AEB,i->AB,sollMenge,planMenge,bestellMenge,geliefertMenge);
   }
}

void auftrag_main::get_ArtikelHerkunft(const AufEintrag& AEB,
                                       std::map<st_index,st_mengen>& map_allart)
{ ManuProC::Trace _t(trace_instanzen, __FUNCTION__,AufEintragBase(AEB),AEB.Artikel());
  std::list<AufEintragZu::st_reflist> artikelreflist=AufEintragZu(AEB).get_Referenz_listFull(false,false);
  if (ManuProC::Tracer::enabled(trace_instanzen))
     ManuProC::Trace(trace_instanzen, __FILELINE__,"size",artikelreflist.size());
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=artikelreflist.begin();i!=artikelreflist.end();++i)
   {
     std::list<cH_Kunde> KL;
     if(auftraege_mit_kunden_bool) 
              KL=AufEintragZu(AEB).get_Referenz_Kunden() ;
     else KL.push_back(cH_Kunde(Kunde::none_id)) ;
      AufEintrag AE(i->AEB);
      
//     ManuProC::Trace(trace_instanzen, __FILELINE__,i->AEB2,i->Art,"menge",i->Menge,
//      		AEB.getRestStk(),AEB.getStueck(),AE.getRestStk(),AE.getStueck());

     for(std::list<cH_Kunde>::const_iterator j=KL.begin();j!=KL.end();++j)
        map_allart[st_index(i->AEB.Instanz(),*j,i->Art)] 
                   += st_mengen(i->Menge,0,AE.getRestStk(),0);     
    }
}


void auftrag_main::on_unselect_row(gint row, gint column, GdkEvent *event)
{
   erfassen_button->set_sensitive(true);
//  show_frame_instanzen_material();
}



void auftrag_main::show_selected_line(bool lager)
{
  if(lager)
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
   {
    try{
       cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
       on_leaf_selected(dt);
      } catch(std::exception &e) {}
    try{
       TreeRow dt(maintree_s->getSelectedNode());
       on_node_selected(dt);
      } catch(std::exception &e) {}
   }
 show_frame_instanzen_material();   
}

void auftrag_main::instanz_menge(const std::map<st_index,st_mengen>& map_allart)
{ ManuProC::Trace _t(trace_instanzen, __FUNCTION__);
  if(!togglebutton_material->get_active() &&
     !togglebutton_auftraege->get_active()) 
   {
    return;
   }
  // 1. Schritt: Wieviel Instanzen gibt es?
  //             Und Artikel nach Instanzen sortiert in Listen schreiben
  std::map<cH_ppsInstanz,std::list<artmeng> > LM;
  for (std::map<st_index,st_mengen>::const_iterator i=map_allart.begin();i!=map_allart.end();++i)
    {
      LM[i->first.instanz].push_back(artmeng(i->first,i->second));
      ManuProC::Trace(trace_instanzen, __FILELINE__,i->first.instanz,i->first.artikel,
      		i->second.sollMenge,i->second.planMenge,i->second.bestellMenge);
    }
  // 2. Schritt Überschriften (Instanzen) in die Tabelle schreiben
  //            UND Listen sortieren UND in Tabelle einfügen
  Gtk::Table *table_materialbedarf = manage(new Gtk::Table(0,0,false));
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
//      l->show();
      table_materialbedarf->attach(*l,col,col+1,0,1,0,0,0,0);

     // Spalten sortieren
     if(materialbedarf_sortiert_nach_artikel->get_active())
        i->second.sort(MatListSort(MatListSort::ARTIKEL)); 
     else if(materialbedarf_sortiert_nach_menge->get_active())
        i->second.sort(MatListSort(MatListSort::MENGE));

     int row=1;
     Gtk::Table *tab=manage(new Gtk::Table(0,0,false));
     for(std::list<artmeng>::const_iterator j=i->second.begin();j!=i->second.end();/*siehe unten*/)
      {
        Einheit E(j->first.artikel);
        std::string menge = Formatiere_short(fixedpoint<2>(j->second.bestellMenge));

        std::string einheit = E.MengenEinheit();
        Gtk::Label *la=manage (new Gtk::Label(cH_ArtikelBezeichnung(j->first.artikel)->Bezeichnung()+" "));
        Gtk::Label *lm=manage (new Gtk::Label(menge+einheit+" "));
        Gtk::Label *lk=manage (new Gtk::Label(j->first.Kunde->firma()+": "));
        ++row;
        la->set_justify(GTK_JUSTIFY_LEFT);
        la->set_alignment(0, 0.5);
        la->set_padding(0, 0);
        lm->set_justify(GTK_JUSTIFY_RIGHT);
        lm->set_alignment(1.0, 0.5);
        lm->set_padding(0, 0);
        lk->set_justify(GTK_JUSTIFY_LEFT);
        lk->set_alignment(1.0, 0.5);
        lk->set_padding(0, 0);
        
#ifdef PETIG_EXTENSIONS        
        if(togglebutton_material->get_active())
        {  
          Gtk::Pixmap *pixmapG;
          if(j->second.gelieferteMenge >= j->second.bestellMenge)
              pixmapG = PPixmap().getPixMap('G'); 
          else if(j->second.bestellMenge == j->second.planMenge)
            {
              if (i->first->LagerInstanz())
                    pixmapG = PPixmap().getPixMap('V');
              else  pixmapG = PPixmap().getPixProz(1);
            }
          else if(j->second.planMenge >= j->second.sollMenge &&
                  j->second.planMenge<  j->second.bestellMenge)
             {  pixmapG = PPixmap().getPixMap('?'); 
             }
          else pixmapG = PPixmap().getPixProz(j->second.planMenge.as_float(),j->second.sollMenge.as_float());            
           tab->attach(*pixmapG,0,1,row,row+1,GTK_FILL,0,0,0);
        }
#endif
        
        int col2=1;
        if(togglebutton_auftraege->get_active() && auftraege_mit_kunden_bool)
         {
          tab->attach(*lk,col2,col2+1,row,row+1,GTK_FILL,0,0,0);
          ++col2;
         }
        if(materialbedarf_sortiert_nach_artikel->get_active())
          { tab->attach(*la,col2  ,col2+1,row,row+1,GTK_FILL,0,0,0);
            tab->attach(*lm,col2+1,col2+2,row,row+1,GTK_FILL,0,0,0); }
        else 
          { tab->attach(*lm,col2  ,col2+1,row,row+1,GTK_FILL,0,0,0);
            tab->attach(*la,col2+1,col2+2,row,row+1,GTK_FILL,0,0,0); }
        ++j;
        if(j==i->second.end())
         {
//           tab->show();
           table_materialbedarf->attach(*tab,col,col+1,1,2,GTK_FILL,GTK_EXPAND|GTK_FILL,0,0);
         }
      }
   }
 viewport_materialbedarf->remove();
 table_materialbedarf->set_col_spacings(5);
 viewport_materialbedarf->add(*table_materialbedarf);
 table_materialbedarf->show_all();
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
  }catch(TreeBase::notLeafSelected &x) {std::cerr << x.what()<<'\n';}
  }catch(TreeBase::noRowSelected &x) {std::cerr << x.what()<<'\n';}
 selected_AufEintrag=0;
}


void auftrag_main::on_button_artikeleingabe_clicked()
{
 if (selected_AufEintrag )//sonst geht das nur bei Kundenaufträgen->  && selected_AufEintrag->ArtId())
  {
    std::string s = "artikeleingabe "+itos(selected_AufEintrag->ArtId())+" &";
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
    rm->activate.connect(SigC::bind(SigC::slot(this,&auftrag_main::radio_instanz_selected),rm,*i));
  }
 main_menubar->append(*instanz);
 instanz->show();
 }catch(SQLerror &e) {meldung->Show(e);}
}

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

void auftrag_main::radio_instanz_selected(const Gtk::RadioMenuItem *rm,const cH_ppsInstanz _instanz)
{
  if(rm->get_active()) instanz_selected(_instanz);
}

void auftrag_main::instanz_selected(const cH_ppsInstanz _instanz)
{
  instanz=_instanz;
  set_title(instanz->get_Name());
  instanz_auftrag=0;

  block_callback=true;
  togglebutton_material->set_active(false);
  togglebutton_auftraege->set_active(false);
  togglebutton_bestellen->set_active(false);

  AufEintrag AEK;
  if(selected_AufEintrag) AEK=selected_AufEintrag->getFirstKundenAuftrag();

  if(AEK.Instanz()!=ppsInstanzID::None)
   {
     maintree_s->Expand_recursively();
     maintree_s->selectFirstMatchingLine(show_maching_compare(AEK));
     if(maintree_s->selection().begin()!=maintree_s->selection().end())
{
//cout << "Row: "<<maintree_s->selection().begin()->get_row_num()<<'\n';
#warning Das move to funktioniert leider nicht MAT
        maintree_s->moveto(maintree_s->selection().begin()->get_row_num(),0,0.5,0);
}
   }

  if(instanz == ppsInstanzID::Kundenauftraege)
       plan_menge_menu->set_active(true);
  else ungeplante_menge_menu->set_active();


  frame_instanzen->hide() ;
  show_frame_instanzen_material();   
  show_main_menu();
  block_callback=false;

  neuladen();

//  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;
//  vpaned_an_mat->set_position(5*psize);
}

void auftrag_main::show_frame_instanzen_material()
{
 button_Kunden_erledigt->hide();  // noch nicht implementiert !
 tree_neuer_auftrag->clear();
 Datum_instanz->setLabel("");
 searchcombo_auftragid->reset();

  if(instanz->LagerInstanz())
   { 
     hpaned_tree_lager->show();
     vpaned_an_mat->hide();
     togglebutton_auftraege->hide();
     togglebutton_material->hide();
     togglebutton_bestellen->hide();
   }
  else
   { 
     vpaned_an_mat->show();
     hpaned_tree_lager->hide();
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
    mainprint_button->show();
    rechnung_button->show();
    togglebutton_auftraege->hide();
    button_faerben->show();    
   }
  else
   {
    mainprint_button->hide();
    rechnung_button->hide();
    button_faerben->hide();    
   }

  if(instanz->ExterneBestellung() || instanz == ppsInstanzID::Kundenauftraege)  
       { kunden_lieferant->show(); 
         erfassen_button->show();
       }
  else { kunden_lieferant->hide(); 
         erfassen_button->hide();
       }
/*
  if((instanz->ExterneBestellung() &&  SelectedAuftragsId()==AuftragBase::plan_auftrag_id)
      || instanz == ppsInstanzID::Kundenauftraege)  
       {erfassen_button->show();}
  else {erfassen_button->hide();}
*/

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

  // frame_materialbedarf
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;
  if( ( maintree_s->selection().size()==0 &&     // keine Zeile gewählt
        tree_lager_verplant->selection().size()==0) ||
     (!togglebutton_material->get_active() &&  // oder
      !togglebutton_auftraege->get_active()))  // kein Button aktiv
   {
    // *5 weil beim Programmstart psize kleiner ist als das Fenster,
    // kleine Ahnung warum.
     vpaned_an_mat->set_position(5*psize);
   }
  else 
   {
     vpaned_an_mat->set_position(2*psize/3.);
     if(maintree_s->selection().size()!=0)
        maintree_s->moveto(maintree_s->selection().begin()->get_row_num(),0);
     else if(tree_lager_verplant->selection().size()!=0)
        tree_lager_verplant->moveto(tree_lager_verplant->selection().begin()->get_row_num(),0);
   }
}


void auftrag_main::show_main_menu()
{
  if(instanz == ppsInstanzID::Kundenauftraege)
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
     OptMen_Instanz_Bestellen->set_nur_geplant_von(instanz);
     if(OptMen_Instanz_Bestellen->get_menu()->items().empty())
          {OptMen_Instanz_Bestellen->hide();
          kunden_lieferant->show();
          }
     else {OptMen_Instanz_Bestellen->show();
     	   kunden_lieferant->hide();
     	  }
     frame_instanzen->show() ;
     maintree_s->set_selection_mode(GTK_SELECTION_MULTIPLE);
   }
  else 
   { 
     frame_instanzen->hide() ;
     maintree_s->set_selection_mode(GTK_SELECTION_SINGLE);
     guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;
     vpaned_an_mat->set_position(5*psize);
   }
}

void auftrag_main::on_togglebutton_material_toggled()
{
  if(togglebutton_material->get_active()) handle_togglebutton('M');
}

void auftrag_main::on_togglebutton_auftraege_toggled()
{
  if(togglebutton_auftraege->get_active()) handle_togglebutton('A');
}

void auftrag_main::handle_togglebutton(char c)
{
  if(block_callback) return;
  block_callback=true;
  switch (c)
   {
     case 'A': 
      {
        togglebutton_material->set_active(false);
        togglebutton_bestellen->set_active(false);
        frame_materialbedarf->set_label("Auftragszusammensetzung");
        break;
      }
     case 'M': 
      {
        togglebutton_auftraege->set_active(false);
        togglebutton_bestellen->set_active(false);
        frame_materialbedarf->set_label("Materialbedarf");
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
      maintree_s->unselect_all();
  if(instanz->LagerInstanz())  show_selected_line(true);
  else  show_selected_line();
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

 Data_auftrag *prd=(Data_auftrag*)user_data;

 for(int i=0;i<deep;i++)
   ret+=prd->Value(seq[i],user_data)->getStrVal()+"&";
 ret+="\\\\\n";

 return ret;
}
