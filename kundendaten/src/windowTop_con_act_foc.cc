#include "windowTop.hh"
#include "MyMessage.h"
#include <gtk--/menu.h>

//////////////////////////////////////////////////////////////////////

void windowTop::changedFktF(Kunde::UpdateBitsFirma e)
{
  if(!fire_enabled) return;
  UpdateFirma =  Kunde::UpdateBitsFirma(UpdateFirma|e);
     if      (e==Kunde::FPlanumsatz    ) kundendaten->set_planumsatz(spinbutton_Planumsatz->get_text());
     else if (e==Kunde::FUmsatz        ) kundendaten->set_umsatz(spinbutton_Umsatz->get_text());
     else if (e==Kunde::FMitarbeiter   ) kundendaten->set_mitarbeiter(atoi(spinbutton_Mitarbeiter->get_text().c_str()));
     else if (e==Kunde::FKundenumsatz  ) kundendaten->set_kundenumsatz(spinbutton_Kundenumsatz->get_text());
     else if (e==Kunde::FFlaeche       ) kundendaten->set_flaeche(atoi(spinbutton_Flaeche->get_text().c_str()));
     else if (e==Kunde::FUnsereKundenNr) kundendaten->set_UnsereKundenNr(entry_eigene_kundennr->get_text()) ;
     else if (e==Kunde::FVerein        ) kundendaten->set_verein(entryVerein->get_text()) ;
     else std::cerr<< "Falsche Funktion konektiert\n";
}


void windowTop::changedFktA(Kunde::UpdateBitsAdresse e)
{
  if(!fire_enabled) return;
  UpdateAdresse = Kunde::UpdateBitsAdresse(UpdateAdresse|e);

//     if      (e==Kunde::FGruppennr  ) kundendaten->set_gruppen_nr(atoi(spinbuttonGruppenNr->get_text().c_str()));
//     else 
     if (e==Kunde::FSortname   ) kundendaten->set_sortname(entrySortiername->get_text());
     else if (e==Kunde::FIdnr       ) kundendaten->set_idnr(entryIdNr->get_text());
     else if (e==Kunde::FFirma      ) kundendaten->set_firma(entryFirma->get_text());
     else if (e==Kunde::FPostanwvor ) kundendaten->set_postanwvor(entryPostanwVor->get_text());
     else if (e==Kunde::FStrasse    ) kundendaten->set_strasse(entryStrasse->get_text());
     else if (e==Kunde::FPostanwnach) kundendaten->set_postanwnach(entryPostanwNach->get_text());
     else if (e==Kunde::FHsnr       ) kundendaten->set_hausnr(entryHausNr->get_text());
     else if (e==Kunde::FPlz        ) kundendaten->set_plz(entryPLZ->get_text());
     else if (e==Kunde::FPostfach   ) kundendaten->set_postfach(entryPostfach->get_text());
     else if (e==Kunde::FPostfachplz) kundendaten->set_postfachplz(entryPostfachPLZ->get_text());
     else if (e==Kunde::FOrt        ) kundendaten->set_ort(entryOrt->get_text());
     else if (e==Kunde::FLkz        ) kundendaten->set_land(landesbox->get_value()) ;
     else if (e==Kunde::FLieferadresse  ) kundendaten->isLieferadresse(checkbuttonLieferAdr->get_active());
     else if (e==Kunde::FRechnungadresse) kundendaten->isRechnungsadresse(checkbuttonRchngAdr->get_active());
     else if (e==Kunde::FAuftragadresse) kundendaten->isAuftragsadresse(checkbuttonAuftrAdr->get_active());
     else if (e==Kunde::FRng_an_postfach) kundendaten->Rng_an_postfach(checkbutton_rng_an_postfach->get_active());
     else if (e==Kunde::FBranr		) kundendaten->set_Anrede(scc_anrede->get_value());
     else if (e==Kunde::FName2		) kundendaten->set_name2(entryName2->get_text());     

     else std::cerr<< "Falsche Funktion konektiert\n";

  if(e==Kunde::FLieferadresse || e==Kunde::FRechnungadresse || 
     e==Kunde::FRng_an_postfach || e==Kunde::FLkz ||
	e==Kunde::FBranr)
    saveAll();
}

