// $Id: Telefon.h,v 1.12 2002/05/09 12:46:00 christof Exp $
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

#include<vector>
#include<string>
#include<Aux/SQLerror.h>
#include<Kunde/TelefonArt.h>
#include<BaseObjects/ManuProcEntity.h>

class cH_Telefon;

class Telefon : public ManuProcEntity
{
public:
// typedef int ID;
 const static int Landeskennzahl=49;
 const static int Vorwahl=202;

 
// static const ID none_id=0;

 struct st_nummer{int land;int vorwahl;int nummer;int durchwahl;TelArt art;
        st_nummer():land(0),vorwahl(0),nummer(0),durchwahl(-1),art(TEL_NONE) {}
        st_nummer(int l,int v,int n,int d,TelArt a) 
                : land(l),vorwahl(v),nummer(n),durchwahl(d),art(a) {}};
 struct st_tel{
 	st_nummer nummer; 
 	ID kid;
 	ID pid; 
 	std::string text;
        st_tel(st_nummer n, ID kundeid,
        	ID personid, const std::string t="") 
        : nummer(n),kid(kundeid),pid(personid),text(t) {}
        };
 
private: 

// ID telid;
 ID kunde;
 ID person;
 st_nummer nummer;
 std::string text; // email, web, ... 

// std::list<st_tel> vec_telefon;

  
 friend class Handle<const Telefon>;
 
public:
 
 Telefon() : kunde(none_id), person(none_id) {}
 Telefon(ID _tid) throw(SQLerror);

 static void getTelIDs(std::vector<ID> &vec, const TelArt &ta, 
 		const ID kid, const ID pid) throw(SQLerror);

 static const cH_Telefon newTelefon(const ID kid, const ID pid,
 		const st_nummer &nr, const std::string t="") throw(SQLerror);

 static void delTelefon(const Telefon::ID tid) throw(SQLerror);
 static void delPersonsTelefon(const ID pid) throw(SQLerror);
 
 st_nummer Nummer() const { return nummer; }
 std::string NummerStr() const;
 std::string Text() const { if(nummer.art==TEL_TEL || nummer.art==TEL_FAX)
 				return NummerStr();
 			    return text; }
 ID Id() const { return entityid; } 

 TelArt TelefonArt() const {return nummer.art;}
 std::string ArtString() const;
 ID getKunde() const { return kunde; }
 ID getPerson() const { return person; }
 void update() const throw(SQLerror);
 bool isPrivat() const { return (kunde==none_id &&
 				 person!=none_id); 
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
    cH_Telefon(const ManuProcEntity::ID _telid);
};


#endif

