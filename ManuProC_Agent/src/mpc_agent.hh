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

typedef enum {KDBOX_NR=0,KDBOX_NAME,KDBOX_ORT} KdTitle;
typedef enum {OD_ROW,OD_ARTICLE,OD_AMOUNT} OderEntries;

class mpc_agent : public mpc_agent_glade
{  
 void on_beenden_activate();
 void on_kunde_activate();

public:
 mpc_agent();

};
#endif
