// $Id: SimpleTreeModel.cc,v 1.1 2002/10/18 10:39:42 christof Exp $
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

void SimpleTreeModel::clear()
{  datavec.clear();
   redraw_needed();
}
	
void SimpleTreeModel::setDataVec(const std::vector<cH_RowDataBase> &d)
{  datavec=d;
   redraw_needed();
}

void SimpleTreeModel::setTitles(const std::vector<std::string>& T)
{  titles=T;
   for (guint i=0;i<T.size();++i) title_changed(i);
}

const std::string SimpleTreeModel::getColTitle(guint idx) const
{  if (idx<titles.size()) return titles[idx];
   return ""; 
}
