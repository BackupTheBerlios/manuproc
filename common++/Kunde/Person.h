// $Id: Person.h,v 1.7 2002/03/20 07:43:31 christof Exp $
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
//#include<Kunde/Kunde.h>
#include<Aux/SQLerror.h>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>

//class cH_Person;
class cH_Telefon;
#include "Anrede.h"
#include <list>

class Person : public HandleContent
{
public:
        typedef long int ID;
private:
// Kunde::ID kundennr;
 ID personennr;	
 std::string name;
 std::string vorname;
 Petig::Datum gebdatum;
 cH_Anrede anrede;
 std::string notiz;
// ,notiz_firma;
 
 friend class Handle<const Person>;
 friend class Handle<Person>;
 static const ID _illegal=-1; 

 static int nextval();

public:
 ID Id() const { return personennr; } 
// Kunde::ID KId() const { return kundennr; }
// Person() : kundennr(0), personennr(0) {}
 Person() : personennr(0) {}
 Person(ID pid) throw(SQLerror);
 static int createPerson(std::string s="");

 static void delPerson(const ID pid) throw(SQLerror);
 
 static const ID none_id=_illegal;

 const std::string Name() const { return name; }
 const std::string Vorname() const { return vorname; }
 const Petig::Datum GebDatum() const { return gebdatum; }
 const cH_Anrede Anrede() const { return anrede; }
// const std::string Position() const { return position; }
 const std::string Notiz() const { return notiz; }
// const std::string NotizFirma() const { return notiz_firma; }

 const std::list<cH_Telefon> getTelefon() const;
 
// void setKundennr(unsigned long int i) { kundennr=i; }
 void setName(const std::string &s) { name=s; }
 void setVorname(const std::string &s) { vorname=s; }
 void setGebDatum(const Petig::Datum &d) { gebdatum=d; }
 void setAnrede(const cH_Anrede &s) { anrede=s; }
// void setPosition(const std::string &s) { position=s; }
 void setNotiz(const std::string &s) { notiz=s; }
// void setNotizFirma(const std::string &s) { notiz_firma=s; }

private:
 enum B_UPDATE_BITS{B_Name,B_Vorname,B_GebDatum,B_Anrede,B_Notiz,B_MaxAnz};
public:
 enum UpdateBits{FName=1<<B_Name,FVorname=1<<B_Vorname,
                 FGebDatum=1<<B_GebDatum,FAnrede=1<<B_Anrede,
                 FNotiz=1<<B_Notiz};
 void update_e(UpdateBits e) throw(SQLerror);	

 bool operator==(const Person& b) const
       {return Id()==b.Id();} 
 bool operator<(const Person& b) const
       {return Id()<b.Id();}
                                                  


};


class cH_Person : public Handle<const Person>
{	
    typedef CacheStatic<Person::ID,cH_Person> cache_t;
    static cache_t cache;
    cH_Person(const Person *p) : Handle<const Person>(p) {}
    friend class std::map<long int, cH_Person>;
    cH_Person() {}
public:
//   cH_Person(const Person *p) : Handle<const Person>(p) {}	
	typedef Person::ID ID;
	static const ID none_id=Person::none_id;
	cH_Person(ID nr);
};

/*
class H_Person : public Handle<Person>
{	
          
  H_Person() {}
public:
  H_Person(Person *p) : Handle<Person>(p) {}	
	typedef Person::ID ID;
	static const ID none_id=Person::none_id;
	H_Person(ID nr);
};
*/

#endif

