/* $Id: TreeRow.h,v 1.6 2002/11/28 17:09:42 christof Exp $ */
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

// $Id: TreeRow.h,v 1.6 2002/11/28 17:09:42 christof Exp $

#ifndef MPC_TREEROW_H
#define MPC_TREEROW_H

#include<string>
#include<vector>
#include "RowDataBase.h"

class TreeRow : public HandleContent
{public:
	virtual ~TreeRow() {}
	
	virtual const cH_EntryValue Value(guint _seqnr,gpointer _g) const = 0;

	virtual void cumulate(const cH_RowDataBase &rd) = 0;
	virtual void deduct(const cH_RowDataBase &rd) = 0;
};

#endif
