// generated 2001/2/22 9:59:33 CET by thoma@ig23.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to windowPersonen.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOWPERSONEN_HH
#  include "windowPersonen_glade.hh"
#  define _WINDOWPERSONEN_HH
#include "glademm_support.hh"
#include "windowTop.hh"

#include<Aux/Datum.h>
#include<Kunde/Person.h>
#include<Kunde/Telefon.h>

class windowPersonen : public windowPersonen_glade
{   
        vector<cH_Telefon> telefon;
        vector<cH_Person> person;
	cH_Kunde kundendaten;

	static int notiz_pos;

        friend class windowPersonen_glade;
        void on_clistPersonenListe_select_row(gint row, gint column, GdkEvent *event);
        void on_clistPersonenListe_unselect_row(gint row, gint column, GdkEvent *event);
        void on_buttonPersonNeu_clicked();
        void on_buttonPersonBearbeiten_clicked();
        void on_buttonPersonLoeschen_clicked();
        void on_buttonPersonenNumUebernehmen_clicked();
        void on_buttonPersonenNumLoeschen_clicked();
        void on_radiobuttonPersonenTelefon_clicked();
        void on_entryPersonenTelefonEintrag_activate();
        void on_radiobuttonPersonenFax_clicked();
        void on_radiobuttonPersonenMobil_clicked();
        void on_checkbuttonPersonenPrivat_clicked();
        void on_buttonPersonenUebernehmen_clicked();
        void on_buttonPersonenAbbrechen_clicked();
        void on_windowPersonen_destroy();
        void show_telefon();
        void load_personen(const Kunde::ID kid);
	void load_telefon_list(const Person::ID pid);
        void Info(string text);
        void save_personen();
	const Telefon::TelArt getTelArt();
   public:
//        windowPersonen(const Person::ID pid, const Kunde::ID kid=Kunde::none_id);
      windowPersonen(const Kunde::ID kid);
        void show_personen();
        void zeige_notiz(string text);
};
#endif
