// generated 2003/6/3 11:30:07 MEST by jacek@ijacek.jacek.de
// using glademm V1.1.3f_cvs
//
// newer (non customized) versions of this file go to auftrag_provision.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _AUFTRAG_PROVISION_HH
#  include "auftrag_provision_glade.hh"
#  define _AUFTRAG_PROVISION_HH

#include <Auftrag/AuftragFull.h>

class auftrag_provision : public auftrag_provision_glade
{  
        
        void on_prov_ok_clicked();
        void on_prov_apply_clicked();
        void on_prov_cancel_clicked();
        void on_prov_enable_toggled();
        void on_prov_verk_activate();
        void on_prov_provsatz_changed();

 void auftrag_provision::fillProvEntryList();

 AuftragFull *auf;

public:
 auftrag_provision(AuftragFull *auftrag);
};
#endif

