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
#include <Kunde/Kundengruppe.h>
#include "kundendaten_aux.hh"

void windowTop::on_notebook1_switch_page(Gtk::Notebook_Helpers::Page* page,guint pagenr)
{
  switch (enum_notebook(pagenr))
   {
     case PAGE_KUNDE          : if(kundendaten->Id()==Kunde::none_id)
                                 { frame_adresse->set_sensitive(false);
                                   frame_kundenkontakt->set_sensitive(false);
                                 }
                                break;
     case PAGE_ZAHLUNG		:
     case PAGE_DETAILS        : if(kundendaten->Id()==Kunde::none_id) 
                                   table_details->set_sensitive(false);
                                else show_details(); 
                                break;
     case PAGE_KONTAKTPERSON  : if(kundendaten->Id()==Kunde::none_id) 
                                   table_kontaktperson->set_sensitive(false);
                                else show_kontaktpersonen(); 
                                break; 
     case PAGE_PERSON         : show_privatpersonen(); break; 
     case PAGE_NOTIZEN		: 
     	try { load_notizen(); }
        catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
     	break;
     case PAGE_GRUPPEN: 
     	try { showGruppen(); }
        catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
     	break;     	
   }
}

windowTop::windowTop()
: kundendaten(Kunde::none_id),person(Person::none_id),
   transaction("",false), 
   UpdateAdresse(Kunde::UpdateBitsAdresse(0)),
   UpdateFirma(Kunde::UpdateBitsFirma(0)), 
   UpdateBank(Kunde::UpdateBitsBank(0)), 
   UpdateSonst(Kunde::UpdateBitsSonst(0)), 
   UpdatePerson(Person::UpdateBits(0)), fire_enabled(true),
   allgrp(NULL),wahlgrp(NULL),bankid(0)
{
 clear_entrys();
 label_speichern->set_text("");
 setTitles();
 button_del_preisliste->set_sensitive(false);
 button_spreis_add->set_sensitive(false);
 

 gruppenwahl->set_value(Kundengruppe::default_ID);
 kundenauswahl->reset();
 kundenauswahl->EinschraenkenKdGr(gruppenwahl->get_value());
 kundenauswahl->setExpandStr1(true);
 kundenauswahl->setExpandStr2(true); 
 geburtstag->setLabel("");
 geburtstag->setExpandYear(false);
 
 rng_an->EinschraenkenKdGr(KundengruppeID::Rechnungsadresse);
 lfr_an->EinschraenkenKdGr(KundengruppeID::Lieferadresse);

  betreuer->setExpandStr1(true);
  betreuer->setExpandStr2(true); 


 scc_verkaeufer->reset();
 scc_verkaeufer->EinschraenkenKdGr(KundengruppeID::Verkaeufer);
 scc_verkaeufer->setExpandStr1(true);
 scc_verkaeufer->setExpandStr2(true); 

 connectFkt();
}

void windowTop::saveAll()
{
   try{
   if(UpdateAdresse!=0) kundendaten->update_e(UpdateAdresse);
   if(UpdateFirma!=0) kundendaten->update_e(UpdateFirma);
   if(UpdateBank!=0) kundendaten->update_e(UpdateBank);
   if(UpdateSonst!=0) kundendaten->update_e(UpdateSonst);
   if (UpdatePerson!=0) update_person();
   
   clear_update_bits();
   
   }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}

void windowTop::clear_update_bits()
{  UpdateAdresse=Kunde::UpdateBitsAdresse(0);
   UpdateFirma=Kunde::UpdateBitsFirma(0);
   UpdateBank=Kunde::UpdateBitsBank(0);
   UpdateSonst=Kunde::UpdateBitsSonst(0);
   UpdatePerson=Person::UpdateBits(0);
}

gint windowTop::on_delete_event(GdkEventAny*)
{
 on_buttonBeenden_clicked();
 return true;
}


void windowTop::on_buttonBeenden_clicked()
{   
   saveAll();
   saveKundenKontakt();
//   update_person(); passiert in saveAll, CP
   Gtk::Main::instance()->quit();
}


   // Details


