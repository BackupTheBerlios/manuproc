// generated 2004/5/7 15:30:30 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to ManuProc_Starter.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _MANUPROC_STARTER_HH
#  include "ManuProc_Starter_glade.hh"
#  define _MANUPROC_STARTER_HH
#include <Aux/dbcapability.h>

class ManuProc_Starter : public ManuProc_Starter_glade
{  
 DBCapability *dbcapability;        
        
        
        void on_pps_start_clicked();
        void on_kunden_start_enter();
        void on_lager_start_clicked();
        void on_artikel_start_clicked();
        void on_preise_start_clicked();
        void on_quit_clicked();
        void on_fibu_start_clicked();
        void on_vertrieb_start_clicked();        
        void on_ooo_clicked();
        
public:
 ManuProc_Starter(void) throw(SQLerror);    
};
#endif
