#include <Misc/i18n.h>
#include "windowTop.hh"
#include <Aux/itos.h>
#include "window_neue_bank_anlegen.hh"
#include <SelectMatching.h>
#include "kundendaten_aux.hh"
#include "MyMessage.h"
#include <Kunde/Kundengruppe.h>

void windowTop::show_details()
{
  table_details->set_sensitive(true);
  fire_enabled=false;
   if (kundendaten->VerkNr()>0)
     scc_verkaeufer->set_value(kundendaten->VerkNr());
   if(kundendaten->getBetreuer() != Kunde::none_id)
     betreuer->set_value(kundendaten->getBetreuer());
   else
     betreuer->reset();

   labelDetailStandEintrag->set_text(kundendaten->stand());

	spinbutton_Flaeche->set_value(kundendaten->flaeche());
	spinbutton_Mitarbeiter->set_value(kundendaten->mitarbeiter());
	spinbutton_Planumsatz->set_value(kundendaten->planumsatz().as_float());
	spinbutton_Umsatz->set_value(kundendaten->umsatz().as_float());
	spinbutton_Kundenumsatz->set_value(kundendaten->kundenumsatz().as_float());
//	spinbutton_Rabatt->set_value(kundendaten->rabatt().as_float());
   rabatt=kundendaten->rabatt().as_float();
   spinbutton_firmenpapier->set_value(kundendaten->anzahl_ausdruck_firmenpapier());
   spinbutton_weissespapier->set_value(kundendaten->anzahl_ausdruck_weissespapier());
   entry_eigene_kundennr->set_text(kundendaten->UnsereKundenNr());
   spinbutton_lieferantenkonto->set_value(kundendaten->getLieferantenkonto());
   spinbutton_gegenkonto->set_value(kundendaten->getGegenkonto());
   spinbutton_debitorenkonto->set_value(kundendaten->getDebitorenkonto());   
	entryVerein->set_text(kundendaten->verein());
	entryBankKonto->set_text(ulltos(kundendaten->getKtnr()));
	entry_blz->set_text(itos(kundendaten->getblz()));
   rng_an->set_value(kundendaten->Rngan());
   lfr_an->set_value(kundendaten->Lfran());

   // zeige nur Kunden, die ein eigenes Schema haben
   extartbez->Einschraenkung(" and kundennr in (select distinct extartbezid"
			" from extbezschema)");
   extartbez->set_value(kundendaten->Schema());

   Waehrung->set_value(kundendaten->getWaehrung());
   geburtstag->set_value(kundendaten->getGebDatum());
   geburtstag->setLabel(_("Geburtstag"));

   checkbutton_zeilenrabatt->set_active(kundendaten->zeilenrabatt());
   checkbuttonLieferAdr->set_active(kundendaten->isInGrp(KundengruppeID::Lieferadresse));
   checkbuttonRchngAdr->set_active(kundendaten->isInGrp(KundengruppeID::Rechnungsadresse));
   checkbuttonAuftrAdr->set_active(kundendaten->isInGrp(KundengruppeID::Auftragsadresse));
   checkbutton_rng_an_postfach->set_active(kundendaten->Rng_an_postfach());
   ab_an_rngadresse->set_active(kundendaten->AB_an_rngadresse());   
   checkbutton_entsorgung->set_active(kundendaten->entsorgung());
   checkbutton_lieferung_frei_haus->set_active(
		kundendaten->get_lieferung_frei_haus());

   try{
   fill_bank_bei(kundendaten->getblz());
    }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
//   Gtk::Menu_Helpers::SelectMatching(*optionmenu_bankbei,(gpointer)kundendaten->getindex());
   textNotiz->get_buffer()->erase(textNotiz->get_buffer()->begin(),textNotiz->get_buffer()->end());
   Gtk::TextBuffer::iterator pos=textNotiz->get_buffer()->begin();
   textNotiz->get_buffer()->insert(pos,kundendaten->notiz());
//   checkbuttonBankeinzug->set_active(kundendaten->bankeinzug());
//	spinbutton_einzugRabatt->set_value(kundendaten->einzugrabatt());
//	spinbutton_Skontofrist->set_value(kundendaten->skontofrist());
//	spinbutton_skontosatz->set_value(kundendaten->skontosatz());
   show_zahlungsziel();

   fillSPreis();

   label_speichern->set_text("");
   SPreisBox->clear();
   fire_enabled=true;
   spinbutton_Flaeche->grab_focus();
   preisautomatik->set_active(kundendaten->Preisautomatik());
}

void windowTop::show_zahlungsziel()
{
   zahlungsartbox->set_value(kundendaten->zahlungsart()->Id());
   checkbuttonBankeinzug->set_active(kundendaten->zahlungsart()->getBankeinzug());
	spinbutton_einzugRabatt->set_value(kundendaten->zahlungsart()->getEinzugrabatt().as_float());
	spinbutton_zahlungsfrist->set_value(kundendaten->zahlungsart()->getZahlungsfrist());
	spinbutton_skontofrist1->set_value(kundendaten->zahlungsart()->getSkonto(1).skontofrist);
	spinbutton_skontosatz1->set_value(kundendaten->zahlungsart()->getSkonto(1).skontosatz.as_float());
	spinbutton_skontofrist2->set_value(kundendaten->zahlungsart()->getSkonto(2).skontofrist);
	spinbutton_skontosatz2->set_value(kundendaten->zahlungsart()->getSkonto(2).skontosatz.as_float());
	spinbutton_skontofrist3->set_value(kundendaten->zahlungsart()->getSkonto(3).skontofrist);
	spinbutton_skontosatz3->set_value(kundendaten->zahlungsart()->getSkonto(3).skontosatz.as_float());
}


