// generated 2001/5/30 14:23:46 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/jacek/pps/auftrag.glade
// for gtk 1.2.8 and gtkmm 1.2.5
//
// Please modify the corresponding derived classes in ./src/auftragliste.cc

#include "config.h"
#include "auftragliste.hh"
#include <gtk--/label.h>
#include <gtk--/table.h>
#include <gtk--/button.h>
#include <gtk--/box.h>

auftragliste_glade::auftragliste_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{   auftragliste = this;
   
   Gtk::Label *label3 = manage(new class Gtk::Label("Auftragsstatus"));
   aufstatcombo = new class SearchComboContent<int>();
   
   Gtk::Table *table2 = manage(new class Gtk::Table(1, 2, false));
   selauftraglist = manage(new class Gtk::CList(4));
   scrolledwindow1 = manage(new class Gtk::ScrolledWindow());
   
   Gtk::Button *closebutton = manage(new class Gtk::Button("Schliessen"));
   Gtk::Table *table3 = manage(new class Gtk::Table(1, 1, false));
   Gtk::VBox *vbox1 = manage(new class Gtk::VBox(false, 0));
   label3->set_alignment(0.5, 0.5);
   label3->set_padding(0, 0);
   table2->set_border_width(5);
   table2->attach(*label3, 0, 1, 0, 1, 0, 0, 0, 0);
   table2->attach(*aufstatcombo, 1, 2, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   selauftraglist->set_column_title(0, "Kunde");
   selauftraglist->set_column_justification(0, GTK_JUSTIFY_CENTER);
   selauftraglist->set_column_width(0, 80);
   selauftraglist->set_column_title(1, "Auftragsnr.");
   selauftraglist->set_column_justification(1, GTK_JUSTIFY_CENTER);
   selauftraglist->set_column_width(1, 80);
   selauftraglist->set_column_title(2, "Auftr.Datum");
   selauftraglist->set_column_justification(2, GTK_JUSTIFY_CENTER);
   selauftraglist->set_column_width(2, 80);
   selauftraglist->set_column_title(3, "Kd.Auftr.Nr.");
   selauftraglist->set_column_justification(3, GTK_JUSTIFY_CENTER);
   selauftraglist->set_column_width(3, 80);
   selauftraglist->column_titles_show();
   scrolledwindow1->set_border_width(5);
   scrolledwindow1->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow1->add(*selauftraglist);
   closebutton->set_border_width(5);
   table3->attach(*closebutton, 0, 1, 0, 1, 0, 0, 0, 0);
   vbox1->pack_start(*table2, false, true, 0);
   vbox1->pack_start(*scrolledwindow1);
   vbox1->pack_start(*table3, false, true, 0);
   auftragliste->set_usize(400, 600);
   auftragliste->set_title("Auftragsauswahl");
   auftragliste->add(*vbox1);
   label3->show();
   aufstatcombo->show();
   table2->show();
   selauftraglist->show();
   scrolledwindow1->show();
   closebutton->show();
   table3->show();
   vbox1->show();
   auftragliste->show();
   selauftraglist->select_row.connect(SigC::slot(static_cast<class auftragliste*>(this), &auftragliste::on_selauftraglist_select_row));
   selauftraglist->click_column.connect(SigC::slot(static_cast<class auftragliste*>(this), &auftragliste::on_selauftraglist_click_column));
   closebutton->clicked.connect(SigC::slot(static_cast<class auftragliste*>(this), &auftragliste::on_closebutton_clicked));
}

auftragliste_glade::~auftragliste_glade()
{   delete aufstatcombo;
}
