/* $Id: db_test.cc,v 1.7 2004/04/29 14:24:40 christof Exp $ */
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
#include <ChoiceButton.h>
#include <iostream>
#include <gdkmm/pixbufloader.h>

extern const unsigned char rot_jpeg_data[],gelb_jpeg_data[],blau_jpeg_data[],gruen_jpeg_data[];
extern const unsigned rot_jpeg_size,gelb_jpeg_size,blau_jpeg_size,gruen_jpeg_size;

class testwindow : public Gtk::Window
{
        Gtk::VBox vbox;
        ManuProC::DoubleButton dbutton;
        ManuProC::ChoiceButton cbutton;
        Gtk::Label label1;

    public:
    	void on_clicked()
    	{  std::cout << "on_clicked\n";
    	}
    	void on_sclicked(int mousebutton)
    	{  std::cout << "on_sclicked ("<<mousebutton<<")\n";
    	}
    	void on_cclicked(std::string farbe)
    	{  std::cout << "on_cclicked ("<<farbe<<")\n";
    	}

        testwindow() 
           : vbox(false,0), label1("Test")
        {
            vbox.add(dbutton);
            vbox.add(cbutton);
            dbutton.add(label1);
            add(vbox);
            vbox.show();
            label1.show();
            dbutton.show();
            cbutton.show();

            dbutton.signal_clicked().connect(SigC::slot(*this,&testwindow::on_clicked));
            dbutton.signal_secondpressed().connect(SigC::slot(*this,&testwindow::on_sclicked));
            
            {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
	       loader->write(rot_jpeg_data, rot_jpeg_size);
	       loader->close();
	       cbutton.add(loader->get_pixbuf(),"rot",SigC::bind(SigC::slot(*this,&testwindow::on_cclicked),"rot"));
	    }{ Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
	       loader->write(blau_jpeg_data, blau_jpeg_size);
	       loader->close();
	       cbutton.add(loader->get_pixbuf(),"blau",SigC::bind(SigC::slot(*this,&testwindow::on_cclicked),"blau"));
	    }{ Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
	       loader->write(gruen_jpeg_data, gruen_jpeg_size);
	       loader->close();
	       cbutton.add(loader->get_pixbuf(),"grün",SigC::bind(SigC::slot(*this,&testwindow::on_cclicked),"gruen"));
	    }{ Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
	       loader->write(gelb_jpeg_data, gelb_jpeg_size);
	       loader->close();
	       cbutton.add(loader->get_pixbuf(),"gelb",SigC::bind(SigC::slot(*this,&testwindow::on_cclicked),"gelb"));
	    }
	    cbutton.set_style(true,false);
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
