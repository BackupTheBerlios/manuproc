/* $Id: wfr_test.cc,v 1.2 2004/08/30 07:35:07 christof Exp $ */
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 2004  Adolf Petig GmbH. & Co. KG
    Developed by Christof Petig <christof.petig@wtal.de>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <string>
#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif
#include <WinFileReq.hh>

class testwindow : public Gtk::Window
{
        Gtk::HBox hbox;
        Gtk::Entry entry;
        Gtk::Button button;

    public:
	void popup_req()
	{  WinFileReq::create(SigC::slot(entry,&Gtk::Entry::set_text),
			entry.get_text(),"","txt","select file",true,this);
	}
	
        testwindow() 
           : hbox(false,0), button("...")
        {
            hbox.add( entry);
            hbox.add( button);
            add(hbox);
            hbox.show();
            entry.show();
            button.show();

            button.signal_clicked().connect(SigC::slot(*this,&testwindow::popup_req));
        }
};


int main( int argc, char **argv )
{   int opt;
    Gtk::Main m(argc, argv );
    testwindow w;

    w.show();

    m.run(w);
    return 0;
}
