/* $Id: ArtikelBase.h,v 1.7 2002/09/02 13:04:03 christof Exp $ */
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

#ifndef ARTIKELBASE
#define ARTIKELBASE

#include<string>
#include"Aux/SQLerror.h"

class ArtikelBase
{
public:
	typedef unsigned long ID;
   static const ID none_id=0;
protected:
	ID artikelid;

public:
 ArtikelBase() : artikelid(none_id) {}
 ArtikelBase(const ID &stamp) throw()
   : artikelid(stamp) {}

 const ID &getArtid() const {return artikelid;}
 /// I prefer this one ... (cp)
 const ID &Id() const {return artikelid;}
 void setID(const ID &id) {artikelid=id;}
 bool valid() const {if(Id()==none_id) return false; else return true;}
 
 bool operator!() const { return !artikelid; }
 bool operator==(const ArtikelBase &b) const { return artikelid==b.artikelid; }
 bool operator<(const ArtikelBase &b) const { return artikelid<b.artikelid; }
};

#endif
