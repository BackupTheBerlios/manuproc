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
#include "auftragliste.hh"
#include "AufEintrag.h"

extern MyMessage *meldung;
extern auftrag_main *auftragmain;

bool stuecksumme;/* ob stueck summiert werden sollen */
bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
                        /* gespeichert werden soll */

#include <Aux/EntryValueIntString.h>
#include <Aux/EntryValueDatum.h>
#include <Aux/EntryValueFixed.h>
#include <tclistnode.h>
#include <Aux/Ausgabe_neu.h>
#include "aktAufEintrag.h" //wg. Lieferwoche ???
//#include <Faeden/Schussfaeden.hh>
#include <Artikel/ArtikelMengeSumme.h>
#include <cstdio>

class Data_auftrag : public RowDataBase
{
   const AufEintragBase &AB ;
   const auftrag_main *AM ;
   const ArtikelMenge menge;

public:
   Data_auftrag(const AufEintragBase& ab, auftrag_main* am) :
      AB(ab),AM(am),menge(AB.ArtId(),AB.getRestStk()) {}

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case auftrag_main::KUNDE : {
       if(AM->get_Instanz()->Id()==ppsInstanz::INST_KNDAUF)
        {
         if (AM->Kunden_nr_bool())
            return cH_EntryValueIntString(AB.getKdNr()); 
         else
            return cH_EntryValueIntString(cH_Kunde(AB.getKdNr())->firma()); 
        }
       else { std::string k;
         list<cH_Kunde> LK=AB.get_Referenz_Kunden();
         for (list<cH_Kunde>::const_iterator i=LK.begin();i!=LK.end();)
           { k+=(*i)->firma();
             if (++i != LK.end()) k+=", ";
           }
         return cH_EntryValueIntString(k);
         }
       }
      case auftrag_main::A1 ... auftrag_main::A4 : {
         cH_ExtBezSchema schema(1,ExtBezSchema::default_Typ);
         if (!AM->interneNamen_bool()) schema = cH_ExtBezSchema(AB.getKdNr(),ExtBezSchema::default_Typ);
         cH_ArtikelBezeichnung artbez(AB.ArtId(),schema);
         return artbez->Komponente_als_EntryValue(seqnr-int(auftrag_main::A1));
         }
      case auftrag_main::LIEFERDATUM : {
         std::string lw;
         if (AM->Zeit_kw_bool())
           {
            int lieferwoche = AB.getLieferdatum().KW().Woche();
            int lieferjahr = AB.getLieferdatum().KW().Jahr();
            string lj=itos (lieferjahr).substr(2);
            lw = itos(lieferwoche)+"'"+lj;
            return cH_EntryValueIntString(lw);
           }
         else   return cH_EntryValueDatum(AB.getLieferdatum());
       }
      case auftrag_main::AUFTRAG : {
            std::string auftrag;
            if(AM->Kunden_anr_bool()) auftrag =      AB.getYourAufNr() ;
            else                      auftrag = itos(AB.getAuftragid()) ;
            return cH_EntryValueIntString(auftrag);}
      case auftrag_main::VERARBEITUNG : {
         std::string verarbeitung;
         try {
            verarbeitung = AB.getProzess()->getTyp()+" "+AB.getProzess()->getText();
#warning das dauert ewig !!!
//            verarbeitung = AB.getProzess2_c_str();
         } catch (std::exception &e ) 
         { verarbeitung=e.what(); }
	 return cH_EntryValueIntString(verarbeitung);
	 }
#ifdef PETIG_EXTENSIONS	 
      case auftrag_main::METER : 
         {ArtikelMenge::mengen_t m=menge.getMenge(Einheit::Meter);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen>(m);
         }
      case auftrag_main::STUECK : 
         {ArtikelMenge::mengen_t m=menge.getMenge(Einheit::Stueck);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen>(m);
         }
#else
      case auftrag_main::METER : 
      	  return cH_EntryValueIntString(menge.abgeleiteteMenge());
      case auftrag_main::STUECK : 
          return cH_EntryValueIntString(menge.Menge());
#endif          
     }
   return cH_EntryValue();
 }

   int offene_Stueck()const {return AB.getRestStk();}
   int get_aid() const {return AB.getAuftragid();} 
   int get_zeilennr() const {return AB.getZnr();} 
   int get_Artikel_ID() const {return AB.ArtId();}
   Petig::Datum get_Lieferdatum() const {return AB.getLieferdatum();}
   std::string ProzessText() const {return AB.getProzess()->getTyp()+" "+AB.getProzess()->getText() ;}
   AufEintragBase& get_AufEintragBase() const {return const_cast<AufEintragBase&>(AB);}
   const ArtikelMenge getArtikelMenge() const { return menge; }
};

