#ifndef SFADEN_HH
#define SFADEN_HH

// $Id: Schussfaeden.hh,v 1.4 2002/05/09 12:46:00 christof Exp $

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