void windowTop::changedFktB(Kunde::UpdateBitsBank e)
{
  if(!fire_enabled) return;
  UpdateBank =  Kunde::UpdateBitsBank(UpdateBank|e);
     if      (e==Kunde::FKtonr       ) kundendaten->set_bank_konto(strtoll(entryBankKonto->get_text().c_str(),NULL,10));
     else if (e==Kunde::FBlz         ) ;//on_entry_blz_activate();
     else if (e==Kunde::FBankindex   ) kundendaten->set_bankindex(bankid);
//     else if (e==Kunde::FBankeinzug  ) kundendaten->update_Bank_einzug(checkbuttonBankeinzug->get_active());
     else if (e==Kunde::FRabatt      ) kundendaten->set_rabatt(spinbutton_Rabatt->get_text());
     else if (e==Kunde::FZeilenrabatt) kundendaten->set_zeilenrabatt(checkbutton_zeilenrabatt->get_active());
     else if (e==Kunde::FWaehrungid  ) kundendaten->setWaehrung(Waehrung->get_value());
//     else if (e==Kunde::FEinzugrabatt) kundendaten->set_einzugrabatt(spinbutton_einzugRabatt->get_text());
//     else if (e==Kunde::FSkontosatz  ) kundendaten->set_skontosatz(spinbutton_skontosatz->get_text());
     else if (e==Kunde::FZahlungsart ) { kundendaten->set_Zahlungsart(zahlungsartbox->get_value()); show_zahlungsziel();}
     else if (e==Kunde::FLieferantenkonto ) kundendaten->set_Lieferantenkonto(atoi(spinbutton_lieferantenkonto->get_text().c_str()));
     else if (e==Kunde::FGegenkonto  ) kundendaten->set_Gegenkonto(atoi(spinbutton_gegenkonto->get_text().c_str()));
     else if (e==Kunde::FDebitorenkonto  ) kundendaten->set_Debitorenkonto(atoi(spinbutton_debitorenkonto->get_text().c_str()));
     else std::cerr<< "Falsche Funktion konektiert\n";

  if(e==Kunde:: FWaehrungid || e==Kunde::FBankindex ||
     e==Kunde::FZeilenrabatt || e==Kunde::FZahlungsart)
    saveAll();
}

void windowTop::changedFktS(Kunde::UpdateBitsSonst e)
{
  if(!fire_enabled) return;
  UpdateSonst =  Kunde::UpdateBitsSonst(UpdateSonst|e);
     if      (e==Kunde::FRechnungan ) kundendaten->RngAn(rng_an->get_value());
     else if (e==Kunde::FLieferscheinan ) kundendaten->LfrAn(lfr_an->get_value());
     else if (e==Kunde::FExtartbezid) kundendaten->set_schema(extartbez->get_value());
     else if (e==Kunde::FNotiz      ) kundendaten->set_notiz(textNotiz->get_chars(0,textNotiz->get_length()));
     else if (e==Kunde::FEntsorgung ) kundendaten->entsorgung(checkbutton_entsorgung->get_active());
     else if (e==Kunde::Flieferung_frei_haus ) kundendaten->set_lieferung_frei_haus(checkbutton_lieferung_frei_haus->get_active());
//     else if (e==Kunde::FVerknr     ) kundendaten->setVerkNr(scc_verkaeufer->Content());
     else if (e==Kunde::FBetreuer     ) kundendaten->setBetreuer(betreuer->get_value()->Id());     
//     else if (e==Kunde::FKalkulation)
     else if (e==Kunde::FKP_Position) ;
     else if (e==Kunde::FKP_Notiz   ) ;
     else if (e==Kunde::FAnzAusFirmenPapier) { spinbutton_firmenpapier->update(); kundendaten->set_anzahl_ausdruck_firmenpapier(spinbutton_firmenpapier->get_value_as_int());}
     else if (e==Kunde::FAnzAusWeissesPapier){ spinbutton_weissespapier->update();kundendaten->set_anzahl_ausdruck_weissespapier(spinbutton_weissespapier->get_value_as_int());}
     else if (e==Kunde::FGebDatum	) kundendaten->setGebDatum(geburtstag->get_value());
     else std::cerr<< "Falsche Funktion konektiert\n";

  if(e==Kunde::FNotiz || e==Kunde::FRechnungan || e==Kunde::FLieferscheinan 
	|| e==Kunde::FExtartbezid 
      	|| e==Kunde::FPreisliste || e==Kunde::FEntsorgung 
	|| e==Kunde::FVerknr || e==Kunde::Flieferung_frei_haus)
    saveAll();
//wirklich bei jedem Changed von Notiz speichern?
}

