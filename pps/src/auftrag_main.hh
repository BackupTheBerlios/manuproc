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
#include <Instanzen/ppsInstanz.h>
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AuftragFull.h>
#include <Auftrag/AufEintragZu.h>
#include<Auftrag/selFullAufEntry.h>
#include <Misc/Long.h>
#include <Misc/UniqueValue.h>
#include <Auftrag/Verfuegbarkeit.h>

class auftrag_main : public auftrag_main_glade
{   
 SelectedFullAufList *allaufids;
 cH_ppsInstanz instanz;
 bool interne_namen_bool, zeit_kw_bool, kunden_nr_bool,
      kunden_anr_bool, auftraege_mit_kunden_bool,
	kunden_mit_ort;
 AufStatVal selected_status;
 AufEintrag *selected_AufEintrag;
 static const unsigned int Artikelgroesse = 4;
 bool block_callback;
 ArtikelTyp atyp; // nur diese Warengruppe anzeigen;

 AuftragBase::ID SelectedAuftragsId() const;
public:
 static const UniqueValue::value_t trace_instanzen;
 
private:
#if 0
   struct st_index {cH_ppsInstanz instanz; cH_Kunde Kunde; ArtikelBase artikel;
          st_index(cH_ppsInstanz i,cH_Kunde k,ArtikelBase a)
              : instanz(i),Kunde(k),artikel(a) {}
          st_index(cH_ppsInstanz i,ArtikelBase a)
              : instanz(i),Kunde(Kunde::default_id),artikel(a) {}
          bool operator<(const st_index& b) const
            {return instanz <b.instanz ||
                   (instanz==b.instanz && Kunde<b.Kunde )|| 
                   (instanz==b.instanz && Kunde==b.Kunde && artikel<b.artikel);} 
          };
   struct st_mengen {AuftragBase::mengen_t sollMenge, planMenge, bestellMenge, gelieferteMenge;
          st_mengen() : sollMenge(0),planMenge(0),bestellMenge(0),gelieferteMenge(0) {} 
          st_mengen(AuftragBase::mengen_t s,AuftragBase::mengen_t g,
                    AuftragBase::mengen_t b,AuftragBase::mengen_t gl) 
               : sollMenge(s),planMenge(g),bestellMenge(b),gelieferteMenge(gl) {} 
          st_mengen operator+=(const st_mengen& b)
            { sollMenge      = b.sollMenge; 
              planMenge     += b.planMenge; 
              bestellMenge  += b.bestellMenge; 
              gelieferteMenge+= b.gelieferteMenge; 
              return *this;
            }
           };
#endif
public:           
   typedef std::pair<Verfuegbarkeit::mapindex,Verfuegbarkeit::Mengen> artmeng;

private:
 std::vector<cH_Prozess> prozlist;
        
        friend class auftrag_main_glade;
        void loadEinstellungen();
	gint on_delete_event(GdkEventAny*);
        void on_beenden_activate();
        void save_WindowSize();
        void on_erfassen_activate();
        void neuladen();
        void on_neuladen_activate();
        void on_main_drucken_activate();
        void on_lieferscheine_activate();
    	  void on_rechnung_activate();
        void on_main_bezeichnung_activate();
        std::string bool2str(bool b);
        bool str2bool(std::string s,bool def);
        void on_zeitdarstellung_activate();
        void on_preload_orders_activate();
        void on_kundendarstellung_activate();
        void on_materialbedarf_sortiert();
        void on_kunden_anr_activate();

        // Statusänderung
        void on_offene_auftraege_activate();
        void on_closed_auftraege_activate();
        void on_storno_auftraege_activate();
        void on_unbest_auftraege_activate();
        void statusaenderung();
        // AuftragsID
        void on_plan_menge_menu_activate(); //0er Aufträge
        void on_ungeplante_menge_menu_activate(); // 1er und 20000er
        void on_dispo_menge_menu_activate(); //2er Aufträge
        void auftrags_id_aenderung();

