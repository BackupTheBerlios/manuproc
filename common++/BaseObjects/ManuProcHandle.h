/* $Id: ManuProcHandle.h,v 1.3 2003/04/25 10:27:26 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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



#ifndef MANU_PROC_HANDLE
#define MANU_PROC_HANDLE

#include<Misc/Handles.h>
#include<BaseObjects/ManuProcEntity.h>
#include <Misc/compiler_ports.h>

template <class T=long int> class ManuProcHandle 
: public ManuProcEntity<T>, public HandleContent
{ 
public:
 typedef NOTGCC295(typename) ManuProcEntity<T>::ID ID;
 ManuProcHandle() : ManuProcEntity<T>(none_id) {}
 ManuProcHandle(const ID i) : ManuProcEntity<T>(i) {}
};

#endif
