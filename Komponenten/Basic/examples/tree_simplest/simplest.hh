// generated 2001/6/20 15:16:37 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to simplest.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _SIMPLEST_HH
#  include "simplest_glade.hh"
#  define _SIMPLEST_HH
class simplest : public simplest_glade
{   
        
        friend class simplest_glade;
        void on_Beenden_activate();
        void on_leaf_selected(cH_RowDataBase d);
   public:
      simplest();
};
#endif
