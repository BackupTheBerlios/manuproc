// generated 2001/2/22 9:59:33 CET by thoma@ig23.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to windowDetails.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "windowDetails.hh"
#include "windowTop.hh"
#include <Aux/itos.h>
#include <Aux/Ausgabe_neu.h>
#include <gtk--/menu.h>
#include <gtk--/menuitem.h>
#include "window_neue_bank_anlegen.hh"
#include "MyMessage.h"

extern MyMessage *Message;

/*
void windowDetails::on_checkbuttonBranche_clicked()
{   
}

void windowDetails::on_checkbuttonEK_Gruppen_clicked()
{   
}

void windowDetails::on_checkbuttonVerbaende_clicked()
{   
}

void windowDetails::on_checkbuttonVK_Formen_clicked()
{   
}

void windowDetails::on_checkbuttonKonkurrenz_clicked()
{   
}

void windowDetails::on_checkbuttonUmsaetze_clicked()
{   
}

void windowDetails::on_checkbuttonBedarf_clicked()
{   
}

void windowDetails::on_checkbuttonVK_Gebiete_clicked()
{   
}
*/

void windowDetails::on_buttonDetailsUebernehmen_clicked()
{   
 kundendaten->set_flaeche(atoi(entryFlaeche->get_text().c_str()));
 kundendaten->set_kundenumsatz(entryKundenumsatz->get_text());
 kundendaten->set_planumsatz(entryPlanumsatz->get_text());
 kundendaten->set_mitarbeiter(atoi(entryMitarbeiter->get_text().c_str()));
 kundendaten->set_umsatz(entryUmsatz->get_text());

 kundendaten->set_rabatt(entryRabatt->get_text());
 kundendaten->set_skontofrist(atoi(entrySkontofrist->get_text().c_str()));
 kundendaten->set_skontosatz(atoi(spinbutton_skontosatz->get_text().c_str()));
 kundendaten->set_verein(entryVerein->get_text());
 kundendaten->set_lkz(entry_lkz->get_text()) ;
 
 kundendaten->isLieferadresse(checkbuttonLieferAdr->get_active());
 kundendaten->isRechnungsadresse(checkbuttonRchngAdr->get_active());

 kundendaten->setWaehrung(Waehrung->get_value());
 kundendaten->update_Bankeinzug(checkbuttonBankeinzug->get_active());
 kundendaten->set_einzugrabatt(einzugRabatt->get_text());
 kundendaten->set_bankkonto(strtoll(entryBankKonto->get_text().c_str(),NULL,10));
 unsigned long int blz = strtol(entry_blz->get_text().c_str(),NULL,10);
 int bank_index = fill_bank_bei(blz);
 if (bank_index==0) 
  {
    label_speichern->set_text("WARNUNG: Bankleitzahl unbekannt");
    string st = "Bankleitzahl unbekannt, soll eine neue Bank angelegt werden?";
    manage(new window_neue_bank_anlegen(this,st,blz));
  }
 else  kundendaten->set_bankindex(bank_index);
 int bank_index_ = bankid;
 kundendaten->set_bankindex(bank_index_);

 kundendaten->set_schema(extartbez->get_value());
 kundendaten->RngAn(rng_an->get_value());
 kundendaten->set_preisliste(preisliste->get_value());

 kundendaten->set_notiz(textNotiz->get_chars(0,textNotiz->get_length())) ;
 kundendaten->update();

 label_speichern->set_text("Alles gespeichert");   
 destroy();
}

void windowDetails::on_buttonAbbrechen_clicked()
{   
   destroy();
}

void windowDetails::on_windowDetails_destroy()
{   
}


