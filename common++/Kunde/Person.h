// $Id: Person.h,v 1.1 2001/04/23 08:11:59 christof Exp $
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


#ifndef KUNDE_PERSON_HH
#define KUNDE_PERSON_HH

#include<Aux/Datum.h>
#include<Kunde/Kunde.h>
#include<Aux/SQLerror.h>

class cH_Person;

class Person : protected HandleContent
{
public:
	typedef int ID;
	
private:
 Kunde::ID kundennr;
 ID personennr;	
 string name;
 string vorname;
 string position;
 Petig::Datum gebdatum;
 string anrede;
 string notiz;
 
 friend class const_Handle<Person>;
 static const ID _illegal=-1; 

public:
 ID Id() const { return personennr; } 
 Kunde::ID KId() const { return kundennr; }
 Person() : kundennr(0), personennr(0) {}
 Person(ID pid) throw(SQLerror);
 static cH_Person newPerson(const Kunde::ID _knr, const string &_name, const string &_vorname,
 	const string &_pos, const Petig::Datum &gd, const string &_anrede,
 	const string &_not) throw(SQLerror);
 void update() throw(SQLerror);	
 static void delPerson(const ID pid) throw(SQLerror);
 
 static const ID none_id=_illegal;

 const string Name() const { return name; }
 const string Vorname() const { return vorname; }
 const Petig::Datum GebDatum() const { return gebdatum; }
 const string Anrede() const { return anrede; }
 const string Position() const { return position; }
 const string Notiz() const { return notiz; }
 
 void setName(const string &s) { name=s; }
 void setVorname(const string &s) { vorname=s; }
 void setGebDatum(const Petig::Datum &d) { gebdatum=d; }
 void setAnrede(const string &s) { anrede=s; }
 void setPosition(const string &s) { position=s; }
 void setNotiz(const string &s) { notiz=s; }
};


class cH_Person : public const_Handle<Person>
{	
  cH_Person(const Person *p) : const_Handle<Person>(p) {}	
  cH_Person() {}
public:
	typedef Person::ID ID;
	static const ID none_id=Person::none_id;
	cH_Person(ID nr);
};


#endif

