/* $Id: Datum.h,v 1.27 2005/09/05 12:27:30 jacek Exp $ */
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

#ifndef DATUM_H
#define DATUM_H
#include <time.h>
//#include <iostream>
#include <exception>
#include <Misc/Kalenderwoche.h>
#include <string>
#include <Misc/compiler_ports.h>

namespace ManuProC
{
class Datumsfehler;
class Datum;
};

#include <iosfwd>
//namespace std { class ostream; }

std::ostream &operator<<(std::ostream&,const ManuProC::Datumsfehler&);

/// Ein Datum ist ungültig
class ManuProC::Datumsfehler : public std::exception
{  /// eine Kombination aus tagfalsch, monatfalsch, jahrfalsch
   int falsch;
public:
   /** Werte für falsch: */
   static const int tagfalsch=1;
   static const int monatfalsch=2;
   static const int jahrfalsch=4;
   Datumsfehler(int _falsch) throw();
   friend std::ostream &NOTGCC295(::)operator<<(std::ostream&,const Datumsfehler &);
   virtual const char* what() const throw() { return "ManuProC::Datumsfehler"; }
};

std::ostream &operator<<(std::ostream&,const ManuProC::Datum&) throw();


class ManuProC::Datum
{	mutable unsigned int woche:6;	/* KW */
	mutable int woche_jahrdiff:2;   /* Diff. des Jahres in KW zum Jahr im Datum */
	mutable unsigned int quart:3;	/* Quartal */
	unsigned int tag:5;  	/* tt */ 
	unsigned int monat:4;	/* mm */
	unsigned int jahr:12;     /* jjjj */


 static const char* const monate[];
 	
public:
	class Formatfehler : public std::exception 
	{public:
		virtual const char* what() const throw() { return "ManuProC::Datum::Formatfehler"; }
	};
	
	/// aus Tag, Monat, Jahr erzeugen (expandyear lässt 2stelliges Jahr zu)
	Datum(int t, int m, int j,bool expandyear=true) throw(Datumsfehler);
	
	Datum() throw() : woche(),woche_jahrdiff(),quart(),
	tag(),monat(),jahr()
	{} /* initialize as invalid */
	 
	/** Datum aus const char * erzeugen, Format erraten */
	Datum(const char *datum) throw(Datumsfehler,Formatfehler) :
		woche(),woche_jahrdiff(),quart()
	{  this->from_auto(datum);  }
	/// Datum aus time_t (time(3)) erzeugen
	explicit Datum(time_t t) throw();
	
	Datum(const Kalenderwoche &kw) throw(Datumsfehler);
	
	/// heutiges Datum
	static Datum today() throw();
	static Datum Infinity() throw();
	
	/// in Menschenlesbare Form bringen (NOT THREAD SAFE!)
	const char *c_str() const throw(Datumsfehler);
	const char *c_str_filled() const throw(Datumsfehler);
	
        const std::string Short() const throw(Datumsfehler);
	/// in Postgres Repräsentation wandeln
	void write_postgres(char *b,unsigned int sz) const throw(Datumsfehler);
   std::string to_iso() const throw(Datumsfehler);
   std::string postgres_null_if_invalid() const;
	/// in Europäisches Format wandeln
	void write_euro(char *b,unsigned int sz) const throw(Datumsfehler);
	std::string write_euro() const throw(Datumsfehler);
	/// Gültigkeit testen
	void teste() const throw(Datumsfehler);
	/// Datumsformat automatisch raten
        void from_auto(const char *datum,const char **endptr=0) throw(Datumsfehler,Formatfehler);
	/// Datum aus Postgres DATE oder TIMESTAMP erzeugen
        void from_postgres(const char *postgres_timestamp) throw(Datumsfehler,Formatfehler)
        {  from_auto(postgres_timestamp); }
        /// Datum aus Menschenlesbarer Form erzeugen
        void from_c_str(const char *s) throw(Datumsfehler,Formatfehler)
        {  from_auto(s); }
        
        /// zwei Daten sind gleich?
        bool operator==(const Datum &b) const throw()
        {  return b.tag==tag && b.monat==monat && b.jahr==jahr; }
        bool operator!=(const Datum &b) const throw()
        {  return !(*this==b);  }
        /// erstes Datum vor dem zweiten?
        bool operator<(const Datum &b) const throw(Datumsfehler);
        /** morgen
            noch nicht implementiert */
        Datum &operator++();
        Datum operator++(int);
        Datum operator+(unsigned int) const throw(Datumsfehler);
        Datum operator-(unsigned int) const throw(Datumsfehler);
        Datum &operator+=(unsigned int tage)
        {  return *this=*this+tage;
        }
        /** gestern
            noch nicht implementiert */
        Datum &operator--();
        Datum operator--(int);
        
        /// Differenz in Tagen
        int operator-(const Datum &b) const throw(Datumsfehler);

	/** führende Leerzeichen überlesen, dann Dezimalziffern in ulong wandeln
	    bis zu einer bestimmten Länge */
	static unsigned long getnum(const unsigned char *s,int len) throw();

	unsigned int Tage_in_Monat() const throw()
	{  return Tage_in_Monat(monat,jahr); }
        /// Wieviel Tage hat der Monat
	static unsigned int Tage_in_Monat(int monat,int jahr) throw()
	{  if (monat==2) return Schaltjahr(jahr)?29:28;
	   return (unsigned int) 31-((monat+1+monat/8)&1);
	}
	/// Ist dies ein Schaltjahr (1900<jahr<2099)
	static bool Schaltjahr(int jahr) throw()
	{  return (jahr%4==0); }
	/** Wieviele Tage hat das Jahr,
	    gültig 1901-2099 */
	static int Tage_im_Jahr(int jahr) throw()
	{  return Schaltjahr(jahr)?366:365; }
	
	/// Der [1..366]. Tag im Jahr
	int Julian() const throw(Datumsfehler);
	/** Tage seit 1.1.1900 
            <br>(1.1.1 waere möglich gewesen, 
	    aber: Kalenderreformationen sind ein Kreuz) */
	int Internal() const throw(Datumsfehler);
	/// Kalenderwoche
	Kalenderwoche KW() const throw(Datumsfehler);
	/// Wochentag: 0=Montag 6=Sonntag
	int Wochentag() const throw(Datumsfehler);
	
	/**member access */
	int Tag() const { return tag; }
	int Monat() const { return monat; }
	int Jahr() const { return jahr; }
	int Quartal() const { if(quart) return quart;
				quart=((monat-1)/3)+1;
			      return quart;
			 }
	std::string MonatName() const { return monate[monat-1];}
	static std::string MonatName(int m) { return monate[m-1];}	
	
	friend std::ostream &NOTGCC295(::)operator<<(std::ostream&,const Datum&) throw();
	
	bool valid() const throw();
	
};

class FetchIStream;
FetchIStream &operator>>(FetchIStream &is, ManuProC::Datum &v);
class ArgumentList;
ArgumentList &operator<<(ArgumentList &, const ManuProC::Datum &v);

namespace Petig
{  using ManuProC::Datumsfehler;
   using ManuProC::Datum;
}

#endif
