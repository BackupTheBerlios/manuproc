// $Id: Telefon.h,v 1.13 2002/06/20 06:29:53 christof Exp $
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
 const static int Landeskennzahl=49;
 const static int Vorwahl=202;

 

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

 ID kunde;
 ID person;
 st_nummer nummer;
 std::string text; // email, web, ... 

  
 friend class Handle<const Telefon>;
 
public:
 
 Telefon() : kunde(none_id), person(none_id) {}
 Telefon(ID _tid) throw(SQLerror);
 Telefon(const TelArt &ta, const ID kid, const ID pid,
 		int _land, int _vorw, int _nummer, int _druchw,
 		const std::string _text)
 : kunde(kid), person(pid), nummer(_land,_vorw,_nummer,_druchw, ta), 
 	text(_text)
 {}

 static void getTelIDs(std::vector<ID> &vec, const TelArt &ta, 
 		const ID kid, const ID pid) throw(SQLerror);

 static const cH_Telefon create(const cH_Telefon newtel) throw(SQLerror); 		
 		
 static const cH_Telefon create(const ID kid, const ID pid,
 		const st_nummer &nr, const std::string t="");

 
 // ich bin für create() CP
 static const cH_Telefon newTelefon(const ID kid, const ID pid,
 		const st_nummer &nr, const std::string t="");

 static void delTelefon(const Telefon::ID tid) throw(SQLerror);
 static void delPersonsTelefon(const ID pid) throw(SQLerror);
 
 st_nummer Nummer() const { return nummer; }
 std::string NummerStr() const;
 std::string Text() const { if(nummer.art==TEL_TEL || nummer.art==TEL_FAX
 				|| nummer.art==TEL_MOB)
 				return NummerStr();
 			    return text; }
 std::string getText() const { return text; } 			    
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
    cH_Telefon(const TelArt &ta,
 		const Telefon::ID kid, const Telefon::ID pid,
 		int _land, int _vorw, int _nummer, int _druchw,
 		const std::string _text)
 	: Handle<const Telefon>(new Telefon(ta,kid,pid,_land,_vorw,_nummer,_druchw,_text))
    {}
};


#endif

