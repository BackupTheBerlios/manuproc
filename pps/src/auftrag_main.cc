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
#include<Auftrag/selFullAufEntry.h>
#include<Auftrag/AufEintragBase2.h>
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

#include "auftragliste.hh"
#include "AufEintrag.h"


extern MyMessage *meldung;

SelectedFullAufList *allaufids;
extern auftrag_main *auftragmain;



bool stuecksumme;/* ob stueck summiert werden sollen */
bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
                        /* gespeichert werden soll */


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
 {  cerr << e << '\n';
    show();
 } 
//XXX selected_Auftrag=AufEintragBase2(instanz);
 selected_AufEintrag=0;
}

void auftrag_main::on_neuladen_activate()
{
#warning Diese Variable ist unnötig ?? (CP)   
// SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Status(OPEN));

 if(allaufids) { delete(allaufids); allaufids=NULL; }
 fill_simple_tree();
}

#define TEXCMD "tex2prn -2 -q -Phl1260"
//#define TEXCMD "tex2prn -2 -G -Phl1260" // Preview

// not the best of all possible solutions ...
static string kopfzeile(int col,const string &typ,const string &title,gpointer user_data)
{  if (!strncmp(title.c_str(),"Verarbeitung",12)) return "p{3.5cm}";
   return typ;
}

static string shorten_some(int col,const string &title,gpointer user_data)
{  if (!strncmp(title.c_str(),"Artikel",7) ||
	!strncmp(title.c_str(),"Breite",6) ||
	!strncmp(title.c_str(),"Farbe",5) ||
	!strncmp(title.c_str(),"Aufmachung",10) ||
	!strncmp(title.c_str(),"offen ",6))
      return "\\tiny "+Gtk2TeX::string2TeX(title);
   else
      return Gtk2TeX::string2TeX(title);
}

void auftrag_main::on_main_drucken_activate()
{  
   maintree_s->Expand_recursively(*maintree_s);
   FILE *f=popen(TEXCMD,"w");
   ofstream os(fileno(f));
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
 interne_namen = main_intbez->get_active();

 cH_ExtBezSchema cs(1,ExtBezSchema::default_Typ);
 for(vector<AufEintragBase>::iterator j = allaufids->aufidliste.begin();
        j!=allaufids->aufidliste.end(); ++j)
     {
//      AufEintrag &af = aufentrymap[(*j).mapKey()];
//      if(interne_namen) cs = cH_ExtBezSchema(af.getKdNr(),ExtBezSchema::default_Typ);
//      af.setArtikelBezeichnung(cs);

      if (selected_AufEintrag)
       {
         if(interne_namen) cs = cH_ExtBezSchema((*selected_AufEintrag).getKdNr(),ExtBezSchema::default_Typ);
         (*selected_AufEintrag).setArtikelBezeichnung(cs);
       }
     else         
       if(interne_namen) cs = cH_ExtBezSchema(1,ExtBezSchema::default_Typ);
     }
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

void auftrag_main::on_mainprint_button_clicked()
{   on_main_drucken_activate();
}


auftrag_main::auftrag_main()
  : instanz(ppsInstanz::INST_KNDAUF), selected_AufEintrag(0)
{
// frame_materialbedarf->hide();
 guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
 vpaned_an_mat->set_position(5*psize); // Großer Wert
 frame_instanzen->hide();
 menu_instanz();
// instanz_menu();
 auftragmain=this;
 allaufids=0;
 interne_namen=true;
 zeit_kw_bool=true;
 kunden_nr_bool=false;
 kunden_anr_bool=true;
 showdeep=0;

 set_column_titles_of_simple_tree();
 fill_simple_tree();
}

void auftrag_main::set_column_titles_of_simple_tree()
{
 vector<string> ct;
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
//cout << "I="<<instanz->get_Name()<<"\t";
 if(!allaufids) 
   { SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),(AufStatVal)OPEN);
     if(interne_namen)
       allaufids = new SelectedFullAufList(psel,
			cH_ExtBezSchema(1,ExtBezSchema::default_Typ));
     else allaufids = new SelectedFullAufList(psel);
   }
//cout << allaufids->size()<<"\n";

 vector<cH_RowDataBase> datavec;
//cout << interne_namen<<"\t"<<zeit_kw_bool<<"\t"<<kunden_nr_bool<<"\n";
 for(vector<AufEintragBase>::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
  {
   datavec.push_back(new Data_auftrag(*i,this));
  }
 maintree_s->setDataVec(datavec);
}

void auftrag_main::on_node_selected(const TCListNode &node)
{
  const Data_Node &dn=dynamic_cast<const Data_Node&>(node);
  instanz_menge(dn.get_vec_ArtikelBase_Stueck());
}

