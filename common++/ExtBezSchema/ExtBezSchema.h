// $Id: ExtBezSchema.h,v 1.20 2003/10/31 15:29:02 jacek Exp $
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
#include<Misc/SQLerror.h>
#include<vector>
#include<string>
#include <Misc/Handles.h>
#include <Artikel/ArtikelTyp.h>
#include <Aux/multi_lang.h>

class cH_ExtBezSchema;

class ExtBezSchema : public HandleContent
{
public:
	typedef unsigned int ID;
	static const ID default_ID=1; // KundenId
	static const ID default_id=default_ID;
	static const ID none_id = 0;
	static const ArtikelTyp::ID default_Typ=ArtikelTyp::default_ID;
	struct BezKomp
	{	int bezkomptype; // =Index
		std::string bezkomptext; // Überschrift
		std::string separator;
		std::string spaltenname;
		int signifikanz;
		bool preissignifikant;
		int folgenr_in_sig;
		std::string TeXtabformat;
		MultiL_Dict::LangTXT textid; // for multilanguage text settings

		BezKomp(int _bztyp, const std::string &_bztxt, 
			const std::string &_bzsep, const std::string &sname,
			int sign,int snr,std::string texf, 
			MultiL_Dict::LangTXT _textid, bool psign=false)
		: bezkomptype(_bztyp), bezkomptext(_bztxt), separator(_bzsep)
			, spaltenname(sname), signifikanz(sign), 
			preissignifikant(psign),
			folgenr_in_sig(snr),TeXtabformat(texf),
			textid(_textid)
		{}
		BezKomp() 
		: bezkomptype(0), signifikanz(0), preissignifikant(false),
			folgenr_in_sig(0), 
			textid(MultiL_Dict::LangTXT(0))
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
	
	class const_psigiterator
	{	typedef const_psigiterator self;
		const_iterator actual;
		const const_iterator end;
		const bool psignifikant;
	public:
		const_psigiterator(const_iterator ci,const_iterator e,bool psig)
			: actual(ci), end(e), psignifikant(psig) 
		{  if (actual!=end && actual->preissignifikant!=psignifikant)
		      ++(*this);
		}
		self &operator++()
		{  while (actual!=end)
		   {  ++actual;
		      if (actual==end || actual->preissignifikant==psignifikant) 
		      	 break;
		   }
		   return *this;
		}
		bool operator==(const const_psigiterator &b) const
		{  return actual==b.actual;
		}
		bool operator!=(const const_psigiterator &b) const
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
	class ArtikelTyp typ;
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
  ExtBezSchema(ID eid=default_ID,const class ArtikelTyp &t=default_Typ) throw(SQLerror);

// int AnzBezKomp() const { return bezkomps.size();}
 ID Id() const { return extartbezid; }
 int JumboIndex() const {return jumboindex;}
 std::string JumboBez() const {return jumbobez;}
 std::string JumboTitel() const {return jumbotitel;}
 std::string JumboSpalte() const {return jumbospalte;}
private:
 static bool exist_schema_for_typ(ID eid,const class ArtikelTyp &t);
public:
 const class ArtikelTyp &Typ() const { return typ; }
 
 size_t size(int signifikanz) const
 {  return sigsize(signifikanz); }
 size_t sigsize(int signifikanz) const;
 const_sigiterator sigbegin(int signifikanz) const
 {  return const_sigiterator(begin(),end(),signifikanz); }
 const_sigiterator sigend(int signifikanz) const
 {  return const_sigiterator(end(),end(),signifikanz); }
 

// für Preissignifikanz; villeicht doch lieber ein template sig_iterator<>
 size_t size(bool psig) const
 {  return psigsize(psig); } 
 size_t psigsize(bool psig) const; 
  const_psigiterator psigbegin(bool psig) const
 {  return const_psigiterator(begin(),end(),psig); }
 const_psigiterator psigend(bool psig) const
 {  return const_psigiterator(end(),end(),psig); }

 
 size_t size() const
 {  return bezkomps.size();}
 const_iterator begin() const
 { return bezkomps.begin(); }
 const_iterator end() const
 { return bezkomps.end(); }
 
private: //wenn alle Clients der API damit kompil. sollte der Op. gelöscht werden
 const std::string operator[](unsigned int i) const
 {  return i<bezkomps.size()?bezkomps[i].bezkomptext:std::string();}

public:
 const BezKomp operator[](const std::string komp_bez) const throw()
 { for(const_iterator i=begin(); i!=end(); ++i)
     if((*i).bezkomptext==komp_bez) return (*i);
   throw;  
 }
 
 const BezKomp operator[](std::pair<int,int> ktyp_sig) const throw()
 { for(const_sigiterator i=sigbegin(ktyp_sig.second); 
 	i!=sigend(ktyp_sig.second); ++i)
     if((*i).bezkomptype==ktyp_sig.first) return (*i);
   throw;  
 } 



 bool operator==(const ExtBezSchema &b) const
 { return Id()==b.Id() && Typ()==b.Typ(); }
 
};

#endif
