/* $Id: */
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


#ifndef RECHNUNGVOLL
#define RECHNUNGVOLL

#include"Rechnung.h"
#include"RechnungEntry.h"
#include<vector>
#include <Aux/SQLerror.h>

class RechnungVoll : public Rechnung
{
 std::vector<RechnungEntry> rentry;

#ifdef DPD_LIEFERSCHEINE
 mutable fixedpoint<1> netto_gewicht;
 mutable fixedpoint<1> brutto_gewicht;  
 mutable int pakete;
#endif
 
public:
 
        typedef std::vector<RechnungEntry>::const_iterator const_iterator;
        typedef std::vector<RechnungEntry>::iterator iterator;

 	RechnungVoll() : Rechnung() {};
 	RechnungVoll(int lid) throw(SQLerror) ;
 	void deleteRow(const RechnungEntry &le);

 const std::vector<RechnungEntry> &REntries() { return rentry; }

 const_iterator begin() const { return rentry.begin();} 
 const_iterator end() const   { return rentry.end();}   
 iterator begin()  { return rentry.begin();} 
 iterator end()  { return rentry.end();}   
 size_t size() const { return rentry.size();}

 void convert_to_gutschrift();

#ifdef DPD_LIEFERSCHEINE
 void setGewicht() const throw(SQLerror);
 fixedpoint<1> const NettoGew() const {return netto_gewicht;}
 fixedpoint<1> const BruttoGew() const {return brutto_gewicht;} 
 int const Pakete() const {return pakete;}
#endif 
 
};

#endif

