// $Id: uiwin_Material.h,v 1.16 2001/01/10 13:26:56 cvs_christof Exp $
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
#ifndef UIWIN_MATERIAL_H
#define UIWIN_MATERIAL_H
#include <gtk--/notebook.h>
#include <gtk--/box.h>
#include <gtk--/label.h>
#include <gtk--/combo.h>
#include <gtk--/checkbutton.h>
#include <SearchCombo.h>
//#include <more_param.h>
#include <Material/Materiallisten.h>

class uiwin_Material;

class uiwin_Mat_Long : public Gtk::HBox
{
public:
  	typedef MaterialBase::ID ID;

private:
	Gtk::Label label;
	Gtk::SearchCombo combo;
	Gtk::Button button;
	uiwin_Material *parent;

	void search(gboolean *cont, GtkSCContext isnew);
	static gint try_grab_focus(GtkWidget *w,gpointer gp) throw();
public:
	void examine_changes();
	uiwin_Mat_Long(uiwin_Material *p);
	void registerActivate(SigC::Signal0<void> &s)
	{  combo.get_entry()->activate.connect(s.slot());
	}
	void erase()
	{  combo.set_text("");
	}
	void setID(const ID &st) throw();
};

class uiwin_Mat_Short : public Gtk::HBox
{	Gtk::Label label;
	Gtk::SearchCombo combo;
	Gtk::Button button;
	uiwin_Material *parent;

	void search(gboolean *cont, GtkSCContext isnew);
	static gint try_grab_focus(GtkWidget *w,gpointer gp) throw();
public:
	typedef uiwin_Mat_Long::ID ID;
	
	void examine_changes();
	uiwin_Mat_Short(uiwin_Material *p);
	void registerActivate(SigC::Signal0<void> &s)
	{  combo.get_entry()->activate.connect(s.slot());
	}
	void erase()
	{  combo.set_text("");
	}
	void setID(const ID &st) throw();
};

class uiwin_Material : public Gtk::Notebook
{
public:
  	static Materiallisten *listen;
  	typedef uiwin_Mat_Long::ID ID;
private:
	static int instances;
	ID current_ID;
	SigC::Connection connection;
	uiwin_Mat_Long *mlong;
	uiwin_Mat_Short *mshort;

	void refresh();
	static gint try_grab_focus(GtkWidget *w,gpointer gp) throw();
	void do_grab_focus() throw();
   public:
	uiwin_Material();
	~uiwin_Material();
	ID value() const throw();
	ID get_value() const throw()
	{  return value(); }
	static const std::string LongByID(ID st);
	const std::string value_as_string() const
	{  return LongByID(value()); }
	const std::string get_value_as_string() const
	{  return value_as_string(); }
	
	SigC::Signal0<void> activate;
	void setID(const ID &st) throw();
	void set_value(const ID &st) throw()
	{  setID(st); }
	void erase()
	{  mlong->erase();
	   mshort->erase();
	}
};
#endif
