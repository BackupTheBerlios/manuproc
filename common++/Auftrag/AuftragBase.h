/* $Id: AuftragBase.h,v 1.4 2001/06/22 09:42:44 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef AUFTRAGBASE_H
#define AUFTRAGBASE_H
#include<Aux/ppsInstanz.h>

class AuftragBase
{
 protected:
        ppsInstanz::ppsInstId instanz; 	
 	int auftragid;	
 public:
	AuftragBase() : instanz(ppsInstanz::INST_KNDAUF), auftragid(0) {}
	AuftragBase(ppsInstanz::ppsInstId _instanz, int aufid) throw() : instanz(_instanz), auftragid(aufid) {}
	int Id() const {return auftragid;}
        ppsInstanz::ppsInstId Instanz() {return instanz; }
};

#endif
