/* $Id: back_test.cc,v 1.4 2004/04/29 14:24:40 christof Exp $ */
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
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gdkmm/pixbufloader.h>
#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>

extern const unsigned char rot_jpeg_data[],gelb_jpeg_data[],blau_jpeg_data[],gruen_jpeg_data[];
extern const unsigned rot_jpeg_size,gelb_jpeg_size,blau_jpeg_size,gruen_jpeg_size;

static const unsigned char blackball_png_data[] = 
{       137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,
        0,0,0,30,0,0,0,30,8,6,0,0,0,59,48,174,
        162,0,0,3,173,73,68,65,84,120,218,165,150,189,82,227,
        72,16,199,127,210,232,211,95,107,75,123,183,183,117,85,100,
        68,132,142,120,0,138,226,53,40,158,130,152,136,103,160,72,
        201,200,9,201,72,120,2,28,80,36,236,222,98,96,189,246,
        73,182,36,207,37,61,58,97,100,12,222,169,234,194,46,60,
        253,235,127,207,244,116,91,172,185,90,173,22,221,110,151,110,
        183,75,175,215,43,45,138,162,87,127,23,13,192,94,23,156,
        36,9,73,146,144,166,41,211,233,148,233,116,202,108,54,123,
        97,89,150,149,150,231,121,105,191,5,46,138,226,5,108,25,
        36,207,115,138,162,120,241,25,192,89,147,235,2,193,120,60,
        118,198,227,177,125,119,119,55,7,114,32,221,223,223,207,170,
        16,3,53,159,215,5,119,129,14,208,4,2,9,192,6,230,
        64,6,164,167,167,167,19,96,116,116,116,244,92,20,5,243,
        249,252,149,1,88,239,4,54,129,207,64,4,124,2,90,64,
        8,120,21,240,12,72,128,49,240,19,120,4,30,206,206,206,
        38,113,28,19,199,49,81,20,17,199,49,237,118,251,93,224,
        30,240,23,240,39,16,139,234,54,208,168,1,255,11,252,2,
        158,129,33,240,15,240,237,226,226,226,169,10,239,116,58,43,
        83,221,3,254,6,190,10,188,170,186,9,248,21,240,20,152,
        84,212,54,37,48,181,187,187,203,245,245,245,19,128,101,89,
        43,207,184,41,176,175,21,248,23,129,27,213,62,160,128,66,
        192,70,109,91,238,128,50,69,208,239,247,103,183,183,183,147,
        247,128,63,75,122,23,225,38,229,29,57,103,3,78,128,145,
        164,56,172,248,46,204,49,216,182,61,177,109,251,77,112,87,
        82,26,75,0,95,22,224,127,136,243,197,21,73,166,92,249,
        158,3,169,28,193,104,99,99,227,103,154,166,207,111,129,59,
        114,142,38,0,163,222,192,213,146,125,161,252,31,41,175,68,
        210,255,83,252,117,148,82,75,193,174,68,221,146,179,50,1,
        196,162,84,173,184,144,74,126,55,169,0,219,226,175,233,186,
        174,171,181,206,234,158,204,64,44,148,146,105,202,198,206,146,
        244,46,83,222,145,125,77,241,19,86,124,215,190,213,142,168,
        246,196,124,177,240,131,175,92,88,217,107,124,185,38,203,117,
        96,187,198,212,59,82,92,151,114,181,196,95,45,120,94,99,
        133,216,135,26,152,88,157,63,236,86,171,133,239,251,40,85,
        10,202,229,70,206,196,166,98,201,71,91,118,101,175,241,149,
        137,127,156,40,138,202,30,58,159,207,25,14,135,169,212,94,
        34,111,239,68,74,98,36,165,21,190,19,58,146,125,19,241,
        147,136,223,180,22,220,235,245,178,193,96,48,145,46,99,106,
        208,60,250,205,21,117,108,82,252,67,126,255,44,251,127,137,
        191,137,214,58,123,1,54,189,83,107,205,96,48,24,85,128,
        143,82,22,97,229,69,90,246,114,37,2,189,151,206,244,32,
        251,77,0,163,242,230,245,251,125,124,223,39,8,2,130,32,
        160,209,104,176,185,185,153,222,220,220,152,82,112,69,161,5,
        104,57,35,115,102,121,165,29,62,2,223,129,111,98,247,98,
        229,119,173,245,247,178,102,163,40,42,71,19,163,88,58,200,
        67,165,231,170,202,197,75,36,250,183,186,211,131,4,97,192,
        70,253,255,181,182,179,179,83,42,14,195,144,48,12,105,52,
        26,108,111,111,103,87,87,87,69,69,169,25,111,166,149,73,
        99,4,60,201,121,254,48,141,191,70,245,189,214,122,84,5,
        91,199,199,199,229,197,50,243,16,128,109,219,216,182,205,225,
        225,225,111,79,32,90,235,167,197,203,96,157,156,156,148,211,
        159,214,186,76,181,101,89,40,165,80,74,113,112,112,176,246,
        204,165,181,158,212,93,125,235,252,252,156,234,52,184,168,88,
        41,133,227,56,184,174,203,222,222,222,202,41,211,244,94,173,
        245,243,91,133,110,93,94,94,150,169,94,84,108,219,54,142,
        227,160,148,194,117,93,60,207,195,243,60,182,182,182,92,1,
        59,21,112,14,164,166,78,87,45,39,138,162,23,55,122,17,
        108,20,27,213,158,231,49,28,14,51,223,247,179,32,8,170,
        79,237,135,214,43,112,153,138,55,192,158,231,149,211,226,186,
        235,63,175,188,26,84,13,177,189,153,0,0,0,0,73,69,
        78,68,174,66,96,130,
};
static const unsigned blackball_png_size = 998;

