// generated 2001/2/22 9:59:33 CET by thoma@ig23.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to windowDetails.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOWDETAILS_HH
#  include "windowDetails_glade.hh"
#  define _WINDOWDETAILS_HH
#include "glademm_support.hh"

#include "windowTop.hh"

class windowDetails : public windowDetails_glade
{   
        H_Kunde kundendaten;
        friend class windowDetails_glade;
        void on_checkbuttonBankeinzug_clicked();

	bool fire_enabled;

        void on_entryPlanumsatz_activate();
        void on_entryRabatt_activate();
        void on_entryMitarbeiter_activate();
        void on_entryFlaeche_activate();
        void on_entryKundenumsatz_activate();
        void on_entryUmsatz_activate();
        void on_entryVerein_activate();
//        void on_entryPreisliste_activate();
        void on_entrySkontofrist_activate();
        void on_spinbutton_skontosatz_activate();
        void on_entry_lkz_activate();
        void on_einzugRabatt_activate();
        void deactivate_bank_bei();
        int fill_bank_bei(unsigned long int blz);
        void on_button_neue_bank_clicked();
        void on_entry_blz_activate();
        void on_entryBankKonto_activate();
        void on_textNotiz_activate();
        void on_Waehrung_activate();

        void extartbez_activate();
        void preisliste_activate();
        void rng_an_activate();
        void on_checkbuttonRchngAdr_clicked();
        void on_checkbuttonLieferAdr_clicked();


/*
        void on_checkbuttonBranche_clicked();
        void on_checkbuttonEK_Gruppen_clicked();
        void on_checkbuttonVerbaende_clicked();
        void on_checkbuttonVK_Formen_clicked();
        void on_checkbuttonKonkurrenz_clicked();
        void on_checkbuttonUmsaetze_clicked();
        void on_checkbuttonBedarf_clicked();
        void on_checkbuttonVK_Gebiete_clicked();
*/
        void on_buttonDetailsUebernehmen_clicked();
        void on_buttonAbbrechen_clicked();
        void on_windowDetails_destroy();
   public:
        windowDetails(H_Kunde& kd);
        void neue_bank_uebernehmen(unsigned long int bank_index);
};
#endif
