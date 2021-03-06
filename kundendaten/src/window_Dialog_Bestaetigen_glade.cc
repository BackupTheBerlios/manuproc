// generated 2006/5/31 10:21:47 CEST by jacek@jaceksdell.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/Monotone/kundendaten/kundendaten.glade
// for gtk 2.8.6 and gtkmm 2.8.0
//
// Please modify the corresponding derived classes in ./src/window_Dialog_Bestaetigen.cc


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
#include "window_Dialog_Bestaetigen_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/table.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


window_Dialog_Bestaetigen_glade::window_Dialog_Bestaetigen_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  window_Dialog_Bestaetigen = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *button_ok = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   Gtk::Button *button_cancel = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   label_text = Gtk::manage(new class Gtk::Label(_("label14")));
   
   Gtk::Table *table6 = Gtk::manage(new class Gtk::Table(2, 2, false));
   button_ok->set_flags(Gtk::CAN_FOCUS);
   button_ok->set_relief(Gtk::RELIEF_NORMAL);
   button_cancel->set_flags(Gtk::CAN_FOCUS);
   button_cancel->set_relief(Gtk::RELIEF_NORMAL);
   label_text->set_alignment(0,0.5);
   label_text->set_padding(0,0);
   label_text->set_justify(Gtk::JUSTIFY_CENTER);
   label_text->set_line_wrap(false);
   label_text->set_use_markup(false);
   label_text->set_selectable(false);
   table6->set_row_spacings(0);
   table6->set_col_spacings(0);
   table6->attach(*button_ok, 0, 1, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 5, 5);
   table6->attach(*button_cancel, 2, 3, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 5, 5);
   table6->attach(*label_text, 0, 3, 0, 1, Gtk::AttachOptions(), Gtk::AttachOptions(), 5, 5);
   window_Dialog_Bestaetigen->set_title(_("Bestätigen"));
   window_Dialog_Bestaetigen->set_modal(true);
   window_Dialog_Bestaetigen->property_window_position().set_value(Gtk::WIN_POS_NONE);
   window_Dialog_Bestaetigen->set_resizable(true);
   window_Dialog_Bestaetigen->property_destroy_with_parent().set_value(false);
   window_Dialog_Bestaetigen->add(*table6);
   button_ok->show();
   button_cancel->show();
   label_text->show();
   table6->show();
   window_Dialog_Bestaetigen->show();
   button_ok->signal_clicked().connect(SigC::slot(*this, &window_Dialog_Bestaetigen_glade::on_button_ok_clicked), false);
   button_cancel->signal_clicked().connect(SigC::slot(*this, &window_Dialog_Bestaetigen_glade::on_button_cancel_clicked), false);
}

window_Dialog_Bestaetigen_glade::~window_Dialog_Bestaetigen_glade()
{  delete gmm_data;
}
