// generated 2001/6/7 12:35:12 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to window_Dialog_Bestaetigen.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOW_DIALOG_BESTAETIGEN_HH
#  include "window_Dialog_Bestaetigen_glade.hh"
#  define _WINDOW_DIALOG_BESTAETIGEN_HH

class windowTop;

class window_Dialog_Bestaetigen : public window_Dialog_Bestaetigen_glade
{   
        
        windowTop *hauptfenster;
        friend class window_Dialog_Bestaetigen_glade;
        void on_button_ok_clicked();
        void on_button_cancel_clicked();
   public:
      window_Dialog_Bestaetigen::window_Dialog_Bestaetigen(windowTop *h, string st);
};
#endif