struct cH_Data_auftrag : public Handle<const Data_auftrag>
{  cH_Data_auftrag(const Data_auftrag *r) : Handle<const Data_auftrag>(r){}
};


class Data_Node : public TreeRow
{
  ArtikelMengeSumme sum;
// na, wäre da eine map nicht sinnvoller ?
// vector<pair<ArtikelBase,long long int> > vec_artbase;
 map<ArtikelBase,double> map_artbase;

public:
 virtual void cumulate(const cH_RowDataBase &rd)
   {  const ArtikelMenge &am=dynamic_cast<const Data_auftrag &>(*rd).getArtikelMenge();
      sum.cumulate(am);
//      vec_artbase.push_back(pair<ArtikelBase,long long int>(am.Artikel(),am.getMenge()));
      map_artbase[am.Artikel()] += am.getMenge();
   }
  const cH_EntryValue Value(guint index,gpointer gp) const
   {
    switch(index) 
      { 
#ifdef PETIG_EXTENSIONS	 
      case auftrag_main::METER : 
         {ArtikelMengeSumme::mengen_t m=sum.Summe(Einheit::Meter);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen,double,long long>(m);
         }
      case auftrag_main::STUECK : 
         {ArtikelMengeSumme::mengen_t m=sum.Summe(Einheit::Stueck);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen,double,long long>(m);
         }
#else
        case auftrag_main::METER : return cH_EntryValueIntString(sum.abgeleiteteMenge());
        case auftrag_main::STUECK : return cH_EntryValueIntString(sum.Menge());
#endif
        default : return cH_EntryValue();
      }
   }
 Data_Node::Data_Node(guint deep,const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand)
   :TreeRow(deep,v,child_s_deep,child_s_data,expand) {}
 static TreeRow *create(guint col, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand)
   {  return new Data_Node(col,v,child_s_deep,child_s_data,expand);
   }

// vector<pair<ArtikelBase,long long int> > get_vec_ArtikelBase_Stueck() const {return vec_artbase;}
// map<ArtikelBase,double> get_map_ArtikelBase_Stueck() const {return map_artbase;}
//  map<AufEintragBase,double> get_map_AufEintrag_Stueck() const;
};


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
 selected_AufEintrag=0;
}

void auftrag_main::on_neuladen_activate()
{
 if(allaufids) { delete(allaufids); allaufids=NULL; }
 fill_simple_tree();
}

#define TEXCMD "tex2prn -2 -q -Phl1260"
//#define TEXCMD "tex2prn -2 -G -Phl1260" // Preview

static string kopfzeile(int col,const string &typ,const string &title,gpointer user_data)
{  if (!strncmp(title.c_str(),"Verarbeitung",12)) return "p{3.5cm}";
   return typ;
}

static string shorten_some(int col,const string &title,gpointer user_data)
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
 interne_namen_bool = main_intbez->get_active();

// cH_ExtBezSchema cs(1,ExtBezSchema::default_Typ);
// for(vector<AufEintragBase>::iterator j = allaufids->aufidliste.begin();
//        j!=allaufids->aufidliste.end(); ++j)
//     {
//      if (selected_AufEintrag)
//         if(interne_namen) cs = cH_ExtBezSchema((*selected_AufEintrag).getKdNr(),ExtBezSchema::default_Typ);
//      else         
//         if(interne_namen) cs = cH_ExtBezSchema(1,ExtBezSchema::default_Typ);
//     }
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
 if(!allaufids) 
   { SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),(AufStatVal)OPEN);
     allaufids = new SelectedFullAufList(psel);
   }

 vector<cH_RowDataBase> datavec;
 for(vector<AufEintragBase>::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
  {
   datavec.push_back(new Data_auftrag(*i,this));
  }
 maintree_s->setDataVec(datavec);
}

void auftrag_main::on_node_selected(const TreeRow &node)
{
  map<AufEintragBase,double> map_aufbase;
//  getAufEintragBase_fromNode(node.getTCL_API().begin(),node.getTCL_API().end(),map_aufbase);
  getAufEintragBase_fromNode(node.begin(),node.end(),map_aufbase);
  instanz_menge(map_aufbase);
}

void auftrag_main::getAufEintragBase_fromNode(TCListRow_API::const_iterator b,
         TCListRow_API::const_iterator e, map<AufEintragBase,double>& M)
{  
 for (TCListRow_API::const_iterator i=b;i!=e;++i)
  {  const TreeRow *tlr=reinterpret_cast<const TreeRow *>((*i).get_user_data());
     if(tlr->Leaf())
      {
        const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*(tlr->LeafData()));
        M[dt->get_AufEintragBase()]=dt->offene_Stueck();
      }
    if ((*i).begin()!=(*i).end()) getAufEintragBase_fromNode((*i).begin(),(*i).end(),M);
  }
}


