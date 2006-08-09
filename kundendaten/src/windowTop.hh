// $Id$

#ifndef _WINDOWTOP_HH
#  include "windowTop_glade.hh"
#  define _WINDOWTOP_HH
//#include "glademm_support.hh"
#include<Aux/fixedpoint.h>
#include<Kunde/Kunde.h>
#include<Kunde/Telefon.h>
#include <Kunde/TelefonArt.h>
#include<vector>
#include <gtkmm/notebook.h>
#include <gtkmm/liststore.h>
#include "kundendaten_aux.hh"
#include <Association.h>
#include <BaseObjects/Model.h>

class windowTop : public windowTop_glade
{   
 // Models for components in Kunde
 Model<fixedpoint<2> > rabatt;

 // Model callbacks
 void Model_Refresh(void *x);

       H_Kunde kundendaten;

       std::vector<Kunde::st_ansprech> AnsprechPersonen;
       cH_Kunde person;
       
       struct KontaktColumns : Gtk::TreeModelColumnRecord
       { Gtk::TreeModelColumn<Glib::ustring> name,name2,position;
         Gtk::TreeModelColumn<std::vector<Kunde::st_ansprech>::iterator> ansprech;
         KontaktColumns();
       };
       KontaktColumns kontakt_cols;
       Glib::RefPtr<Gtk::ListStore> KontaktStore;

        Transaction transaction; // Für die SearchCombos
        friend class windowTop_glade;
        enum enum_notebook{PAGE_KUNDE,PAGE_DETAILS,PAGE_ZAHLUNG,
			PAGE_KONTAKTPERSON,
			PAGE_NOTIZEN, PAGE_GRUPPEN};
        enum enum_zahl_verfahren{PAGE_DTAUS,PAGE_RIBA,PAGE_LCR};
        void on_notebook1_switch_page(GtkNotebookPage* page,guint pagenr);
        void on_gruppenwahl_activate();

        Kunde::UpdateBitsAdresse UpdateAdresse;
        Kunde::UpdateBitsFirma UpdateFirma;
        Kunde::UpdateBitsBank UpdateBank;
        Kunde::UpdateBitsSonst UpdateSonst;
//        Person::UpdateBits UpdatePerson;

//        list<PreisListe::ID> sonder_preis_liste;

        void saveAll();
        void saveKundenKontakt();

        void connectFkt();
        void changedFktA(Kunde::UpdateBitsAdresse e) ;
        void changedFktF(Kunde::UpdateBitsFirma e);
        void changedFktB(Kunde::UpdateBitsBank e);
        void changedFktS(Kunde::UpdateBitsSonst e);
        void activateFktA(Kunde::UpdateBitsAdresse e);
        void activateFktF(Kunde::UpdateBitsFirma e);
        void activateFktB(Kunde::UpdateBitsBank e);
        void activateFktS(Kunde::UpdateBitsSonst e);
        gint focus_outFktA(GdkEventFocus *ev,Kunde::UpdateBitsAdresse e);
        gint focus_outFktF(GdkEventFocus *ev,Kunde::UpdateBitsFirma e);
        gint focus_outFktB(GdkEventFocus *ev,Kunde::UpdateBitsBank e);
        gint focus_outFktS(GdkEventFocus *ev,Kunde::UpdateBitsSonst e);
//        void changedFktP(Person::UpdateBits e);
//        void activateFktP(Person::UpdateBits e);
//        gint focus_outFktP(GdkEventFocus *ev,Person::UpdateBits e);

        void on_landesbox_activate();
        void on_entry_blz_activate();
        void on_load_activate();
	bool on_delete_event(GdkEventAny*);
        void on_buttonBeenden_clicked();
        void on_buttonAuswaehlen_clicked();
//        void on_buttonSpeichern_clicked();
        void on_buttonNeu_clicked();
        void on_button_kunde_loeschen_clicked();
        void on_KundenTelefon_activate(cH_Telefon t);
        void get_entrys();
//        void save_kunde();
        void save_kunde_anlegen();
        void show_kundendaten();
        void Info(std::string text);
        void clear_entrys();
        void on_gruppe_in_clicked();
        void on_gruppe_out_clicked();
        void on_entryPersonenPosition_activate();

