// generated 2001/2/22 9:59:33 CET by thoma@ig23.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to windowTop.cc_new

// This file is for your program, I won't touch it again!

//#include "config.h"
#include "windowTop.hh"
#include "MyMessage.h"
#include <gtk--/main.h>
#include <gtk--/notebook.h>

void windowTop::on_notebook1_switch_page(Gtk::Notebook_Helpers::Page* page,guint pagenr)
{
  switch (enum_notebook(pagenr))
   {
     case PAGE_KUNDE          : break;
     case PAGE_DETAILS        : if(kundendaten->Id()==Kunde::none_id) 
                                   table_details->set_sensitive(false);
                                else show_details(); 
                                break;
     case PAGE_KONTAKTPERSON  : if(kundendaten->Id()==Kunde::none_id) 
                                   table_kontaktperson->set_sensitive(false);
                                else show_kontaktpersonen(); 
                                break; 
     case PAGE_PERSON         : show_privatpersonen(); break; 
   }
}

windowTop::windowTop()
: kundendaten(Kunde::none_id),person(Person::none_id),
   transaction("",false), fire_enabled(true) 
{
 label_speichern->set_text("");
 setTitles();
 button_del_preisliste->set_sensitive(false);
 button_spreis_add->set_sensitive(false);

 connectFkt();
}

void windowTop::saveAll()
{
   try{

   if(UpdateAdresse!=0) kundendaten->update_e(UpdateAdresse);
   if(UpdateFirma!=0) kundendaten->update_e(UpdateFirma);
   if(UpdateBank!=0) kundendaten->update_e(UpdateBank);
   if(UpdateSonst!=0) kundendaten->update_e(UpdateSonst);
   
   UpdateAdresse=Kunde::UpdateBitsAdresse(0);
   UpdateFirma=Kunde::UpdateBitsFirma(0);
   UpdateBank=Kunde::UpdateBitsBank(0);
   UpdateSonst=Kunde::UpdateBitsSonst(0);

   }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}



void windowTop::on_buttonBeenden_clicked()
{   
   Gtk::Main::instance()->quit();
}


   // Details


void windowTop::clear_entrys()
{   
   fire_enabled=false;
   
   spinbuttonGruppenNr->set_value(0);
   entrySortiername->set_text("");
   entryPostanwVor->set_text("");
   entryPostanwNach->set_text("");
   entryPLZ->set_text("");
   entryOrt->set_text("");
   entryStrasse->set_text("");
   entryHausNr->set_text("");
   entryFirma->set_text("");
   entryPostfach->set_text("");
   entryPostfachPLZ->set_text("");
   entryIdNr->set_text("");
   KundenTelefon->clear();
//   entryTelefonNr->set_text("");

   // Details
   scc_verkaeufer->setContent("",0);
   spinbutton_Flaeche->set_value(0);
   spinbutton_Kundenumsatz->set_value(0);
   spinbutton_Planumsatz->set_value(0);
   spinbutton_Rabatt->set_value(0);
   spinbutton_Mitarbeiter->set_value(1);
   spinbutton_Umsatz->set_value(0);
   spinbutton_Skontofrist->set_value(0);
   spinbutton_skontosatz->set_value(0);
   entryVerein->set_text("");
   entry_lkz->set_text("");
   spinbutton_einzugRabatt->set_value(0);
   entryBankKonto->set_text("");
   entry_blz->set_text("");
   extartbez->reset();
   rng_an->reset();
   preisliste->reset();
   textNotiz->delete_text(0,textNotiz->get_length());;
   
   checkbutton_zeilenrabatt->set_active(false);
   checkbuttonBankeinzug->set_active(false);
   checkbuttonRchngAdr->set_active(false);
   checkbuttonLieferAdr->set_active(false);
   
   kundendaten=H_Kunde(Kunde::none_id);
//   telefon.erase(telefon.begin(),telefon.end());

   fire_enabled=true;
}


void windowTop::setTitles()
{
 std::vector<string> preis;
 preis.push_back("Nr");
 preis.push_back("Bezeichnung");
 SonderPreislisteTree->setTitles(preis);
}


/////////////////////////////////////////////////////////////////////////
// Extras
/////////////////////////////////////////////////////////////////////////


void windowTop::on_button_neu_spl_nummer_clicked()
{
  unsigned int n=PreisListe::nextval();
  spinbutton_new_spreislist->set_value(n);
}

void windowTop::on_button_neue_spl_anlegen_clicked()
{
  std::string e=entry_new_spreislist_name->get_text();
  if(e=="") 
   { 
     label_speichern->set_text("FEHLER: Name muß eingegeben werden");
     return;
   }
 PreisListe::createPreisliste(spinbutton_new_spreislist->get_value_as_int(),e);
}
