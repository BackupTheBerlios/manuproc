// generated 2004/5/17 16:24:34 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0.2
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- mpc_agent.glade
// for gtk 2.2.4 and gtkmm 2.2.8
//
// Please modify the corresponding derived classes in ./src/kunden_selector.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include "kunden_selector_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/alignment.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>

kunden_selector_glade::kunden_selector_glade(
)
{  
   
   Gtk::Dialog *kunden_selector = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Image *image1 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-cancel"), Gtk::IconSize(4)));
   Gtk::Label *label9 = Gtk::manage(new class Gtk::Label("Cancel"));
   Gtk::HBox *hbox5 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment1 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   Gtk::Button *cancel_kunde_select = Gtk::manage(new class Gtk::Button());
   ok_kunde_select = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   kunden_select_liste = new class SimpleTree(3);
   
   Gtk::ScrolledWindow *scrolledwindow2 = Gtk::manage(new class Gtk::ScrolledWindow());
   image1->set_alignment(0.5,0.5);
   image1->set_padding(0,0);
   label9->set_alignment(0.5,0.5);
   label9->set_padding(0,0);
   label9->set_justify(Gtk::JUSTIFY_LEFT);
   label9->set_line_wrap(false);
   label9->set_use_markup(false);
   label9->set_selectable(false);
   hbox5->pack_start(*image1, Gtk::PACK_SHRINK, 0);
   hbox5->pack_start(*label9, Gtk::PACK_SHRINK, 0);
   alignment1->add(*hbox5);
   cancel_kunde_select->set_flags(Gtk::CAN_FOCUS);
   cancel_kunde_select->set_relief(Gtk::RELIEF_NORMAL);
   cancel_kunde_select->add(*alignment1);
   ok_kunde_select->set_flags(Gtk::CAN_FOCUS);
   ok_kunde_select->set_relief(Gtk::RELIEF_NORMAL);
   kunden_selector->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   scrolledwindow2->set_flags(Gtk::CAN_FOCUS);
   scrolledwindow2->set_shadow_type(Gtk::SHADOW_NONE);
   scrolledwindow2->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   scrolledwindow2->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   scrolledwindow2->add(*kunden_select_liste);
   kunden_selector->get_vbox()->set_homogeneous(false);
   kunden_selector->get_vbox()->set_spacing(0);
   kunden_selector->get_vbox()->pack_start(*scrolledwindow2);
   kunden_selector->set_title("customer selection");
   kunden_selector->set_default_size(640,480);
   kunden_selector->set_modal(true);
   kunden_selector->property_window_position().set_value(Gtk::WIN_POS_CENTER);
   kunden_selector->set_resizable(true);
   kunden_selector->property_destroy_with_parent().set_value(true);
   kunden_selector->set_has_separator(true);
   kunden_selector->add_action_widget(*cancel_kunde_select, -6);
   kunden_selector->add_action_widget(*ok_kunde_select, -5);
   image1->show();
   label9->show();
   hbox5->show();
   alignment1->show();
   cancel_kunde_select->show();
   ok_kunde_select->show();
   kunden_select_liste->show();
   scrolledwindow2->show();
   kunden_selector->show();
   cancel_kunde_select->signal_clicked().connect(SigC::slot(*this, &kunden_selector_glade::on_cancel_kunde_select_clicked));
   ok_kunde_select->signal_clicked().connect(SigC::slot(*this, &kunden_selector_glade::on_ok_kunde_select_clicked));
   kunden_select_liste->signal_leaf_selected().connect(SigC::slot(*this, &kunden_selector_glade::on_kundenlist_leaf_selected));
}

kunden_selector_glade::~kunden_selector_glade()
{  delete kunden_select_liste;
   delete gmm_data;
}