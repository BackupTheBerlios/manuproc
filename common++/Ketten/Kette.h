/* $Id: Kette.h,v 1.1 2001/04/23 08:11:58 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef KETTE_HH
#define KETTE_HH
#include <Aux/Datum.h>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

class Kette {
	int maschine;
	Petig::Datum schaerdatum;
public:
	class illegal_format : exception
	{public:
		virtual const char* what() const throw() { return "Kette::illegal_format"; }
	};
	
	Kette(int m,const Petig::Datum _schaerdatum) throw() 
		: maschine(m), schaerdatum(_schaerdatum)
	{}
	Kette(int m,const char *adabasDatum) throw(Petig::Datumsfehler)
		: maschine(m), schaerdatum(adabasDatum)
	{}
	Kette(const char *barconame) throw(Petig::Datumsfehler,illegal_format);
	Kette() : maschine(0), schaerdatum() {}
	const Petig::Datum &Schaerdatum() const throw() { return schaerdatum; }
	
	string barco_format() const throw();
	void write(int &a, char*, unsigned int) const throw();
	void write(char *x,unsigned int sz) const throw();
	bool operator==(const Kette &b) const throw()
	{  return maschine==b.maschine && schaerdatum==b.schaerdatum; }
	const Kette &operator=(const char *barconame);
	
	friend ostream& operator<<(ostream &o,const Kette &k);
};

extern inline ostream& operator<<(ostream &o,const Kette &k)
{  return o<<k.barco_format();
}

#endif
