/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef _AUFTRAG_MAIN_HH
#  include "auftrag_main_glade.hh"
#  define _AUFTRAG_MAIN_HH

#include <Artikel/Prozess.h>
#include <vector>
#include <Aux/ppsInstanz.h>
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/AuftragFull.h>
#include<Auftrag/selFullAufEntry.h>
#include <Aux/Long.h>

class auftrag_main : public auftrag_main_glade
{   
 SelectedFullAufList *allaufids;
 cH_ppsInstanz instanz;
 bool interne_namen_bool, zeit_kw_bool, kunden_nr_bool,
      kunden_anr_bool,auftraege_mit_kunden_bool;
 AufStatVal selected_status;
 AufEintragBase *selected_AufEintrag;
 static const unsigned int Artikelgroesse = 4;

public:

private:
   struct st_index {cH_ppsInstanz instanz; cH_Kunde Kunde; ArtikelBase artikel;
          st_index(cH_ppsInstanz i,cH_Kunde k,ArtikelBase a)
              : instanz(i),Kunde(k),artikel(a) {}
          bool operator<(const st_index& b) const
            {return instanz <b.instanz ||
                   (instanz==b.instanz && Kunde<b.Kunde )|| 
                   (instanz==b.instanz && Kunde==b.Kunde && artikel<b.artikel);} 
          };
   struct st_mengen {long sollMenge;long planMenge;long bestellMenge;
          st_mengen() : sollMenge(0),planMenge(0),bestellMenge(0) {} 
          st_mengen(long s,long g,long b) 
               : sollMenge(s),planMenge(g),bestellMenge(b) {} 
          st_mengen operator+=(const st_mengen& b)
            { sollMenge     += b.sollMenge; 
              planMenge     += b.planMenge; 
              bestellMenge  += b.bestellMenge; 
              return *this;
            }
           };
//   typedef pair<ArtikelBase,st_mengen> artmeng;
   typedef pair<st_index,st_mengen> artmeng;

 std::vector<cH_Prozess> prozlist;
        
        friend class auftrag_main_glade;
        void on_beenden_activate();
        void on_erfassen_activate();
        void on_neuladen_activate();
        void on_main_drucken_activate();
        void on_abschreiben_activate();
    	  void on_rechnung_activate();
        void on_main_bezeichnung_activate();
        void on_zeitdarstellung_activate();
        void on_kundendarstellung_activate();
        void on_materialbedarf_sortiert(){};
        void on_kunden_anr_activate();
        void on_offene_auftraege_activate();
        void on_auftraege_kunde_activate();
        void on_mainprint_button_clicked();
        void on_leaf_selected(cH_RowDataBase d);
        void on_node_selected(const TreeRow &node);
        void on_unselect_row(gint row, gint column, GdkEvent *event);
        void on_togglebutton_material_toggled();
        void on_togglebutton_auftraege_toggled();
        void show_selected_line();
        void on_button_auftrag_erledigt_clicked();
        void instanz_menge(const std::map<st_index,st_mengen>& map_allart);
        void get_ArtikelZusammensetzung(const ArtikelBase& art,const AufEintragBase& AEB,std::map<st_index,st_mengen>& map_allart);
        void get_ArtikelHerkunft(const ArtikelBase& art,const AufEintragBase& AEB,std::map<st_index,st_mengen>& map_allart);
        void geplanteMenge(const AufEintragBase& AEB,std::map<ArtikelBase,Long>& planmap);
        void getAufEintragBase_fromNode(TCListRow_API::const_iterator b,
            TCListRow_API::const_iterator e, std::map<st_index,st_mengen>& M);
        void fillStamm(int *cont, GtkSCContext newsearch);

        void set_column_titles_of_simple_tree();
        void fill_simple_tree();

        void on_button_artikeleingabe_clicked();
        void menu_instanz();
        std::map<int,std::string> get_all_instanz();
        void instanz_selected(int _instanz_);


public:
  // Spaltenbezeichnungen
   enum {KUNDE,A1,A2,A3,A4,LIEFERDATUM,AUFTRAG,VERARBEITUNG,METER,STUECK};

 bool interneNamen_bool() const { return interne_namen_bool; }
 bool Zeit_kw_bool() const { return zeit_kw_bool; }
 bool Kunden_nr_bool() const { return kunden_nr_bool; }
 bool Kunden_anr_bool() const {return kunden_anr_bool;}
 cH_ppsInstanz get_Instanz() const {return instanz;}
 
 auftrag_main();

private:
   // Ab hier für die Produktionsplanung ////////////////////////
   AuftragFull *instanz_auftrag;

//   void neue_auftraege_beruecksichtigen(cH_ppsInstanz instanz); //WEG
//   std::vector<AufEintragBase2> get_new_aufids(cH_ppsInstanz instanz); //WEG
   void on_searchcombo_auftragid_activate();
   void on_searchcombo_auftragid_search(int *cont, GtkSCContext newsearch) throw(SQLerror);
   void on_button_neue_anr_clicked();
   gint on_button_instanz_print_clicked(GdkEventButton *ev);
   void instanz_tree_titel_setzen();
   void neuer_auftrag_tree_titel_setzen();
   void instanz_leaf_auftrag(AufEintragBase& selected_AufEintrag);
   void show_neuer_auftrag();
   void tree_neuer_auftrag_leaf_selected(cH_RowDataBase d);
   void loadAuftragInstanz(const AuftragBase& auftragbase);
   int get_next_entry_znr(AuftragBase& auftrag);
//   void AuftragsEntryZuordnung(const AufEintragBase& AEB,long menge,const AuftragBase& AB,int znr);
   void on_togglebutton_geplante_menge_toggled();
   void on_button_Kunden_erledigt_clicked();
};

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
               case(MENGE)   : return x.second.sollMenge > y.second.sollMenge ;
               case(ARTIKEL) : return cH_ArtikelBezeichnung(x.first.artikel) < cH_ArtikelBezeichnung(y.first.artikel) ;
           }return false;}
};

#endif
