// generated 2001/6/5 15:00:29 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to window_neue_Kundennummer.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOW_NEUE_KUNDENNUMMER_HH
#  include "window_neue_Kundennummer_glade.hh"
#  define _WINDOW_NEUE_KUNDENNUMMER_HH
#include "windowTop.hh"

class windowTop;

class window_neue_Kundennummer : public window_neue_Kundennummer_glade
{   
        
        windowTop* hauptfenster;
        friend class window_neue_Kundennummer_glade;
        void on_kundennr_activate();
        void on_firma_activate();
        void on_button_ok_clicked();
        void on_button_abbrechen_clicked();
   public:
       window_neue_Kundennummer::window_neue_Kundennummer(windowTop* h);
};
#endif
