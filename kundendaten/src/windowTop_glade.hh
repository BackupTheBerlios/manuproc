// generated 2005/6/22 13:15:51 CEST by jacek@jaceksdell.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/cvs/kundendaten/kundendaten.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/windowTop.hh and./src/windowTop.cc

#ifndef _WINDOWTOP_GLADE_HH
#  define _WINDOWTOP_GLADE_HH

#include <gtk--/window.h>
#include <gtk--/tooltips.h>
#include <gtk--/button.h>
#include "Optionmenu_Kundengruppe.hh"
#include "WTelefon.hh"
#include <gtk--/frame.h>
#include <gtk--/entry.h>
#include "LandesBox.hh"
#include "SearchComboContent.hh"
#include <gtk--/checkbutton.h>
#include <gtk--/label.h>
#include <gtk--/spinbutton.h>
#include <gtk--/text.h>
#include "Datewin.hh"
#include "PersonenBox.hh"
#include "KundenBox2.hh"
#include <gtk--/table.h>
#include "WWaehrung.hh"
#include "ZahlungsArtBox.hh"
#include <gtk--/notebook.h>
#include "PreisBox.hh"
#include "SimpleTree.hh"
#include <gtk--/clist.h>

class windowTop_glade : public Gtk::Window
{  
        
        Gtk::Tooltips _tooltips;
protected:
        class Gtk::Button * new_entry_button;
        class Gtk::Button * clear_button;
        class Optionmenu_Kundengruppe * gruppenwahl;
        class WTelefon * KundenTelefon;
        class Gtk::Frame * frame_kundenkontakt;
        class Gtk::Entry * entryFirma;
        class Gtk::Entry * entryPostanwVor;
        class Gtk::Entry * entryPostanwNach;
        class Gtk::Entry * entryHausNr;
        class Gtk::Entry * entryStrasse;
        class Gtk::Entry * entryOrt;
        class Gtk::Entry * entryPLZ;
        class Gtk::Entry * entryPostfach;
        class Gtk::Entry * entryPostfachPLZ;
        class LandesBox * landesbox;
        class Gtk::Entry * entryName2;
        class SearchComboContent<int> * scc_anrede;
        class Gtk::Entry * entryIdNr;
        class Gtk::Entry * entrySortiername;
        class Gtk::Frame * frame_adresse;
        class Gtk::CheckButton * kunden_status;
        class Gtk::Label * labelDetailStandEintrag;
        class Gtk::SpinButton * spinbutton_Flaeche;
        class Gtk::SpinButton * spinbutton_Mitarbeiter;
        class Gtk::SpinButton * spinbutton_Kundenumsatz;
        class Gtk::SpinButton * spinbutton_Planumsatz;
        class Gtk::SpinButton * spinbutton_Umsatz;
        class Gtk::Entry * entryVerein;
        class Gtk::Text * textNotiz;
        class Datewin * geburtstag;
        class Gtk::Entry * entry_eigene_kundennr;
        class Gtk::SpinButton * spinbutton_lieferantenkonto;
        class Gtk::SpinButton * spinbutton_debitorenkonto;
        class Gtk::SpinButton * spinbutton_gegenkonto;
        class Gtk::SpinButton * spinbutton_firmenpapier;
        class Gtk::SpinButton * spinbutton_weissespapier;
        class PersonenBox * scc_verkaeufer;
        class PersonenBox * betreuer;
        class KundenBox2 * extartbez;
        class KundenBox2 * rng_an;
        class Gtk::CheckButton * checkbuttonAuftrAdr;
        class Gtk::CheckButton * checkbuttonLieferAdr;
        class Gtk::CheckButton * checkbuttonRchngAdr;
        class Gtk::CheckButton * checkbutton_rng_an_postfach;
        class Gtk::CheckButton * ab_an_rngadresse;
        class KundenBox2 * lfr_an;
        class Gtk::Table * table_details;
        class Gtk::SpinButton * spinbutton_Rabatt;
        class Gtk::CheckButton * checkbutton_zeilenrabatt;
        class Gtk::SpinButton * spinbutton_einzugRabatt;
        class WWaehrung * Waehrung;
        class Gtk::CheckButton * checkbutton_entsorgung;
        class ZahlungsArtBox * zahlungsartbox;
        class Gtk::SpinButton * spinbutton_skontofrist2;
        class Gtk::SpinButton * spinbutton_skontofrist3;
        class Gtk::SpinButton * spinbutton_skontofrist1;
        class Gtk::SpinButton * spinbutton_skontosatz2;
        class Gtk::SpinButton * spinbutton_skontosatz3;
        class Gtk::SpinButton * spinbutton_skontosatz1;
        class Gtk::SpinButton * spinbutton_zahlungsfrist;
        class Gtk::Entry * entryBankKonto;
        class Gtk::Entry * entry_blz;
        class Gtk::Button * button_neue_bank;
        class Gtk::Entry * bankname_entry;
        class Gtk::CheckButton * checkbuttonBankeinzug;
        class Gtk::Entry * abi_entry;
        class Gtk::Entry * cab_entry;
        class Gtk::Button * riba_save;
        class Gtk::Button * riba_abbruch;
        class Gtk::Entry * bankname_italy;
        class Gtk::Entry * acc_entry;
        class Gtk::Entry * iban_entry;
        class Gtk::Button * iban_save;
        class Gtk::Button * iban_abbruch;
        class Gtk::Notebook * zahlverfahren_book;
        class Gtk::CheckButton * checkbutton_lieferung_frei_haus;
        class Gtk::Button * button_del_preisliste;
        class Gtk::Button * button_spreis_add;
        class PreisBox * SPreisBox;
        class SimpleTree * SonderPreislisteTree;
        class Gtk::CheckButton * preisautomatik;
        class Gtk::Frame * frame_preislisten;
        class Gtk::CList * clistPersonenListe;
        class Gtk::Text * textPersonenFirmaNotiz;
        class WTelefon * KontaktPersonenTel;
        class Gtk::Button * button_kontakt_loeschen;
        class Gtk::Entry * entryPersonenPosition;
        class PersonenBox * KontaktPersonenBox;
        class Gtk::Table * table_kontaktperson;
        class Gtk::Button * aufnotiz_save;
        class Gtk::Text * aufnotiz;
        class Gtk::Button * liefnotiz_save;
        class Gtk::Text * liefnotiz;
        class Gtk::Button * rngnotiz_save;
        class Gtk::Text * rngnotiz;
        class SimpleTree * alle_gruppen;
        class Gtk::Button * gruppe_in;
        class Gtk::Button * gruppe_out;
        class SimpleTree * gewaehlte_gruppen;
        class Gtk::Notebook * notebook_main;
        class KundenBox2 * kundenauswahl;
        class Gtk::Label * label_speichern;
        
