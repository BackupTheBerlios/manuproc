// generated 2003/3/17 17:16:49 CET by christof@puck.petig-baender.de
// using glademm V1.1.3e_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- Komponenten.glade
// for gtk 2.2.1 and gtkmm 2.2.0
//
// Please modify the corresponding derived classes in ././IntStringBox.cc

#include "config.h"
#include "IntStringBox_glade.hh"
#include <gtkmm/button.h>
#include <gtkmm/table.h>

IntStringBox_glade::IntStringBox_glade(GlademmData *_data
)
{  
   
   Gtk::EventBox *IntStringBox = this;
   label_int = Gtk::manage(new class Gtk::Label("Int"));
   label_string = Gtk::manage(new class Gtk::Label("Name1"));
   label_string2 = Gtk::manage(new class Gtk::Label("Name2"));
   sc_int = new class SearchCombo();
   sc1_string = new class SearchCombo();
   sc2_string = new class SearchCombo();
   
   Gtk::Button *button3 = Gtk::manage(new class Gtk::Button("leeren"));
   Gtk::Table *table = Gtk::manage(new class Gtk::Table(2, 2, false));
   label_int->set_alignment(0,0.5);
   label_int->set_padding(0,0);
   label_int->set_justify(Gtk::JUSTIFY_CENTER);
   label_int->set_line_wrap(false);
   label_int->set_use_markup(false);
   label_string->set_alignment(0,0.5);
   label_string->set_padding(0,0);
   label_string->set_justify(Gtk::JUSTIFY_CENTER);
   label_string->set_line_wrap(false);
   label_string->set_use_markup(false);
   label_string2->set_alignment(0,0.5);
   label_string2->set_padding(0,0);
   label_string2->set_justify(Gtk::JUSTIFY_CENTER);
   label_string2->set_line_wrap(false);
   label_string2->set_use_markup(false);
   button3->set_flags(Gtk::CAN_FOCUS);
   button3->set_relief(Gtk::RELIEF_NORMAL);
   table->set_row_spacings(0);
   table->set_col_spacings(0);
   table->attach(*label_int, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table->attach(*label_string, 2, 3, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table->attach(*label_string2, 3, 4, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table->attach(*sc_int, 1, 2, 1, 2, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table->attach(*sc1_string, 2, 3, 1, 2, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table->attach(*sc2_string, 3, 4, 1, 2, Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table->attach(*button3, 0, 1, 1, 2, Gtk::AttachOptions(), Gtk::AttachOptions(), 0, 0);
   IntStringBox->set_events(Gdk::BUTTON_PRESS_MASK);
   IntStringBox->add(*table);
   label_int->show();
   label_string->show();
   label_string2->show();
   sc_int->show();
   sc1_string->show();
   sc2_string->show();
   button3->show();
   table->show();
   IntStringBox->show();
   sc_int->signal_activate().connect(SigC::slot(*this, &IntStringBox_glade::int_activate));
   sc_int->signal_search().connect(SigC::slot(*this, &IntStringBox_glade::int_search));
   sc1_string->signal_activate().connect(SigC::slot(*this, &IntStringBox_glade::string1_activate));
   sc1_string->signal_search().connect(SigC::slot(*this, &IntStringBox_glade::string1_search));
   sc2_string->signal_activate().connect(SigC::slot(*this, &IntStringBox_glade::string2_activate));
   sc2_string->signal_search().connect(SigC::slot(*this, &IntStringBox_glade::string2_search));
   button3->signal_clicked().connect(SigC::slot(*this, &IntStringBox_glade::reset));
   table->signal_button_press_event().connect(SigC::slot(*this, &IntStringBox_glade::MouseButton));
}

IntStringBox_glade::~IntStringBox_glade()
{  delete sc_int;
   delete sc1_string;
   delete sc2_string;
}
