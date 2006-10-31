// $Id: Person.h,v 1.17 2006/10/31 16:03:12 christof Exp $
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

#include<Misc/Datum.h>
#include<Misc/SQLerror.h>
#include <Misc/Handles.h>
#include <Misc/CacheStatic.h>
#include <Kunde/TelefonArt.h>
#include <BaseObjects/ManuProcHandle.h>
#include <Kunde/KontaktListe.h>
#include <Kunde/Anrede.h>
#include <list>

class Kunde;

class Person : public ManuProcHandle<>
{
public:
private:
 std::string name;
 std::string vorname;
 ManuProC::Datum gebdatum;
 cH_Anrede anrede;
 std::string notiz;
 mutable KontaktListe kontakt;
 
 friend class Handle<const Person>;
 friend class Handle<Person>;

 static int nextval();

public:
 ID Id() const { return entityid; } 
 Person() : anrede(Anrede::default_id) {}
 Person(const ID pid) throw(SQLerror);
 static int createPerson(std::string s="");

 static void delPerson(const ID pid) throw(SQLerror);
 
 const std::string Name() const { return name; }
 const std::string Vorname() const { return vorname; }
 const ManuProC::Datum GebDatum() const { return gebdatum; }
 const cH_Anrede Anrede() const { return anrede; }
 const std::string Notiz() const { return notiz; }

 std::string Kontakt(const TelArt& art, ManuProcHandle<>::ID kundeid,
 		bool forcereload=false) const;
 const std::list<cH_Telefon> getTelefon(ManuProcHandle<>::ID kundenid=
 				ManuProcHandle<>::none_id,
 				bool forcereload=false) const; 
 
 void setName(const std::string &s) { name=s; }
 void setVorname(const std::string &s) { vorname=s; }
 void setGebDatum(const ManuProC::Datum &d) { gebdatum=d; }
 void setAnrede(const cH_Anrede &s) { anrede=s; }
 void setNotiz(const std::string &s) { notiz=s; }

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
                                                  

friend enum Person::B_UPDATE_BITS &operator++(enum Person::B_UPDATE_BITS &s);
};


class cH_Person : public Handle<const Person>
{	
    typedef CacheStatic<Person::ID,cH_Person> cache_t;
    static cache_t cache;
    cH_Person(const Person *p) : Handle<const Person>(p) {}
    friend class std::map<long int, cH_Person>;
    cH_Person() {}
public:
	typedef Person::ID ID;
	static const ID none_id=Person::none_id;
	cH_Person(ID nr);
};


#endif