//////////////////////////////////////////////////////////////////////

void windowTop::activateFktS(Kunde::UpdateBitsSonst e)
{
     if      (e==Kunde::FRechnungan ) ;//nicht konnektiert
     else if (e==Kunde::FExtartbezid) ;//nicht konnektiert
     else if (e==Kunde::FPreisliste ) ;//nicht konnektiert
     else if (e==Kunde::FNotiz      ) ; //  kein sinnvolles Ziel, oben speichern
     else if (e==Kunde::FEntsorgung ) ;//nicht konnektiert
     else if (e==Kunde::FVerknr     ) ;
//     else if (e==Kunde::FKalkulation)
     else if (e==Kunde::FKP_Position) ;
     else if (e==Kunde::FKP_Notiz   ) ;
     else if (e==Kunde::FAnzAusFirmenPapier) spinbutton_weissespapier->grab_focus();
     else if (e==Kunde::FAnzAusWeissesPapier) ;
     else std::cerr<< "Falsche Funktion konektiert\n";
}



void windowTop::activateFktB(Kunde::UpdateBitsBank e)
{
     if      (e==Kunde::FKtonr       ) entry_blz->grab_focus();
     else if (e==Kunde::FBlz         ) {extartbez->grab_focus(); /*on_entry_blz_activate();*/}
     else if (e==Kunde::FBankindex   ) ; // optionmenu nicht konnektiert
//     else if (e==Kunde::FBankeinzug  ) ; //spinbutton nicht konnektiert
     else if (e==Kunde::FRabatt      ) spinbutton_einzugRabatt->grab_focus();//spinbutton_Skontofrist->grab_focus();
     else if (e==Kunde::FWaehrungid  ) ; // optionmenu nicht konnektiert
//     else if (e==Kunde::FEinzugrabatt) spinbutton_Skontofrist->grab_focus();//entryBankKonto->grab_focus();
//     else if (e==Kunde::FSkontosatz  ) entryVerein->grab_focus();
//     else if (e==Kunde::FSkontofrist ) spinbutton_skontosatz->grab_focus();
     else if (e==Kunde::FZahlungsart ) spinbutton_Rabatt->grab_focus();
     else if (e==Kunde::FLieferantenkonto ) spinbutton_debitorenkonto->grab_focus();
     else if (e==Kunde::FDebitorenkonto ) spinbutton_gegenkonto->grab_focus();
     else if (e==Kunde::FGegenkonto  ) ;
     else std::cerr<< "Falsche Funktion konektiert\n";
}


void windowTop::activateFktA(Kunde::UpdateBitsAdresse e)
{
//     if      (e==Kunde::FGruppennr  ) entrySortiername->grab_focus();
//     else 
	if (e==Kunde::FSortname   ) entryIdNr->grab_focus();
     else if (e==Kunde::FIdnr       ) entryName2->grab_focus();
     else if (e==Kunde::FName2      ) entryFirma->grab_focus();	     
     else if (e==Kunde::FFirma      ) entryPostanwVor->grab_focus();
     else if (e==Kunde::FPostanwvor ) entryStrasse->grab_focus();
     else if (e==Kunde::FStrasse    ) entryHausNr->grab_focus();
     else if (e==Kunde::FPostanwnach) entryPLZ->grab_focus();
     else if (e==Kunde::FHsnr       ) entryPostanwNach->grab_focus();
     else if (e==Kunde::FPlz        ) entryOrt->grab_focus();
     else if (e==Kunde::FPostfach   ) entryPostfachPLZ->grab_focus();
     else if (e==Kunde::FPostfachplz) KundenTelefon->grab_focus();
     else if (e==Kunde::FOrt        ) entryPostfach->grab_focus();
     else if (e==Kunde::FLieferadresse  ) ;//die spinbuttons sind nicht konnektiert
     else if (e==Kunde::FRechnungadresse) ;
     else if (e==Kunde::FRng_an_postfach) ;
     else std::cerr<< "Falsche Funktion konektiert\n";
}

