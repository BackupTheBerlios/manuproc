// generated 2001/6/6 11:43:39 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to window_neue_bank_anlegen.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOW_NEUE_BANK_ANLEGEN_HH
#  include "window_neue_bank_anlegen_glade.hh"
#  define _WINDOW_NEUE_BANK_ANLEGEN_HH

class windowTop;

class window_neue_bank_anlegen : public window_neue_bank_anlegen_glade
{   
        
        windowTop* hauptfenster;
        friend class window_neue_bank_anlegen_glade;
        void on_entry_bankname_activate();
        void on_entry_blz_activate();
        void on_button_uebernehmen_clicked();
        void on_button_abbrechen_clicked();
   public:
        window_neue_bank_anlegen(windowTop* h, string st, long int blz);
};
#endif