windowDetails::windowDetails(H_Kunde& kd)
: kundendaten(kd), fire_enabled(false)
{
//   button_neue_bank->hide();

   labelDetailKdNrEintrag->set_text(itos(kundendaten->Id()));
   labelDetailStandEintrag->set_text(kundendaten->stand());
   labelDetailFirmaEintrag->set_text(kundendaten->firma());

	entryFlaeche->set_value(kundendaten->flaeche());
	entryMitarbeiter->set_value(kundendaten->mitarbeiter());
	entryPlanumsatz->set_value(kundendaten->planumsatz());
	entryUmsatz->set_value(kundendaten->umsatz());
	entryKundenumsatz->set_value(kundendaten->kundenumsatz());
	entryRabatt->set_value(kundendaten->rabatt());
	einzugRabatt->set_value(kundendaten->einzugrabatt());
	entrySkontofrist->set_value(kundendaten->skontofrist());
	spinbutton_skontosatz->set_value(kundendaten->skontosatz());
   entry_lkz->set_text(kundendaten->lkz());
	entryVerein->set_text(kundendaten->verein());
	entryBankKonto->set_text(ulltos(kundendaten->getKtnr()));
	entry_blz->set_text(itos(kundendaten->getblz()));
   rng_an->set_value(kundendaten->Rngan());
   extartbez->set_value(kundendaten->Schema());
   preisliste->set_value(kundendaten->Preisliste());

   Waehrung->set_value(kundendaten->getWaehrung());

   checkbuttonLieferAdr->set_active(kundendaten->isLieferadresse());
   checkbuttonRchngAdr->set_active(kundendaten->isRechnungsadresse());

/*
   Gtk::Menu *_m(manage(new Gtk::Menu()));
   Gtk::MenuItem *_mi;
   _mi = manage(new Gtk::MenuItem(kundendaten->getbank()));
   _m->append(*_mi) ; _mi->show() ; optionmenu_bankbei->set_menu(*_m) ;
*/

   textNotiz->delete_text(0,textNotiz->get_length());
   gint pos=0;
   textNotiz->insert_text(kundendaten->notiz().c_str(), (kundendaten->notiz()).size(), &pos);
   checkbuttonBankeinzug->set_active(kundendaten->bankeinzug());
   label_speichern->set_text("");

  fire_enabled=true;
}




void windowDetails::on_entryPlanumsatz_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_planumsatz(entryPlanumsatz->get_text()); 
 on_buttonDetailsUebernehmen_clicked();
 entryUmsatz->grab_focus();
// label_speichern->set_text("Planumsatz gespeichert");
}

void windowDetails::on_entryRabatt_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_rabatt(entryRabatt->get_text());
 on_buttonDetailsUebernehmen_clicked();
 entrySkontofrist->grab_focus();
// label_speichern->set_text("Rabatt gespeichert");
}

void windowDetails::on_entryMitarbeiter_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_mitarbeiter(atoi(entryMitarbeiter->get_text().c_str()));
 on_buttonDetailsUebernehmen_clicked();
 entryKundenumsatz->grab_focus();
// label_speichern->set_text("Mitarbeiter gespeichert");
}

void windowDetails::on_entryFlaeche_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_flaeche(atoi(entryFlaeche->get_text().c_str()));
 on_buttonDetailsUebernehmen_clicked();
 entryMitarbeiter->grab_focus();
// label_speichern->set_text("Fläche gespeichert");
}

void windowDetails::on_entryKundenumsatz_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_kundenumsatz(entryKundenumsatz->get_text());
 on_buttonDetailsUebernehmen_clicked();
 entryPlanumsatz->grab_focus();
// label_speichern->set_text("Kundenumsatz gespeichert");
}

void windowDetails::on_entryUmsatz_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_umsatz(entryUmsatz->get_text());
 on_buttonDetailsUebernehmen_clicked();
 entryRabatt->grab_focus();
// label_speichern->set_text("Umsatz gespeichert");
}


void windowDetails::on_entrySkontofrist_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_skontofrist(atoi(entrySkontofrist->get_text().c_str()));
 on_buttonDetailsUebernehmen_clicked();
 spinbutton_skontosatz->grab_focus();
// label_speichern->set_text("Skontofrist gespeichert");
}

void windowDetails::on_spinbutton_skontosatz_activate()
{
// (const_cast<Kunde*>(&*kundendaten))->set_skontosatz(atoi(entrySkontosatz->get_text().c_str()));
 on_buttonDetailsUebernehmen_clicked();
 entryVerein->grab_focus();
// label_speichern->set_text("Skontosatz gespeichert");
}

void windowDetails::on_entryVerein_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_verein(entryVerein->get_text());
 on_buttonDetailsUebernehmen_clicked();
 entry_lkz->grab_focus();
// label_speichern->set_text("Verein gespeichert");
}

void windowDetails::on_entry_lkz_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_lkz(entry_lkz->get_text());
// einzugRabatt->grab_focus();
 on_buttonDetailsUebernehmen_clicked();
