// $Id: SimpleTreeModel.h,v 1.9 2002/12/11 15:40:43 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef SIMPLE_TREE_MODEL_H
#define SIMPLE_TREE_MODEL_H

#include <string>
#include <vector>
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <glib/gtypes.h>
#include <RowDataBase.h>

class SimpleTreeModel : SigC::Object
{public:
	typedef std::vector<cH_RowDataBase> datavec_t;
private:
	datavec_t datavec;
	std::vector<std::string> titles;
	std::vector<bool> column_editable;

	SigC::Signal1<void,cH_RowDataBase> line_appended;
	SigC::Signal1<void,cH_RowDataBase> line_to_remove;
	SigC::Signal0<void> redraw_needed;
//	SigC::Signal2<void,cH_RowDataBase,cH_RowDataBase> value_changed;
	SigC::Signal1<void,guint> title_changed;
public:
	void append_line(const cH_RowDataBase &row);
	void remove_line(const cH_RowDataBase &row);
	void clear();
	
	void setDataVec(const datavec_t &d);
	const datavec_t &getDataVec() const
	{  return datavec; }
	void setTitles(const std::vector<std::string>& T);
	const std::string getColTitle(guint idx) const;

	SigC::Signal1<void,cH_RowDataBase> &signal_line_appended()
	{  return line_appended; }
	SigC::Signal1<void,cH_RowDataBase> &signal_line_to_remove()
	{  return line_to_remove; }
	SigC::Signal1<void,guint> &signal_title_changed()
	{  return title_changed; }
	SigC::Signal0<void> &signal_redraw_needed()
	{  return redraw_needed; }

	void about_to_change(const cH_RowDataBase &row)
	{  signal_line_to_remove()(row); }
	void has_changed(const cH_RowDataBase &row)
	{  signal_line_appended()(row); }
};

#endif
