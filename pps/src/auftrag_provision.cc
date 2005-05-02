// generated 2003/6/3 11:30:07 MEST by jacek@ijacek.jacek.de
// using glademm V1.1.3f_cvs
//
// newer (non customized) versions of this file go to auftrag_provision.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include <ManuProCConfig.h>
#include "auftrag_provision.hh"
#include <Gtk_OStream.h>
#include "MyMessage.h"  
#include <Kunde/Kunde.h>
#include "ja_nein_frage.hh"

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
   }
 else
   {
    prov_enable->set_active(true);
    verkprov_frame->set_sensitive(true);   
    try {prov_verkaeufer->set_value(getVerknr());
    }
    catch(SQLerror &e)
    { if(e.Code()==100) meldung->Show(
	std::string("Der Verkäufer mit der Nr.:")+
	itos(getVerknr())+" nicht gefunden");
    }
   }
#endif


}



auftrag_provision::auftrag_provision(RechnungVoll *rechnung) :
aufp(NULL),rngp(rechnung)
{
 main_frame->set_label("Rechnung");
 run_mode=RECHNUNG;
 init_prov_dialog();
 
 DBCapability::WhiteColumn wc("rechnung","verknr");

 verkprov_frame->set_sensitive(
		dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::UPDATE)) 
		);
}

auftrag_provision::auftrag_provision(AuftragFull *auftrag) :
aufp(auftrag),rngp(NULL)
{
 main_frame->set_label("Auftrag");
 run_mode=AUFTRAG;
 init_prov_dialog();
 

 DBCapability::WhiteColumn wc("auftrag","verknr");

 verkprov_frame->set_sensitive(
		dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::UPDATE)) 
		);
}


void auftrag_provision::fillProvEntryList()
{
 prov_aufentries->clear();

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

 for(guint i=0; i<prov_aufentries->columns().size(); i++)
   prov_aufentries->set_column_auto_resize(i,true);
}

void auftrag_provision::on_prov_ok_clicked()
{  
}


void auftrag_provision::on_prov_apply_clicked()
{  
 if(prov_aufentries->selection().empty()) 
   return;

 ja_nein_frage fr("Wollen Sie jetzt die Provisionssätze ändern ?");

 fr.set_transient_for(*this);
 int ret=fr.run(); 

 if(ret!=0) return;

#ifdef MABELLA_EXTENSIONS
  assert(run_mode!=UNKNOWN);
  if(run_mode==AUFTRAG)
  {
   for(Gtk::CList::SelectionList::iterator s=prov_aufentries->selection().begin();
	s!=prov_aufentries->selection().end(); ++s)
    {
     int znr(atoi((*s)->begin()->get_text().c_str()));

     Query("update auftragentry set provsatz=? where "
	" (instanz,auftragid,zeilennr)=(?,?,?)")
	<< prov_provsatz->get_value_as_float()
	<< aufp->InstanzID()
	<< aufp->Id()
	<< znr;
     SQLerror::test(__FILELINE__);
    }
   } 
  else  
  if(run_mode==RECHNUNG)
  {
   for(Gtk::CList::SelectionList::iterator s=prov_aufentries->selection().begin();
	s!=prov_aufentries->selection().end(); ++s)
    {
     int znr(atoi((*s)->begin()->get_text().c_str()));

     Query("update rechnungentry set provsatz=? where "
	" (rngid,zeilennr)=(?,?)")
	<< prov_provsatz->get_value_as_float()
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
    verkprov_frame->set_sensitive(true);

    try {prov_verkaeufer->set_value(getVerknr());
    }
    catch(SQLerror &e)
    { if(e.Code()==100) meldung->Show(
	std::string("Der Verkäufer mit der Nr.:")+
	itos(getVerknr())+" nicht gefunden");
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
    verkprov_frame->set_sensitive(false);
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



void auftrag_provision::setVerknr(const Kunde::ID) throw(SQLerror)
{
 if(run_mode==AUFTRAG) return aufp->setVerknr(prov_verkaeufer->get_value());
 if(run_mode==RECHNUNG) return rngp->setVerknr(prov_verkaeufer->get_value());
}



