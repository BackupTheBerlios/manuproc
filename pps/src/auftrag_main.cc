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

// generated 2000/11/22 20:43:27 MET by jacek@mimi.
// using glademm V0.5.10
//
// newer (non customized) versions of this file go to auftrag_main.cc_glade

// This file is for your program, I won't touch it again!

#include "auftrag_main.hh"
#include "auftrag_bearbeiten.hh"
#include"auftragbase.h"
#include<Auftrag/AufEintragBase2.h>
#include<Auftrag/AuftragsBaum.h>
#include<Auftrag/auftrag_status.h>
#include"auftrag_lieferschein.hh"
#include"auftrag_rechnung.hh"
#include <Gtk2TeX.h>
#include <fstream.h>
#include <gtk--/radiomenuitem.h>
#include <Artikel/ArtikelBaumFull.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/Einheiten.h>
#include <algorithm>
#include <Aux/Global_Settings.h>
#include "MyMessage.h"
#include "AufEintrag.h"
#include <PPixmap.hh>

extern MyMessage *meldung;
extern auftrag_main *auftragmain;

bool stuecksumme;/* ob stueck summiert werden sollen */
bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
                        /* gespeichert werden soll */

#include <Aux/Ausgabe_neu.h>
#include "aktAufEintrag.h" //wg. Lieferwoche ???
#include <cstdio>
#include "class_auftrag_tree.hh"

void auftrag_main::on_beenden_activate()
{   
 maintree_s->detach_from_clist();
 Gtk::Main::instance()->quit();
}

void auftrag_main::on_erfassen_activate()
{   
 hide();
 try
 { if (selected_AufEintrag) manage(new auftrag_bearbeiten(*selected_AufEintrag));
   else  manage(new auftrag_bearbeiten(AufEintragBase2(instanz->Id())));
 } catch (SQLerror &e)
 {  std::cerr << e << '\n';
    show();
 } 
 selected_AufEintrag=0;
}

void auftrag_main::on_neuladen_activate()
{
 if(allaufids) { delete(allaufids); allaufids=NULL; }
 fill_simple_tree();
}

#define TEXCMD "tex2prn -2 -q -Phl1260"
//#define TEXCMD "tex2prn -2 -G -Phl1260" // Preview

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
   maintree_s->Expand_recursively(*maintree_s);
   FILE *f=popen(TEXCMD,"w");
   std::ofstream os(fileno(f));
   Gtk2TeX::HeaderFlags fl;
   fl.ptsize=10;
   fl.leftmargin=0.5;
   fl.leftfoot=Gtk2TeX::string2TeX("offene Aufträge");
   fl.rightfoot="\\today";
   Gtk2TeX::Header(os,fl);
   Gtk2TeX::TableFlags tf;
   tf.columntype_cb=&kopfzeile;
   tf.columntitle_cb=&shorten_some;
   Gtk2TeX::CList2Table(os,maintree_s,tf);
   Gtk2TeX::Footer(os);
   os.close();
   pclose(f);
}

void auftrag_main::on_abschreiben_activate()
{   
 manage (new auftrag_lieferschein(instanz));
}

void auftrag_main::on_rechnung_activate()
{   
 manage (new auftrag_rechnung(instanz));
}

void auftrag_main::on_main_bezeichnung_activate()
{
 interne_namen_bool = main_intbez->get_active();
 fill_simple_tree();
}
void auftrag_main::on_kunden_anr_activate()
{
  kunden_anr_bool=kundenauftragsnummer->get_active();
  fill_simple_tree();
}
void auftrag_main::on_zeitdarstellung_activate()
{
  zeit_kw_bool=kalenderwoche->get_active();
  fill_simple_tree();
}
void auftrag_main::on_kundendarstellung_activate()
{
  kunden_nr_bool=kunden_nr->get_active();
  fill_simple_tree();
}
void auftrag_main::on_offene_auftraege_activate()
{
#warning diese Funktion wird durch jeden Mausklick
#warning 2x aufgerufen!!!! MAT
  if (offene_auftraege->get_active())             selected_status=OPEN;
  else if (unbestaetigte_auftraege->get_active()) selected_status=UNCOMMITED;
  else if (stornierte_auftraege->get_active())    selected_status=STORNO;
  else if (geschlossene_auftraege->get_active())  selected_status=CLOSED;
  else assert(!"Falscher Status");
  on_neuladen_activate();
}

