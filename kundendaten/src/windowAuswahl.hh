// generated 2001/2/22 9:59:33 CET by thoma@ig23.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to windowAuswahl.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOWAUSWAHL_HH
#  include "windowAuswahl_glade.hh"
#  define _WINDOWAUSWAHL_HH
#include "glademm_support.hh"

#include "windowTop.hh"

class windowAuswahl : public windowAuswahl_glade
{   
        windowTop* hauptfenster;
           
        friend class windowAuswahl_glade;
        void on_clistAuswahl_select_row(gint row, gint column, GdkEvent *event);
        void on_radiobuttonSortiername_clicked();
        void on_radiobuttonKdNr_clicked();
        void on_windowAuswahl_destroy();
        void windowAuswahl::load_liste();
   public:
        windowAuswahl(windowTop* h);
        ~windowAuswahl();
};
#endif
