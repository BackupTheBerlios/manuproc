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
typedef enum {OD_ROW,OD_ARTICLE,OD_NAME,OD_EAN,OD_AMOUNT} OrderEntries;

class mpc_agent : public mpc_agent_glade
{  
        void on_beenden_activate();
        void on_neu_clicked();
        void on_senden_clicked();
        void on_kunde_activate();
        void on_activate_entry(int enr);
        void on_clear_activate();
        void on_customer_search_clicked();
        void on_artikel_aktivate();
        void on_artikel_activate_entry(int enr);
        void on_menge_editing_done();
        void on_artikel_ok_clicked();
        void on_artikel_cancel_clicked();
        void on_orderid_activate();
        void on_orderid_search(gboolean *cont,GtkSCContext context);

        void on_article_entry_search(gboolean *cont,GtkSCContext context);
        void on_ean_entry_search(gboolean *cont,GtkSCContext context);
        void on_article_entry_activate();
        void on_ean_entry_activate();
        void on_width_entry_search(gboolean *cont,GtkSCContext context);
        void on_width_entry_activate();
        void on_color_entry_search(gboolean *cont,GtkSCContext context);
        void on_color_entry_activate();
        void on_makeup_entry_search(gboolean *cont,GtkSCContext context);
        void on_makeup_entry_activate();
        void on_artikel_del_clicked();
        void on_order_leaf_selected(cH_RowDataBase leaf);
        void on_order_leaf_unselected();

	void clear_order();
	void load_order(int oid);
	int create_new_order(int kdnr);
	void artikel_clean();

public:
 mpc_agent();

};
#endif
