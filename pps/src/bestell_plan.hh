// generated 2005/1/27 10:54:39 CET by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// newer (non customized) versions of this file go to bestell_plan.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _BESTELL_PLAN_HH
#  include "bestell_plan_glade.hh"
#  define _BESTELL_PLAN_HH

#include <vector.h>
#include <string.h>
#include <Artikel/ArtikelBase.h>

class bestell_plan : public bestell_plan_glade
{  
 std::vector<ArtikelBase> artikel;
 ArtikelBase akt_artikel;
 int akt_index;
 
        void on_bp_quit_clicked();
        void on_prev_artikel_clicked();
        void on_next_artikel_clicked();
        void on_plan_artikel_activate();
 void clear_all();
 void load_artikel_list();
 void load_data(const ArtikelBase a) throw(SQLerror);
 void set_akt_index();
 
public:
 bestell_plan(const ArtikelBase ab=ArtikelBase());
        
};
#endif