void auftrag_main::on_auftraege_kunde_activate()
{
  auftraege_mit_kunden_bool=auftraege_mit_kunde->get_active();
}



void auftrag_main::on_mainprint_button_clicked()
{   on_main_drucken_activate();
}


auftrag_main::auftrag_main()
  : instanz(ppsInstanz::INST_KNDAUF), selected_AufEintrag(0),
    instanz_auftrag(0)
{
 guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
 vpaned_an_mat->set_position(5*psize); // Großer Wert
 frame_instanzen->hide();
 menu_instanz();
 auftragmain=this;
 allaufids=0;
 interne_namen_bool=true;
 zeit_kw_bool=true;
 kunden_nr_bool=false;
 kunden_anr_bool=true;
 auftraege_mit_kunden_bool=false;
 selected_status=OPEN;

 info_label_instanzen->hide();
 button_Kunden_erledigt->hide();
 set_column_titles_of_simple_tree();
 fill_simple_tree();
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
 ct.push_back("Verarbeitung");
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
 if(!allaufids) 
   { 
     SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),selected_status);
     allaufids = new SelectedFullAufList(psel);
   }
 std::vector<cH_RowDataBase> datavec;
 for(std::vector<AufEintragBase>::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
  {
   datavec.push_back(new Data_auftrag(*i,this));
  }
 maintree_s->setDataVec(datavec);
}

void auftrag_main::on_node_selected(const TreeRow &node)
{
  std::map<st_index,st_mengen> map_allart;
  getAufEintragBase_fromNode(node.begin(),node.end(),map_allart);
  instanz_menge(map_allart);
}

void auftrag_main::getAufEintragBase_fromNode(TCListRow_API::const_iterator b,
         TCListRow_API::const_iterator e, std::map<st_index,st_mengen>& M)
{  
 for (TCListRow_API::const_iterator i=b;i!=e;++i)
  {  const TreeRow *tlr=reinterpret_cast<const TreeRow *>((*i).get_user_data());
     if(tlr->Leaf())
      {
        const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*(tlr->LeafData()));
        if(togglebutton_material->get_active()) 
          get_ArtikelZusammensetzung(ArtikelBase(dt->get_Artikel_ID()),
                                        dt->get_AufEintragBase(),M);
        else if(togglebutton_auftraege->get_active())
          get_ArtikelHerkunft(ArtikelBase(dt->get_Artikel_ID()),
                                        dt->get_AufEintragBase(),M);
      }
    if ((*i).begin()!=(*i).end()) getAufEintragBase_fromNode((*i).begin(),(*i).end(),M);
  }
}


void auftrag_main::on_leaf_selected(cH_RowDataBase d)
{
 try{
 const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*d);
 selected_AufEintrag = &dt->get_AufEintragBase();

 if(instanz->Id() != 1 && !togglebutton_material->get_active()
                       && !togglebutton_auftraege->get_active()) 
      instanz_leaf_auftrag(*selected_AufEintrag);
 else
  {
 // Zum Anzeigen des benötigten Materials
    std::map<st_index,st_mengen> map_allart;
    if(togglebutton_material->get_active()) 
      get_ArtikelZusammensetzung(ArtikelBase(dt->get_Artikel_ID()),
                                        dt->get_AufEintragBase(),
                                        map_allart);
    else if(togglebutton_auftraege->get_active())
       get_ArtikelHerkunft(ArtikelBase(dt->get_Artikel_ID()),
                                        dt->get_AufEintragBase(),
                                        map_allart);
    instanz_menge(map_allart); 
  }
 }catch(std::exception &e) {std::cerr<<e.what();}
}


