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

auftrag_provision::auftrag_provision(AuftragFull *auftrag) :
auf(auftrag)
{
 prov_aufnr->set_text(itos(auftrag->Id()));
 prov_aufdatum->set_value(auftrag->getDatum());
 prov_aufdatum->setLabel("");
 prov_aufkunde->set_value(auftrag->getKundennr());

#ifdef MABELLA_EXTENSIONS
 prov_verkaeufer->EinschraenkenKdGr(KundengruppeID::Verkaeufer);

 fillProvEntryList();

 if(auftrag->getVerknr()==Kunde::none_id)
   {
    verkprov_frame->set_sensitive(false);
    prov_enable->set_active(false);
   }
 else
   {
    prov_enable->set_active(true);
    verkprov_frame->set_sensitive(true);   
    try {prov_verkaeufer->set_value(auftrag->getVerknr());
    }
    catch(SQLerror &e)
    { if(e.Code()==100) meldung->Show(
	std::string("Der Verkäufer mit der Nr.:")+
	itos(auftrag->getVerknr())+" nicht gefunden");
    }
   }
#endif
}

void auftrag_provision::fillProvEntryList()
{
 prov_aufentries->clear();

 Gtk::OStream os(prov_aufentries);
 
 for(AuftragFull::const_iterator i=auf->begin(); i!=auf->end(); ++i)
   {
    os << (*i).getZnr() << "\t";
    os << (*i).getStueck() << "\t";
    cH_ArtikelBezeichnung ab(ArtikelBase((*i).ArtId()));
    os << ab->Bezeichnung() << "\t";
    os << (*i).EPreis().Wert().String() << "\t";
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
  for(Gtk::CList::SelectionList::iterator s=prov_aufentries->selection().begin();
	s!=prov_aufentries->selection().end(); ++s)
    {
     int znr(atoi((*s)->begin()->get_text().c_str()));

     Query("update auftragentry set provsatz=? where "
	" (instanz,auftragid,zeilennr)=(?,?,?)")
	<< prov_provsatz->get_value_as_float()
	<< auf->InstanzID()
	<< auf->Id()
	<< znr;
     SQLerror::test(__FILELINE__);
    }
#endif

 auf->loadEntries();
 fillProvEntryList();
 prov_provsatz->set_value(0);
}

void auftrag_provision::on_prov_cancel_clicked()
{  
}

void auftrag_provision::on_prov_enable_toggled()
{  
 if(prov_enable->get_active())
   {
    cH_Kunde kunde(auf->getKundennr());
    try {
    	auf->setVerknr(kunde->VerkNr());
    }
    catch(SQLerror &e)
    {
    	meldung->Show(e); return;
    }
    prov_verkaeufer->grab_focus();
    verkprov_frame->set_sensitive(true);

    try {prov_verkaeufer->set_value(auf->getVerknr());
    }
    catch(SQLerror &e)
    { if(e.Code()==100) meldung->Show(
	std::string("Der Verkäufer mit der Nr.:")+
	itos(auf->getVerknr())+" nicht gefunden");
    }
   }
 else
  {
    try {
    	auf->setVerknr(Kunde::none_id);
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
   auf->setVerknr(verkaeufer->Id());
 }
 catch(SQLerror &e)
   { meldung->Show(e); }
  
 for(AuftragFull::const_iterator i=auf->begin(); i!=auf->end(); ++i)
   {
//    (*i).setProvSatz();
   }

 fillProvEntryList();
}


void auftrag_provision::on_prov_provsatz_changed()
{
 prov_apply->set_sensitive(true);  
}
