// generated 2001/6/20 15:16:37 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to with_class.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WITH_CLASS_HH
#  include "with_class_glade.hh"
#  define _WITH_CLASS_HH
class with_class : public with_class_glade
{   
        
        friend class with_class_glade;
        void on_Beenden_activate();
        void on_leaf_selected(cH_RowDataBase d);
   public:
      with_class();
};
#endif
