// $Id: StringBox.hh,v 1.2 2004/11/08 07:55:09 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef _STRINGBOX_HH
#  define _STRINGBOX_HH

#include <Aux/Transaction.h>
#include <BaseObjects/ManuProcEntity.h>
#include <SearchCombo.h>

class StringBox : public Gtk::SearchCombo
{	
        void string1_activate();
        void string1_search(gboolean *_continue, GtkSCContext newsearch);

	Transaction tr;

protected:
   std::string _tabelle_,_string1_;

public:
	StringBox();

	std::string get_value() const { return get_text(); }
	void set_value(const std::string &s)
	{  set_text(s); }
   void clear(){reset();}
   void reset();

	SigC::Signal0<void> reset_signal;
};
#endif