void auftrag_main::get_ArtikelZusammensetzung(const ArtikelBase& art,
                                              const AufEintragBase& AEB,
                                              std::map<st_index,st_mengen>& map_allart)
{
  std::list<AufEintragZu::st_reflist> artikelreflist=AufEintragZu(AEB).get_Referenz_listFull(true,false);

  std::map<ArtikelBase,Long> planmap;
  if(geplanter_materialbedarf->get_active() && togglebutton_material->get_active())
    { 
      AufEintragBase2 aeb=AEB;
      std::list<AufEintragZu::st_reflist> KL=AufEintragZu(AEB).get_Referenz_listFull(false);
#warning HACK ersten Kundenauftrag nehmen
      if(!KL.empty()) aeb=KL.begin()->AEB2 ;
      geplanteMenge(aeb,planmap) ;
    }
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=artikelreflist.begin();i!=artikelreflist.end();++i)
   {
     ArtikelBaum::menge_t faktor = ArtikelBaum(art).Faktor(i->AB);
     map_allart[st_index(i->AEB2.Instanz(),cH_Kunde(1),i->AB)] 
                          += st_mengen(AEB.getRestStk()*faktor,
                                       planmap[i->AB],
                                       AEB.getStueck()*faktor);     
   }
}

void auftrag_main::get_ArtikelHerkunft(const ArtikelBase& art,
                                       const AufEintragBase& AEB,
                                       std::map<st_index,st_mengen>& map_allart)
{
  if(togglebutton_auftraege->get_active())
    { 
      std::list<AufEintragZu::st_reflist> artikelreflist=AufEintragZu(AEB).get_Referenz_listFull(false,false);
//      AuftragsBaum AB(AEB,false); //Elternaufträge
//      for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
       for(std::list<AufEintragZu::st_reflist>::const_iterator i=artikelreflist.begin();i!=artikelreflist.end();++i)
       {
         std::list<cH_Kunde> KL;
         if(auftraege_mit_kunden_bool) 
            KL=AufEintragZu(AEB).get_Referenz_Kunden() ;
         else KL.push_back(cH_Kunde(1)) ;
/*
cout << i->AEB2.Instanz()<<' '<<cH_ArtikelBezeichnung(i->AB)->Bezeichnung()
<<' '<<i->menge<<'\t';
cout << AEB.getRestStk()<<'\t'<<AEB.getStueck()<<'\n';
cout << AufEintragBase(i->AEB2).getRestStk()<<'\t'<<AufEintragBase(i->AEB2).getStueck()<<'\n';
*/
         for(std::list<cH_Kunde>::const_iterator j=KL.begin();j!=KL.end();++j)
           map_allart[st_index(i->AEB2.Instanz(),*j,i->AB)] += st_mengen(AufEintragBase(i->AEB2).getRestStk(),
                                       i->Menge,
                                       AufEintragBase(i->AEB2).getStueck());     
       }
    }
}


void auftrag_main::geplanteMenge(const AufEintragBase& AEB,std::map<ArtikelBase,Long>& planmap)
{
  AuftragsBaum L(AEB,true);
  for(AuftragsBaum::const_iterator i=L.begin();i!=L.end();++i)
   if(i->AEB2.Id()!=0)
     planmap[i->AB]+=i->menge;
     // es kann nur Artikel in der Liste geben, die in der std::map schon drin sind.
}



void auftrag_main::on_togglebutton_material_toggled()
{
  if(togglebutton_material->get_active()) 
   { togglebutton_auftraege->set_active(false);
     frame_materialbedarf->set_label("Materialbedarf");
   }
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
  if(!togglebutton_material->get_active() || !selected_AufEintrag) vpaned_an_mat->set_position(psize);
  show_selected_line();
}
void auftrag_main::on_togglebutton_auftraege_toggled()
{
  if(togglebutton_auftraege->get_active()) 
   { togglebutton_material->set_active(false);
     frame_materialbedarf->set_label("Auftragszusammensetzung");
   }
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
  if(!togglebutton_auftraege->get_active() || !selected_AufEintrag) vpaned_an_mat->set_position(psize);
  show_selected_line();
}

void auftrag_main::show_selected_line()
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