void windowTop::activateFktF(Kunde::UpdateBitsFirma e)
{
     if      (e==Kunde::FPlanumsatz    ) spinbutton_Rabatt->grab_focus();
     else if (e==Kunde::FUmsatz        ) spinbutton_Kundenumsatz->grab_focus();
     else if (e==Kunde::FMitarbeiter   ) spinbutton_Umsatz->grab_focus();
     else if (e==Kunde::FKundenumsatz  ) spinbutton_Planumsatz->grab_focus();
     else if (e==Kunde::FFlaeche       ) spinbutton_Mitarbeiter->grab_focus();
     else if (e==Kunde::FUnsereKundenNr ) spinbutton_lieferantenkonto->grab_focus();
     else if (e==Kunde::FVerein        ) ;
     else std::cerr<< "Falsche Funktion konektiert\n";
}

//////////////////////////////////////////////////////////////////////
gint windowTop::focus_outFktA(GdkEventFocus *ev,Kunde::UpdateBitsAdresse e)
{
  saveAll();
  return 0;
}
gint windowTop::focus_outFktF(GdkEventFocus *ev,Kunde::UpdateBitsFirma e)
{
  saveAll();
  return 0;
}
gint windowTop::focus_outFktB(GdkEventFocus *ev,Kunde::UpdateBitsBank e)
{
  if      (e==Kunde::FBlz) on_entry_blz_activate();
  else saveAll();
  return 0;
}
gint windowTop::focus_outFktS(GdkEventFocus *ev,Kunde::UpdateBitsSonst e)
{
  if (e==Kunde::FKP_Position || e==Kunde::FKP_Notiz) saveKundenKontakt();
  else saveAll();
  return 0;
}

void windowTop::connectFkt()
{
#if 0
#define CF_SPINBUTTON(widget,changedFktA,FGruppennr) \
  spinbuttonGruppenNr->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FGruppennr));
  spinbuttonGruppenNr->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FGruppennr));
  spinbuttonGruppenNr->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FGruppennr));

  CF_SPINBUTTON(spinbuttonGruppenNr,FktA,FGruppennr);
#endif

//  spinbuttonGruppenNr->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FGruppennr));
//  spinbuttonGruppenNr->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FGruppennr));
//  spinbuttonGruppenNr->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FGruppennr));

  entrySortiername->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FSortname));
  entrySortiername->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FSortname));
  entrySortiername->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FSortname));

  entryFirma->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FFirma));
  entryFirma->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FFirma));
  entryFirma->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FFirma));

  entryName2->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FName2));
  entryName2->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FName2));
  entryName2->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FName2));  

  entryPostanwVor->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FPostanwvor));
  entryPostanwVor->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FPostanwvor));
  entryPostanwVor->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FPostanwvor));

  entryIdNr->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FIdnr));
  entryIdNr->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FIdnr));
  entryIdNr->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FIdnr));

  entryStrasse->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FStrasse));
  entryStrasse->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FStrasse));
  entryStrasse->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FStrasse));

  entryPostanwNach->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FPostanwnach));
  entryPostanwNach->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FPostanwnach));
  entryPostanwNach->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FPostanwnach));

  entryHausNr->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FHsnr));
  entryHausNr->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FHsnr));
  entryHausNr->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FHsnr));

  entryPLZ->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FPlz));
  entryPLZ->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FPlz));
  entryPLZ->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FPlz));

  entryPostfach->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FPostfach));
  entryPostfach->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FPostfach));
  entryPostfach->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FPostfach));

  entryPostfachPLZ->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FPostfachplz));
  entryPostfachPLZ->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FPostfachplz));
  entryPostfachPLZ->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FPostfachplz));

  entryOrt->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FOrt));
  entryOrt->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktA),Kunde::FOrt));
  entryOrt->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktA),Kunde::FOrt));

  checkbuttonLieferAdr->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FLieferadresse));
  checkbuttonRchngAdr->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FRechnungadresse));
  checkbuttonAuftrAdr->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FAuftragadresse));
  checkbutton_rng_an_postfach->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FRng_an_postfach));

  landesbox->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FLkz));

  geburtstag->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FGebDatum));
  scc_anrede->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktA),Kunde::FBranr));

  //Kunde::BFirma

  spinbutton_Flaeche->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktF),Kunde::FFlaeche));
  spinbutton_Flaeche->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktF),Kunde::FFlaeche));
  spinbutton_Flaeche->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktF),Kunde::FFlaeche));

  spinbutton_Kundenumsatz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktF),Kunde::FKundenumsatz));
  spinbutton_Kundenumsatz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktF),Kunde::FKundenumsatz));
  spinbutton_Kundenumsatz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktF),Kunde::FKundenumsatz));

  spinbutton_Planumsatz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktF),Kunde::FPlanumsatz));
  spinbutton_Planumsatz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktF),Kunde::FPlanumsatz));
  spinbutton_Planumsatz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktF),Kunde::FPlanumsatz));

  spinbutton_Mitarbeiter->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktF),Kunde::FMitarbeiter));
  spinbutton_Mitarbeiter->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktF),Kunde::FMitarbeiter));
  spinbutton_Mitarbeiter->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktF),Kunde::FMitarbeiter));

  spinbutton_Umsatz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktF),Kunde::FUmsatz));
  spinbutton_Umsatz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktF),Kunde::FUmsatz));
  spinbutton_Umsatz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktF),Kunde::FUmsatz));

  entryVerein->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktF),Kunde::FVerein));
  entryVerein->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktF),Kunde::FVerein));
  entryVerein->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktF),Kunde::FVerein));

  entry_eigene_kundennr->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktF),Kunde::FUnsereKundenNr));
  entry_eigene_kundennr->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktF),Kunde::FUnsereKundenNr));
  entry_eigene_kundennr->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktF),Kunde::FUnsereKundenNr));

  zahlungsartbox->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FZahlungsart));

  //Kunde::BBank

  entryBankKonto->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FKtonr));
  entryBankKonto->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FKtonr));
  entryBankKonto->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FKtonr));

  entry_blz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FBlz));
  entry_blz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FBlz));
  entry_blz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FBlz));
