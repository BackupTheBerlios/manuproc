// generated 2001/9/18 9:40:08 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to Artikeleingabe.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _ARTIKELEINGABE_HH
#  include "Artikeleingabe_glade.hh"
#  define _ARTIKELEINGABE_HH

#include <vector>
#include <list>
#include <Artikel/ArtikelBezeichnung.h>
#include <Instanzen/ppsInstanz.h>
#include <Artikel_Bestellen.hh>
#include <Aux/Ausgabe_neu.h>

#define MAXCOLUMN 10

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
        ArtikelBase fuer_artikel,von_artikel;
        // keine Bezeichnung, leere Spalte
        cH_ArtikelBezeichnung leer;

        friend class Artikeleingabe_glade;
        void on_button_close_clicked();
        void on_button_drucken_clicked();
        void on_button_artikel_delete_clicked();
        void on_button_artikel_wechsel_clicked();
        void on_neuladen_clicked();
        void set_tree_titels();

        void fill_datavec(datavec_t& datavec,const ArtikelBase& AB);
        void push_Artikel(vec_zeile_t &vec_zeile, const zeile_t &z);
        void ArtikelBaum_Pfad(ArtikelBase AB,menge_t menge,
        	datavec_t& datavec,vec_zeile_t vec_zeile);

        void get_artikel_list(ppsInstanz::ID instanz);
        void set_Data_from_artikelliste();
        void on_leaf_selected(cH_RowDataBase d);
        void on_node_selected(const TCListNode &n);
        void on_unselect_row(gint row, gint column, GdkEvent *event);
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
        vector<st_eingabe> vec_eingabe,vec_eingabe2;
        std::list<int> list_sig,list_sig2;
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
        void on_eingabe2_activate();
        void save_edited_artikel2();   
        bool update_edited_artikel2();
        void on_optionmenu_warengruppe_activate();

        void on_button_verschmelzen_clicked();

        void on_togglebutton_neue_toggled();
        void on_standard_einheit_activate();
        void on_optionmenu_standardinstanz_activate();
        

   public: 
       Artikeleingabe(int argc, char **argv);
};

#endif