void auftrag_main::instanz_menge(const std::map<st_index,st_mengen>& map_allart)
{
  if(!togglebutton_material->get_active() &&
     !togglebutton_auftraege->get_active()) return;

  // 1. Schritt: Wieviel Instanzen gibt es?
  //             Und Artikel nach Instanzen sortiert in Listen schreiben
  std::map<cH_ppsInstanz,std::list<artmeng> > LM;
  for (std::map<st_index,st_mengen>::const_iterator i=map_allart.begin();i!=map_allart.end();++i)
    {
      LM[i->first.instanz].push_back(artmeng(i->first,i->second));
    }
  // 2. Schritt Überschriften (Instanzen) in die Tabelle schreiben
  //            UND Listen sortieren UND in Tabelle einfügen
  Gtk::Table *table_materialbedarf = manage(new Gtk::Table(0,0,false));
  for(std::map<cH_ppsInstanz,std::list<artmeng> >::iterator i=LM.begin();i!=LM.end();++i)
    {
      // Überschriften
      Gtk::Label *l=manage (new Gtk::Label(" "+i->first->get_Name()+" "));
      int col = i->first->Id();
      std::string pat;
      for (guint u=0;u<i->first->get_Name().size()+2;++u) pat+="_";
      l->set_pattern(pat);
      l->set_alignment(0, 0.5);
      l->set_padding(0, 0);
      l->show();
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
//std::cout << i->first->get_Name()<<'\t'<<j->second.sollMenge<<'\t'<<j->second.planMenge
//      <<'\t'<<j->second.bestellMenge<<"\tFaktor = "
//      <<double(j->second.planMenge)/j->second.sollMenge<<'\n';
        std::string menge = Formatiere_short(fixedpoint<2>(j->second.sollMenge));

        std::string einheit = E.MengenEinheit();
        Gtk::Label *la=manage (new Gtk::Label(cH_ArtikelBezeichnung(j->first.artikel)->Bezeichnung()+" "));
        Gtk::Label *lm=manage (new Gtk::Label(menge+einheit+" "));
        Gtk::Label *lk=manage (new Gtk::Label(j->first.Kunde->firma()+": "));
        ++row;
        la->set_justify(GTK_JUSTIFY_LEFT);
        la->set_alignment(0, 0.5);
        la->set_padding(0, 0);
        la->show();
        lm->set_justify(GTK_JUSTIFY_RIGHT);
        lm->set_alignment(1.0, 0.5);
        lm->set_padding(0, 0);
        lm->show();
        lk->set_justify(GTK_JUSTIFY_LEFT);
        lk->set_alignment(1.0, 0.5);
        lk->set_padding(0, 0);
        lk->show();
        if(geplanter_materialbedarf->get_active())
        {  
           Gtk::Pixmap *pixmapG;
           if(j->second.bestellMenge<j->second.planMenge)
             pixmapG = PPixmap().getPixProz();
           else
             pixmapG = PPixmap().getPixProz(j->second.planMenge,j->second.sollMenge);
           pixmapG->show();
           tab->attach(*pixmapG,0,1,row,row+1,GTK_FILL,0,0,0);
        }
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
           tab->show();
           table_materialbedarf->attach(*tab,col,col+1,1,2,GTK_FILL,GTK_EXPAND|GTK_FILL,0,0);
         }
      }
   }
 viewport_materialbedarf->remove();
 table_materialbedarf->set_col_spacings(5);
 viewport_materialbedarf->add(*table_materialbedarf);
 table_materialbedarf->show();
 guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
 vpaned_an_mat->set_position(psize/3.);
}


void auftrag_main::on_unselect_row(gint row, gint column, GdkEvent *event)
{
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
  vpaned_an_mat->set_position(psize);
}

#if 0
void auftrag_main::on_prozlistscombo_search(int *cont, GtkSCContext newsearch)
{if (newsearch!=GTK_SEARCH_OPEN || !selected_AufEintrag) return;
     {  
        ArtikelBaum artb(selected_AufEintrag->ArtId());
        cH_Prozess p(Prozess::default_id);
        ArtikelBase::ID tmpid;   
        prozlist.erase(prozlist.begin(),prozlist.end());

        while((p=artb.getErzeugendenProzess())->Id()!=Prozess::default_id)
         {
          prozlist_scombo->add_item(
          		itos(p->getProzessID())+":"+p->getTyp()+" "+p->getText(),
					p->getProzessID());
          prozlist.push_back(p);
          tmpid=artb.ParentArtikelID();
          if(tmpid)
            artb=ArtikelBaum(tmpid);
          else break;
        }
     }
 *cont=false;
}

