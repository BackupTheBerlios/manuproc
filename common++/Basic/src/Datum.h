/* $Id: Datum.h,v 1.10 2002/06/20 06:29:53 christof Exp $ */
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
#include <iostream>
#include <exception>
#include <Aux/Kalenderwoche.h>
#include <string>

namespace Petig
{
class Datumsfehler;
class Datum;
};

/// Ein Datum ist ungültig
class Petig::Datumsfehler : public std::exception
{  /// eine Kombination aus tagfalsch, monatfalsch, jahrfalsch
   int falsch;
public:
   /** Werte für falsch: */
   static const int tagfalsch=1;
   static const int monatfalsch=2;
   static const int jahrfalsch=4;
   Datumsfehler(int _falsch) throw() 
   	:  falsch(_falsch)
   {}
   friend std::ostream &operator<<(std::ostream&,const Datumsfehler &);
   virtual const char* what() const throw() { return "Petig::Datumsfehler"; }
};

std::ostream &operator<<(std::ostream&,const Petig::Datumsfehler&);

class Petig::Datum
{	int tag;  	/* tt */ 
	int monat;	/* mm */
	int jahr;     /* jjjj */
	
public:
	class Formatfehler : public std::exception 
	{public:
		virtual const char* what() const throw() { return "Petig::Datum::Formatfehler"; }
	};
	
	/// aus Tag, Monat, Jahr erzeugen
	Datum(int t, int m, int j) throw(Datumsfehler)
	{	tag = t;
		monat = m;
		jahr = j;
		teste();
	}
	Datum() throw() /* initialize as invalid */
	{       tag=monat=jahr=0;
	}
	 
	/** Datum aus const char * erzeugen, Format erraten */
	Datum(const char *datum) throw(Datumsfehler)
	{  this->from_auto(datum);  }
	/// Datum aus time_t (time(3)) erzeugen
	explicit Datum(time_t t) throw();
	
	Datum(const Kalenderwoche &kw) throw(Datumsfehler);
	
	/// heutiges Datum
	static Datum today() throw();
	
	/// in Menschenlesbare Form bringen (NOT THREAD SAFE!)
	const char *c_str() const throw(Datumsfehler);
        const std::string Short() const throw(Datumsfehler);
	/// in Postgres Repräsentation wandeln
	void write_postgres(char *b,unsigned int sz) const throw(Datumsfehler);
   std::string to_iso() const throw(Datumsfehler);
	/// in Europäisches Format wandeln
	void write_euro(char *b,unsigned int sz) const throw(Datumsfehler);
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
        Datum operator+(int) const throw(Datumsfehler);
        Datum &operator+=(int tage)
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

	int Tage_in_Monat() const throw()
	{  return Tage_in_Monat(monat,jahr); }
        /// Wieviel Tage hat der Monat
	static int Tage_in_Monat(int monat,int jahr) throw()
	{  if (monat==2) return Schaltjahr(jahr)?29:28;
	   return 31-((monat+1+monat/8)&1);
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
	int Quartal() const { if(monat<4) return 1;
			      if(monat<7) return 2;
			      if(monat<10) return 3;
			      return 4;		
			 }
	
	friend std::ostream &operator<<(std::ostream&,const Datum&) throw();
	
	bool valid() const throw();
};

std::ostream &operator<<(std::ostream&,const Petig::Datum&) throw();

#endif
