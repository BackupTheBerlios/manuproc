// generated 2004/9/13 10:59:17 CEST by christof@puck.petig-baender.de
// using glademm V2.6.0_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- --noaccelerators Komponenten.glade
// for gtk 2.4.9 and gtkmm 2.4.5
//
// Please modify the corresponding derived classes in ././datewin.cc


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
#include "datewin_glade.hh"
#include <gtkmm/adjustment.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/handlebox.h>
#include <gtkmm/button.h>

datewin_glade::datewin_glade(
)
{  datewin = this;
   
   Gtk::Adjustment *tag_adj = Gtk::manage(new class Gtk::Adjustment(1, 1, 31, 1, 7, 7));
   tag = Gtk::manage(new class Gtk::SpinButton(*tag_adj, 1, 0));
   
   Gtk::Label *label2 = Gtk::manage(new class Gtk::Label("."));
   Gtk::Adjustment *monat_adj = Gtk::manage(new class Gtk::Adjustment(1, 1, 12, 1, 3, 3));
   monat = Gtk::manage(new class Gtk::SpinButton(*monat_adj, 1, 0));
   
   Gtk::Label *label3 = Gtk::manage(new class Gtk::Label("."));
   Gtk::Adjustment *jahr_adj = Gtk::manage(new class Gtk::Adjustment(2002, 1800, 2100, 1, 10, 10));
   jahr = Gtk::manage(new class Gtk::SpinButton(*jahr_adj, 1, 0));
   
   Gtk::HBox *hbox9 = Gtk::manage(new class Gtk::HBox(false, 1));
   datum_label = Gtk::manage(new class Gtk::Label("Datum"));
   
   Gtk::Adjustment *kw_spinbutton_adj = Gtk::manage(new class Gtk::Adjustment(1, 1, 56, 1, 4, 4));
   kw_spinbutton = Gtk::manage(new class Gtk::SpinButton(*kw_spinbutton_adj, 1, 0));
   
   Gtk::Label *label7 = Gtk::manage(new class Gtk::Label("'"));
   Gtk::Adjustment *jahr_spinbutton_adj = Gtk::manage(new class Gtk::Adjustment(2002, 1800, 2099, 1, 10, 10));
   jahr_spinbutton = Gtk::manage(new class Gtk::SpinButton(*jahr_spinbutton_adj, 1, 0));
   
   Gtk::HBox *hbox1 = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::Label *label4 = Gtk::manage(new class Gtk::Label("Woche"));
   calendar1 = Gtk::manage(new class Gtk::Calendar());
   
   Gtk::HandleBox *handlebox1 = Gtk::manage(new class Gtk::HandleBox());
   Gtk::Label *label5 = Gtk::manage(new class Gtk::Label("Kalender"));
   Gtk::Button *button1 = Gtk::manage(new class Gtk::Button("Datum setzen"));
   Gtk::Label *label6 = Gtk::manage(new class Gtk::Label("leer"));
   tag->set_flags(Gtk::CAN_FOCUS);
   tag->set_update_policy(Gtk::UPDATE_ALWAYS);
   tag->set_numeric(true);
   tag->set_digits(0);
   tag->set_wrap(true);
   label2->set_alignment(0.5,0.5);
   label2->set_padding(0,0);
   label2->set_justify(Gtk::JUSTIFY_CENTER);
   label2->set_line_wrap(false);
   label2->set_use_markup(false);
   label2->set_selectable(false);
   monat->set_flags(Gtk::CAN_FOCUS);
   monat->set_update_policy(Gtk::UPDATE_ALWAYS);
   monat->set_numeric(true);
   monat->set_digits(0);
   monat->set_wrap(true);
   label3->set_alignment(0.5,0.5);
   label3->set_padding(0,0);
   label3->set_justify(Gtk::JUSTIFY_CENTER);
   label3->set_line_wrap(false);
   label3->set_use_markup(false);
   label3->set_selectable(false);
   jahr->set_flags(Gtk::CAN_FOCUS);
   jahr->set_update_policy(Gtk::UPDATE_ALWAYS);
   jahr->set_numeric(true);
   jahr->set_digits(0);
   jahr->set_wrap(false);
   hbox9->set_border_width(1);
   hbox9->pack_start(*tag, Gtk::PACK_SHRINK, 0);
   hbox9->pack_start(*label2, Gtk::PACK_SHRINK, 0);
   hbox9->pack_start(*monat, Gtk::PACK_SHRINK, 0);
   hbox9->pack_start(*label3, Gtk::PACK_SHRINK, 0);
   hbox9->pack_start(*jahr, Gtk::PACK_SHRINK, 0);
   datum_label->set_alignment(0.5,0.5);
   datum_label->set_padding(0,0);
   datum_label->set_justify(Gtk::JUSTIFY_CENTER);
   datum_label->set_line_wrap(false);
   datum_label->set_use_markup(false);
   datum_label->set_selectable(false);
   kw_spinbutton->set_flags(Gtk::CAN_FOCUS);
   kw_spinbutton->set_update_policy(Gtk::UPDATE_ALWAYS);
   kw_spinbutton->set_numeric(false);
   kw_spinbutton->set_digits(0);
   kw_spinbutton->set_wrap(false);
   label7->set_alignment(0.5,0.5);
   label7->set_padding(0,0);
   label7->set_justify(Gtk::JUSTIFY_CENTER);
   label7->set_line_wrap(false);
   label7->set_use_markup(false);
   label7->set_selectable(false);
   jahr_spinbutton->set_flags(Gtk::CAN_FOCUS);
   jahr_spinbutton->set_update_policy(Gtk::UPDATE_ALWAYS);
   jahr_spinbutton->set_numeric(false);
   jahr_spinbutton->set_digits(0);
   jahr_spinbutton->set_wrap(false);
   hbox1->pack_start(*kw_spinbutton);
   hbox1->pack_start(*label7, Gtk::PACK_SHRINK, 0);
   hbox1->pack_start(*jahr_spinbutton);
   label4->set_alignment(0.5,0.5);
   label4->set_padding(0,0);
   label4->set_justify(Gtk::JUSTIFY_CENTER);
   label4->set_line_wrap(false);
   label4->set_use_markup(false);
   label4->set_selectable(false);
   calendar1->set_flags(Gtk::CAN_FOCUS);
   calendar1->display_options(Gtk::CALENDAR_SHOW_HEADING|Gtk::CALENDAR_SHOW_DAY_NAMES);
   handlebox1->set_shadow_type(Gtk::SHADOW_OUT);
   handlebox1->set_handle_position(Gtk::POS_RIGHT);
   handlebox1->set_snap_edge(Gtk::POS_TOP);
   handlebox1->add(*calendar1);
   label5->set_alignment(0.5,0.5);
   label5->set_padding(0,0);
   label5->set_justify(Gtk::JUSTIFY_CENTER);
   label5->set_line_wrap(false);
   label5->set_use_markup(false);
   label5->set_selectable(false);
   button1->set_flags(Gtk::CAN_FOCUS);
   button1->set_relief(Gtk::RELIEF_NORMAL);
   label6->set_alignment(0.5,0.5);
   label6->set_padding(0,0);
   label6->set_justify(Gtk::JUSTIFY_CENTER);
   label6->set_line_wrap(false);
   label6->set_use_markup(false);
   label6->set_selectable(false);
   datewin->set_flags(Gtk::CAN_FOCUS);
   datewin->set_show_tabs(true);
   datewin->set_show_border(false);
   datewin->set_tab_pos(Gtk::POS_LEFT);
   datewin->set_scrollable(true);
   datewin->append_page(*hbox9, *datum_label);
   datewin->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   datewin->append_page(*hbox1, *label4);
   datewin->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   datewin->append_page(*handlebox1, *label5);
   datewin->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   datewin->append_page(*button1, *label6);
   datewin->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   tag->show();
   label2->show();
   monat->show();
   label3->show();
   jahr->show();
   hbox9->show();
   datum_label->show();
   kw_spinbutton->show();
   label7->show();
   jahr_spinbutton->show();
   hbox1->show();
   label4->show();
   label5->show();
   button1->show();
   label6->show();
   datewin->show();
   tag->signal_activate().connect(SigC::slot(*this, &datewin_glade::on_activate), true);
   monat->signal_activate().connect(SigC::slot(*this, &datewin_glade::on_activate), true);
   jahr->signal_activate().connect(SigC::slot(*this, &datewin_glade::datum_activate), true);
   kw_spinbutton->signal_activate().connect(SigC::slot(*jahr_spinbutton, &Gtk::Widget::grab_focus), true);
   jahr_spinbutton->signal_activate().connect(SigC::slot(*this, &datewin_glade::kw_activate), true);
   calendar1->signal_day_selected().connect(SigC::slot(*this, &datewin_glade::on_day_selected), false);
   button1->signal_clicked().connect(SigC::slot(*this, &datewin_glade::datum_setzen), false);
   datewin->signal_switch_page().connect(SigC::slot(*this, &datewin_glade::on_datewin_switch_page), true);
}

datewin_glade::~datewin_glade()
{  
}
