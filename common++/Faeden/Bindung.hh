// $Id: Bindung.hh,v 1.3 2002/07/05 12:35:01 christof Exp $
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

#ifndef BINDUNG_HH
#define BINDUNG_HH

#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algo.h>
#include <Aux/SQLerror.h>

class Bindung {
public:
	typedef int ID;
	typedef enum {NIL=0,EFFBREITE=1} ExtraFunc;

private:
	string name;
	ID id;
	typedef map<ExtraFunc, string> Efs;
	Efs ef;
	friend ostream& operator<< (ostream&, const Bindung&);
public:
	Bindung () {}
	Bindung (const ID i) throw(SQLerror);
	Bindung (const string &s, const ID i) : name(s), id(i) {}
	bool operator== (const Bindung&) const;
	bool operator!= (const Bindung&) const;
	bool operator== (const ID i) const { return i == id; }
	bool operator!= (const ID i) const { return i != id; }
	bool operator== (const char *s) { return s == name; }
	bool operator!= (const char *s) { return s != name; }
	bool operator== (const string s) const { return s == name; }
	bool operator!= (const string s) const { return s != name; }
	string getName() const { return name; }
	const char* getCName() const { return name.c_str(); }
	ID getId() const { return id; }
	const string extraFunctionData(const ExtraFunc efid) const 
			{ return const_cast<Efs&>(ef)[efid]; }
	bool haveExtraFunction(ExtraFunc ex) const;
};

class Bindungsliste {
	vector<Bindung> liste;
public:
	typedef vector<Bindung>::const_iterator const_iterator;
	typedef Bindung::ID ID;
	Bindungsliste() : liste() {}
	void Load();
	const_iterator begin() const { return liste.begin(); }
	const_iterator end() const { return liste.end(); }
	size_t size() const { return liste.size(); }
	void add (const Bindung b) { liste.push_back (b); }
	int getIdByName (const char*) const;
	Bindung getByName (const string) const;
	Bindung	getById (const ID) const;
	void print_out() const;
	bool empty() const { return liste.empty(); }
};

#endif
