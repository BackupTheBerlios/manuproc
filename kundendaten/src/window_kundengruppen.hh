// $Id: window_kundengruppen.hh,v 1.4 2006/01/10 12:08:27 christof Exp $

#ifndef _WINDOW_KUNDENGRUPPEN_HH
#  include "window_kundengruppen_glade.hh"
#  define _WINDOW_KUNDENGRUPPEN_HH

class windowTop;

class window_kundengruppen : public window_kundengruppen_glade
{	void on_gruppenwahl_activate();
        void on_schliessen_clicked();
        void neu();
        void loeschen();
        void entfernen();
        void add();
        void laden();
        void bearbeiten();
        
        windowTop *main;
public:
  window_kundengruppen(windowTop *m,int id);
};
#endif
