// generated 2004/10/22 18:13:20 CEST by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/cvs/mingw32/ManuProC_Agent/mpc_agent.glade
// for gtk 2.4.9 and gtkmm 2.4.3
//
// Please modify the corresponding derived classes in ./src/yes_no_dialog.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/compatibility.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "yes_no_dialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/alignment.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>

yes_no_dialog_glade::yes_no_dialog_glade(
)
{  
   
   Gtk::Dialog *yes_no_dialog = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Image *image3 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-no"), Gtk::IconSize(4)));
   Gtk::Label *label18 = Gtk::manage(new class Gtk::Label("No"));
   Gtk::HBox *hbox13 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment3 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   Gtk::Button *cancelbutton1 = Gtk::manage(new class Gtk::Button());
   Gtk::Image *image2 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-yes"), Gtk::IconSize(4)));
   Gtk::Label *label17 = Gtk::manage(new class Gtk::Label("Yes"));
   Gtk::HBox *hbox12 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment2 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   Gtk::Button *okbutton1 = Gtk::manage(new class Gtk::Button());
   msg_text = Gtk::manage(new class Gtk::Label("Are you sure ?"));
   image3->set_alignment(0.5,0.5);
   image3->set_padding(0,0);
   label18->set_alignment(0.5,0.5);
   label18->set_padding(0,0);
   label18->set_justify(Gtk::JUSTIFY_LEFT);
   label18->set_line_wrap(false);
   label18->set_use_markup(false);
   label18->set_selectable(false);
   hbox13->pack_start(*image3, Gtk::PACK_SHRINK, 0);
   hbox13->pack_start(*label18, Gtk::PACK_SHRINK, 0);
   alignment3->add(*hbox13);
   cancelbutton1->set_flags(Gtk::CAN_FOCUS);
   cancelbutton1->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton1->set_relief(Gtk::RELIEF_NORMAL);
   cancelbutton1->add(*alignment3);
   image2->set_alignment(0.5,0.5);
   image2->set_padding(0,0);
   label17->set_alignment(0.5,0.5);
   label17->set_padding(0,0);
   label17->set_justify(Gtk::JUSTIFY_LEFT);
   label17->set_line_wrap(false);
   label17->set_use_markup(false);
   label17->set_selectable(false);
   hbox12->pack_start(*image2, Gtk::PACK_SHRINK, 0);
   hbox12->pack_start(*label17, Gtk::PACK_SHRINK, 0);
   alignment2->add(*hbox12);
   okbutton1->set_flags(Gtk::CAN_FOCUS);
   okbutton1->set_flags(Gtk::CAN_DEFAULT);
   okbutton1->set_relief(Gtk::RELIEF_NORMAL);
   okbutton1->add(*alignment2);
   yes_no_dialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   msg_text->set_alignment(0.5,0.5);
   msg_text->set_padding(0,0);
   msg_text->set_justify(Gtk::JUSTIFY_LEFT);
   msg_text->set_line_wrap(false);
   msg_text->set_use_markup(false);
   msg_text->set_selectable(false);
   yes_no_dialog->get_vbox()->set_homogeneous(false);
   yes_no_dialog->get_vbox()->set_spacing(0);
   yes_no_dialog->get_vbox()->pack_start(*msg_text, Gtk::PACK_EXPAND_PADDING, 0);
   yes_no_dialog->set_title("Question");
   yes_no_dialog->set_modal(true);
   yes_no_dialog->property_window_position().set_value(Gtk::WIN_POS_CENTER);
   yes_no_dialog->set_resizable(true);
   yes_no_dialog->property_destroy_with_parent().set_value(false);
   yes_no_dialog->set_has_separator(true);
   yes_no_dialog->add_action_widget(*cancelbutton1, -9);
   yes_no_dialog->add_action_widget(*okbutton1, -8);
   image3->show();
   label18->show();
   hbox13->show();
   alignment3->show();
   cancelbutton1->show();
   image2->show();
   label17->show();
   hbox12->show();
   alignment2->show();
   okbutton1->show();
   msg_text->show();
   yes_no_dialog->show();
}

yes_no_dialog_glade::~yes_no_dialog_glade()
{  delete gmm_data;
}
