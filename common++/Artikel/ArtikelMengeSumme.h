/* $Id: ArtikelMengeSumme.h,v 1.5 2002/11/22 15:31:04 christof Exp $ */
/*  pps: ManuProC's production planning system
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

#ifndef ARTIKELMENGESUMME_H
#  define ARTIKELMENGESUMME_H

#include <Aux/fixedpoint.h>
#include <vector>
#include <string>
#include <algorithm>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/Einheiten.h>

class ArtikelMenge
{
public:
	typedef fixedpoint<ManuProC::Precision::ArtikelMenge> mengen_t;
	typedef std::pair<Einheit,mengen_t> pair_t;
	typedef std::vector<pair_t> vector_t;

	typedef fixedpoint<ManuProC::Precision::ArtikelMenge,double,long long> mengen_l_t;
	typedef std::pair<Einheit,mengen_l_t> pair_l_t;
	typedef std::vector<pair_l_t> vector_l_t;
private:
	ArtikelBaum ab;
	vector_t emenge;

	mengen_t &lvalue(const Einheit &e);
	void summiere(const ArtikelBaum &baum, mengen_t menge,const Einheit &e, bool add=true);
public:
	ArtikelMenge(const ArtikelBase &ab, int Stueck, mengen_t Menge=0);
	std::string abgeleiteteMenge() const;
	std::string Menge() const;
	std::string Menge(const Einheit &e) const;
	const vector_t &abgeleiteteMenge_V() const
	{ return emenge; }
	Einheit getEinheit() const 
	{ return emenge.begin()==emenge.end()?Einheit(Einheit::default_id):emenge[0].first; }
	mengen_t getMenge() const 
	{ return emenge.begin()==emenge.end()?mengen_t(0):emenge[0].second; }
	// Menge in einer Einheit
	mengen_t getMenge(const Einheit &e) const;
	
	const ArtikelBaum &Artikel() const
	{ return ab; }
	
	static std::string Formatiere(const pair_t &p);
	static std::string Formatiere(const pair_l_t &p);
	
	typedef vector_t::const_iterator const_iterator;
	const_iterator begin() const
	{ return emenge.begin(); }
	const_iterator end() const
	{ return emenge.end(); }
	vector_t::size_type size() const
	{ return emenge.size(); }
	bool empty() const
	{ return emenge.empty(); }
};

class ArtikelMengeSumme
{public:
	typedef ArtikelMenge::mengen_l_t mengen_t;
	typedef ArtikelMenge::pair_l_t pair_t;
	typedef ArtikelMenge::vector_l_t vector_t;
private:
	vector_t emenge;
	bool einfach;

	mengen_t &lvalue(const Einheit &e);
public:
	ArtikelMengeSumme() : einfach(true) {}
	void cumulate(const ArtikelMenge &am);
	std::string abgeleiteteMenge() const;
	std::string Menge() const;
	std::string Menge(const Einheit &e) const;
	const vector_t &Summe() const
	{  return emenge; }
	// Menge in einer Einheit
	mengen_t Summe(const Einheit &e) const;
	bool ist_einfach() const
	{  return einfach; }
	bool empty() const
	{  return emenge.empty(); }
};
#endif