void windowTop::clear_entrys()
{   
   frame_adresse->set_sensitive(false);
   frame_kundenkontakt->set_sensitive(false);
   fire_enabled=false;
   
//   spinbuttonGruppenNr->set_value(0);
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
   landesbox->clear();

   // Details
   scc_verkaeufer->clear();
   scc_anrede->clear();
   betreuer->reset();
   spinbutton_Flaeche->set_value(0);
   spinbutton_Kundenumsatz->set_value(0);
   spinbutton_Planumsatz->set_value(0);
   spinbutton_Rabatt->set_value(0);
   spinbutton_Mitarbeiter->set_value(1);
   spinbutton_Umsatz->set_value(0);
   spinbutton_skontofrist1->set_value(0);
   spinbutton_skontosatz1->set_value(0);
   spinbutton_skontofrist2->set_value(0);
   spinbutton_skontosatz2->set_value(0);
   spinbutton_skontofrist3->set_value(0);
   spinbutton_skontosatz3->set_value(0);
   entryVerein->set_text("");
   spinbutton_einzugRabatt->set_value(0);
   entryBankKonto->set_text("");
   entry_blz->set_text("");
   extartbez->reset();
   rng_an->reset();
   lfr_an->reset();
   textNotiz->delete_text(0,textNotiz->get_length());;
   
   checkbutton_zeilenrabatt->set_active(false);
   checkbuttonBankeinzug->set_active(false);
   checkbuttonRchngAdr->set_active(false);
   checkbuttonLieferAdr->set_active(false);
   checkbutton_lieferung_frei_haus->set_active(true);
   
   kundendaten=H_Kunde(Kunde::none_id);
//   telefon.erase(telefon.begin(),telefon.end());

   ab_an_rngadresse->set_active(false);
   checkbutton_rng_an_postfach->set_active(false);

   clear_update_bits();
   fire_enabled=true;
   kunden_status->set_active(true);
   preisautomatik->set_active(false);

   iban_entry->set_text("");
   cab_entry->set_text("");
   abi_entry->set_text("");
   bankname_italy->set_text("");
   iban_save->set_sensitive(false);
   iban_abbruch->set_sensitive(false);      
   riba_save->set_sensitive(false);
   riba_abbruch->set_sensitive(false);     
   zahlverfahren_book->set_page(PAGE_DTAUS); 
   
   aufnotiz->delete_text(0,-1);
   rngnotiz->delete_text(0,-1);
   liefnotiz->delete_text(0,-1);
   aufnotiz_save->set_sensitive(false);
   rngnotiz_save->set_sensitive(false);   
   liefnotiz_save->set_sensitive(false);   
   
   geburtstag->set_value(ManuProC::Datum());
   
}


void windowTop::setTitles()
{
 std::vector<std::string> preis;
 preis.push_back("Suchreihenfolge");
 preis.push_back("Bezeichnung");
 preis.push_back("Nr");
 SonderPreislisteTree->setTitles(preis);
 
 std::vector<std::string> grp_titel(alle_gruppen->Cols());
 grp_titel[Data_Gruppe::GRP_ID]="Nr.";
 grp_titel[Data_Gruppe::GRP_OBID]="Obergruppe";
 grp_titel[Data_Gruppe::GRP_BEZ]="Gruppe";
 grp_titel[Data_Gruppe::GRP_KOMM]="Kommentar"; 
 alle_gruppen->setTitles(grp_titel); 
 gewaehlte_gruppen->setTitles(grp_titel);  
}

void windowTop::on_kunden_status_toggled()
{   
 if(kundendaten->getNummer()==Kunde::none_id) return;
 try{
 kundendaten->setAktiv(kunden_status->get_active());
 }
 catch(SQLerror &e)
 {
  MyMessage *m=manage(new MyMessage()); m->Show(e);
 }
}

void windowTop::on_preisautomatik_clicked()
{  
 if(kundendaten->getNummer()==Kunde::none_id) return;
 try{
 	kundendaten->Preisautomatik(preisautomatik->get_active());
    }
 catch(SQLerror &e)
 {
  MyMessage *m=manage(new MyMessage()); m->Show(e);
 }

}


