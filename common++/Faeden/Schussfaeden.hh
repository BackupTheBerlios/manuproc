// $Id: Schussfaeden.hh,v 1.8 2003/07/17 07:26:09 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002-2003 Adolf Petig GmbH & Co. KG
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

#ifndef SFADEN_HH
#define SFADEN_HH

#include <Faeden/Faden.hh>
#include <Artikel/ArtikelBase.h>
#include <Aux/fixedpoint.h>

class Schussfaeden 
{
private:
	std::vector<Faden> schussfaeden;
public:
	typedef std::vector<Faden>::const_iterator const_iterator;
	
	Schussfaeden() {}
	void Load(const ArtikelBase &ab);

//	Schussdichte() and Fangfaden() are in Webangaben

	const_iterator begin() const { return schussfaeden.begin(); }
	const_iterator end() const { return schussfaeden.end(); }
	size_t size() const { return schussfaeden.size(); }
	void clear() { schussfaeden.clear(); }
	void reset() { clear(); }
	const Faden &operator[](size_t index) const { return schussfaeden[index]; }

	std::vector<Faden> &get_vector() { return schussfaeden; }
	typedef std::vector<Faden>::iterator iterator;
	iterator begin() { return schussfaeden.begin(); }
};
#endif
