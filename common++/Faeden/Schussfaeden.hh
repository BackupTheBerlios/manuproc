// $Id: Schussfaeden.hh,v 1.7 2002/10/24 14:06:50 thoma Exp $
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

#ifndef SFADEN_HH
#define SFADEN_HH

// $Id: Schussfaeden.hh,v 1.7 2002/10/24 14:06:50 thoma Exp $

#include <Faeden/Faden.hh>
#include <Artikel/ArtikelBase.h>
#include <Aux/fixedpoint.h>

class Schussfaeden 
{
//public:
//	typedef fixedpoint<1> schussdichte_t;
private:
	std::vector<Faden> schussfaeden;
//	schussdichte_t schussdichte;
//	ArtikelBase::ID fangfaden;
public:
	typedef std::vector<Faden>::const_iterator const_iterator;
	
	Schussfaeden() {}
	void Load(const ArtikelBase &ab);

//	use: Webangaben::Schussdichte()
// schussdichte_t Schussdichte_cm() const { return schussdichte; }
// use: Webangaben::Fangfaden()
//	ArtikelBase::ID Fangfaden() const { return fangfaden; }

	const_iterator begin() const { return schussfaeden.begin(); }
	const_iterator end() const { return schussfaeden.end(); }
	size_t size() const { return schussfaeden.size(); }
	void clear() { schussfaeden.clear(); }
	void reset() { clear(); }
	const Faden &operator[](size_t index) const { return schussfaeden[index]; }

#if 0
	// ***** these do not yet alter the database ********
	void Schussdichte(schussdichte_t s) { schussdichte=s; }
	// ***** do not use these in new code! **************
#endif
	std::vector<Faden> &get_vector() { return schussfaeden; }
	typedef std::vector<Faden>::iterator iterator;
	iterator begin() { return schussfaeden.begin(); }
#if 0
	iterator end() { return schussfaeden.end(); }
	schussdichte_t Schussdichte() const { return Schussdichte_cm(); }
#endif
};
#endif