void windowTop::on_gruppenwahl_activate()
{  
 kundenauswahl->reset();
 kundenauswahl->EinschraenkenKdGr(gruppenwahl->get_value());
 clear_entrys();
 notebook_main->set_page(PAGE_KUNDE);
}


void windowTop::on_riba_save_clicked()
{
#ifdef MABELLA_EXTENSIONS 
 try{
 kundendaten->setABI_CAB(abi_entry->get_text(),cab_entry->get_text());
 abi_entry->set_text(kundendaten->getABI_Code(true));
 cab_entry->set_text(kundendaten->getCAB_Code());
 }  
 catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e); }   

 bankname_italy->set_text(kundendaten->getABIBankName());
 
 riba_save->set_sensitive(false);
 riba_abbruch->set_sensitive(false); 
#endif 
}

void windowTop::on_riba_abbruch_clicked()
{  
#ifdef MABELLA_EXTENSIONS 
 riba_save->set_sensitive(false);
 riba_abbruch->set_sensitive(false);
 
 try{
 abi_entry->set_text(kundendaten->getABI_Code());
 cab_entry->set_text(kundendaten->getCAB_Code());
 } 
 catch(SQLerror &e) 
  { if(e.Code()==100) 
      {
       abi_entry->set_text("");
       cab_entry->set_text("");
       riba_save->set_sensitive(false);
       riba_abbruch->set_sensitive(false);       
       return; 
      }
   MyMessage *m=manage(new MyMessage()); m->Show(e); 
  } 
#endif
}

void windowTop::on_iban_abbruch_clicked()
{
#ifdef MABELLA_EXTENSIONS 
 iban_save->set_sensitive(false);
 iban_abbruch->set_sensitive(false);  

 try{iban_entry->set_text(kundendaten->getIBAN_Code());}
 catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e); } 
#endif
}

void windowTop::on_iban_save_clicked()
{  
#ifdef MABELLA_EXTENSIONS 
 try{
 kundendaten->setIBAN(iban_entry->get_text());
 iban_entry->set_text(kundendaten->getIBAN_Code(true));
 }
 catch(SQLerror &e) 
   { MyMessage *m=manage(new MyMessage()); m->Show(e); } 

 iban_save->set_sensitive(false);
 iban_abbruch->set_sensitive(false);
#endif
}

gint windowTop::on_abi_entry_changed(GdkEventFocus *e)
{
 riba_save->set_sensitive(true);
 riba_abbruch->set_sensitive(true);  
 return false;
}

gint windowTop::on_cab_entry_changed(GdkEventFocus *e)
{  
 riba_save->set_sensitive(true);
 riba_abbruch->set_sensitive(true);  
 return false; 
}


gint windowTop::on_iban_entry_changed(GdkEventFocus *e)
{
 iban_save->set_sensitive(true);
 iban_abbruch->set_sensitive(true);    
 return false;  
}

void windowTop::on_zahlverfahren_book_switch_page(Gtk::Notebook_Helpers::Page *p0, guint pagenr)
{  
#ifdef MABELLA_EXTENSIONS 
 try{
 switch(enum_zahl_verfahren(pagenr))
   {
    case PAGE_DTAUS: break;
    case PAGE_RIBA:
    	 abi_entry->set_text(kundendaten->getABI_Code());
 	 cab_entry->set_text(kundendaten->getCAB_Code());
 	 bankname_italy->set_text(kundendaten->getABIBankName());
 	 break;
    case PAGE_LCR:
   	 iban_entry->set_text(kundendaten->getIBAN_Code(true)); 	 
   	 break;
    default: return;
   }
 }
 catch(SQLerror &e) { if(e.Code()==100) return;
 	MyMessage *m=manage(new MyMessage()); m->Show(e); } 
#endif
 
}


/////////////////////////////////////////////////////////////////////////
// Extras
/////////////////////////////////////////////////////////////////////////

/*
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
*/



