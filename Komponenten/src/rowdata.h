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

// $Id: rowdata.h,v 1.8 2001/04/20 07:10:52 christof Exp $

#ifndef KOMPONENTEN_ROWDATA_H
#define KOMPONENTEN_ROWDATA_H

#include <Aux/Handles.h>
#include <Aux/EntryValueBase.h>

class RowDataBase : public HandleContent
{
public:
 virtual const cH_EntryValue Value(int _seqnr) const=0;
 virtual ~RowDataBase(){}
};

class cH_RowDataBase : public const_Handle<RowDataBase>
{
protected:
 cH_RowDataBase() {}
 cH_RowDataBase(const RowDataBase *r) : const_Handle<RowDataBase>(r){}
};

#endif
