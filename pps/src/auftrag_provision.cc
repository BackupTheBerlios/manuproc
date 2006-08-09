// $Id: auftrag_provision.cc,v 1.17 2006/08/09 15:36:51 christof Exp $

#include "config.h"
#include <ManuProCConfig.h>
#include "auftrag_provision.hh"
#include <Gtk_OStream.h>
#include "MyMessage.h"  
#include <Kunde/Kunde.h>
#include "ja_nein_frage.hh"
#include <Misc/i18n.h>
#include <Misc/ucompose.hpp>

extern MyMessage *meldung;

#include <Aux/dbcapability.h>
extern DBCapability *dbcapability;


void auftrag_provision::init_prov_dialog()
{
 prov_aufnr->set_text(itos(Id()));
 prov_aufdatum->set_value(getDatum());
 prov_aufdatum->setLabel("");
 prov_aufkunde->set_value(getKundennr());

#ifdef MABELLA_EXTENSIONS
 prov_verkaeufer->EinschraenkenKdGr(KundengruppeID::Verkaeufer);

 fillProvEntryList();

 if(getVerknr()==Kunde::none_id)
   {
    verkprov_frame->set_sensitive(false);
    prov_enable->set_active(false);
    prov_verkaeufer->reset();
   }
 else
   {
    prov_enable->set_active(true);
    verkprov_frame->set_sensitive(true);   
    try {prov_verkaeufer->set_value(getVerknr());
    }
    catch(SQLerror &e)
    { if(e.Code()==100) 
        meldung->Show(String::ucompose(_("Der Verkäufer mit der Nr. %1 wurde nicht gefunden")
            ,getVerknr()));
    }
   }
#endif


}

void auftrag_provision::init()
{
 store=Gtk::ListStore::create(cols);
 prov_aufentries->set_model(store);
 prov_aufentries->append_column(_("Zeile"),cols.zeile);
 prov_aufentries->append_column(_("Stück"),cols.stueck);
 prov_aufentries->append_column(_("Artikel"),cols.artikel);
 prov_aufentries->append_column(_("Einzel-\npreis"),cols.epreis);
 prov_aufentries->append_column(_("Rabatt"),cols.rabatt);
 prov_aufentries->append_column(_("Gesamt-\npreis"),cols.gpreis);
 prov_aufentries->append_column(_("Provions-\nsatz"),cols.provsatz);
 prov_aufentries->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
}

auftrag_provision::auftrag_provision(RechnungVoll *rechnung) :
 aufp(),rngp(rechnung)
{
 entry_frame->set_label(_("Rechnungspositionen"));
 main_frame->set_label(_("Rechnung"));
 run_mode=RECHNUNG;
 init();
 init_prov_dialog();

 DBCapability::WhiteColumn wc("rechnung","verknr");

 verkprov_frame->set_sensitive(
		dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::UPDATE)) 
		);
}

auftrag_provision::auftrag_provision(AuftragFull *auftrag) :
aufp(auftrag),rngp()
{
 entry_frame->set_label(_("Auftragspositionen"));
 main_frame->set_label(_("Auftrag"));
 run_mode=AUFTRAG;
 init();
 init_prov_dialog();

 DBCapability::WhiteColumn wc("auftrag","verknr");

 verkprov_frame->set_sensitive(
		dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::UPDATE)) 
		);
}


void auftrag_provision::fillProvEntryList()
{
 store->clear();

 Gtk::OStream os(prov_aufentries);
 
 assert(run_mode!=UNKNOWN);
 if(run_mode==AUFTRAG)
 for(AuftragFull::const_iterator i=aufp->begin(); i!=aufp->end(); ++i)
   {
    os << (*i).getZnr() << "\t";
    os << (*i).getStueck() << "\t";
    cH_ArtikelBezeichnung ab((*i).Artikel());
    os << ab->Bezeichnung() << "\t";
    os << (*i).EPreis().Wert().String() << "\t";
    os << (*i).Rabatt().String() << "\t";
    os << (*i).GPreis().Wert().String() << "\t";
    os << (*i).ProvSatz().String() << "\n";
   }
 else  
 if(run_mode==RECHNUNG)
 for(RechnungVoll::const_iterator i=rngp->begin(); i!=rngp->end(); ++i)
   {
    os << (*i).Zeile() << "\t";
    os << (*i).Stueck() << "\t";
    cH_ArtikelBezeichnung ab((*i).Artikel());
    os << ab->Bezeichnung() << "\t";
    os << (*i).getPreis().Wert().String() << "\t";
    os << (*i).Rabatt().String() << "\t";
    os << (*i).GPreis().Wert().String() << "\t";
    os << (*i).ProvSatz().String() << "\n";
   } 
}

void auftrag_provision::on_prov_ok_clicked()
{  
}

auftrag_provision::Columns::Columns()
{ add(zeile); add(stueck); add(artikel); add(epreis); add(rabatt);
  add(gpreis); add(provsatz);
}