//  checkbuttonBankeinzug->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FBankeinzug));
  checkbutton_zeilenrabatt->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FZeilenrabatt));

//  spinbutton_einzugRabatt->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FEinzugrabatt));
//  spinbutton_einzugRabatt->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FEinzugrabatt));
//  spinbutton_einzugRabatt->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FEinzugrabatt));

  spinbutton_Rabatt->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FRabatt));
  spinbutton_Rabatt->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FRabatt));
  spinbutton_Rabatt->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FRabatt));

//  spinbutton_Skontofrist->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FSkontofrist));
//  spinbutton_Skontofrist->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FSkontofrist));
//  spinbutton_Skontofrist->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FSkontofrist));

//  spinbutton_skontosatz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FSkontosatz));
//  spinbutton_skontosatz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FSkontosatz));
//  spinbutton_skontosatz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FSkontosatz));

  spinbutton_lieferantenkonto->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FLieferantenkonto));
  spinbutton_lieferantenkonto->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FLieferantenkonto));
  spinbutton_lieferantenkonto->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FLieferantenkonto));

  spinbutton_gegenkonto->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FGegenkonto));
  spinbutton_gegenkonto->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FGegenkonto));
  spinbutton_gegenkonto->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FGegenkonto));

  spinbutton_debitorenkonto->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FDebitorenkonto));
  spinbutton_debitorenkonto->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktB),Kunde::FDebitorenkonto));
  spinbutton_debitorenkonto->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktB),Kunde::FDebitorenkonto));  

  Waehrung->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktB),Kunde::FWaehrungid));

  //Kunde::BSonst  

  textNotiz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FNotiz));
  textNotiz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktS),Kunde::FNotiz));
  textNotiz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktS),Kunde::FNotiz));

  extartbez->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FExtartbezid));
  rng_an->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FRechnungan));
  lfr_an->activate.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FLieferscheinan));

  checkbutton_entsorgung->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FEntsorgung));
  checkbutton_lieferung_frei_haus->toggled.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::Flieferung_frei_haus));

  spinbutton_firmenpapier->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FAnzAusFirmenPapier));
  spinbutton_firmenpapier->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktS),Kunde::FAnzAusFirmenPapier));
  spinbutton_firmenpapier->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktS),Kunde::FAnzAusFirmenPapier));

  spinbutton_weissespapier->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FAnzAusWeissesPapier));
  spinbutton_weissespapier->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktS),Kunde::FAnzAusWeissesPapier));
  spinbutton_weissespapier->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktS),Kunde::FAnzAusWeissesPapier));