        void on_auftraege_kunde_activate();
        gint on_mainprint_button_clicked(GdkEventButton *ev);
        void on_leaf_selected(cH_RowDataBase d);
        void on_leaf_unselected(cH_RowDataBase d);
        void on_node_selected(const TreeRow &node);
        void show_something_for(AufEintrag& selAufEintrag);
        void on_unselect_row(gint row, gint column, GdkEvent *event);
        void on_togglebutton_bestellen_toggled();
        void on_togglebutton_material_toggled();
        void on_togglebutton_auftraege_toggled();
        void handle_togglebutton(char c);
        void show_selected_line(bool lager=false);
        void on_button_auftrag_erledigt_clicked();
        void instanz_menge(const Verfuegbarkeit::map_t& map_allart);
        void get_ArtikelZusammensetzung(const AufEintrag& AEB,Verfuegbarkeit::map_t& map_allart);
        void get_ArtikelHerkunft(const AufEintrag& AEB,Verfuegbarkeit::map_t& map_allart);
        void getAufEintrag_fromNode(TCListRow_API::const_iterator b,
            TCListRow_API::const_iterator e, Verfuegbarkeit::map_t& M);
        void fillStamm(int *cont, GtkSCContext newsearch);

        void set_column_titles_of_simple_tree();
        void fill_simple_tree();

        void on_button_artikeleingabe_clicked();
        void menu_instanz();

        std::vector<cH_ppsInstanz> get_all_instanz();
        void radio_instanz_selected(const Gtk::RadioMenuItem *rm,const cH_ppsInstanz instanz);
        void instanz_selected(const cH_ppsInstanz instanz);
         
        void on_button_faerben_clicked(); 
        void on_offwarengrp_activate();

	static std::string FirstRow(gpointer user_data, int deep, std::deque<guint> seq);

public:
  // Spaltenbezeichnungen
   enum {KUNDE=0,A1,A2,A3,A4,LIEFERDATUM,AUFTRAG,LETZEPLANINSTANZ,
         VERARBEITUNG,LETZTELIEFERUNG,METER,STUECK};

 cH_ppsInstanz Instanz() const {return instanz;}
 bool interneNamen_bool() const { return interne_namen_bool; }
 bool Zeit_kw_bool() const { return zeit_kw_bool; }
 bool Kunden_nr_bool() const { return kunden_nr_bool; }
 bool Kunden_anr_bool() const {return kunden_anr_bool;}
 bool Kunden_mit_ort() const { return kunden_mit_ort;}
 cH_ppsInstanz get_Instanz() const {return instanz;}
 
 auftrag_main();

private:
   // Ab hier für die Produktionsplanung ////////////////////////
   AuftragFull *instanz_auftrag;

   void show_frame_instanzen_material();
   void show_main_menu();
   void on_searchcombo_auftragid_activate();
   void on_searchcombo_auftragid_search(int *cont, GtkSCContext newsearch) throw(SQLerror);
   void on_button_neue_anr_clicked();
   void on_kunden_lieferant_activate();
   gint on_button_instanz_print_clicked(GdkEventButton *ev);
   void instanz_tree_titel_setzen();
   void neuer_auftrag_tree_titel_setzen();
   void instanz_auftrag_anlegen(AufEintrag& AE);
   std::vector<cH_RowDataBase> ausgewaehlte_artikel();
   bool alle_ausgewaehlten_artikel_gleich(std::vector<cH_RowDataBase> L=std::vector<cH_RowDataBase>());
   AuftragBase::mengen_t restmengensumme_aller_ausgewaehlten_artikel(std::vector<cH_RowDataBase> L=std::vector<cH_RowDataBase>());
   void show_neuer_auftrag();
   void tree_neuer_auftrag_leaf_selected(cH_RowDataBase d);
   void loadAuftragInstanz(const AuftragBase& auftragbase);
   int get_next_entry_znr(AuftragBase& auftrag);
   void on_togglebutton_geplante_menge_toggled();
   void on_button_Kunden_erledigt_clicked();
   void on_button_instanz_get_selection_clicked();
   void on_OptMenu_Instanz_Bestellen_activate(){};

  // Ab hier fürs Lager
  void lager_zeigen();
  void lager_ueberschrift();
//  SelectedFullAufList lager_auftraege();
  struct st_tree_lager {AufEintrag AE;AuftragBase::mengen_t verplant;
                        AuftragBase::mengen_t bestellt;
       st_tree_lager(AufEintrag ae, AuftragBase::mengen_t v,AuftragBase::mengen_t b)
       :AE(ae),verplant(v),bestellt(b) {}};

  std::list<st_tree_lager> auftrags_lager_mengen(ArtikelBase artikel,SelectedFullAufList& AuftragsListe);
  void on_lager_leaf_selected(cH_RowDataBase d);
  void on_lager_node_selected(const TreeRow &node);
  void on_lager_unselect_row(gint row, gint column, GdkEvent *event);

};

#endif