void windowTop::on_entry_blz_activate()
{
 unsigned long int blz = strtol(entry_blz->get_text().c_str(),NULL,10);
// if(blz==0) return;
 try{
 int bank_index = fill_bank_bei(blz);
 if (bank_index==0 && blz!=0) 
  {
    label_speichern->set_text(_("WARNUNG: Bankleitzahl unbekannt"));
    std::string st = _("Bankleitzahl unbekannt, soll eine neue Bank angelegt werden?");
    manage(new window_neue_bank_anlegen(this,st,blz));
  }
 else  
  {
   kundendaten->set_bankindex(bank_index);
   changedFktB(Kunde::FBankindex);   
   saveAll();
  }
 }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}

void windowTop::on_button_neue_bank_clicked()
{
  manage(new window_neue_bank_anlegen(this,"",strtol(entry_blz->get_text().c_str(),NULL,10)));
}

void windowTop::neue_bank_uebernehmen(unsigned long int bank_index)
{
// entry_blz->set_text(itos(bank_index));
 try{
 kundendaten->get_blz_from_bankindex(bank_index);
 fill_bank_bei(kundendaten->getblz());
 entry_blz->set_text(itos(kundendaten->getblz()));
 }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
 changedFktB(Kunde::FBankindex);
 saveAll();
}



void windowTop::scc_verkaeufer_activate()
{
 if(kundendaten->Id() == Kunde::none_id) return;

 Transaction tr;

 Kunde::ID old_verk=kundendaten->VerkNr();

 try {
 // initial setting for new customers in prov_config table
 if(kundendaten->VerkNr()==Kunde::none_id)
   {
    Query("delete from prov_config where kundennr=?") << kundendaten->Id();
    Query("insert into prov_config (artikel,provsatznr,kundennr) "
		" (select distinct artikel,provsatznr,? from prov_config "
		" where provsatznr=0)") << kundendaten->Id();
   }

 cH_Kunde new_verk(scc_verkaeufer->get_value());
 if(new_verk->Id() != kundendaten->VerkNr())
 {
  kundendaten->setVerkNr(new_verk->Id());
 
  Query q_del("delete from prov_verkaeufer where kundennr=?");
  q_del << kundendaten->Id();
  SQLerror::test(__FILELINE__,100);
  
  Query qi("SELECT p.provsatz1,p.provsatz2,p.rabatt from"
    " prov_verkaeufer p join kunden k on (p.kundennr=k.kundennr and "
    "  k.verknr=?) "
    " where p.verknr=? group by p.rabatt,p.provsatz1,p.provsatz2 "
    " order by rabatt,count(*) desc");
    
  qi << new_verk->Id()
	<< new_verk->Rngan();
  SQLerror::test(__FILELINE__,100);
	
  FetchIStream fi=qi.Fetch();
  fixedpoint<2> p1,p2,rab_old=-1,rab=0;  
  while(fi.good())
    {
    fi >> p1 >> p2 >> rab;
    if(rab!=rab_old)
      {Query("insert into prov_verkaeufer "
            " (verknr,kundennr,provsatz1,provsatz2,rabatt) values "
              "(?,?,?,?,?)")
              << new_verk->Rngan()
              << kundendaten->Id()
              << p1 << p2 << rab;
       rab_old=rab; 
      }
    fi=qi.Fetch();
    }

  
  Query q("update kunden set stand=now(), verknr=? where kundennr=?");
   q << Query::NullIf(scc_verkaeufer->get_value()->Id(),Kunde::none_id) << kundendaten->Id();
 }
 
 }

 catch(SQLerror &e)
	{ MyMessage *m=manage(new MyMessage()); m->Show(e); 
          scc_verkaeufer->set_value(old_verk);
          return;}
 tr.commit();

//  changedFktS(Kunde::FVerknrku);
}

void windowTop::fillSPreis()
{
  std::vector<cH_RowDataBase> datavec;
  try{
  for(std::list<std::pair<int,PreisListe::ID> >::const_iterator i=kundendaten->Preislisten().begin();
  		i!=kundendaten->Preislisten().end();++i)
   {
     cH_PreisListe P(i->second);
     datavec.push_back(new Data_SPreis(i->first,P));
   }
  SonderPreislisteTree->setDataVec(datavec);
   }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}

void windowTop::on_button_del_preisliste_clicked()
{
  cH_RowDataBase rdb(SonderPreislisteTree->getSelectedRowDataBase());
  const Data_SPreis *dt=dynamic_cast<const Data_SPreis*>(&*rdb);
  try {  kundendaten->deletePreisListe(dt->getPreisListe()->Id());}
  catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
   fillSPreis();
}

void windowTop::on_button_spreis_add_clicked()
{
 try{ kundendaten->push_backPreisListe(SPreisBox->get_value());}
 catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
 button_spreis_add->set_sensitive(false);
   fillSPreis();
}


void windowTop::on_SPreisListe_activate()
{
 button_spreis_add->set_sensitive(true);
}

void windowTop::on_spreis_leaf_selected(cH_RowDataBase d)
{
  button_del_preisliste->set_sensitive(true);
}

void windowTop::on_spreis_unselect_row()
{
  button_del_preisliste->set_sensitive(false);
}

void windowTop::on_betreuer_activate()
{  
  changedFktS(Kunde::FBetreuer);
}

