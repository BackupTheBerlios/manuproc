/* $Id: tree.cc,v 1.1 2003/01/06 15:07:33 christof Exp $ */
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

#include <gtk--.h>
#include <iostream.h>
#include<Gtk_OStream.h>
#include<string.h>

class subtree : public Gtk::Tree
{
        Gtk::TreeItem a,b,c;
    public:
        subtree() : a("Eins"),b("Zwei"),c("Drei")
        {
            append(a);
            append(b);
            append(c);
            a.show(); b.show(); c.show();
        }
};
class mytree : public Gtk::Tree
{
        Gtk::TreeItem a,b,c,d;
        subtree sub;
    public:
        mytree() : a("Eins"),b("Zwei"),c("Drei"),d("Sub")
        {

            append(a);
            append(b);
            append(c);
            append(d);
            d.set_subtree(sub);
            a.show(); b.show(); c.show(); sub.show();
            d.show();
            d.expand();
            d.collapse();
        }
};

class testwindow : public Gtk::Window
{
        Gtk::Button b,b2;
        Gtk::HBox hbox;
        Gtk::VBox vbox;
        Gtk::Label text;
        mytree tree;
        void pressed()
        {
            text.set("Button pushed");
        }
        void pressed2()
        {
            text.set("Button 2 pushed");
        }

    public:

        testwindow() : vbox(false,0)
        {
            hbox.add( b );
            hbox.add( b2 );
            vbox.add( text);
            vbox.add( hbox);
            vbox.add( tree);
            add(vbox);
            b.clicked.connect(SigC::slot(this,&testwindow::pressed));
            b2.clicked.connect(SigC::slot(this,&testwindow::pressed2));
            b.show();
            b2.show();
            hbox.show();
            text.show();
            vbox.show();
            tree.show();

        }
        gint delete_event_impl(GdkEventAny *)
        {
            std::cout << "Close Window" << endl;
            Gtk::Main::instance()->quit();
            return 0;
        }
};


int main( int argc, char **argv )
{
    Gtk::Main m(argc, argv );
    testwindow w;

    /*  w.set_usize( 100,20 );  */
    w.show();

    m.run();
    return 0;
}
