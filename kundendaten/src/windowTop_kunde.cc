#include "windowTop.hh"
#include "window_neue_Kundennummer.hh"
#include <Gtk_OStream.h>
#include "window_Dialog_Bestaetigen.hh"
#include <Aux/itos.h>
#include "MyMessage.h"


void windowTop::on_buttonNeu_clicked()
{   
   clear_entrys();
   manage(new window_neue_Kundennummer(this));   
}



void windowTop::show_kundendaten()
{
   fire_enabled=false;
   
   spinbuttonGruppenNr->set_value(kundendaten->GruppenId());
   entrySortiername->set_text(kundendaten->sortname());
   entryPostanwVor->set_text(kundendaten->postanwvor());
   entryPostanwNach->set_text(kundendaten->postanwnach());
   entryPLZ->set_text(kundendaten->plz());
   entryOrt->set_text(kundendaten->ort());
   entryStrasse->set_text(kundendaten->strasse());
   entryHausNr->set_text(kundendaten->hausnr());
   entryFirma->set_text(kundendaten->firma());
   entryPostfach->set_text(kundendaten->postfach());
   entryPostfachPLZ->set_text(kundendaten->postfachplz());
   entryIdNr->set_text(kundendaten->idnr());

   KundenTelefon->showTel(kundendaten->getTelefon());
  fire_enabled=true;
}


void windowTop::kunden_activate()
{
   try{
   kundendaten=H_Kunde(kundenauswahl->get_value());
   }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
   show_kundendaten();
}

void windowTop::on_KundenTelefon_activate()
{
   try{
   Telefon::newTelefon(kundendaten->Id(),Person::none_id,
            KundenTelefon->get_value());
    }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
   KundenTelefon->showTel(kundendaten->getTelefon());
}

void windowTop::on_button_kunde_loeschen_clicked()
{
  std::string st = "Soll der Kunde '"+kundendaten->firma()+"' mit \n"
   +"der Kundennummer '"+itos(kundendaten->getNummer())+"' wirklich gelöscht werden?";
  manage(new window_Dialog_Bestaetigen(this,st));
}

void windowTop::kunde_loeschen()
{
  Kunde::ID kid = kundendaten->getNummer();
  Kunde K;
  K.delete_Kunde(kid);
  clear_entrys();
}



void windowTop::neue_kundennmmer(unsigned int kid)
{
  kundenauswahl->set_value(kid);
  kunden_activate();
}

