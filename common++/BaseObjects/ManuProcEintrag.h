/* $Id: ManuProcEintrag.h,v 1.4 2002/09/02 13:04:03 christof Exp $ */
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


#ifndef MANU_PROC_EINTRAG
#define MANU_PROC_EINTRAG

#include <BaseObjects/ManuProcVorgang.h>


class ManuProcEintrag : public ManuProcVorgang
{
private:
 int zeilennr;
 
public:
 static const ID none_znr = 0;
 ManuProcEintrag() : zeilennr(none_znr) {}
 ID Zeilennr() const {return zeilennr;}
 // none_znr soll in -1 geändert werden
};

#endif
