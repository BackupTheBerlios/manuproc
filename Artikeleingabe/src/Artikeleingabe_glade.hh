// generated 2004/8/19 20:22:05 CEST by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/cvs/Artikeleingabe/artikeleingabe.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/Artikeleingabe.hh and./src/Artikeleingabe.cc

#ifndef _ARTIKELEINGABE_GLADE_HH
#  define _ARTIKELEINGABE_GLADE_HH

#include <gtk--/window.h>
#include <gtk--/label.h>
#include "ArtikelBox.hh"
#include <gtk--/checkbutton.h>
#include "WasTun.hh"
#include "Optionmenu_Instanz.hh"
#include "Optionmenu_Einheit.hh"
#include <gtk--/spinbutton.h>
#include "EingabeBox.hh"
#include <gtk--/pixmap.h>
#include "Optionmenu_Warengruppe.hh"
#include "KundenBox2.hh"
#include <gtk--/button.h>
#include <gtk--/toolbar.h>
#include "Artikel_Bestellen.hh"
#include <gtk--/table.h>
#include "SimpleTree.hh"
#include <gtk--/progressbar.h>
#include <gtk--/notebook.h>

class Artikeleingabe_glade : public Gtk::Window
{  
protected:
        
        class Gtk::Label * label_warnung;
        class ArtikelBox * artikelbox;
        class ArtikelBox * artikelbox_neu;
        class Gtk::CheckButton * no_instanz;
        class WasTun * was_tun;
        class Optionmenu_Instanz * standard_instanz;
        class Optionmenu_Einheit * standard_einheit;
        class Optionmenu_Einheit * OM_Einheit;
        class Gtk::CheckButton * mindbest_check;
        class Gtk::SpinButton * mindbestand;
        class EingabeBox * eingabebox;
        class Gtk::Pixmap * pixmap_edit;
        class Optionmenu_Warengruppe * alias_warengruppe;
        class KundenBox2 * alias_schema;
        class EingabeBox * alias_eingabe;
        class Gtk::Pixmap * alias_pixmap;
        class Optionmenu_Instanz * Artikel_Bestellen_bei;
        class Gtk::CheckButton * change_no_instanz;
        class Gtk::CheckButton * show_in_prlist;
        class Gtk::Toolbar * toolbar_loeschen;
        class Gtk::Button * button_artikel_delete;
        class Gtk::Button * button_artikel_wechsel;
        class Artikel_Bestellen * artikelboxb;
        class Gtk::Table * table_artikelherkunft;
        class SimpleTree * tree;
        class Gtk::CheckButton * checkbutton_offene_auftraege;
        class Optionmenu_Instanz * optionmenu_instanz;
        class Gtk::ProgressBar * progressbar;
        class ArtikelBox * artbox_verschmelzen;
        class Gtk::Table * table_verschmelzen;
        class Gtk::Notebook * top_notebook;
        
        Artikeleingabe_glade();
        
        ~Artikeleingabe_glade();
private:
        virtual void on_button_close_clicked() = 0;
        virtual void on_button_drucken_clicked() = 0;
        virtual void artikelbox_activate() = 0;
        virtual void artikelbox_neu_activate() = 0;
        virtual void on_no_instanz_toggled() = 0;
        virtual void on_was_tun_activate() = 0;
        virtual void on_optionmenu_standardinstanz_activate() = 0;
        virtual void on_standard_einheit_activate() = 0;
        virtual void on_einheit_activate() = 0;
        virtual void on_mindbest_check_toggled() = 0;
        virtual void on_mindbestand_activate() = 0;
        virtual void eingabe_activate() = 0;
        virtual void on_alias_warengruppe_activate() = 0;
        virtual void on_kunde_activate() = 0;
        virtual void on_alias_eingabe_activate() = 0;
        virtual void optionmenu_bestellen_bei_activate() = 0;
        virtual void on_change_no_instanz_toggled() = 0;
        virtual void on_show_in_prlist_toggled() = 0;
        virtual void on_button_artikel_delete_clicked() = 0;
        virtual void on_button_artikel_wechsel_clicked() = 0;
        virtual void on_Artikel_Bestellen_activate() = 0;
        virtual void on_leaf_selected(cH_RowDataBase leaf) = 0;
        virtual void on_node_selected(const TreeRow &node) = 0;
        virtual void on_unselect_row(gint row, gint column, GdkEvent *event) = 0;
        virtual void on_checkbutton_offene_auftraege_toggled() = 0;
        virtual void optionmenu_instanz_activate() = 0;
        virtual void on_button_verschmelzen_clicked() = 0;
};
#endif
