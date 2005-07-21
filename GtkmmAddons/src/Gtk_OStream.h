// $Id: Gtk_OStream.h,v 1.6 2005/07/21 09:06:26 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998-2002  Adolf Petig GmbH. & Co. KG
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

#ifndef GTKMM_OSTREAM_H
#define GTKMM_OSTREAM_H
#include <callbackbuf.h> 
#include <gtk/gtktypeutils.h> // for gpointer, GtkDestroyNotify
#include <sigc++/signal.h>
#include <string.h>

namespace Gtk {
class OptionMenu;
class Label;
class Menu;
class TreeView;
};

// 2do: 

namespace Gtk {

class OStream;

class OStreamBase : public std::ostream
{protected:
	typedef void (OStream::*flush_cbt)();
        typedef void (OStream::*close_cbt)();
        typedef std::streamsize (OStream::*data_cbt)(const char*,std::streamsize);
        typedef void (OStream::*line_cbt)(const std::string &line);
        flush_cbt flush_impl;
        close_cbt close_impl;
        data_cbt data_impl;
        line_cbt line_impl;
        
        std::string data;
        gpointer user_data;
        GtkDestroyNotify notify;
        SigC::Signal0<void> flushed;
        callbackbuf<Gtk::OStreamBase> buf;
        
        std::streamsize data_cb(const char_type* __s, std::streamsize __n);
public:
	OStreamBase(line_cbt l,close_cbt c=0);
	OStreamBase(data_cbt d);
        ~OStreamBase(void);
        // the arguments are for convenience only, set_user_data works the same
        void flush(gpointer user_data=0,GtkDestroyNotify d=0);
        void set_user_data(gpointer _user_data=0,GtkDestroyNotify d=0)
        {  user_data=_user_data;
           notify=d;
        }
        
        SigC::Signal0<void> &signal_flushed() { return flushed; }
};

class OStream : public OStreamBase
{       // specific information
        friend class OStreamBase;
        struct data_stream { std::ostream *os; };
        struct data_optionmenu { Gtk::OptionMenu *widget; Gtk::Menu *menu; };
        struct data_label { Label *widget; };
        struct data_treeview { Gtk::TreeView *view; };
        union
        {  	struct data_stream stream;
                struct data_optionmenu optionmenu;
                struct data_label label;
                struct data_treeview treeview;
        } handler_data;

        std::streamsize default_data(const char *,std::streamsize);
        void erase_OptionMenu(openmode m);
        void erase_Label(openmode m);
        void erase_TreeView(openmode m);
        std::streamsize data_stream(const char *,std::streamsize);
        std::streamsize data_Label(const char *,std::streamsize);
        void line_OptionMenu(const std::string &line);
        void line_TreeView(const std::string &line);
        void close_OptionMenu();
    public:
        // add your own post flushing functionality (e.g. scrolling)

        OStream() : OStreamBase(&OStream::data_stream)
        {   handler_data.stream.os=&std::cout;
        }
        OStream(std::ostream *stream) : OStreamBase(&OStream::data_stream)
        {   handler_data.stream.os=stream;
        }
	OStream(Gtk::TreeView *tv,openmode mode=(std::ios::out|std::ios::trunc))
		: OStreamBase(&OStream::line_TreeView)
	{   handler_data.treeview.view=tv;
	    erase_TreeView(mode);
	}
        OStream(Gtk::OptionMenu *o,openmode mode=(std::ios::out|std::ios::trunc)) throw()
                : OStreamBase(&OStream::line_OptionMenu, &OStream::close_OptionMenu)
        {   handler_data.optionmenu.widget=o;
            erase_OptionMenu(mode);
        }
        OStream(Gtk::Label *l,openmode mode=(std::ios::out|std::ios::trunc)) throw()
                : OStreamBase(&OStream::data_Label)
        {   handler_data.label.widget=l;
            erase_Label(mode);
        }
        ~OStream();
};

};

#endif
