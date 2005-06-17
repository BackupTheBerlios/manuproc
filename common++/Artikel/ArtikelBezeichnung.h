// $Id: ArtikelBezeichnung.h,v 1.24 2005/06/17 15:17:06 christof Exp $
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


#ifndef ARTIKELBEZEICHNUNG_H
#define ARTIKELBEZEICHNUNG_H

#include <Artikel/ArtikelBase.h>
#include <Misc/EntryValueIntString.h>
#include <Kunde/Kunde.h>
#include <vector>
#include <string>
#include <ExtBezSchema/ExtBezSchemaHandle.h>

class cH_ArtikelBezeichnung;

class ArtikelBezeichnung : public virtual ArtikelBase, protected HandleContent
{	friend class Handle<const ArtikelBezeichnung>;
	friend class cH_ArtikelBezeichnung;
public:
	typedef std::vector<cH_EntryValue>::const_iterator const_iterator;

	class const_sigiterator
	{	typedef const_sigiterator self;
		ExtBezSchema::const_sigiterator actual,end;
		const ArtikelBezeichnung &container;
	public:
		const_sigiterator(ExtBezSchema::const_sigiterator a,
				ExtBezSchema::const_sigiterator e,
				const ArtikelBezeichnung &ab)
		: actual(a), end(e), container(ab)
		{}
		const_sigiterator(ExtBezSchema::const_sigiterator e,
				const ArtikelBezeichnung &ab)
		: actual(e), end(e), container(ab)
		{}
		self &operator++()
		{  ++actual; return *this; }
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
		// damit bin ich noch nicht gluecklich, wegen der notwendigen
		// 2. Indirektion, vielleicht sollte ein const EntryValueBase&
		// zurueckgegeben werden ?
		// Untersuchen bezueglich Reference Count etc.
		const cH_EntryValue operator*() const;
		// geht nicht da temporary
//		const cH_EntryValue *operator->() const;
		
		const ExtBezSchema::BezKomp &BezKomp() const
		{  return *actual;
		}
	};
	
private:
 	cH_ExtBezSchema schema;
	std::vector<cH_EntryValue> value;

 // eigentlich nicht so toll wird aber ab und an intern verwendet
 const ArtikelBezeichnung &operator=(const ArtikelBezeichnung &b);
 ArtikelBezeichnung(const ArtikelBezeichnung &b);
 void setID(const ArtikelBase &id) throw(SQLerror);

// nur über cH_ArtikelBezeichnung verwenden !!!
 ArtikelBezeichnung(const ArtikelBase &artikel,
 	const ExtBezSchema::ID schema=ExtBezSchema::default_id) throw();

/// Artikel zu der externen Bezeichnung für einen Kunden erzeugen 
/// (ID nach Bezeichnung ermitteln)
/// Vorsicht: Der Artikeltyp muss stimmen!
 ArtikelBezeichnung(int signifikanz, const std::vector<cH_EntryValue> &values, const cH_ExtBezSchema &schema) throw(SQLerror);

// keine Datenbankkzugriffe nötig, ArtikelBase wird bereits übergeben
 ArtikelBezeichnung(const ArtikelBase &artikel, const std::vector<cH_EntryValueIntString> &values, const cH_ExtBezSchema &schema) throw(); 

public:
 ArtikelBezeichnung() : schema(ExtBezSchema::default_id) {}

 std::string Bezeichnung(int signifikanz=1, char separator=0,int felder=-1) const throw();
 std::string Komponente(int feldnr,int signifikanz=1) const throw()
 {  return Komponente_als_EntryValue(feldnr,signifikanz)->getStrVal(); }
 cH_EntryValue Komponente_als_EntryValue(int feldnr,int signifikanz=1) const throw();
 
 void setzeExtBezSchema(const cH_ExtBezSchema &schema) throw(SQLerror);
 void setzeExtBezSchema(const std::vector<cH_EntryValueIntString> &_val,
		const cH_ExtBezSchema &_schema);
 
 const cH_ExtBezSchema getExtBezSchema() const throw()
 {  return schema;
 }
 bool operator<(const ArtikelBezeichnung& b) const
   { cH_EntryValue a1=Komponente_als_EntryValue(0);
     cH_EntryValue a2=Komponente_als_EntryValue(1);
     cH_EntryValue a3=Komponente_als_EntryValue(2);
     cH_EntryValue a4=Komponente_als_EntryValue(3);
     cH_EntryValue b1=b.Komponente_als_EntryValue(0);
     cH_EntryValue b2=b.Komponente_als_EntryValue(1);
     cH_EntryValue b3=b.Komponente_als_EntryValue(2);
     cH_EntryValue b4=b.Komponente_als_EntryValue(3);
     return   a1 <b1 || 
             (a1==b1 && (a2 <b2 ||
                       (a2==b2 && (a3 <b3 ||
                                  (a3==b3 && a4 <b4)))));
         }

