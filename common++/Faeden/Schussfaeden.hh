#ifndef SFADEN_HH
#define SFADEN_HH

// $Id: Schussfaeden.hh,v 1.1 2001/10/01 13:03:01 christof Exp $

#include "Faden.hh"
#include <Artikel/ArtikelBase.h>

class Schussfaeden
{	vector<Faden> schussfaeden;
	float schussdichte;
	ArtikelBase::ID fangfaden;
public:
	typedef vector<Faden>::const_iterator const_iterator;
	
	Schussfaeden() : schussdichte(0), fangfaden(0)
	{}
	void Load(const ArtikelBase &ab);
	float Schussdichte_cm() const { return schussdichte; }
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
	void Schussdichte(float s) { schussdichte=s; }
	// ***** do not use these in new code! **************
	vector<Faden> &get_vector() { return schussfaeden; }
	typedef vector<Faden>::iterator iterator;
	iterator begin() { return schussfaeden.begin(); }
	iterator end() { return schussfaeden.end(); }
	float Schussdichte() const { return Schussdichte_cm(); }
};
#endif
