// $Id: Kalenderwoche.h,v 1.6 2003/01/30 17:26:58 christof Exp $
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

#ifndef AUX_KALENDERWOCHE_H
#define AUX_KALENDERWOCHE_H
#include <exception>

class Kalenderwoche
{	int yyyyww;
 public:
	class error : public std::exception 
	{public:
		virtual const char* what() const throw() { return "Kalenderwoche::error"; }
	};

 	Kalenderwoche(int _yyyyww) throw(error) : yyyyww(_yyyyww)
 	{ check(); }
 	// fixes 2 digit years
 	Kalenderwoche(int week,int year) throw(error)
 	{ if (year<1900) year+=1900;
 	  if (year<1970) year+=100;
 	  yyyyww=year*100+week;
 	  check(); 
 	}
 	Kalenderwoche() throw() : yyyyww(0) {}
 	int Jahr() const throw() { return yyyyww/100; }
 	int Woche() const throw() { return yyyyww%100; }
 	operator int() const throw() { return yyyyww; }
 	bool valid() const throw()
 	{ return Jahr()<=2100 && Jahr()>=1970 && Woche()>=1 && Woche()<=53; }
 	void check() const throw(error) 
 		{ 
 			if (!valid()) 
 				throw error(); 
 		}
};
#endif
