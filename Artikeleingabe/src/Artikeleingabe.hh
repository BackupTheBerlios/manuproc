#ifndef _ARTIKELEINGABE_HH
#  include "Artikeleingabe_glade.hh"
#  define _ARTIKELEINGABE_HH

#include <vector>
#include <list>
#include <Artikel/ArtikelBezeichnung.h>
#include <Instanzen/ppsInstanz.h>
#include <Artikel_Bestellen.hh>
#include <Aux/Ausgabe_neu.h>
#include <gtkmm/notebook.h>

struct Artikelgruppe
{  int schema,warengruppe,bestellen_bei,einheit;
   Artikelgruppe() 
   : schema(-1),warengruppe(-1),bestellen_bei(ppsInstanzID::None),einheit(-1) {}
   bool valid() const 
   { return schema!=-1 && warengruppe!=-1 && 
	bestellen_bei!=ppsInstanzID::Kundenauftraege && einheit!=-1; }
};

class Artikeleingabe : public Artikeleingabe_glade
{   
        static const unsigned int nachkommastellen=5;
        typedef fixedpoint<nachkommastellen> menge_t;
        typedef std::pair<cH_ArtikelBezeichnung,menge_t > zeile_t;
        typedef std::vector<zeile_t > vec_zeile_t; 
        typedef std::vector<ArtikelBase> vec_artbase_t;
        typedef std::vector<cH_ppsInstanz> instanz_spalte_t;
        typedef std::vector<cH_RowDataBase> datavec_t;
        
        vec_artbase_t vec_artbase;
        instanz_spalte_t instanz_spalte;

// ich denke ist unnötig, JJ 21.04.04
//        ArtikelBase fuer_artikel,von_artikel;

        // keine Bezeichnung, leere Spalte
        cH_ArtikelBezeichnung leer;

        friend class Artikeleingabe_glade;
        void on_button_close_clicked();
        void on_button_drucken_clicked();
        void on_button_artikel_delete_clicked();
        void on_button_artikel_wechsel_clicked();
        void on_neuladen_clicked();
        void set_tree_titels();
        void on_notebook1_switch_page(GtkNotebookPage *p0, guint p1);

        void fill_datavec(datavec_t& datavec,const ArtikelBase& AB);
        void push_Artikel(vec_zeile_t &vec_zeile, const zeile_t &z);
        void ArtikelBaum_Pfad(ArtikelBase AB,menge_t menge,
        	datavec_t& datavec,vec_zeile_t vec_zeile);

        void get_artikel_list(ppsInstanz::ID instanz);
        void set_Data_from_artikelliste();
        void on_leaf_selected(cH_RowDataBase d);
        void on_node_selected(const TreeRow &n);
        void on_unselect_row();
        void Eingabe_fuer(const ArtikelBase& art);
        void Loeschen_von(const ArtikelBase& art);
        void set_Prozess();
        void on_Artikel_Bestellen_activate();
        void optionmenu_instanz_activate();
        void load_for_optionmenu_instanz();
        void optionmenu_bestellen_bei_activate();
        void artikelbox_activate();
        void on_checkbutton_offene_auftraege_toggled();

        struct st_eingabe {std::string spalte; int signifikanz; 
            st_eingabe(std::string sp, int si) 
               : spalte(sp),signifikanz(si) {} 
            };
        std::vector<st_eingabe> vec_eingabe;
        std::list<int> list_sig;
        void on_togglebutton_edit_toggled();
        void fill_eingabebox(int nr);
        void eingabe_activate();
        void on_einheit_activate();
        void save_edited_artikel();   
        void warnung(std::string s);
        gint timeout_save_edited_artikel();
        gint timeout_save_edited_artikel2();
        gint timeout_warnung();
        SigC::Connection des,des2,des3;
        void on_kunde_activate();
        void save_edited_artikel2();   
        bool update_edited_artikel2();

//****** später
        void on_button_verschmelzen_clicked();

//****** edit ******
	bool fire_toggles;

//****** alias *****
        void on_alias_eingabe_activate();
        void on_alias_warengruppe_activate();
        std::vector<st_eingabe> vec_alias_eingabe;
        std::list<int> list_sig2;

//****** neu *******
	Artikelgruppe aktuelle_gruppe;
	
        void on_standard_einheit_activate();
        void on_optionmenu_standardinstanz_activate();
        void on_was_tun_activate();
        void on_no_instanz_toggled();
        void on_show_in_prlist_toggled();
        void on_change_no_instanz_toggled();
        void artikelbox_neu_activate();
        void neuenArtikelAnlegen(unsigned warengruppe,unsigned schema,
                const std::vector<std::string>&entries);

        void on_mindbest_check_toggled();
        void on_mindbestand_activate();

   public: 
       Artikeleingabe(int argc, char **argv);
};

#endif
