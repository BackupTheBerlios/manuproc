// generated 2004/2/23 11:50:39 CET by jacek@woody.mabella.de
// using glademm V2.0.0c_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/cvs/pps/auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.8
//
// Please modify the corresponding derived classes in ./src/buchen_dialog.cc

#include "config.h"
#include "buchen_dialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtk--/accelgroup.h>
#include <vector>
#include <libgnomeui/gnome-stock.h>
#include <gtk--/button.h>
#include <gtk--/buttonbox.h>
#include <gtk--/toolbar.h>
#include <gtk--/box.h>

buchen_dialog_glade::buchen_dialog_glade(
) : Gnome::Dialog()
{  buchen_dialog = this;
   
   Gtk::AccelGroup *buchen_dialog_accgrp = Gtk::AccelGroup::create();
   gmm_data = new GlademmData(buchen_dialog_accgrp);
   buchen_dialog->append_button(GNOME_STOCK_BUTTON_YES);
   buchen_dialog->append_button(GNOME_STOCK_BUTTON_NO);
   buchen_dialog->append_button(GNOME_STOCK_BUTTON_CANCEL);
   
   Gtk::Toolbar *toolbar17 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   text = manage(new class Gtk::Label(""));
   
   Gtk::HBox *hbox34 = manage(new class Gtk::HBox(false, 0));
   
   toolbar17->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_HELP)), 0));
   Gtk::Button *button37 = static_cast<Gtk::Button *>(toolbar17->tools().back()->get_widget());
   buchen_dialog->get_button(0)->set_flags(GTK_CAN_FOCUS);
   buchen_dialog->get_button(1)->set_flags(GTK_CAN_FOCUS);
   buchen_dialog->get_button(2)->set_flags(GTK_CAN_FOCUS);
   button37->set_sensitive(false);
   toolbar17->set_space_size(5);
   toolbar17->set_tooltips(true);
   toolbar17->set_space_style(GTK_TOOLBAR_SPACE_EMPTY);
   text->set_alignment(0.5,0.5);
   text->set_padding(0,0);
   text->set_justify(GTK_JUSTIFY_CENTER);
   text->set_line_wrap(false);
   hbox34->pack_start(*toolbar17, false, false, 0);
   hbox34->pack_start(*text, false, false, 0);
   buchen_dialog->get_vbox()->set_homogeneous(false);
   buchen_dialog->get_vbox()->set_spacing(8);
   buchen_dialog->get_vbox()->pack_start(*hbox34, false, false, 0);
   buchen_dialog->set_modal(true);
   buchen_dialog->set_position(GTK_WIN_POS_CENTER);
   buchen_dialog->set_policy(false, false, false);
   buchen_dialog->add_accel_group(*(gmm_data->getAccelGroup()));
   
   Gtk::HButtonBox *dialog_action_area5 = Gtk::wrap((GtkHButtonBox*)GNOME_DIALOG(buchen_dialog->gtkobj())->action_area);
   dialog_action_area5->set_spacing(8);
   dialog_action_area5->set_spacing(8);
   buchen_dialog->close_hides(false);
   toolbar17->show();
   text->show();
   hbox34->show();
   buchen_dialog->show();
}

buchen_dialog_glade::~buchen_dialog_glade()
{  delete gmm_data;
}