void auftrag_main::on_leaf_selected(cH_RowDataBase d)
{
 try{
 const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*d);
 selected_AufEintrag = &dt->get_AufEintragBase();

 prozlist_scombo->reset();
 prozlist_scombo->set_text(dt->ProzessText());

 if(instanz->Id() != 1) instanz_leaf_auftrag(*selected_AufEintrag);

 // Zum Anzeigen der benötigten Menge
// map<ArtikelBase,double> map_artbase;
// map_artbase[dt->get_Artikel_ID()]=dt->offene_Stueck();
// instanz_menge(map_artbase);
 map<AufEintragBase,double> map_aufbase;
 map_aufbase[dt->get_AufEintragBase()]=dt->offene_Stueck();
 instanz_menge(map_aufbase);
 }catch(std::exception &e) {cerr<<e.what();}
}

void auftrag_main::on_togglebutton_material_toggled()
{
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
//  if(!togglebutton_material->get_active() || !selected_Artikel) vpaned_an_mat->set_position(psize);
  if(!togglebutton_material->get_active() || !selected_AufEintrag) vpaned_an_mat->set_position(psize);
}

std::map<auftrag_main::st_AufArt,fixedpoint<5,double,long long int> > auftrag_main::get_artikelmap(const map<AufEintragBase,double>& map_aufbase)
{
  map<st_AufArt,fixedpoint<5,double,long long int> > map_artikel_all;
  for(map<AufEintragBase,double>::const_iterator i=map_aufbase.begin();i!=map_aufbase.end();++i)
   {  
     ArtikelBaumFull ABF(i->first.ArtId());
     map<ArtikelBase,fixedpoint<5> > map_artikel_one = ABF.get_Artikel_Map();
     for(map<ArtikelBase,fixedpoint<5> >::const_iterator m=map_artikel_one.begin();m!=map_artikel_one.end();++m)
      {
        map_artikel_all[st_AufArt(m->first,i->first)] += (i->second) * (m->second);
      }
   }
  return map_artikel_all;
}

#include "Geplant_small.xpm"

//void auftrag_main::instanz_menge(const map<ArtikelBase,double>& map_artbase)
void auftrag_main::instanz_menge(const map<AufEintragBase,double>& map_aufbase)
{
 if(!togglebutton_material->get_active()) return;
  map<st_AufArt,fixedpoint<5,double,long long> > map_artikel = get_artikelmap(map_aufbase);

  // 1. Schritt: Wieviel Instanzen gibt es?
  vector<cH_ppsInstanz> vec_instanz;
  for (map<st_AufArt,fixedpoint<5,double,long long int> >::const_iterator i=map_artikel.begin();i!=map_artikel.end();++i)
    {
     ArtikelStamm AS(i->first.AB);
     // Wenn die Instanz noch nicht drin ist, dann anfügen
     if (vec_instanz.end()==find(vec_instanz.begin(),vec_instanz.end(),AS.BestellenBei()))
       vec_instanz.push_back(AS.BestellenBei());
    }
  // 2. Schritt Überschriften (Instanzen) in die Tabelle schreiben
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
  // 3. Schritt: Artikel nach Instanzen sortiert in Listen schreiben
//  std::map<int,std::list<pair<cH_ArtikelBezeichnung,long long int> > > LM;
  std::map<int,std::list<pair<st_AufArt,long long int> > > LM;
  for (map<st_AufArt,fixedpoint<5,double,long long int> >::const_iterator i=map_artikel.begin();i!=map_artikel.end();++i)
   {
     ArtikelStamm AS(i->first.AB);
     int col = AS.BestellenBei()->Id();
//     cH_ArtikelBezeichnung AB(i->first.AB);
     long long int menge=i->second;
//     LM[col].push_back(pair<cH_ArtikelBezeichnung,long long int>(AB,menge));
     LM[col].push_back(pair<st_AufArt,long long int>(i->first,menge));
   }
  // 4. Schritt Listen sortieren und in Tabelle einfügen
//  for (std::map<int,std::list<pair<cH_ArtikelBezeichnung,long long int> > >::iterator i=LM.begin();i!=LM.end();++i)
  for (std::map<int,std::list<pair<st_AufArt,long long int> > >::iterator i=LM.begin();i!=LM.end();++i)
   {
     if(materialbedarf_sortiert_nach_artikel->get_active())
        i->second.sort(MatListSort(MatListSort::ARTIKEL)); 
     else
        i->second.sort(MatListSort(MatListSort::MENGE)); 

     int col = i->first;
     int row=1;
     for(std::list<pair<st_AufArt,long long int> >::const_iterator j=i->second.begin();j!=i->second.end();++j)
      {
        Einheit E(j->first.AB);
        std::string menge = Formatiere_short(fixedpoint<2>(j->second));
        std::string einheit = E.MengenEinheit();
        Gtk::Label *l;
        if(materialbedarf_sortiert_nach_artikel->get_active())
          l=manage (new Gtk::Label(cH_ArtikelBezeichnung(j->first.AB)->Bezeichnung()+' '+menge+einheit));
        else 
          l=manage (new Gtk::Label(menge+einheit+' '+cH_ArtikelBezeichnung(j->first.AB)->Bezeichnung()));
        ++row;
        l->set_justify(GTK_JUSTIFY_LEFT);
        l->set_alignment(0, 0.5);
        l->set_padding(0, 0);
        l->show();
        Gtk::Pixmap *pixmapG = manage(new class Gtk::Pixmap(Geplant_small_xpm));
        Gtk::HBox *hb = manage(new class Gtk::HBox(false, 0));
        hb->pack_start(*pixmapG,false,false,0);
        hb->pack_start(*l, false, false, 0);
        if(auftrag_geplant(j->first.AEB,i->first)) pixmapG->show();
        hb->show();
        table_materialbedarf->attach(*hb,col,col+1,row,row+1,GTK_FILL,0,0,0);
      }
   }
 viewport_materialbedarf->remove();
 table_materialbedarf->set_col_spacings(5);
 viewport_materialbedarf->add(*table_materialbedarf);
 table_materialbedarf->show();
 guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
 vpaned_an_mat->set_position(psize/3.);
}

