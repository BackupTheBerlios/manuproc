// $Id: Zeitpunkt_new.h,v 1.14 2004/02/06 14:33:14 christof Exp $
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


#ifndef ZEITPUNKT_HH
#define ZEITPUNKT_HH

#include <Misc/Postgres.h>
#include <Misc/Datum.h>
//#include <Misc/relops.h>
#include <iosfwd>
#include <time.h>

//: Zeitpunkt an einem Tag
// möglicherweise von Datum ableiten?
// Dann würde allerdings manches komplizierter werden !!!
class Zeitpunkt_new
{public:
   class Days 
   {	int payload;
    public:
   	Days(int m) : payload(m) {}
   	operator int() const { return payload; }
   };
   class Hours 
   {	int payload;
    public:
   	Hours(int m) : payload(m) {}
   	operator int() const { return payload; }
   };
   class Minutes 
   {	int payload;
    public:
   	Minutes(int m) : payload(m) {}
   	operator int() const { return payload; }
   };
   class Seconds
   {	int payload;
    public:
   	Seconds(int m) : payload(m) {}
   	operator int() const { return payload; }
   };

   /// not fully implemented yet!
   enum precision { days,hours,minutes,seconds,milliseconds };
   class illegal_value {};
private:
//   static const int standard_zone=120;
   ManuProC::Datum datum;
   int hour,minute,second,millisecond;
   mutable int minutes_from_gmt; // time zone, CE[TD]ST=120
   precision prec;
   
   void calculate_TZ(int isdst=-1) const throw();
   void normalize_TZ() const throw();
public:
   Zeitpunkt_new() throw() : hour(0), minute(0), second(0), millisecond(0), minutes_from_gmt(0), prec(days) {}
   Zeitpunkt_new(ManuProC::Datum d) throw() 
   	: datum(d), hour(0), minute(0), second(0), millisecond(0), 
   	  minutes_from_gmt(0), prec(days) 
   {  calculate_TZ(); }
   Zeitpunkt_new(ManuProC::Datum d, int h) throw() 
   	: datum(d), hour(h), minute(0), second(0), millisecond(0), minutes_from_gmt(0), prec(hours) 
   {  calculate_TZ(); }
   Zeitpunkt_new(ManuProC::Datum d, int h, int m) throw() 
   	: datum(d), hour(h), minute(m), second(0), millisecond(0), minutes_from_gmt(0), prec(minutes) 
   {  calculate_TZ(); }
   Zeitpunkt_new(ManuProC::Datum d, int h, int m, int s) throw() 
   	: datum(d), hour(h), minute(m), second(s), millisecond(0), minutes_from_gmt(0), prec(seconds) 
   {  calculate_TZ(); }
   // correct wrapper
   Zeitpunkt_new(const PostgresTimestamp &a)
   {  *this=Zeitpunkt_new((const char *)a);
   }
   explicit Zeitpunkt_new(const char *a);
   explicit Zeitpunkt_new(time_t t) throw();
   
   /// write into char[]
   void write(PostgresTimestamp a) const;
   // convert to string
   std::string write() const;
   
   /// Differenz in min(Precision)
   int operator-(const Zeitpunkt_new &b) const throw();
   bool operator<(const Zeitpunkt_new &b) const throw();
   bool operator==(const Zeitpunkt_new &b) const throw();
   /// in Precision
//   Zeitpunkt_new operator+(int dist) const throw();
   Zeitpunkt_new operator+(const Days &dist) const throw();
   Zeitpunkt_new operator+(const Minutes &dist) const throw();
   Zeitpunkt_new operator+(const Seconds &dist) const throw();
   Zeitpunkt_new &Precision(precision p)
   {  prec=p; return *this; }
   // return a copy since we should not alter it
   Zeitpunkt_new Precision(precision p) const
   {  return withPrecision(p); }
   // make a copy
   Zeitpunkt_new withPrecision(precision p) const
   {  Zeitpunkt_new res=*this; res.Precision(p); return res; }
   precision Precision() const
   {  return prec; }
   
   operator ManuProC::Datum() const throw()
   {  return Datum(); }
   const ManuProC::Datum &Datum() const throw();
   operator time_t() throw();
   
   friend std::ostream &operator<<(std::ostream&,const Zeitpunkt_new&);
   
   unsigned int Jahr() const { return datum.Jahr(); }
   unsigned int Monat() const { return datum.Monat(); }
   unsigned int Tag() const { return datum.Tag(); }
   unsigned int Stunde() const { return hour; }
   unsigned int Minute() const { return minute; }
   unsigned int Sekunde() const { return second; }
   // falscher Name
   unsigned int Minuten() const { return minute; }
   
    bool valid() const throw() { return datum.valid(); }
};

std::ostream &operator<<(std::ostream&,const Zeitpunkt_new&);
class FetchIStream;
FetchIStream &operator>>(FetchIStream &is, Zeitpunkt_new &v);
class ArgumentList;
ArgumentList &operator<<(ArgumentList &, const Zeitpunkt_new &v);

#endif