// label_speichern->set_text("LKZ gespeichert");
}

void windowDetails::on_einzugRabatt_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_einzugrabatt(einzugRabatt->get_text());
 on_buttonDetailsUebernehmen_clicked();
 entryBankKonto->grab_focus();
// label_speichern->set_text("Einzugs-Rabatt gespeichert");
}

void windowDetails::on_textNotiz_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_notiz(textNotiz->get_chars(0,textNotiz->get_length())) ;
 on_buttonDetailsUebernehmen_clicked();
// label_speichern->set_text("Notiz gespeichert");
}


void windowDetails::on_Waehrung_activate()
{
//  (const_cast<Kunde*>(&*kundendaten))->setWaehrung(Waehrung->get_value());
  on_buttonDetailsUebernehmen_clicked();
//  label_speichern->set_text("Währung gespeichert");  
}


void windowDetails::on_checkbuttonBankeinzug_clicked()
{ 
 if(fire_enabled) //{
    on_buttonDetailsUebernehmen_clicked();
/*
 try{
	kundendaten->update_Bankeinzug(checkbuttonBankeinzug->get_active());
    }
 catch(SQLerror &e)
   {Message->Show(e); 
    label_speichern->set_text("Fehler beim Speichern");
    return;}
 label_speichern->set_text("Bankeinzug gespeichert");
 }
*/
}

void windowDetails::on_entryBankKonto_activate()
{
// (const_cast<Kunde*>(&*kundendaten))->set_bankkonto(strtoll(entryBankKonto->get_text().c_str(),NULL,10));
 on_buttonDetailsUebernehmen_clicked();
 entry_blz->grab_focus();
// label_speichern->set_text("Kontonr. gespeichert");
}

void windowDetails::on_entry_blz_activate()
{
 on_buttonDetailsUebernehmen_clicked();
}

void windowDetails::deactivate_bank_bei()
{
 on_buttonDetailsUebernehmen_clicked();
// label_speichern->set_text("Bank gespeichert");
}

void windowDetails::on_button_neue_bank_clicked()
{
  manage(new window_neue_bank_anlegen(this,"",0));
}

void windowDetails::neue_bank_uebernehmen(unsigned long int bank_index)
{
 entry_blz->set_text(itos(bank_index));
 kundendaten->get_blz_from_bankindex(bank_index);
 fill_bank_bei(kundendaten->getblz());

 on_buttonDetailsUebernehmen_clicked();
}


void windowDetails::extartbez_activate()
{
// (const_cast<Kunde*>(&*kundendaten))->set_schema(extartbez->get_value());
//  label_speichern->set_text("Bezeichnungsschema gespeichert");
 
  on_buttonDetailsUebernehmen_clicked();
  rng_an->grab_focus();
}

void windowDetails::rng_an_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->RngAn(rng_an->get_value());
// label_speichern->set_text("'Rechnung an'  gespeichert");
  on_buttonDetailsUebernehmen_clicked();
  preisliste->grab_focus();
}

void windowDetails::preisliste_activate()
{   
// (const_cast<Kunde*>(&*kundendaten))->set_preisliste(preisliste->get_value());
//  label_speichern->set_text("Preisliste gespeichert");
  on_buttonDetailsUebernehmen_clicked();
  textNotiz->grab_focus();
}

void windowDetails::on_checkbuttonRchngAdr_clicked()
{
 if(fire_enabled) //{
  on_buttonDetailsUebernehmen_clicked();
// try{kundendaten->isRechnungsadresse(checkbuttonRchngAdr->get_active());}
/*
 catch(SQLerror &e)
   {Message->Show(e); 
    label_speichern->set_text("Fehler beim Speichern");
    return;}
 label_speichern->set_text("Rechnungsadresse gespeichert");
 }
*/
}

void windowDetails::on_checkbuttonLieferAdr_clicked()
{   
 if(fire_enabled) //{
  on_buttonDetailsUebernehmen_clicked();
/*
 try{kundendaten->isLieferadresse(checkbuttonLieferAdr->get_active());}
 catch(SQLerror &e)
   {Message->Show(e); 
    label_speichern->set_text("Fehler beim Speichern");
    return;}
 label_speichern->set_text("Lieferadresse gespeichert");
 }
*/ 
}