void windowTop::on_aufnotiz_save_clicked()
{  
 if(kundendaten->getNummer()==Kunde::none_id) return; 
 
 kundendaten->setFixeNotiz(Kunde::AUF_NOTIZ,
 	aufnotiz->get_chars(0,aufnotiz->get_length()));
 aufnotiz_save->set_sensitive(false); 
}

void windowTop::on_aufnotiz_changed()
{  
 aufnotiz_save->set_sensitive(true);
}

void windowTop::on_liefnotiz_save_clicked()
{  
 if(kundendaten->getNummer()==Kunde::none_id) return; 
 
 kundendaten->setFixeNotiz(Kunde::LIEF_NOTIZ,
 	liefnotiz->get_chars(0,liefnotiz->get_length()));
 liefnotiz_save->set_sensitive(false); 
}

void windowTop::on_liefnotiz_changed()
{  
 liefnotiz_save->set_sensitive(true); 
}

void windowTop::on_rngnotiz_save_clicked()
{  
 if(kundendaten->getNummer()==Kunde::none_id) return; 
 
 kundendaten->setFixeNotiz(Kunde::RNG_NOTIZ,
 	rngnotiz->get_chars(0,rngnotiz->get_length()));
 rngnotiz_save->set_sensitive(false); 
}

void windowTop::on_rngnotiz_changed()
{  
 rngnotiz_save->set_sensitive(true);
}

void windowTop::load_notizen()
{
 if(kundendaten->getNummer()==Kunde::none_id) return; 

 gint pos=0; 
 std::string n;
 
 n=kundendaten->fixeNotiz(Kunde::AUF_NOTIZ);
 aufnotiz->insert_text(n.c_str(),n.size(),&pos);
 aufnotiz_save->set_sensitive(false);

 n=kundendaten->fixeNotiz(Kunde::RNG_NOTIZ); 
 rngnotiz->insert_text(n.c_str(),n.size(),&pos);
 rngnotiz_save->set_sensitive(false);
 
 n=kundendaten->fixeNotiz(Kunde::LIEF_NOTIZ); 
 liefnotiz->insert_text(n.c_str(),n.size(),&pos);
 liefnotiz_save->set_sensitive(false); 
 
}


void windowTop::on_selectrow_allegruppen(cH_RowDataBase leaf)
{
 gruppe_in->set_sensitive(true);
 allgrp=const_cast<Data_Gruppe*>(dynamic_cast<const Data_Gruppe*>(&*leaf)); 
}

void windowTop::on_unselectrow_allegruppen(gint row, gint column, GdkEvent *event)
{  
 gruppe_in->set_sensitive(false);
 allgrp=NULL;
}

void windowTop::on_selectrow_gewaehltegruppen(cH_RowDataBase leaf)
{  
 gruppe_out->set_sensitive(true);
 wahlgrp=const_cast<Data_Gruppe*>(dynamic_cast<const Data_Gruppe*>(&*leaf)); 
}

void windowTop::on_unselectrow_gewaehltegruppen(gint row, gint column, GdkEvent *event)
{  
 gruppe_out->set_sensitive(false);
 wahlgrp=NULL;
}



void windowTop::on_lfran_activate()
{
   
}

void windowTop::on_gruppe_in_clicked()
{  
 if(kundendaten->Id()==Kunde::none_id) return;
 kundendaten->putInGrp(allgrp->GrpId());
 showGruppen(); 
 allgrp=NULL;
}

void windowTop::on_gruppe_out_clicked()
{  
 if(kundendaten->Id()==Kunde::none_id) return;
 kundendaten->pullFromGrp(wahlgrp->GrpId());
 showGruppen();
 wahlgrp=NULL; 
}


void windowTop::on_clear_button_clicked()
{
 on_gruppenwahl_activate();
}


void windowTop::on_ab_an_rngadresse_toggled()
{
 if(kundendaten->Id()==Kunde::none_id) return;
   kundendaten->AB_an_rngadresse(ab_an_rngadresse->get_active());  
}

void windowTop::scc_verkaeufer_reset()
{  
}

