// generated 2004/5/11 13:36:34 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0.2
//
// newer (non customized) versions of this file go to kunden_selector.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _KUNDEN_SELECTOR_HH
#  include "kunden_selector_glade.hh"
#  define _KUNDEN_SELECTOR_HH

#include <FetchIStream.h>


class kunden_selector : public kunden_selector_glade
{  
 std::string *nr,*name,*ort;
        
        void on_cancel_kunde_select_clicked();
        void on_ok_kunde_select_clicked();
        void on_kundenlist_leaf_selected(cH_RowDataBase d);

public:
 kunden_selector(Query &q,std::string *_nr,std::string *_name,std::string *_ort);
 
};
#endif
