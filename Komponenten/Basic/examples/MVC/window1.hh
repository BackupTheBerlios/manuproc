// generated 2002/7/5 22:59:43 CEST by christof@puck.petig-baender.de
// using glademm V1.1.0d_cvs
//

#ifndef _WINDOW1_HH
#define _WINDOW1_HH

#include "window1_glade.hh"
#include <int_SpinButton.hh>
#include <string_Label.hh>

class window1 : public window1_glade
{	friend class window1_glade;
        void loeschen();
        void int_changed(gpointer);
        void string_changed(gpointer);
        void someth_changed(gpointer);
        void bool_changed(gpointer);
        void randomize();
        
        int_SpinButton::Connection isconn;
        string_Label::Connection slconn;
public:
	window1();
};
#endif
