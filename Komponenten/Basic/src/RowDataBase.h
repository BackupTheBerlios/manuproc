/* $Id: RowDataBase.h,v 1.10 2005/11/03 21:05:18 christof Exp $ */
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

// $Id: RowDataBase.h,v 1.10 2005/11/03 21:05:18 christof Exp $

#ifndef KOMPONENTEN_ROWDATA_H
#define KOMPONENTEN_ROWDATA_H

#include <Misc/Handles.h>
#include <Misc/EntryValueBase.h>
//#include <glib.h>
//#include <cassert>
#include <glibmm/ustring.h>

struct RowDataBase : HandleContent
{
 virtual ~RowDataBase(){}

 virtual cH_EntryValue Value(guint _seqnr,gpointer _g) const=0;
 virtual bool changeValue(guint _seqnr,gpointer _g, const Glib::ustring &newvalue)
 { return false; } // true bedeutet: Wert geändert
};

class H_RowDataBase : public Handle<RowDataBase>
{
public:
 H_RowDataBase() {}
 H_RowDataBase(RowDataBase *r) : Handle<RowDataBase>(r){}
};

class cH_RowDataBase : public Handle<const RowDataBase>
{
public:
 cH_RowDataBase() {}
 cH_RowDataBase(const H_RowDataBase &h) : Handle<const RowDataBase>(&*h) {}
 cH_RowDataBase(const RowDataBase *r) : Handle<const RowDataBase>(r){}
 cH_RowDataBase(const Handle<const RowDataBase> &h) : Handle<const RowDataBase>(h){}
};

#endif
