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

class auftrag_main : public auftrag_main_glade
{   
 SelectedFullAufList *allaufids;
 cH_ppsInstanz instanz;
 bool interne_namen_bool, zeit_kw_bool, kunden_nr_bool,kunden_anr_bool;
 AufEintragBase *selected_AufEintrag;
 static const unsigned int Artikelgroesse = 4;

public:
   struct st_AufArt {ArtikelBase AB;AufEintragBase AEB;
         st_AufArt(ArtikelBase ab,AufEintragBase aeb):AB(ab),AEB(aeb){}  
         bool operator<(const st_AufArt& b) const
            {return AB<b.AB || (AB==b.AB&&AEB<b.AEB);} 
         };
private:

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
        void on_mainprint_button_clicked();
	     void on_prozlistscombo_search(int *cont, GtkSCContext newsearch);
        void on_prozlistscombo_activate();
        void on_button_auftraege_clicked();
        void on_leaf_selected(cH_RowDataBase d);
        void on_node_selected(const TreeRow &node);
        void on_unselect_row(gint row, gint column, GdkEvent *event);
        void on_togglebutton_material_toggled();
        void on_button_auftrag_erledigt_clicked();
//        void instanz_menge(const map<ArtikelBase,double>& map_artbase);
        void instanz_menge(const map<AufEintragBase,double>& map_aufbase);
        map<st_AufArt,fixedpoint<5,double,long long> > get_artikelmap(const map<AufEintragBase,double>& map_aufbase);
        void getAufEintragBase_fromNode(TCListRow_API::const_iterator b,
            TCListRow_API::const_iterator e, map<AufEintragBase,double>& M);
        void fillStamm(int *cont, GtkSCContext newsearch);
        bool auftrag_geplant(const AufEintragBase& aeb,int instanz);

        void set_column_titles_of_simple_tree();
        void fill_simple_tree();

        void on_button_artikeleingabe_clicked();
        void menu_instanz();
        map<int,std::string> get_all_instanz();
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

   void neue_auftraege_beruecksichtigen(cH_ppsInstanz instanz);
   vector<AufEintragBase2> get_new_aufids(cH_ppsInstanz instanz);
   void on_searchcombo_auftragid_activate();
   void on_searchcombo_auftragid_search(int *cont, GtkSCContext newsearch) throw(SQLerror);
   void on_button_neue_anr_clicked();
   gint on_button_instanz_print_clicked(GdkEventButton *ev);
   void instanz_tree_titel_setzen();
   void neuer_auftrag_tree_titel_setzen();
   void instanz_leaf_auftrag(AufEintragBase& selected_AufEintrag);
   void show_neuer_auftrag();
   void tree_neuer_auftrag_leaf_selected(cH_RowDataBase d);
   void loadAuftrag(const AuftragBase& auftragbase);
   int get_next_entry_znr(AuftragBase& auftrag);
   void AuftragsEntryZuordnung(const AufEintragBase& AEB,long menge,const AuftragBase& AB,int znr);
   void on_togglebutton_geplante_menge_toggled();
};

class MatListSort
{
   public:  
     enum esort {MENGE,ARTIKEL};
   private:
     esort es;
   public:
     MatListSort(enum esort _es):es(_es) {}
/*
     bool operator() (pair<cH_ArtikelBezeichnung,long long int> x,
                      pair<cH_ArtikelBezeichnung,long long int> y) const
           { switch(es) {
               case(MENGE)   : return x.second > y.second  ; break;
               case(ARTIKEL) : return x.first  < y.first   ; break;
           }}
*/
     bool operator() (pair<auftrag_main::st_AufArt,long long int> x,
                      pair<auftrag_main::st_AufArt,long long int> y) const
           { cH_ArtikelBezeichnung ABx(x.first.AB);
             cH_ArtikelBezeichnung ABy(y.first.AB);
             switch(es) {
               case(MENGE)   : return x.second > y.second  ; break;
               case(ARTIKEL) : return ABx      < ABy   ; break;
           }}
};



#endif