void auftrag_provision::on_prov_apply_clicked()
{  
 if(!prov_aufentries->get_selection()->count_selected_rows()) 
   return;

 ja_nein_frage fr(_("Wollen Sie jetzt die Provisionssätze ändern ?"));

 fr.set_transient_for(*this);
 int ret=fr.run(); 

 if(ret!=Gtk::RESPONSE_YES) return;

#ifdef MABELLA_EXTENSIONS
 if(!prov_aufentries->get_selection()->count_selected_rows()) 
   return;
  assert(run_mode!=UNKNOWN);
  Gtk::TreeSelection::ListHandle_Path lh=prov_aufentries->get_selection()->get_selected_rows();
  if(run_mode==AUFTRAG)
  {
   for(Gtk::TreeSelection::ListHandle_Path::iterator s=lh.begin();s!=lh.end(); ++s)
    {
     Gtk::TreeIter it=prov_aufentries->get_model()->get_iter(*s);
     int znr(atoi(static_cast<Glib::ustring>((*it)[cols.zeile]).c_str()));

     Query("update auftragentry set provsatz=? where "
	" (instanz,auftragid,zeilennr)=(?,?,?)")
	<< prov_provsatz->get_value()
	<< aufp->InstanzID()
	<< aufp->Id()
	<< znr;
     SQLerror::test(__FILELINE__);
    }
   } 
  else  
  if(run_mode==RECHNUNG)
  {
   for(Gtk::TreeSelection::ListHandle_Path::iterator s=lh.begin();s!=lh.end(); ++s)
    {
     Gtk::TreeIter it=prov_aufentries->get_model()->get_iter(*s);
     int znr(atoi(static_cast<Glib::ustring>((*it)[cols.zeile]).c_str()));

     Query("update rechnungentry set provsatz=? where "
	" (rngid,zeilennr)=(?,?)")
	<< prov_provsatz->get_value()
	<< Id()
	<< znr;
     SQLerror::test(__FILELINE__);
    }
   } 
#endif

 assert(run_mode!=UNKNOWN);
 LoadEntries();
 fillProvEntryList();
 prov_provsatz->set_value(0);
}

void auftrag_provision::LoadEntries()
{
 if(run_mode==AUFTRAG) aufp->loadEntries();
 else if(run_mode==RECHNUNG) rngp->loadEntries(true);
}


/*void auftrag_provision::on_prov_cancel_clicked()
{  
}
*/

void auftrag_provision::on_prov_enable_toggled()
{  
 if(prov_enable->get_active())
   {
    cH_Kunde kunde(getKundennr());
    try {
    	 setVerknr(kunde->VerkNr());
    }
    catch(SQLerror &e)
    {
    	meldung->Show(e); return;
    }
    prov_verkaeufer->grab_focus();
//    verkprov_frame->set_sensitive(true);

    try {prov_verkaeufer->set_value(getVerknr());
    }
    catch(SQLerror &e)
    { if(e.Code()==100) 
        meldung->Show(String::ucompose(_("Der Verkäufer mit der Nr. %1 "
            "wurde nicht gefunden"),getVerknr()));
    }
   }
 else
  {
    try {
    	 setVerknr(Kunde::none_id);
    }
    catch(SQLerror &e)
    {
    	meldung->Show(e); return;
    }
//    verkprov_frame->set_sensitive(false);
    prov_verkaeufer->reset();
  }


   
}

void auftrag_provision::on_prov_verk_activate()
{
 cH_Kunde verkaeufer(prov_verkaeufer->get_value());

 try {
     setVerknr(verkaeufer->Id());
 }
 catch(SQLerror &e)
   { meldung->Show(e); }
 
 cH_Kunde kunde(getKundennr());
 
 if(run_mode==AUFTRAG)
 for(AuftragFull::iterator i=aufp->begin(); i!=aufp->end(); ++i)
    {fixedpoint<2> p=kunde->getProvSatz_Artikel((*i).Artikel(),(*i).Rabatt());
     (*i).setProvSatz(p);
    }
 else if(run_mode==RECHNUNG)
 for(RechnungVoll::iterator i=rngp->begin(); i!=rngp->end(); ++i)
    {fixedpoint<2> p=kunde->getProvSatz_Artikel((*i).Artikel(),(*i).Rabatt());
     (*i).setProvSatz(p);
    }

 fillProvEntryList();
}


void auftrag_provision::on_prov_provsatz_changed()
{
 prov_apply->set_sensitive(true);  
}



ManuProcEntity<>::ID auftrag_provision::Id() const 
{if(run_mode==AUFTRAG) return aufp->Id();
 if(run_mode==RECHNUNG) return rngp->Id();
 return ManuProcEntity<>::none_id;
}


ManuProC::Datum auftrag_provision::getDatum() const
{
 if(run_mode==AUFTRAG) return aufp->getDatum();
 if(run_mode==RECHNUNG) return rngp->getDatum();
 return ManuProC::Datum();
}

Kunde::ID auftrag_provision::getKundennr() const
{
 if(run_mode==AUFTRAG) return aufp->getKundennr();
 if(run_mode==RECHNUNG) return rngp->KdNr();
 return ManuProcEntity<>::none_id;
}

Kunde::ID auftrag_provision::getVerknr() const
{
 if(run_mode==AUFTRAG) return aufp->getVerknr();
 if(run_mode==RECHNUNG) return rngp->getVerknr();
 return ManuProcEntity<>::none_id;
}



void auftrag_provision::setVerknr(const Kunde::ID vkid) throw(SQLerror)
{
 if(run_mode==AUFTRAG) return aufp->setVerknr(vkid);
 if(run_mode==RECHNUNG) return rngp->setVerknr(vkid);
}



