/* $Id: ManuProcVorgang.h,v 1.5 2002/10/24 14:06:49 thoma Exp $ */
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

#ifndef MANU_PROC_VORGANG
#define MANU_PROC_VORGANG


#include <BaseObjects/ManuProcEntity.h>

class ManuProcVorgang : public ManuProcEntity<>
{
private:
 ID instanzid;
 
public:
 ManuProcVorgang() : instanzid(none_id) {}
 ID Instanz() const {return instanzid;}

};

#endif