void auftrag_main::on_prozlistscombo_activate()
{
 if (selected_AufEintrag)
     {
      int pid=atoi(prozlist_scombo->get_text().c_str());
      try{selected_AufEintrag->setVerarbeitung(cH_Prozess(pid));
         cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
         maintree_s->redisplay(dt,VERARBEITUNG);         
      }catch(SQLerror &e)
        {meldung->Show(e); return;}
     }
}
#endif



void auftrag_main::on_button_auftrag_erledigt_clicked()
{
  try{
  try{
  try{
     cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
     dynamic_cast<const Data_auftrag*>(&*dt)->get_AufEintragBase().setStatus(CLOSED);
     dynamic_cast<Data_auftrag&>(const_cast<RowDataBase&>(*dt)).redisplayMenge(maintree_s);
  }catch(SQLerror &e) {meldung->Show(e);}
  }catch(TreeBase::notLeafSelected &x) {std::cerr << x.what()<<'\n';}
  }catch(TreeBase::noRowSelected &x) {std::cerr << x.what()<<'\n';}
 selected_AufEintrag=0;
}


void auftrag_main::on_button_artikeleingabe_clicked()
{
//std::cout << selected_Artikel.Id()<<'\n';
 if (selected_AufEintrag && selected_AufEintrag->ArtId())
  {
    std::string s = "artikeleingabe "+itos(selected_AufEintrag->ArtId())+" &";
    system(s.c_str());
    ArtikelBaum::UnCache(selected_AufEintrag->ArtId());
  }
}


void auftrag_main::menu_instanz()
{
 Gtk::Menu *instanz_menu = manage(new class Gtk::Menu());
 Gtk::MenuItem *instanz = manage(new class Gtk::MenuItem("Instanz"));
 instanz->set_submenu(*instanz_menu);

 std::map<int,std::string> map_instanz = get_all_instanz();
 Gtk::RadioMenuItem::Group _RadioMIGroup_;
 for (std::map<int,std::string>::const_iterator i=map_instanz.begin();i!=map_instanz.end();++i)
  {
    Gtk::RadioMenuItem *rm = manage(new class Gtk::RadioMenuItem(_RadioMIGroup_,i->second));
    instanz_menu->append(*rm);
    rm->show();    
    rm->activate.connect(SigC::bind(SigC::slot(this,&auftrag_main::instanz_selected),i->first));
  }
 main_menubar->append(*instanz);
 instanz->show();
}

void auftrag_main::instanz_selected(int _instanz_)
{
#warning drei SINNLOSE Zeilen wegen eines Bugs???? MAT
  static int x=1;
  if(x==1) {++x;return;}
  else x=1;
  
  instanz=(ppsInstanz::ID) _instanz_;
  set_title(instanz->get_Name());
  instanz_auftrag=0;

  if(_instanz_==1)
   {
    guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
    vpaned_an_mat->set_position(5*psize); // Großer Wert
    maintree_s->set_column_visibility(KUNDE,true);
   }

//  if(_instanz_!=1) neue_auftraege_beruecksichtigen(instanz);
  on_neuladen_activate();

  if (_instanz_ != 1)
   {
    maintree_s->set_column_visibility(KUNDE,false); 
    erfassen_button->hide();
    mainprint_button->hide();
    rechnung_button->hide();
    lieferschein_button->hide();
//    table_prod_prozess->hide();
    button_auftrag_erledigt->hide();
    frame_instanzen->show();
    neuer_auftrag_tree_titel_setzen(); // Kann auch in den Konstruktor MAT
    tree_neuer_auftrag->clear();
    Datum_instanz->setLabel("");
    spinbutton_geplante_menge->hide();
    searchcombo_auftragid->reset();
    togglebutton_geplante_menge->set_active(false);
    button_Kunden_erledigt->show();
   }
  else 
   { 
    erfassen_button->show();
    mainprint_button->show();
    rechnung_button->show();
    lieferschein_button->show();
//    table_prod_prozess->show();
    button_auftrag_erledigt->show();
    button_Kunden_erledigt->hide();
    frame_instanzen->hide();
   }
}

