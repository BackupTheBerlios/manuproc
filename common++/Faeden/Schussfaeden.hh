// $Id: Schussfaeden.hh,v 1.5 2002/07/05 12:35:01 christof Exp $
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

// $Id: Schussfaeden.hh,v 1.5 2002/07/05 12:35:01 christof Exp $

#include <Faeden/Faden.hh>
#include <Artikel/ArtikelBase.h>
#include <Aux/fixedpoint.h>

class Schussfaeden
{public:
	typedef fixedpoint<1> schussdichte_t;
private:
	vector<Faden> schussfaeden;
	schussdichte_t schussdichte;
	ArtikelBase::ID fangfaden;
public:
	typedef vector<Faden>::const_iterator const_iterator;
	
	Schussfaeden() : schussdichte(0), fangfaden(0)
	{}
	void Load(const ArtikelBase &ab);
	schussdichte_t Schussdichte_cm() const { return schussdichte; }
	ArtikelBase::ID Fangfaden() const { return fangfaden; }
	const_iterator begin() const { return schussfaeden.begin(); }
	const_iterator end() const { return schussfaeden.end(); }
	size_t size() const { return schussfaeden.size(); }
	void clear() 
	{ schussfaeden.clear(); schussdichte=0; fangfaden=0; }
	void reset()
	{ clear(); }
	const Faden &operator[](size_t index) const { return schussfaeden[index]; }

	// ***** these do not yet alter the database ********
	void Schussdichte(schussdichte_t s) { schussdichte=s; }
	// ***** do not use these in new code! **************
	vector<Faden> &get_vector() { return schussfaeden; }
	typedef vector<Faden>::iterator iterator;
	iterator begin() { return schussfaeden.begin(); }
	iterator end() { return schussfaeden.end(); }
	schussdichte_t Schussdichte() const { return Schussdichte_cm(); }
};
#endif
