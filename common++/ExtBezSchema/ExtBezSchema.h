// $Id: ExtBezSchema.h,v 1.6 2001/08/31 10:03:07 christof Exp $
/*  libcommonc++: ManuProC's main OO library
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

#ifndef EXTBEZSCHEMA
#define EXTBEZSCHEMA
#include<Aux/SQLerror.h>
#include<vector>
#include<string>
#include <Aux/Handles.h>
#include <Artikel/ArtikelTyp.h>

class cH_ExtBezSchema;

class ExtBezSchema : public HandleContent
{
public:
	typedef unsigned int ID;
	static const ID default_ID=1;
	static const ID default_id=default_ID;
	static const ArtikelTyp::typ default_Typ=ArtikelTyp::AufgemachtesBand;
	struct BezKomp
	{	int bezkomptype; // =Index
		std::string bezkomptext; // Überschrift
		std::string separator;
		std::string spaltenname;
		int signifikanz;
		std::string TeXtabformat;

		BezKomp(int _bztyp, const std::string &_bztxt, 
			const std::string &_bzsep, const std::string &sname,int sign,std::string texf)
		: bezkomptype(_bztyp), bezkomptext(_bztxt), separator(_bzsep)
			, spaltenname(sname), signifikanz(sign), TeXtabformat(texf)
		{}
	};
	typedef std::vector<BezKomp>::const_iterator const_iterator;
	
	class const_sigiterator
	{	typedef const_sigiterator self;
		const_iterator actual;
		const const_iterator end;
		const int signifikanz;
	public:
		const_sigiterator(const_iterator ci,const_iterator e,int s)
			: actual(ci), end(e), signifikanz(s) 
		{  if (actual!=end && actual->signifikanz!=signifikanz)
		      ++(*this);
		}
		self &operator++()
		{  while (actual!=end)
		   {  ++actual;
		      if (actual==end || actual->signifikanz==signifikanz) 
		      	 break;
		   }
		   return *this;
		}
		self operator++(int)
		{  self tmp=*this;
		   ++(*this);
		   return tmp;
		}
		bool operator==(const const_sigiterator &b) const
		{  return actual==b.actual;
		}
		bool operator!=(const const_sigiterator &b) const
		{  return actual!=b.actual;
		}
		const BezKomp &operator*() const
		{  return *actual;
		}
		const BezKomp *operator->() const
		{  return &*actual;
		}
	};
  
private:
	ID extartbezid;
	ArtikelTyp typ;
        std::string jumbotitel,jumbobez,jumbospalte;
        int jumboindex;
   
	std::vector<BezKomp> bezkomps;
 
	// for the handle class
//	friend class ExtBezSchemaHandle;
	friend class cH_ExtBezSchema;

private: // I don't like these API - Christof
 const std::string BezKompText(unsigned int bkt) const 
 { return bkt<bezkomps.size()?bezkomps[bkt].bezkomptext:std::string(); }
 const std::string BezKompText(const_iterator i) const { return (*i).bezkomptext;}

 int BezKompType(unsigned int bkt) const 
 { return bkt<bezkomps.size()?bezkomps[bkt].bezkomptype:-1;}  
 int BezKompType(const_iterator i) const { return (*i).bezkomptype;}  

 const std::string BezSeparator(const_iterator i) const { return (*i).separator; }
 const std::string BezSeparator(unsigned int bkt) const 
 { return bkt<bezkomps.size()?bezkomps[bkt].separator:std::string(); }

 const static unsigned int first_index=1; // this is now invalid

public:
  ExtBezSchema(ID eid=default_ID,const ArtikelTyp &t=default_Typ) throw(SQLerror);

// int AnzBezKomp() const { return bezkomps.size();}
 ID Id() const { return extartbezid; }
 int JumboIndex() const {return jumboindex;}
 std::string JumboBez() const {return jumbobez;}
 std::string JumboTitel() const {return jumbotitel;}
 std::string JumboSpalte() const {return jumbospalte;}

 const ArtikelTyp &Typ() const { return typ; }
 
 size_t size(int signifikanz) const
 {  return sigsize(signifikanz); }
 size_t sigsize(int signifikanz) const;
 const_sigiterator sigbegin(int signifikanz) const
 {  return const_sigiterator(begin(),end(),signifikanz); }
 const_sigiterator sigend(int signifikanz) const
 {  return const_sigiterator(end(),end(),signifikanz); }
 
 size_t size() const
 {  return bezkomps.size();}
 const_iterator begin() const
 { return bezkomps.begin(); }
 const_iterator end() const
 { return bezkomps.end(); }
 const std::string operator[](unsigned int i) const
 {  return i<bezkomps.size()?bezkomps[i].bezkomptext:std::string();}

 bool operator==(const ExtBezSchema &b) const
 { return Id()==b.Id() && Typ()==b.Typ(); }
};

#endif
