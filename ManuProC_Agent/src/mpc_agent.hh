// generated 2004/3/12 22:15:24 CET by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to mpc_agent.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _MPC_AGENT_HH
#  include "mpc_agent_glade.hh"
#  define _MPC_AGENT_HH

#ifdef DEF_VERKNR
#define VERKNR		DEF_VERKNR
#else
#define VERKNR		0
#endif


typedef enum {KDBOX_NR=0,KDBOX_NAME,KDBOX_ORT} KdEntries;
typedef enum {OD_ROW,OD_ARTICLE,OD_AMOUNT} OrderEntries;

class mpc_agent : public mpc_agent_glade
{  
        void on_beenden_activate();
        void on_neu_clicked();
        void on_senden_clicked();
        void on_kunde_activate();
        void on_activate_entry(int enr);
        void on_customer_clear_activate();
        void on_order_clear_clicked();
        void on_artikel_aktivate();
        void on_artikel_activate_entry(int enr);
        void on_spinbutton1_editing_done();
        void on_artikel_ok_clicked();
        void on_artikel_cancel_clicked();
        void on_orderid_entry_editing_done();
        void on_orderid_activate();
        void on_orderid_search(gboolean *cont,GtkSCContext context);

public:
 mpc_agent();

};
#endif
