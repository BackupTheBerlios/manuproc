// $Id: datewin.h,v 1.7 2002/06/24 07:45:24 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef _DATEWIN_HH
#  include "datewin_glade.hh"
#  define _DATEWIN_HH
#include <Misc/Datum.h>

class datewin : public datewin_glade
{  	friend class datewin_glade;
	static gint try_grab_focus(GtkWidget *w,gpointer gp) throw();
        enum FELD {TAG=1,MONAT};
        enum PAGE { p_Datum, p_Woche, p_Kalender, p_leer };
        bool block;
        std::string instance;

	void on_activate(int i);
        void datum_activate();
        void kw_activate();
        void on_day_selected();
        void datum_setzen();
        void save_settings() const;
        int load_settings() const;
        void on_datewin_switch_page(Gtk::Notebook_Helpers::Page *p0, guint p1);

   public:
	datewin(const std::string &instance="");
#if 0 // COMPATIBILITY ONLY
	Petig::Datum get_Datum() const throw()
	{  return get_value(); }
	void set_Datum(const Petig::Datum &d) throw()
	{  set_value(d); }
#endif	
	Petig::Datum get_value() const throw(); 
	void set_value(const Petig::Datum &d) throw();
	void setLabel(const std::string &s);
	void setInstance(const std::string &s);
	
	SigC::Signal0<void> activate;
};

// compatibility
class Datewin : public datewin {};
#endif