bool auftrag_main::auftrag_geplant(const AufEintragBase& aeb,int instanz)
{
  list<pair<AufEintragBase2,long> > L=aeb.get_Referenz_AufEintragBase2(true,true);
  for(list<pair<AufEintragBase2,long> >::const_iterator i=L.begin();i!=L.end();++i)
{
//cout << instanz<<'\t'<<i->first.Instanz()<<'\t'<<i->first.Id()<<'\n';
     //Aufträge mit Id=0 sind ungeplant!
     if(i->first.Instanz()==instanz && i->first.Id()!=0)
        return true;
}
  return false;
}

void auftrag_main::on_unselect_row(gint row, gint column, GdkEvent *event)
{
  guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
  vpaned_an_mat->set_position(psize);
}

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

void auftrag_main::on_button_auftrag_erledigt_clicked()
{
 if (selected_AufEintrag)
     {
      try{
         selected_AufEintrag->setStatus(CLOSED);
         cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
         maintree_s->redisplay(dt,METER);         
         maintree_s->redisplay(dt,STUECK);         
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

 map<int,std::string> map_instanz = get_all_instanz();
 Gtk::RadioMenuItem::Group _RadioMIGroup_;
 for (map<int,std::string>::const_iterator i=map_instanz.begin();i!=map_instanz.end();++i)
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

  if(_instanz_==1)
   {
    guint psize=GTK_WIDGET(vpaned_an_mat->gtkobj())->allocation.height;  
    vpaned_an_mat->set_position(5*psize); // Großer Wert
   }

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
    spinbutton_geplante_menge->hide();
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
 try{ 
  Auftrag A(AuftragBase(instanz->Id(), 0)); 
//cout << "Neue Aufträge "<<new_aufids.size()<<'\n';
  unsigned int count=0;
  for (vector<AufEintragBase2>::const_iterator i=new_aufids.begin();i!=new_aufids.end();++i)
   {
     AufEintragBase AEB(*i);
     ArtikelBaumFull ABF(AEB.ArtId());
     map<ArtikelBase,fixedpoint<5> > artmap = ABF.get_Artikel_Map();
     for (map<ArtikelBase,fixedpoint<5> >::const_iterator m=artmap.begin();m!=artmap.end();++m)
      {
        if(ArtikelStamm(m->first.Id()).BestellenBei() == instanz)
         {
            int znr = A.insertNewEntry(double(m->second) * AEB.getStueck(),AEB.getLieferdatum(),m->first.Id());
            AuftragsEntryZuordnung(AEB,AEB.getStueck(),A,znr);
            ++count;
         }
      }
   }
  info_label_instanzen->set_text("Es sind "+itos(count)+" neue Zeilen hinzugekommen.     ");
  info_label_instanzen->show();
 }catch(...){}
}