     void kunden_activate();
     void on_kunden_reset();
     void on_clear_button_clicked();
     void on_ab_an_rngadresse_toggled();

     const TelArt getKTelArt();

        

      ///////////////////////////////////////////////
      // Ab hier folgen die Details
      bool fire_enabled;

      void setTitles();
      void fillSPreis();
      void on_spreis_leaf_selected(cH_RowDataBase d);
      void on_spreis_unselect_row();
        void on_preisautomatik_clicked();
      void on_button_spreis_add_clicked();
      void on_SPreisListe_activate();
      void saveSonderpreisliste();
//      void on_button_neu_spl_nummer_clicked();
//      void on_button_neue_spl_anlegen_clicked();
        void scc_verkaeufer_activate();
        void on_betreuer_activate();
	void scc_verkaeufer_reset();
        void scc_anrede_search(gboolean *_continue, GtkSCContext newsearch);
        void on_checkbuttonBankeinzug_toggled();
        int fill_bank_bei(unsigned long int blz);
        void on_button_neue_bank_clicked();

	bool on_acc_entry_changed(GdkEventFocus *e);
        bool on_abi_entry_changed(GdkEventFocus *e);
        bool on_cab_entry_changed(GdkEventFocus *e);
        bool on_iban_entry_changed(GdkEventFocus *e);

        void on_riba_save_clicked();
        void on_riba_abbruch_clicked();
        void on_iban_save_clicked();
        void on_iban_abbruch_clicked();
        void on_zahlverfahren_book_switch_page(GtkNotebookPage *p0, guint p1);
        void extartbez_activate();
        void preisliste_activate();
        void rng_an_activate();
        void on_checkbuttonRchngAdr_toggled();
        void on_checkbuttonLieferAdr_toggled();
        void on_checkbutton_rng_an_postfach_toggled();
        void on_checkbutton_entsorgung_toggled();
        void show_details();
        void show_zahlungsziel();
        void on_button_del_preisliste_clicked();
        void on_kunden_status_toggled();
        
        void on_KundenTelefon_remove(cH_Telefon t) {}
        void on_KontaktPersonTelefon_remove(cH_Telefon t) {}
        void on_PersonenPrivatTelefon_remove(cH_Telefon t) {}
        void clear_update_bits();

        void on_aufnotiz_save_clicked();
        void on_aufnotiz_changed();
        void on_liefnotiz_save_clicked();
        void on_liefnotiz_changed();
        void on_rngnotiz_save_clicked();
        void on_rngnotiz_changed();

// war public:

        // Kontaktpersonen
        void on_kontakt_personen_box_activate();
        void on_KontaktPersonTelefon_activate(cH_Telefon t);
        void show_kontaktpersonen();
        void on_button_kontakt_loeschen_clicked();

        //Personen
        void on_clistPersonenListe_selection_changed();
        bool get_selected_person(Kunde::st_ansprech& P);
        void on_buttonPersonNeu_clicked();
        void show_privatpersonen();
        void on_buttonPersonLoeschen_clicked();
        void on_PersonenPrivatTelefon_activate(cH_Telefon t);
        void zeige_notiz(Kunde::st_ansprech &P);
        void on_personen_telefon_deactivate();
        void on_personen_box_activate();
        void update_person();
        void clear_PersonenEntrys();

	void load_notizen();

        void on_lfran_activate();
        void on_selectrow_allegruppen(cH_RowDataBase leaf);
        void on_unselectrow_allegruppen();
        void on_selectrow_gewaehltegruppen(cH_RowDataBase leaf);
        void on_unselectrow_gewaehltegruppen();
        
        void gruppen_bearbeiten();
public:

        windowTop(unsigned kid);

        void kunde_loeschen();
	void showGruppen();

        // Details
        void neue_bank_uebernehmen(unsigned long int bank_index);      

        void neue_kundennmmer(unsigned kid);
        void laden(cH_Kunde const& k);

private:
	Data_Gruppe *allgrp;
	Data_Gruppe *wahlgrp;
public:
	int bankid;
};
#endif

