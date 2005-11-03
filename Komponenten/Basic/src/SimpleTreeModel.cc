// $Id: SimpleTreeModel.cc,v 1.10 2005/11/03 21:05:21 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

