// $Id: EingabeBox.hh,v 1.5 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// $Id: EingabeBox.hh,v 1.5 2002/07/05 12:36:56 christof Exp $

#ifndef _EINGABEBOX_HH
#define _EINGABEBOX_HH
#include <gtk--/table.h>
#include <gtk--/label.h>
#include <gtk--/entry.h>
#include <gtk--/widget.h>
#include <vector>

class EingabeBox : public Gtk::Table
{	typedef std::vector<Gtk::Label *> t_labels;
	typedef std::vector<Gtk::Entry *> t_entries;
	typedef std::vector<SigC::Connection> t_cons;

        t_labels labels;
        t_entries entries;
        // activate->grab_focus connects
        // x -0-> 0 -1-> 1 -last-> y
        t_cons cons;
        SigC::Connection last_con;
        unsigned int visible_size;
        
//        Signal_Converter::signal0_on_focus_in signal_adaptor;
//	friend gint try_grab_focus(GtkWidget *w,gpointer);
        SigC::Signal0<void> grab_focus_tried;
        
        void grow(int cols);
        void check() const;
        static gint try_grab_focus(GtkWidget *w,gpointer gp);
public:
        EingabeBox(int cols=0);
        ~EingabeBox();
	const std::string get_label(int col) const;
	void set_label(int col,const std::string &s);
	const std::string get_value(int col) const;
   void set_value(int col,const std::string &s);
	void set_width(int col,int width);
	void set_size(int cols);
//   void activate_entry(int nr);
	int size() const
	{  return visible_size; }
	void reset();

	SigC::Signal0<void> activate;
};
#endif
