/* $Id: RechnungBase.h,v 1.4 2002/05/06 13:41:23 christof Exp $ */
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


#ifndef RECHNUNGBASE
#define RECHNUNGBASE

#include <Aux/SQLerror.h>
#include <Artikel/Preis.h>
#include <Lieferschein/LieferscheinBase.h>

class RechnungBase
{
 protected:
  	int rngid;	
	static const int _illegal=-1;
 public:
	typedef int ID;
	static const ID none_id=_illegal;
	typedef Preis::rabatt_t rabatt_t; // in Prozent
	typedef Preis::geldbetrag_t geldbetrag_t;
	typedef LieferscheinBase::mengen_t mengen_t;
	
	RechnungBase() : rngid(none_id) {}
	RechnungBase(int rid) : rngid(rid) {}

	ID Id() const {return rngid;}
	
	void setEingezogen(int refnr) throw(SQLerror);
};

#endif


