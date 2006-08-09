// $Id: auftrag_provision.hh,v 1.6 2006/08/09 15:36:51 christof Exp $

#ifndef _AUFTRAG_PROVISION_HH
#  include "auftrag_provision_glade.hh"
#  define _AUFTRAG_PROVISION_HH

#include <Auftrag/AuftragFull.h>
#include <Lieferschein/RechnungVoll.h>
#include <gtkmm/liststore.h>

class auftrag_provision : public auftrag_provision_glade
{  
        
        void on_prov_ok_clicked();
        void on_prov_apply_clicked();
//        void on_prov_cancel_clicked();
        void on_prov_enable_toggled();
        void on_prov_verk_activate();
        void on_prov_provsatz_changed();

 void fillProvEntryList();
 
 enum Run_Mode {UNKNOWN=0, AUFTRAG, RECHNUNG} run_mode;

 AuftragFull *aufp;
 RechnungVoll *rngp;
 
 void init_prov_dialog();
 void init();
 
 ManuProcEntity<>::ID Id() const;
 ManuProC::Datum getDatum() const;
 Kunde::ID getKundennr() const;
 Kunde::ID getVerknr() const;
 void LoadEntries();
 void setVerknr(const Kunde::ID) throw(SQLerror);
 
  struct Columns : Gtk::TreeModelColumnRecord
  { Gtk::TreeModelColumn<Glib::ustring> zeile,stueck,artikel,epreis,rabatt;
    Gtk::TreeModelColumn<Glib::ustring> gpreis,provsatz;
    
    Columns();
  };
  Columns cols;
  Glib::RefPtr<Gtk::ListStore> store;
public:
 auftrag_provision(AuftragFull *auftrag);
 auftrag_provision(RechnungVoll *rechnung); 
};
#endif

