/* $Id: db_test.cc,v 1.1 2003/04/08 08:56:54 christof Exp $ */
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998  Adolf Petig GmbH. & Co. KG
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
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <DoubleButton.h>
#include <iostream>

class testwindow : public Gtk::Window
{
        Gtk::VBox vbox;
        ManuProC::DoubleButton dbutton;
        Gtk::Label label1;

    public:
    	void on_clicked()
    	{  std::cout << "on_clicked\n";
    	}
    	void on_sclicked(int mousebutton)
    	{  std::cout << "on_sclicked ("<<mousebutton<<")\n";
    	}

        testwindow() 
           : vbox(false,0), label1("Test")
        {
            vbox.add(dbutton);
            dbutton.add(label1);
            add(vbox);
            vbox.show();
            label1.show();
            dbutton.show();

            dbutton.signal_clicked().connect(SigC::slot(*this,&testwindow::on_clicked));
            dbutton.signal_secondpressed().connect(SigC::slot(*this,&testwindow::on_sclicked));
        }
        bool delete_event_impl(GdkEventAny *)
        {
            std::cout << "Close Window" << std::endl;
            Gtk::Main::instance()->quit();
            return 0;
        }
};


int main( int argc, char **argv )
{   Gtk::Main m(argc, argv );
    testwindow w;
    w.show();
    m.run(w);
    return 0;
}