#warning TODO Verkäufer nummer
//  entry->changed.connect(SigC::bind(SigC::slot(this,&windowTop::ChangedEnum),Kunde::BSonst));
//  entry->changed.connect(SigC::bind(SigC::slot(this,&windowTop::ChangedEnum),Kunde::BSonst));

  /////////////////////////////////////////////////////////////////////////
  // Kontaktperson
  ////////////////////////////////////////////////////////////////////////
  entryPersonenPosition->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FKP_Position));
  entryPersonenPosition->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktS),Kunde::FKP_Position));
  entryPersonenPosition->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktS),Kunde::FKP_Position));

  textPersonenFirmaNotiz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktS),Kunde::FKP_Notiz));
  textPersonenFirmaNotiz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktS),Kunde::FKP_Notiz));
  textPersonenFirmaNotiz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktS),Kunde::FKP_Notiz));

  /////////////////////////////////////////////////////////////////////////
  // Person
  ////////////////////////////////////////////////////////////////////////
/*
  entryPersonenDatenName->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktP),Person::FName));
  entryPersonenDatenName->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktP),Person::FName));
  entryPersonenDatenName->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktP),Person::FName));

  entryPersonenDatenVorname->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktP),Person::FVorname));
  entryPersonenDatenVorname->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktP),Person::FVorname));
  entryPersonenDatenVorname->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktP),Person::FVorname));


  textPersonenPrivatNotiz->changed.connect(SigC::bind(SigC::slot(this,&windowTop::changedFktP),Person::FNotiz));
  textPersonenPrivatNotiz->activate.connect_after(SigC::bind(SigC::slot(this,&windowTop::activateFktP),Person::FNotiz));
  textPersonenPrivatNotiz->focus_out_event.connect(SigC::bind(SigC::slot(this,&windowTop::focus_outFktP),Person::FNotiz));
*/


}

////////////////////////////////////////////////////////////////////////////
// Person
///////////////////////////////////////////////////////////////////////////

/*
void windowTop::changedFktP(Person::UpdateBits e)
{
  if(!fire_enabled) return;
  UpdatePerson = Person::UpdateBits(UpdatePerson|e);
  if      (e==Person::FName    ) (const_cast<Person*>(&*person))->setName(entryPersonenDatenName->get_text());
  else if (e==Person::FVorname ) (const_cast<Person*>(&*person))->setVorname(entryPersonenDatenVorname->get_text());
  else if (e==Person::FGebDatum) ;// {(const_cast<Person*>(&*person))->setGebDatum(geburtstag->get_value());scc_anrede->grab_focus();}
  else if (e==Person::FAnrede  ) ;// {(const_cast<Person*>(&*person))->setAnrede(scc_anrede->get_value());textPersonenPrivatNotiz->grab_focus();}
  else if (e==Person::FNotiz   ) ;// (const_cast<Person*>(&*person))->setNotiz(textPersonenPrivatNotiz->get_chars(0,textPersonenPrivatNotiz->get_length()));
  else std::cerr<< "Falsche Funktion konektiert\n";
  if(e==Person::FGebDatum || e==Person::FAnrede)
    update_person();
}


void windowTop::activateFktP(Person::UpdateBits e)
{
     if      (e==Person::FName    ) entryPersonenDatenVorname->grab_focus();
     else if (e==Person::FVorname ) geburtstag->grab_focus();
     else if (e==Person::FGebDatum) ;//nicht konnektiert
     else if (e==Person::FAnrede  ) ;//nicht konnektiert
     else if (e==Person::FNotiz   ) ;//nicht konnektiert
     else std::cerr<< "Falsche Funktion konektiert\n";
}


gint windowTop::focus_outFktP(GdkEventFocus *ev,Person::UpdateBits e)
{
  update_person();
  return 0;
}
*/

/*
void windowTop::update_person()
{
  try{
   if(UpdatePerson!=0) (const_cast<Person*>(&*person))->update_e(UpdatePerson);
   UpdatePerson=Person::UpdateBits(0);
   if(kundendaten->Id()!=Kunde::none_id) show_kontaktpersonen();
  }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}
*/

