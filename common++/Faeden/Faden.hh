#ifndef FADEN_HH
#define FADEN_HH

#include <string>
#include <vector>
#include <iterator>
#include <algo.h>
#include "Bindung.hh"
#include <Artikel/ArtikelBase.h>
#include <Aux/fixedpoint.h>

class Faden {
        unsigned int anzahl;
        ArtikelBase::ID material;
        Bindung bindung;
	friend ostream& operator<< (ostream&, const Faden&);
public:
	Faden (unsigned int, ArtikelBase::ID, Bindung=Bindung());
	bool operator== (const Faden&) const;
	bool operator!= (const Faden&) const;
	int getAnzahl() const { return anzahl; }
	int displayBreite() const;
	ArtikelBase::ID getMaterial() const { return material; }
	Bindung getBindung() const { return bindung; }
	void setAnzahl (const int z) { anzahl = z; }
   fixedpoint<5> get_Gewicht_kg_pro_km_Faden() const ;
   fixedpoint<5> get_Gewicht_kg_mal_anzschfaeden_pro_km_und_breite_mm_und_schussdichte_cm() const ;
};

class Wiederholung {
	unsigned int start, end, anzahl;
public:
	Wiederholung (const unsigned int s, const unsigned int e, const unsigned int a);
	bool operator== (const Wiederholung&) const;
	bool operator< (const Wiederholung&) const;
	unsigned int getStart() const { return start; }
	unsigned int getEnd() const { return end; }
	unsigned int getAnzahl() const { return anzahl; }
	void setStart (const unsigned int z) { start = z; }
	void setEnd (const unsigned int z) { end = z; }
	void setAnzahl (const unsigned int z) { anzahl = z; }
};

class Fadenliste {
	vector<Faden> liste;
	vector<Faden> sumliste;
	vector<Wiederholung> repliste;
	vector<unsigned int> repnumliste;
public:
	typedef vector<Faden>::const_iterator const_iterator;
	typedef vector<Faden>::const_iterator const_sumiterator;
	typedef vector<Wiederholung>::const_iterator const_repiterator;
	
	Fadenliste() : liste(), sumliste(), repliste(), repnumliste() {}
	void Load(const ArtikelBase &ab,const Bindungsliste &bindungsliste);
	
	const_iterator begin() const { return liste.begin(); }
	const_iterator end() const { return liste.end(); }
	const_sumiterator sumbegin() const { return sumliste.begin(); }
	const_sumiterator sumend() const { return sumliste.end(); }
	const_repiterator repbegin() const { return repliste.begin(); }
	const_repiterator repend() const { return repliste.end(); }
	
	int add (const Faden, const unsigned int);
	bool del (const unsigned int, unsigned  int&);
	bool rep_add (const unsigned int, const unsigned int, const unsigned int);
	bool rep_del (const unsigned int, const unsigned int);
	void erase();
	Faden getSumByIndex (const unsigned int i) const { return sumliste [i]; }
	size_t size() const { return liste.size(); }
	size_t sumsize() const { return sumliste.size(); }
	void print_out() const;
	
	void sort_sumliste();
	void EntfalteWiederholungen(vector<Faden> &liste_out) const;
private:
	void EntfalteWiederholungen_recurse(vector<Faden> &liste_out,
		const_repiterator ri,const_iterator i,
		const_iterator e,unsigned int index) const;
};

#endif
