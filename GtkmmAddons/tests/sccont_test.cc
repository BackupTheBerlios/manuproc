/*  Gtk--addons: a collection of gtk-- addons
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// $Id: sccont_test.cc,v 1.4 2004/04/29 14:24:40 christof Exp $

#include<SearchComboContent.h>
#include <cstdio>
#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/window.h>

class testwindow : public Gtk::Window
{
        SearchComboContent<int> testscc;

    public:

	void suchfunc(gboolean *cont, GtkSCContext newsearch)
	{   if (newsearch!=GTK_SEARCH_OPEN) return;
	    for (int i=0;i<10;++i)
	    {  char buf[20];
	       snprintf(buf,sizeof buf,"%d",i);
	       testscc.add_item(buf,i*i);
	    }
	}
	void activatefunc()
	{   std::cout << testscc.get_text() << '=' << testscc.Content() << '\n';
	}
        testwindow() 
        {   add(testscc);
            testscc.show();
            testscc.signal_search().connect(SigC::slot(*this,&testwindow::suchfunc));
            testscc.signal_activate().connect(SigC::slot(*this,&testwindow::activatefunc));
        }
        bool delete_event_impl(GdkEventAny *)
        {
            std::cout << "Close Window" << std::endl;
            Gtk::Main::instance()->quit();
            return 0;
        }
};


int main(int argc,char **argv)
{
    Gtk::Main m(argc, argv );
    testwindow w;
    
    w.show();
    m.run(w);
    return 0;

}
