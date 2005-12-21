// generated 2005/12/19 18:03:59 CET by christof@vesta.petig-baender.de
// using glademm V2.10.0.0
//
// newer (non customized) versions of this file go to datewin_popup.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _DATEWIN_POPUP_HH
#  include "datewin_popup_glade.hh"
#  define _DATEWIN_POPUP_HH
class datewin_popup : public datewin_popup_glade
{  
        
        void datum_clicked();
        void woche_clicked();
        void empty_clicked();
        void on_day_selected();
};
#endif
