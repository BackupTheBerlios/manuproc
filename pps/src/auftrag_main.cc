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
   else  manage(new auftrag_bearbeiten(AufEintragBase2(instanz)));
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
 table_instanzen->hide();
 menu_instanz();
 instanz_menu();
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
}

void auftrag_main::fill_simple_tree()
{
//cout << "I="<<instanz<<"\t";
 if(!allaufids) 
   { SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz,(AufStatVal)OPEN);
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
// ist in glade drin: maintree_s->leaf_selected.connect(SigC::slot(this,&auftrag_main::on_leaf_selected));
}


void auftrag_main::on_leaf_selected(cH_RowDataBase d)
{
 const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*d);
 selected_AufEintrag = &dt->get_AufEintragBase();

 selected_Artikel=dt->get_Artikel_ID();
 prozlist_scombo->reset();
 prozlist_scombo->set_text(dt->ProzessText());

 if(instanz.Id() != 1) instanz_tree_inhalt_setzen(dt->get_Artikel_ID(),
      dt->offene_Meter(),dt->get_Lieferdatum());
}

void auftrag_main::on_prozlistscombo_search(int *cont, GtkSCContext newsearch)
{if (newsearch!=GTK_SEARCH_OPEN) return;
     {  //ArtikelBaum artb(selectedmyrow->getArtID());
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
      try{selected_AufEintrag->setVerarbeitung(cH_Prozess(pid));}
      catch(SQLerror &e)
        {meldung->Show(e); return;}
//      foot_statusbar->push(0,selectedmyrow->Description(
//                                cH_ExtBezSchema(ExtBezSchema::default_ID)));

      cH_EntryValue ev=AufEintrag(*selected_AufEintrag).getSeqValue(PROZ_SEQ);
      deque<guint> buf = maintree_s->get_seq();
      size_t i;
      for(i=0; (i<buf.size()) && (buf[i]!=PROZ_SEQ); i++);

// Schneller aber (noch?) nicht implementiert
//      selectedmyrow->setText(i,ev->getStrVal());
// stattdessen Neueinlesen der Liste:
      fill_simple_tree();
     }
}

void auftrag_main::on_button_auftraege_clicked()
{
 hide();
 manage (new auftragliste(instanz));
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
    rm->activate.connect(SigC::bind(SigC::slot(this,&auftrag_main::instanz_selected),i->first));
    rm->show();    
    if (i->first==1) rm->set_active();
  }
 main_menubar->append(*instanz);
 instanz->show();
       
}

void auftrag_main::instanz_selected(int _instanz_)
{
  instanz=(ppsInstanz::ppsInstId) _instanz_;
  on_neuladen_activate();

  if (_instanz_ != 1)
   {
    label_instanz->set_text(instanz.get_Name());
    erfassen_button->hide();
    mainprint_button->hide();
    rechnung_button->hide();
    lieferschein_button->hide();
    table_prod_prozess->hide();
    table_instanzen->show();
    instanz_tree_titel_setzen(); // Kann auch in den Konstruktor MAT
    neuer_auftrag_tree_titel_setzen(); // Kann auch in den Konstruktor MAT
    instanz_tree_herkunft->clear();
    datavec_instanz_auftrag.clear();
    tree_neuer_auftrag->clear();
    selected_instanz_znr=-1; // Kann auch in den Konstruktor MAT
    instanz_eintrag_status->hide(); // Kann auch in den Konstruktor MAT
   }
  else 
   { 
    erfassen_button->show();
    mainprint_button->show();
    rechnung_button->show();
    lieferschein_button->show();
    table_prod_prozess->show();
    table_instanzen->hide();
   }
}
