// generated 2004/11/8 10:36:07 CET by christof@felix.petig.de
// using glademm V2.6.0_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- artikeleingabe.glade
// for gtk 2.4.13 and gtkmm 2.4.5
//
// Please modify the corresponding derived classes in ./src/Artikeleingabe.hh and./src/Artikeleingabe.cc

#ifndef _ARTIKELEINGABE_GLADE_HH
#  define _ARTIKELEINGABE_GLADE_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include "ArtikelBox.hh"
#include <gtkmm/checkbutton.h>
#include "WasTun.hh"
#include "Optionmenu_Instanz.hh"
#include "Optionmenu_Einheit.hh"
#include <gtkmm/spinbutton.h>
#include "EingabeBox.hh"
#include <gtkmm/image.h>
#include "Optionmenu_Warengruppe.hh"
#include "KundenBox2.hh"
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <gtkmm/toolbutton.h>
#else //
#include <gtkmm/button.h>
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#include <gtkmm/toolbar.h>
#include "Artikel_Bestellen.hh"
#include <gtkmm/table.h>
#include "SimpleTree.hh"
#include <gtkmm/progressbar.h>
#include <gtkmm/notebook.h>

class Artikeleingabe_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
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
        class Gtk::Image * pixmap_edit;
        class Optionmenu_Warengruppe * alias_warengruppe;
        class KundenBox2 * alias_schema;
        class EingabeBox * alias_eingabe;
        class Gtk::Image * alias_pixmap;
        class Optionmenu_Instanz * Artikel_Bestellen_bei;
        class Gtk::CheckButton * change_no_instanz;
        class Gtk::CheckButton * show_in_prlist;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolButton * button_artikel_delete;
#else //
        class Gtk::Button * button_artikel_delete;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolButton * button_artikel_wechsel;
#else //
        class Gtk::Button * button_artikel_wechsel;
#endif //
        class Gtk::Toolbar * toolbar_loeschen;
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
        virtual void on_unselect_row() = 0;
        virtual void on_checkbutton_offene_auftraege_toggled() = 0;
        virtual void optionmenu_instanz_activate() = 0;
        virtual void on_button_verschmelzen_clicked() = 0;
        virtual void on_notebook1_switch_page(GtkNotebookPage *p0, guint p1) = 0;
};
#endif
