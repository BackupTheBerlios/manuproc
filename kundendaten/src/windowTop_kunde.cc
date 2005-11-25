#include <Misc/i18n.h>
#include "windowTop.hh"
#include "window_neue_Kundennummer.hh"
#include <Gtk_OStream.h>
#include "window_Dialog_Bestaetigen.hh"
#include <Aux/itos.h>
#include "MyMessage.h"


void windowTop::on_buttonNeu_clicked()
{   
   clear_entrys();
   manage(new window_neue_Kundennummer(this,gruppenwahl->get_value()));   
}

void windowTop::on_kunden_reset()
{
  clear_entrys();
  notebook_main->set_current_page(PAGE_KUNDE);
}

void windowTop::show_kundendaten()
{
   fire_enabled=false;
   
//   spinbuttonGruppenNr->set_value(kundendaten->GruppenId());
   entrySortiername->set_text(kundendaten->sortname());
   entryPostanwVor->set_text(kundendaten->postanwvor());
   entryPostanwNach->set_text(kundendaten->postanwnach());
   entryPLZ->set_text(kundendaten->plz());
   entryOrt->set_text(kundendaten->ort());
   entryStrasse->set_text(kundendaten->strasse());
   entryHausNr->set_text(kundendaten->hausnr());
   entryFirma->set_text(kundendaten->firma());
   entryName2->set_text(kundendaten->name2());
  scc_anrede->setContent(kundendaten->Anrede()->getBrAnrede(),kundendaten->Anrede()->Id());
   entryPostfach->set_text(kundendaten->postfach());
   entryPostfachPLZ->set_text(kundendaten->postfachplz());
   entryIdNr->set_text(kundendaten->idnr());
   landesbox->set_value(kundendaten->land()->Id());

   KundenTelefon->showTel(kundendaten->getTelefon());
  fire_enabled=true;
  kunden_status->set_active(kundendaten->Aktiv());
}


void windowTop::kunden_activate()
{
  clear_entrys();
  try{

// saveAll?   
   assert (UpdateAdresse==0);
   assert(UpdateFirma==0);
   assert(UpdateBank==0);
   assert(UpdateSonst==0);
//   assert(UpdatePerson==0);
//   if(UpdatePerson!=0) std::cout << UpdatePerson<<'\n';
//   clear_update_bits();

   kundendaten=H_Kunde(kundenauswahl->get_value());
   frame_adresse->set_sensitive(true);
   frame_kundenkontakt->set_sensitive(true);
   
   }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}


   show_kundendaten();
   KundenTelefon->setKdPer(kundendaten->Id(),Kunde::none_id);
   notebook_main->set_current_page(PAGE_KUNDE);
}

void windowTop::on_KundenTelefon_activate(cH_Telefon ct)
{
std::cout << "activated";
//   KundenTelefon->showTel(kundendaten->getTelefon());
}

void windowTop::on_button_kunde_loeschen_clicked()
{
  std::string st = _("Soll der Kunde '")+kundendaten->firma()+_("' mit \n"
   "der Kundennummer '")+itos(kundendaten->getNummer())+_("' wirklich gelöscht werden?");
  manage(new window_Dialog_Bestaetigen(this,st));
}


void windowTop::neue_kundennmmer(unsigned int kid)
{
  kundenauswahl->set_value(kid);
  kunden_activate();
}

