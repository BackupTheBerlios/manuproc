// generated 2006/5/22 16:46:04 CEST by christof@vesta.petig-baender.de
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- auftrag.glade
// for gtk 2.8.17 and gtkmm 2.8.5
//
// Please modify the corresponding derived classes in ./src/termsplidial.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#endif
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/compatibility.h>
#include <sigc++/bind.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "termsplidial_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/table.h>
#include <gtkmm/box.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


termsplidial_glade::termsplidial_glade(
)
{  
   
   Gtk::Dialog *termsplidial = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *cancbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   Gtk::Button *okbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   Gtk::Label *label36 = Gtk::manage(new class Gtk::Label(_("neuer Termin")));
   Gtk::Adjustment *menge_neu_adj = Gtk::manage(new class Gtk::Adjustment(1, 1, 1000000, 1, 10, 10));
   menge_neu = Gtk::manage(new class Gtk::SpinButton(*menge_neu_adj, 1, 0));
   termin_neu = new class datewin();
   
   Gtk::Label *label2 = Gtk::manage(new class Gtk::Label(_("Teilmenge")));
   Gtk::Table *table1 = Gtk::manage(new class Gtk::Table(2, 2, false));
   cancbutton->set_flags(Gtk::CAN_FOCUS);
   cancbutton->set_relief(Gtk::RELIEF_NORMAL);
   okbutton->set_flags(Gtk::CAN_FOCUS);
   okbutton->set_relief(Gtk::RELIEF_NORMAL);
   termsplidial->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   label36->set_alignment(0,0.5);
   label36->set_padding(0,0);
   label36->set_justify(Gtk::JUSTIFY_LEFT);
   label36->set_line_wrap(false);
   label36->set_use_markup(false);
   label36->set_selectable(false);
   menge_neu->set_flags(Gtk::CAN_FOCUS);
   menge_neu->set_update_policy(Gtk::UPDATE_ALWAYS);
   menge_neu->set_numeric(true);
   menge_neu->set_digits(0);
   menge_neu->set_wrap(false);
   label2->set_alignment(0,0.5);
   label2->set_padding(0,0);
   label2->set_justify(Gtk::JUSTIFY_LEFT);
   label2->set_line_wrap(false);
   label2->set_use_markup(false);
   label2->set_selectable(false);
   table1->set_border_width(5);
   table1->set_row_spacings(5);
   table1->set_col_spacings(0);
   table1->attach(*label36, 0, 1, 0, 1, Gtk::EXPAND, Gtk::AttachOptions(), 0, 0);
   table1->attach(*menge_neu, 1, 2, 1, 2, Gtk::AttachOptions(), Gtk::AttachOptions(), 0, 0);
   table1->attach(*termin_neu, 1, 2, 0, 1, Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 0, 0);
   table1->attach(*label2, 0, 1, 1, 2, Gtk::AttachOptions(), Gtk::AttachOptions(), 0, 0);
   termsplidial->get_vbox()->set_homogeneous(false);
   termsplidial->get_vbox()->set_spacing(0);
   termsplidial->get_vbox()->pack_start(*table1, Gtk::PACK_SHRINK, 0);
   termsplidial->set_title(_("Termin splitten"));
   termsplidial->set_modal(false);
   termsplidial->property_window_position().set_value(Gtk::WIN_POS_NONE);
   termsplidial->set_resizable(true);
   termsplidial->property_destroy_with_parent().set_value(false);
   termsplidial->set_has_separator(true);
   termsplidial->add_action_widget(*cancbutton, -6);
   termsplidial->add_action_widget(*okbutton, -5);
   cancbutton->show();
   okbutton->show();
   label36->show();
   menge_neu->show();
   termin_neu->show();
   label2->show();
   table1->show();
   termsplidial->show();
   cancbutton->signal_clicked().connect(SigC::slot(*this, &termsplidial_glade::on_cancbutton_clicked), false);
   okbutton->signal_clicked().connect(SigC::slot(*this, &termsplidial_glade::on_okbutton_clicked), false);
}

termsplidial_glade::~termsplidial_glade()
{  delete termin_neu;
   delete gmm_data;
}
