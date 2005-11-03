// $Id: SimpleTreeModel.h,v 1.16 2005/11/03 21:05:18 christof Exp $
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

class SimpleTreeStore;
class SimpleTree_Basic;

// this is only a container, properties of the data belong into STM_Properties

class SimpleTreeModel : SigC::Object
{public:
	typedef std::vector<cH_RowDataBase> datavec_t;
	
private:
	friend class SimpleTreeStore; // für signal_value_changed
	friend class SimpleTree_Basic;
	
	datavec_t datavec;

	SigC::Signal1<void,cH_RowDataBase> line_appended;
	SigC::Signal1<void,cH_RowDataBase> line_to_remove;
//	SigC::Signal0<void> redraw_needed;
	SigC::Signal0<void> please_detach;
	SigC::Signal0<void> please_attach;
	// a column was changed, change data, redraw?
	SigC::Signal4<void,cH_RowDataBase,unsigned,const std::string &,bool &> value_changed;

protected:
	// sollte besser über RowDataBase::changeValue gehen, ist sauberer
	SigC::Signal4<void,cH_RowDataBase,unsigned,std::string const&,bool &> 
		&signal_value_changed()
	{  return value_changed; }
public:
	// more STLish names
	void push_back(const cH_RowDataBase &row)
	{ append_line(row); }
	void erase(const cH_RowDataBase &row)
	{ remove_line(row); }
	void clear();
	void operator=(const datavec_t &d)
	{ setDataVec(d); }
	typedef datavec_t::const_iterator const_iterator;
	typedef datavec_t::iterator iterator;
	typedef datavec_t::size_type size_type;
	const_iterator begin() const { return datavec.begin(); }
	const_iterator end() const { return datavec.end(); }
	iterator begin() { return datavec.begin(); }
	iterator end() { return datavec.end(); }
	bool empty() { return datavec.empty(); }
	size_type size() { return datavec.size(); }

	// please prefer the more STLish variants
	void append_line(const cH_RowDataBase &row);
	void remove_line(const cH_RowDataBase &row);
	
	void setDataVec(const datavec_t &d);
	const datavec_t &getDataVec() const
	{  return datavec; }

	SigC::Signal1<void,cH_RowDataBase> &signal_line_appended()
	{  return line_appended; }
	SigC::Signal1<void,cH_RowDataBase> &signal_line_to_remove()
	{  return line_to_remove; }
	// a big reorganization is about to occur
	SigC::Signal0<void> &signal_please_detach()
	{  return please_detach; }
	// reorganization finished, you can redisplay
	SigC::Signal0<void> &signal_please_attach()
	{  return please_attach; }

	void about_to_change(const cH_RowDataBase &row)
	{  signal_line_to_remove()(row); }
	void has_changed(const cH_RowDataBase &row)
	{  signal_line_appended()(row); }
};

#endif
