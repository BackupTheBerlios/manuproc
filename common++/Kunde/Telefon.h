// $Id: Telefon.h,v 1.7 2002/03/20 07:43:31 christof Exp $
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


#ifndef KUNDE_TELEFON_HH
#define KUNDE_TELEFON_HH

#include<Aux/Datum.h>
#include<Kunde/Kunde.h>
#include<Kunde/Person.h>
#include<list>
#include<vector>
#include<Kunde/TelefonArt.h>

class cH_Telefon;

class Telefon : public HandleContent
{
public:
 typedef int ID;
 const static int Landeskennzahl=49;
 const static int Vorwahl=202;

 
 static const ID none_id=0;
// typedef enum {TEL_NONE=0,TEL_TEL='T',TEL_FAX='F',TEL_MOB='M',
//        TEL_E_MAIL='E',TEL_HOMEPAGE='W'} TelArt;
 struct st_nummer{int land;int vorwahl;int nummer;TelArt art;
        st_nummer():land(0),vorwahl(0),nummer(0),art(TEL_NONE) {}
        st_nummer(int l,int v,int n,TelArt a) 
                : land(l),vorwahl(v),nummer(n),art(a) {}};
 struct st_tel{st_nummer nummer;Kunde::ID kid;Person::ID pid;
        st_tel(st_nummer n,Kunde::ID k,Person::ID p) 
        : nummer(n),kid(k),pid(p) {}};
 
private: 

 ID telid;
 Kunde::ID kunde;
 Person::ID person;
 st_nummer nummer;
 std::list<st_tel> vec_telefon;
 
 friend class Handle<const Telefon>;
 
public:
 Telefon() : telid(none_id), kunde(Kunde::none_id), 
 		person(Person::none_id) {}
 Telefon(ID _tid) throw(SQLerror);
 std::list<Telefon::st_tel> get_Telefon(int nr,const std::string& mod) throw(SQLerror);

 static const cH_Telefon newTelefon(const Kunde::ID kid, const Person::ID pid,
 		const st_nummer &nr) throw(SQLerror);

 static void delTelefon(const ID tid) throw(SQLerror);
 static void delPersonsTelefon(const ID pid) throw(SQLerror);
 
 st_nummer Nummer() const { return nummer; }
 std::string NummerStr() const;
 ID Id() const { return telid; } 

 std::string ArtString() const;
 Kunde::ID getKunde() const { return kunde; }
 Person::ID getPerson() const { return person; }
 void update() const throw(SQLerror);
 bool isPrivat() const { return (kunde==Kunde::none_id &&
 				 person!=Person::none_id); 
 			}
// std::string PrivatStr() const {if (isPrivat()) return "*";else return "";}
 static vector<pair<std::string,TelArt> > getTelArtVec();

 bool operator==(const Telefon& b) const {return Id()==b.Id();}
};


class cH_Telefon : public Handle<const Telefon>
{	
  cH_Telefon() {}
public:
    cH_Telefon(const Telefon *p) : Handle<const Telefon>(p) {}	
    cH_Telefon(const Telefon::ID _tid);
};


#endif