 const_sigiterator begin() const
 {  return sigbegin(1);
 }
 const_sigiterator end() const
 {  return sigend(1);
 }
 size_t size() const
 {  return sigsize(1);
 }
 size_t sigsize(int signifikanz=1) const
 {  return schema->sigsize(signifikanz);
 }
 const_sigiterator sigbegin(int signifikanz=1) const
 {  return const_sigiterator(schema->sigbegin(signifikanz),
 	schema->sigend(signifikanz),*this);
 }
 const_sigiterator sigend(int signifikanz=1) const
 {  return const_sigiterator(schema->sigend(signifikanz),*this);
 }
 // Zuweisung nicht sinnvoll !? daher const,
 // this might be not the right function for you, use sigbegin()/sigend() !
 const cH_EntryValue operator[](int feld) const throw(SQLerror)
 {  return feld<(int)value.size()?value[feld]:cH_EntryValue(); }

#if (defined PETIG_EXTENSIONS)  || (defined MABELLA_EXTENSIONS)
 int Breite() const { return Komponente_als_EntryValue(1,1)->getIntVal(); }
 bool Jumbo() const { return (*this)[schema->JumboIndex()]->getStrVal()==schema->JumboBez(); }
#endif

  static void Anlegen(const cH_ExtBezSchema &schema,const ArtikelBase &art,
  	const std::string &columns, const std::string &values);
  static std::string Tabellenname(const cH_ExtBezSchema &s);
 
// deprecated
private:
 ArtikelBezeichnung(const ArtikelBase &artikel,const cH_ExtBezSchema &schema) throw();
};

class cH_ArtikelBezeichnung : public Handle<const ArtikelBezeichnung>
{	// cache
	struct cache_key
	{  ExtBezSchema::ID sid;
	   ArtikelBase::ID aid;
	   
	   cache_key(const ExtBezSchema::ID &s,const ArtikelBase::ID &a)
	   : sid(s), aid(a) {}
	   bool operator<(const cache_key &b) const
	   {  return sid<b.sid || (sid==b.sid && aid<b.aid); }
	};
	typedef CacheStatic<cache_key,cH_ArtikelBezeichnung> cache_t;
	static cache_t cache;
	
	cH_ArtikelBezeichnung(const ArtikelBezeichnung *b)
		: Handle<const ArtikelBezeichnung>(b) {}
//	friend cache_t::stl_type;
	friend class std::map<cache_key, cH_ArtikelBezeichnung>;
	cH_ArtikelBezeichnung() {}
	
	// you don't want this to work silently !
	cH_ArtikelBezeichnung(const ArtikelBezeichnung &b);

public:
	// please use variant 2
	cH_ArtikelBezeichnung(const ArtikelBase &artikel,const cH_ExtBezSchema &schema) throw(SQLerror);
	// recommended variant!
	cH_ArtikelBezeichnung(const ArtikelBase &artikel,const ExtBezSchema::ID id=ExtBezSchema::default_id) throw(SQLerror);

/// Artikel zu der externen Bezeichnung für einen Kunden erzeugen 
/// (ID nach Bezeichnung ermitteln)
	cH_ArtikelBezeichnung(int signifikanz, const std::vector<cH_EntryValue> &values, const cH_ExtBezSchema &schema) throw(SQLerror)
		: Handle<const ArtikelBezeichnung>(new ArtikelBezeichnung(signifikanz,values,schema))
	{}

/// Objekt erzugen ohne Datenbankzugriffe, alles wird bereits übergeben
	cH_ArtikelBezeichnung(
	              const ArtikelBase &artikel, 
	              const std::vector<cH_EntryValueIntString> &values, 
                      const cH_ExtBezSchema &schema) throw()
		: Handle<const ArtikelBezeichnung>(new ArtikelBezeichnung(artikel,values,schema))
	{}	
	
/// default ctor
	struct Default { Default(){} };
	// call it like: cH_ArtikelBezeichnung(cH_ArtikelBezeichnung::Default());
	cH_ArtikelBezeichnung(const Default &d) 
		: Handle<const ArtikelBezeichnung>(new ArtikelBezeichnung())
	{}

       static void Deregister(ExtBezSchema::ID sid, ArtikelBase::ID aid)
           {cache.deregister(cache_key(sid,aid));}
};

#endif
