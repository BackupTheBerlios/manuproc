// generated 2004/5/17 17:14:57 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0.2
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/cvs/manuproc/ManuProC_Agent/mpc_agent.glade
// for gtk 2.2.4 and gtkmm 2.2.8
//
// Please modify the corresponding derived classes in ./src/mpc_export.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include "mpc_export_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/alignment.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>

mpc_export_glade::mpc_export_glade(
)
{  
   
   Gtk::Dialog *mpc_export = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Image *image5 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-close"), Gtk::IconSize(4)));
   Gtk::Label *label23 = Gtk::manage(new class Gtk::Label("Close"));
   Gtk::HBox *hbox15 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment5 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   Gtk::Button *okbutton2 = Gtk::manage(new class Gtk::Button());
   Gtk::Label *label24 = Gtk::manage(new class Gtk::Label("File"));
   export_file_name = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Button *file_select = Gtk::manage(new class Gtk::Button("...\n"
		""));
   Gtk::HBox *hbox18 = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::Image *image6 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-execute"), Gtk::IconSize(4)));
   Gtk::Label *label25 = Gtk::manage(new class Gtk::Label("Export"));
   Gtk::HBox *hbox19 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment6 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   Gtk::Button *do_file_export = Gtk::manage(new class Gtk::Button());
   Gtk::HBox *hbox16 = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::VBox *vbox5 = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::Label *label21 = Gtk::manage(new class Gtk::Label("into a file"));
   Gtk::Frame *frame11 = Gtk::manage(new class Gtk::Frame());
   Gtk::Label *label32 = Gtk::manage(new class Gtk::Label("To"));
   Gtk::Entry *entry2 = Gtk::manage(new class Gtk::Entry());
   Gtk::HBox *hbox28 = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::Image *image7 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-execute"), Gtk::IconSize(4)));
   Gtk::Label *label33 = Gtk::manage(new class Gtk::Label("Send"));
   Gtk::HBox *hbox30 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment7 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   Gtk::Button *do_file_send = Gtk::manage(new class Gtk::Button());
   Gtk::HBox *hbox29 = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::VBox *vbox6 = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::Label *label31 = Gtk::manage(new class Gtk::Label("send per email"));
   Gtk::Frame *frame12 = Gtk::manage(new class Gtk::Frame());
   Gtk::VBox *vbox4 = Gtk::manage(new class Gtk::VBox(false, 0));
   image5->set_alignment(0.5,0.5);
   image5->set_padding(0,0);
   label23->set_alignment(0.5,0.5);
   label23->set_padding(0,0);
   label23->set_justify(Gtk::JUSTIFY_LEFT);
   label23->set_line_wrap(false);
   label23->set_use_markup(false);
   label23->set_selectable(false);
   hbox15->pack_start(*image5, Gtk::PACK_SHRINK, 0);
   hbox15->pack_start(*label23, Gtk::PACK_SHRINK, 0);
   alignment5->add(*hbox15);
   okbutton2->set_flags(Gtk::CAN_FOCUS);
   okbutton2->set_relief(Gtk::RELIEF_NORMAL);
   okbutton2->add(*alignment5);
   mpc_export->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   label24->set_alignment(0,0.5);
   label24->set_padding(5,0);
   label24->set_justify(Gtk::JUSTIFY_LEFT);
   label24->set_line_wrap(false);
   label24->set_use_markup(false);
   label24->set_selectable(false);
   export_file_name->set_flags(Gtk::CAN_FOCUS);
   export_file_name->set_visibility(true);
   export_file_name->set_editable(true);
   export_file_name->set_max_length(0);
   export_file_name->set_text("");
   export_file_name->set_has_frame(true);
   export_file_name->set_activates_default(false);
   file_select->set_flags(Gtk::CAN_FOCUS);
   file_select->set_relief(Gtk::RELIEF_NORMAL);
   hbox18->pack_start(*label24, Gtk::PACK_SHRINK, 0);
   hbox18->pack_start(*export_file_name);
   hbox18->pack_start(*file_select, Gtk::PACK_SHRINK, 0);
   image6->set_alignment(0.5,0.5);
   image6->set_padding(0,0);
   label25->set_alignment(0.5,0.5);
   label25->set_padding(0,0);
   label25->set_justify(Gtk::JUSTIFY_LEFT);
   label25->set_line_wrap(false);
   label25->set_use_markup(false);
   label25->set_selectable(false);
   hbox19->pack_start(*image6, Gtk::PACK_SHRINK, 0);
   hbox19->pack_start(*label25, Gtk::PACK_SHRINK, 0);
   alignment6->add(*hbox19);
   do_file_export->set_flags(Gtk::CAN_FOCUS);
   do_file_export->set_relief(Gtk::RELIEF_NORMAL);
   do_file_export->add(*alignment6);
   hbox16->pack_start(*do_file_export, Gtk::PACK_SHRINK, 0);
   vbox5->set_border_width(5);
   vbox5->pack_start(*hbox18);
   vbox5->pack_start(*hbox16, Gtk::PACK_SHRINK, 5);
   label21->set_alignment(0.5,0.5);
   label21->set_padding(0,0);
   label21->set_justify(Gtk::JUSTIFY_LEFT);
   label21->set_line_wrap(false);
   label21->set_use_markup(false);
   label21->set_selectable(false);
   frame11->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
   frame11->set_label_align(0,0.5);
   frame11->add(*vbox5);
   frame11->set_label_widget(*label21);
   label32->set_alignment(0.5,0.5);
   label32->set_padding(5,0);
   label32->set_justify(Gtk::JUSTIFY_LEFT);
   label32->set_line_wrap(false);
   label32->set_use_markup(false);
   label32->set_selectable(false);
   entry2->set_flags(Gtk::CAN_FOCUS);
   entry2->set_visibility(true);
   entry2->set_editable(true);
   entry2->set_max_length(0);
   entry2->set_text("");
   entry2->set_has_frame(true);
   entry2->set_activates_default(false);
   hbox28->pack_start(*label32, Gtk::PACK_SHRINK, 0);
   hbox28->pack_start(*entry2);
   image7->set_alignment(0.5,0.5);
   image7->set_padding(0,0);
   label33->set_alignment(0.5,0.5);
   label33->set_padding(0,0);
   label33->set_justify(Gtk::JUSTIFY_LEFT);
   label33->set_line_wrap(false);
   label33->set_use_markup(false);
   label33->set_selectable(false);
   hbox30->pack_start(*image7, Gtk::PACK_SHRINK, 0);
   hbox30->pack_start(*label33, Gtk::PACK_SHRINK, 0);
   alignment7->add(*hbox30);
   do_file_send->set_flags(Gtk::CAN_FOCUS);
   do_file_send->set_relief(Gtk::RELIEF_NORMAL);
   do_file_send->add(*alignment7);
   hbox29->pack_start(*do_file_send, Gtk::PACK_SHRINK, 0);
   vbox6->set_border_width(5);
   vbox6->pack_start(*hbox28);
   vbox6->pack_start(*hbox29, Gtk::PACK_EXPAND_WIDGET, 5);
   label31->set_alignment(0.5,0.5);
   label31->set_padding(0,0);
   label31->set_justify(Gtk::JUSTIFY_LEFT);
   label31->set_line_wrap(false);
   label31->set_use_markup(false);
   label31->set_selectable(false);
   frame12->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
   frame12->set_label_align(0,0.5);
   frame12->add(*vbox6);
   frame12->set_label_widget(*label31);
   vbox4->pack_start(*frame11, Gtk::PACK_SHRINK, 0);
   vbox4->pack_start(*frame12, Gtk::PACK_SHRINK, 5);
   mpc_export->get_vbox()->set_homogeneous(false);
   mpc_export->get_vbox()->set_spacing(0);
   mpc_export->get_vbox()->pack_start(*vbox4);
   mpc_export->set_title("Export");
   mpc_export->set_default_size(400,300);
   mpc_export->set_modal(true);
   mpc_export->property_window_position().set_value(Gtk::WIN_POS_CENTER);
   mpc_export->set_resizable(true);
   mpc_export->property_destroy_with_parent().set_value(true);
   mpc_export->set_has_separator(true);
   mpc_export->add_action_widget(*okbutton2, -7);
   image5->show();
   label23->show();
   hbox15->show();
   alignment5->show();
   okbutton2->show();
   label24->show();
   export_file_name->show();
   file_select->show();
   hbox18->show();
   image6->show();
   label25->show();
   hbox19->show();
   alignment6->show();
   do_file_export->show();
   hbox16->show();
   vbox5->show();
   label21->show();
   frame11->show();
   label32->show();
   entry2->show();
   hbox28->show();
   image7->show();
   label33->show();
   hbox30->show();
   alignment7->show();
   do_file_send->show();
   hbox29->show();
   vbox6->show();
   label31->show();
   frame12->show();
   vbox4->show();
   mpc_export->show();
   file_select->signal_clicked().connect(SigC::slot(*this, &mpc_export_glade::on_file_select_clicked));
   do_file_export->signal_clicked().connect(SigC::slot(*this, &mpc_export_glade::on_do_file_export_clicked));
   do_file_send->signal_clicked().connect(SigC::slot(*this, &mpc_export_glade::on_do_file_send_clicked));
}

mpc_export_glade::~mpc_export_glade()
{  delete gmm_data;
}