        windowTop_glade();
        
        ~windowTop_glade();
private:
        virtual void on_buttonBeenden_clicked() = 0;
        virtual void on_buttonNeu_clicked() = 0;
        virtual void on_clear_button_clicked() = 0;
        virtual void on_gruppenwahl_activate() = 0;
        virtual void on_KundenTelefon_activate(cH_Telefon element) = 0;
        virtual void on_KundenTelefon_remove(cH_Telefon element) = 0;
        virtual void scc_anrede_search(gboolean *cont,GtkSCContext context) = 0;
        virtual void on_kunden_status_toggled() = 0;
        virtual void scc_verkaeufer_reset() = 0;
        virtual void scc_verkaeufer_activate() = 0;
        virtual void on_betreuer_activate() = 0;
        virtual void on_ab_an_rngadresse_toggled() = 0;
        virtual void on_lfran_activate() = 0;
        virtual void on_button_neue_bank_clicked() = 0;
        virtual gint on_abi_entry_changed(GdkEventFocus *ev) = 0;
        virtual gint on_cab_entry_changed(GdkEventFocus *ev) = 0;
        virtual void on_riba_save_clicked() = 0;
        virtual void on_riba_abbruch_clicked() = 0;
        virtual gint on_acc_entry_changed(GdkEventFocus *ev) = 0;
        virtual gint on_iban_entry_changed(GdkEventFocus *ev) = 0;
        virtual void on_iban_save_clicked() = 0;
        virtual void on_iban_abbruch_clicked() = 0;
        virtual void on_button_del_preisliste_clicked() = 0;
        virtual void on_button_spreis_add_clicked() = 0;
        virtual void on_SPreisListe_activate() = 0;
        virtual void on_spreis_leaf_selected(cH_RowDataBase leaf) = 0;
        virtual void on_spreis_unselect_row(gint row, gint column, GdkEvent *event) = 0;
        virtual void on_preisautomatik_clicked() = 0;
        virtual void on_clistPersonenListe_select_row(gint row, gint column, GdkEvent *event) = 0;
        virtual void on_clistPersonenListe_unselect_row(gint row, gint column, GdkEvent *event) = 0;
        virtual void on_KontaktPersonTelefon_activate(cH_Telefon element) = 0;
        virtual void on_KontaktPersonTelefon_remove(cH_Telefon element) = 0;
        virtual void on_button_kontakt_loeschen_clicked() = 0;
        virtual void on_entryPersonenPosition_activate() = 0;
        virtual void on_kontakt_personen_box_activate() = 0;
        virtual void on_aufnotiz_save_clicked() = 0;
        virtual void on_aufnotiz_changed() = 0;
        virtual void on_liefnotiz_save_clicked() = 0;
        virtual void on_liefnotiz_changed() = 0;
        virtual void on_rngnotiz_save_clicked() = 0;
        virtual void on_rngnotiz_changed() = 0;
        virtual void on_selectrow_allegruppen(cH_RowDataBase leaf) = 0;
        virtual void on_unselectrow_allegruppen(gint row, gint column, GdkEvent *event) = 0;
        virtual void on_gruppe_in_clicked() = 0;
        virtual void on_gruppe_out_clicked() = 0;
        virtual void on_selectrow_gewaehltegruppen(cH_RowDataBase leaf) = 0;
        virtual void on_unselectrow_gewaehltegruppen(gint row, gint column, GdkEvent *event) = 0;
        virtual void kunden_activate() = 0;
        virtual void on_kunden_reset() = 0;
        virtual gint on_delete_event(GdkEventAny *ev) = 0;
};
#endif
