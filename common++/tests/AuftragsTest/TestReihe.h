/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef TESTREIHE_H
#define TESTREIHE_H
#include <string>

struct TestReihe
{	typedef bool (*callback_t)(); // Rückgabe bislang ignoriert
	typedef bool (*callback_auftrag_t)(AufEintrag &);

	callback_t aufruf;
	callback_auftrag_t aufruf_mit_auftrag;
	std::string kuerzel,bezeichnung;
	TestReihe *next;
	
	static TestReihe *first;
	
	TestReihe(callback_t cb,const std::string &b,const std::string &k)
		: aufruf(cb), aufruf_mit_auftrag(), kuerzel(k), bezeichnung(b),
		  next(first)
	{  first=this;
	}
	TestReihe(callback_auftrag_t cb,const std::string &b,const std::string &k)
		: aufruf(), aufruf_mit_auftrag(cb), kuerzel(k), bezeichnung(b),
		  next(first)
	{  first=this;
	}
};

#endif
