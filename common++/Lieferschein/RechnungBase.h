/* $Id: RechnungBase.h,v 1.9 2003/01/08 09:46:57 christof Exp $ */
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

#include <Misc/SQLerror.h>
#include <Artikel/Preis.h>
#include <Lieferschein/LieferscheinBase.h>
#include <BaseObjects/ManuProcEntity.h>
#include <Misc/Ausgabe_neu.h>

// ableiten von ManuProcEntity wg. HandleCOntent nicht m�glich :-( MAT
class RechnungBase // : ManuProcEntity
{
 protected:
  	int rngid;	
//	static const int _illegal=none_id;
 public:
	typedef ManuProcEntity<>::ID ID;
	static const ID none_id=ManuProcEntity<>::none_id;
	typedef Preis::rabatt_t rabatt_t; // in Prozent
	typedef Preis::geldbetrag_t geldbetrag_t;
	typedef LieferscheinBase::mengen_t mengen_t;
	
	RechnungBase() : rngid(none_id) {}
	RechnungBase(ID rid) : rngid(rid) {}

	ID Id() const {return rngid;}
	std::string getRngidToStr() const
	{ return Formatiere(rngid,0,6,"","",'0'); }

   // sobald von ManuProcEntity abgeleitet die folgende Zeile l�schen MAT
   bool Valid() const {return ManuProcEntity<>::Valid(rngid);}
	
	void setEingezogen(int refnr) throw(SQLerror);
};

#endif


