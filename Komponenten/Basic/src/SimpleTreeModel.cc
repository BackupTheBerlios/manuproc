// $Id: SimpleTreeModel.cc,v 1.6 2004/12/04 10:53:34 christof Exp $
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

#include <SimpleTreeModel.h>

void SimpleTreeModel::append_line(const cH_RowDataBase &row)
{  datavec.push_back(row);
   line_appended(row);
}

namespace { struct remove_line_compare {
	cH_RowDataBase a;
	bool operator()(const cH_RowDataBase &b) { return &*a == &*b; }
	remove_line_compare(const cH_RowDataBase &_a) : a(_a) {}
};}

void SimpleTreeModel::remove_line(const cH_RowDataBase &row)
{  line_to_remove(row);
   // I'd also have to call erase if I used remove_if()
   datavec_t::iterator i=find_if(datavec.begin(),datavec.end(),remove_line_compare(row));
   if (i!=datavec.end()) datavec.erase(i);
}

void SimpleTreeModel::clear()
{  please_detach();
   datavec.clear();
   please_attach();
}
	
void SimpleTreeModel::setDataVec(const std::vector<cH_RowDataBase> &d)
{  please_detach();
   datavec=d;
   please_attach();
}

void SimpleTreeModel::setTitles(const std::vector<std::string> &T)
{  titles=T;
   for (guint i=0;i<T.size();++i) title_changed(i);
}

const std::string SimpleTreeModel::getColTitle(guint idx) const
{  if (idx<titles.size()) return titles[idx];
   return ""; 
}

bool SimpleTreeModel::is_editable(unsigned idx) const
{  if (idx>=column_editable.size()) return false;
   return column_editable[idx];
}

SimpleTreeModel::column_type_t SimpleTreeModel::get_column_type(unsigned idx) const
{  if (idx>=column_type.size()) return ct_string;
   return column_type[idx];
}

void SimpleTreeModel::set_editable(unsigned idx,bool v)
{  if (idx>=column_editable.size()) column_editable.resize(idx+1);
   column_editable[idx]=v;
   title_changed(idx);
}

void SimpleTreeModel::set_column_type(unsigned idx, column_type_t t)
{  if (idx>=column_type.size()) column_type.resize(idx+1);
   column_type[idx]=t;
   title_changed(idx);
}