void auftrag_main::on_leaf_selected(cH_RowDataBase d)
{
 const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*d);
 selected_AufEintrag = &dt->get_AufEintragBase();

 selected_Artikel=dt->get_Artikel_ID();
 prozlist_scombo->reset();
 prozlist_scombo->set_text(dt->ProzessText());

 if(instanz->Id() != 1) instanz_leaf_auftrag(*selected_AufEintrag);

 // Zum Anzeigen der benötigten Menge
     vector<pair<ArtikelBase,long long int> > vec_artbase;
     vec_artbase.push_back(pair<ArtikelBase,long long int>(ArtikelBase(selected_Artikel),dt->offene_Stueck()));
     instanz_menge(vec_artbase);
}

void auftrag_main::on_togglebutton_material_toggled()
{
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
//cout << psize<<'\n';
  if(!togglebutton_material->get_active() || !selected_Artikel) vpaned_an_mat->set_position(psize);
/*
  else  {
     vector<pair<ArtikelBase,long long int> > vec_artbase;
     vec_artbase.push_back(pair<ArtikelBase,guint>(ArtikelBase(selected_Artikel)),meter);
     instanz_menge(vec_artbase);
   }
*/
}


map<ArtikelBase,fixedpoint<5,double,long long int> > auftrag_main::get_artikelmap(const vector<pair<ArtikelBase, long long int> >& vec_artbase)
{
  map<ArtikelBase,fixedpoint<5,double,long long int> > map_artikel_all;
  for(vector<pair<ArtikelBase,long long int> >::const_iterator i=vec_artbase.begin();i!=vec_artbase.end();++i)
   {  
     ArtikelBaumFull ABF(i->first);
//     map<ArtikelBase,fixedpoint<5,double,long long int> > map_artikel_one = ABF.get_Artikel_Map();
     map<ArtikelBase,fixedpoint<5> > map_artikel_one = ABF.get_Artikel_Map();
//     for(map<ArtikelBase,fixedpoint<5,double,long long int> >::const_iterator m=map_artikel_one.begin();m!=map_artikel_one.end();++m)
     for(map<ArtikelBase,fixedpoint<5> >::const_iterator m=map_artikel_one.begin();m!=map_artikel_one.end();++m)
      {
        map_artikel_all[m->first] += (i->second) * (m->second);
      }
   }
  return map_artikel_all;
}

void auftrag_main::instanz_menge(const vector<pair<ArtikelBase,long long int> >& vec_artbase)
{
 if(!togglebutton_material->get_active()) return;
  map<ArtikelBase,fixedpoint<5,double,long long> > map_artikel = get_artikelmap(vec_artbase);

  vector<cH_ppsInstanz> vec_instanz;
  for (map<ArtikelBase,fixedpoint<5,double,long long int> >::const_iterator i=map_artikel.begin();i!=map_artikel.end();++i)
    {
     ArtikelStamm AS(i->first);
     // Wenn die Instanz noch nicht drin ist, dann anfügen
     if (vec_instanz.end()==find(vec_instanz.begin(),vec_instanz.end(),AS.BestellenBei()))
       vec_instanz.push_back(AS.BestellenBei());
    }
  Gtk::Table *table_materialbedarf = manage(new Gtk::Table(0,0,false));
  for(vector<cH_ppsInstanz>::const_iterator i=vec_instanz.begin();i!=vec_instanz.end();++i)
    {
      Gtk::Label *l=manage (new Gtk::Label(" "+(*i)->get_Name()+" "));
      int col = (*i)->Id();
      std::string pat;
      for (guint u=0;u<(*i)->get_Name().size()+2;++u) pat+="_";
      l->set_pattern(pat);
      l->set_alignment(0, 0.5);
      l->set_padding(0, 0);
      l->show();
      table_materialbedarf->attach(*l,col,col+1,0,1,0,0,0,0);
    }
   
  map<int,int> map_col_row;
  for (map<ArtikelBase,fixedpoint<5,double,long long int> >::const_iterator i=map_artikel.begin();i!=map_artikel.end();++i)
   {
     cH_ArtikelBezeichnung AB(i->first);
     ArtikelStamm AS(i->first);
     Einheit E(i->first);
     std::string menge = Formatiere_short(fixedpoint<2>(i->second));
     std::string einheit = E.MengenEinheit();
     Gtk::Label *l=manage (new 
            Gtk::Label(menge+einheit+' '+AB->Bezeichnung()));
     int col = AS.BestellenBei()->Id();
     int row = ++map_col_row[col];
     l->set_justify(GTK_JUSTIFY_LEFT);
     l->set_alignment(0, 0.5);
     l->set_padding(0, 0);
     l->show();
     table_materialbedarf->attach(*l,col,col+1,row,row+1,GTK_FILL,0,0,0);
     
//cout <<AB.Bezeichnung()<<'\t'<<AS.BestellenBei()->get_Name()<<'\t'<<i->second<<'\n';
   }
 viewport_materialbedarf->remove();
 table_materialbedarf->set_col_spacings(5);
 viewport_materialbedarf->add(*table_materialbedarf);
 table_materialbedarf->show();
 guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
// if (vpaned_an_mat->get_position() != psize/5. && vpaned_an_mat->get_position() != psize)
   vpaned_an_mat->set_position(psize/3.);
}