class testwindow : public Gtk::Window
{
        Gtk::Label label1,label2;
        Glib::RefPtr<Gdk::Pixmap> pm,pm2;
        Gtk::VBox vb;
        Gtk::HBox hb;
        Gtk::EventBox eb;
	Gtk::Image imag;

    public:
        testwindow() 
           : label1("Test 012345\n0\n1\n2\n3\n4\n5"), label2("ABC")
        {   add(vb);
            vb.add(label1);
            vb.add(eb);
            eb.add(hb);
            hb.add(label2);
            hb.add(imag);
            
            label1.show();
            label2.show();
            imag.show();
            hb.show();
            vb.show();
            eb.show();

//            show();
            realize();
            Gtk::Widget &x=*this;
            if (x.get_window())
            {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
	       loader->write(rot_jpeg_data, rot_jpeg_size);
	       loader->close();
	       Glib::RefPtr<Gdk::Pixbuf> pb=loader->get_pixbuf();
	       pm=Gdk::Pixmap::create(x.get_window(),pb->get_width(),pb->get_height(),x.get_window()->get_depth());
	       Glib::RefPtr<Gdk::GC> gc=Gdk::GC::create(pm);
	       pm->draw_pixbuf(gc,pb,0,0,0,0,-1,-1,Gdk::RGB_DITHER_NORMAL,0,0);
	    }
	    Glib::RefPtr<Gtk::Style> st=eb.get_style()->copy();
	    st->set_bg_pixmap(Gtk::STATE_NORMAL,pm);
	    eb.set_style(st);
            if (x.get_window())
            {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
	       loader->write(gelb_jpeg_data, gelb_jpeg_size);
	       loader->close();
	       Glib::RefPtr<Gdk::Pixbuf> pb=loader->get_pixbuf();
	       pm2=Gdk::Pixmap::create(x.get_window(),pb->get_width(),pb->get_height(),x.get_window()->get_depth());
	       Glib::RefPtr<Gdk::GC> gc=Gdk::GC::create(pm2);
	       pm2->draw_pixbuf(gc,pb,0,0,0,0,-1,-1,Gdk::RGB_DITHER_NORMAL,0,0);
	    }
	    Glib::RefPtr<Gtk::Style> st2=get_style()->copy();
	    st2->set_bg_pixmap(Gtk::STATE_NORMAL,pm2);
	    set_style(st2);
            {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
	       loader->write(blackball_png_data, blackball_png_size);
	       loader->close();
	       imag.set(loader->get_pixbuf());
	    }
        }
};


int main( int argc, char **argv )
{   Gtk::Main m(argc, argv );
    testwindow w;
    w.show();
    m.run(w);
    return 0;
}
