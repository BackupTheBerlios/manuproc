#include "windowTop.hh"
#include <Aux/itos.h>
#include "window_neue_bank_anlegen.hh"
#include <SelectMatching.h>
#include "class_Preislisten.hh"
#include "MyMessage.h"


void windowTop::show_details()
{
  table_details->set_sensitive(true);
  fire_enabled=false;
   scc_verkaeufer->setContent(kundendaten->getVerkaeufer().name,kundendaten->getVerkaeufer().verknr);

   labelDetailKdNrEintrag->set_text(itos(kundendaten->Id()));
   labelDetailStandEintrag->set_text(kundendaten->stand());
   labelDetailFirmaEintrag->set_text(kundendaten->firma());

	spinbutton_Flaeche->set_value(kundendaten->flaeche());
	spinbutton_Mitarbeiter->set_value(kundendaten->mitarbeiter());
	spinbutton_Planumsatz->set_value(kundendaten->planumsatz());
	spinbutton_Umsatz->set_value(kundendaten->umsatz());
	spinbutton_Kundenumsatz->set_value(kundendaten->kundenumsatz());
	spinbutton_Rabatt->set_value(kundendaten->rabatt());
	spinbutton_einzugRabatt->set_value(kundendaten->einzugrabatt());
	spinbutton_Skontofrist->set_value(kundendaten->skontofrist());
	spinbutton_skontosatz->set_value(kundendaten->skontosatz());
   entry_lkz->set_text(kundendaten->lkz());
   entry_eigene_kundennr->set_text(kundendaten->UnsereKundenNr());
   spinbutton_lieferantenkonto->set_value(kundendaten->getLieferantenkonto());
   spinbutton_gegenkonto->set_value(kundendaten->getGegenkonto());
	entryVerein->set_text(kundendaten->verein());
	entryBankKonto->set_text(ulltos(kundendaten->getKtnr()));
	entry_blz->set_text(itos(kundendaten->getblz()));
   rng_an->set_value(kundendaten->Rngan());
   extartbez->set_value(kundendaten->Schema());
   preisliste->set_value(kundendaten->preisliste());

   Waehrung->set_value(kundendaten->getWaehrung());

   checkbutton_zeilenrabatt->set_active(kundendaten->zeilenrabatt());
   checkbuttonLieferAdr->set_active(kundendaten->isLieferadresse());
   checkbuttonRchngAdr->set_active(kundendaten->isRechnungsadresse());
   checkbutton_rng_an_postfach->set_active(kundendaten->Rng_an_postfach());
   checkbutton_entsorgung->set_active(kundendaten->entsorgung());

   try{
   fill_bank_bei(kundendaten->getblz());
    }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
   Gtk::Menu_Helpers::SelectMatching(*optionmenu_bankbei,(gpointer)kundendaten->getindex());
   textNotiz->delete_text(0,textNotiz->get_length());
   gint pos=0;
   textNotiz->insert_text(kundendaten->notiz().c_str(), (kundendaten->notiz()).size(), &pos);
   checkbuttonBankeinzug->set_active(kundendaten->bankeinzug());
   fillSPreis();

   label_speichern->set_text("");
   SPreisBox->clear();
   fire_enabled=true;
   spinbutton_Flaeche->grab_focus();
}

void windowTop::on_entry_blz_activate()
{
 unsigned long int blz = strtol(entry_blz->get_text().c_str(),NULL,10);
 try{
 int bank_index = fill_bank_bei(blz);
 if (bank_index==0) 
  {
    label_speichern->set_text("WARNUNG: Bankleitzahl unbekannt");
    std::string st = "Bankleitzahl unbekannt, soll eine neue Bank angelegt werden?";
    manage(new window_neue_bank_anlegen(this,st,blz));
  }
 else  
  {
   kundendaten->set_bankindex(bank_index);
   saveAll();
  }
 }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}

void windowTop::on_button_neue_bank_clicked()
{
  manage(new window_neue_bank_anlegen(this,"",0));
}

void windowTop::neue_bank_uebernehmen(unsigned long int bank_index)
{
// entry_blz->set_text(itos(bank_index));
 entry_blz->set_text(itos(kundendaten->getblz()));
 try{
 kundendaten->get_blz_from_bankindex(bank_index);
 fill_bank_bei(kundendaten->getblz());
 }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
 changedFktB(Kunde::FBankindex);
 saveAll();
}



void windowTop::scc_verkaeufer_activate()
{
  changedFktS(Kunde::FVerknr);
}

void windowTop::fillSPreis()
{
  std::vector<cH_RowDataBase> datavec;
  try{
  sonder_preis_liste=kundendaten->Sonderpreislisten();
  for(std::list<PreisListe::ID>::const_iterator i=sonder_preis_liste.begin();i!=sonder_preis_liste.end();++i)
   {
     cH_PreisListe P(*i);
     datavec.push_back(new Data_SPreis(P));
   }
  SonderPreislisteTree->setDataVec(datavec);
   }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}

void windowTop::on_button_del_preisliste_clicked()
{
  cH_RowDataBase rdb(SonderPreislisteTree->getSelectedRowDataBase());
  const Data_SPreis *dt=dynamic_cast<const Data_SPreis*>(&*rdb);
  cH_PreisListe PL=dt->getPreisListe();
  sonder_preis_liste.remove(PL->Id());
  saveSonderpreisliste();
}

void windowTop::on_button_spreis_add_clicked()
{
 sonder_preis_liste.push_back(SPreisBox->get_value());
 button_spreis_add->set_sensitive(false);
 saveSonderpreisliste();
}

void windowTop::saveSonderpreisliste()
{
  try{
   kundendaten->setSonderpreisliste(sonder_preis_liste);
   fillSPreis();
  }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}



void windowTop::on_SPreisListe_activate()
{
 button_spreis_add->set_sensitive(true);
}

void windowTop::on_spreis_leaf_selected(cH_RowDataBase d)
{
  button_del_preisliste->set_sensitive(true);
}

void windowTop::on_spreis_unselect_row(gint row, gint column, GdkEvent *event)
{
  button_del_preisliste->set_sensitive(false);
}