void auftrag_main::on_unselect_row(gint row, gint column, GdkEvent *event)
{
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
  vpaned_an_mat->set_position(psize);
}

void auftrag_main::on_prozlistscombo_search(int *cont, GtkSCContext newsearch)
{if (newsearch!=GTK_SEARCH_OPEN) return;
     {  
        ArtikelBaum artb(selected_Artikel.Id());
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
         maintree_s->redisplay(dt,Data_auftrag::VERARBEITUNG);         
      }catch(SQLerror &e)
        {meldung->Show(e); return;}
     }
}

void auftrag_main::on_button_auftrag_erledigt_clicked()
{
 if (selected_AufEintrag)
     {
      try{
//         int auftragid = selected_AufEintrag->getAuftragid();
//         int zeilennr  = selected_AufEintrag->getZnr();
         selected_AufEintrag->setStatus(CLOSED);
         cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
         maintree_s->redisplay(dt,Data_auftrag::METER);         
         maintree_s->redisplay(dt,Data_auftrag::STUECK);         
      }catch(SQLerror &e)
        {meldung->Show(e); return;}
     }
}


void auftrag_main::on_button_auftraege_clicked()
{
 hide();
 manage (new auftragliste(instanz));
}

void auftrag_main::on_button_artikeleingabe_clicked()
{
//cout << selected_Artikel.Id()<<'\n';
 if (selected_Artikel.Id())
  {
    std::string s = "artikeleingabe "+itos(selected_Artikel.Id())+" &";
    system(s.c_str());
    ArtikelBaum::UnCache(selected_Artikel);
  }
}


void auftrag_main::menu_instanz()
{
 Gtk::Menu *instanz_menu = manage(new class Gtk::Menu());
 Gtk::MenuItem *instanz = manage(new class Gtk::MenuItem("Instanz"));
 instanz->set_submenu(*instanz_menu);

 map<int,std::string> map_instanz = get_all_instanz();
 Gtk::RadioMenuItem::Group _RadioMIGroup_;
 for (map<int,std::string>::const_iterator i=map_instanz.begin();i!=map_instanz.end();++i)
  {
    Gtk::RadioMenuItem *rm = manage(new class Gtk::RadioMenuItem(_RadioMIGroup_,i->second));
    instanz_menu->append(*rm);
//    if (i->first==1) rm->set_active();
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

  if(_instanz_!=1) neue_auftraege_beruecksichtigen(instanz);
  on_neuladen_activate();

  if (_instanz_ != 1)
   {
    erfassen_button->hide();
    mainprint_button->hide();
    rechnung_button->hide();
    lieferschein_button->hide();
    table_prod_prozess->hide();
    frame_instanzen->show();
    neuer_auftrag_tree_titel_setzen(); // Kann auch in den Konstruktor MAT
    tree_neuer_auftrag->clear();
    Datum_instanz->setLabel("");
   }
  else 
   { 
    erfassen_button->show();
    mainprint_button->show();
    rechnung_button->show();
    lieferschein_button->show();
    table_prod_prozess->show();
    frame_instanzen->hide();
   }
}

void auftrag_main::neue_auftraege_beruecksichtigen(cH_ppsInstanz instanz)
{
//cout << instanz->get_Name()<<'\t';
  vector<AufEintragBase2> new_aufids =  get_new_aufids(instanz);
  Global_Settings GS(0,"pps",instanz->get_Name());
  GS.update_Date();
  Auftrag A(AuftragBase(instanz->Id(), 0));

//cout << "Neue Aufträge "<<new_aufids.size()<<'\n';
  unsigned int count=0;
  for (vector<AufEintragBase2>::const_iterator i=new_aufids.begin();i!=new_aufids.end();++i)
   {
     AufEintragBase AEB(*i);
     ArtikelBaumFull ABF(AEB.ArtikelID());
     map<ArtikelBase,fixedpoint<5> > artmap = ABF.get_Artikel_Map();
     for (map<ArtikelBase,fixedpoint<5> >::const_iterator m=artmap.begin();m!=artmap.end();++m)
      {
        if(ArtikelStamm(m->first.Id()).BestellenBei() == instanz)
         {
            int znr = A.insertNewEntry(double(m->second) * AEB.getStueck(),AEB.getLieferdatum(),m->first.Id());
            AuftragsEntryZuordnung(AEB,A,znr);
            ++count;
         }
      }
   }
  info_label_instanzen->set_text("Es sind "+itos(count)+" neue Zeilen hinzugekommen.     ");
  info_label_instanzen->show();
}
