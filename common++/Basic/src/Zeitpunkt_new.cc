// $Id: Zeitpunkt_new.cc,v 1.3 2001/08/20 08:24:31 christof Exp $
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

#define CENTRAL_EUROPE

#include <Aux/Zeitpunkt_new.h>
#include <cassert>

int Zeitpunkt_new::operator-(const Zeitpunkt_new &b) const throw()
{  int prec2=prec<b.prec?prec:b.prec;
   switch (prec2)
   {  case days: return datum-b.datum;
      case hours: return (datum-b.datum)*24+hour-b.hour+(b.minutes_from_gmt-minutes_from_gmt)/60;
      case minutes: return ((datum-b.datum)*24+hour-b.hour)*60+minute-b.minute+(b.minutes_from_gmt-minutes_from_gmt);
      case seconds: 
         return (((datum-b.datum)*24+hour-b.hour)*60+minute-b.minute+(b.minutes_from_gmt-minutes_from_gmt))*60+second-b.second;
      case milliseconds: 
         return ((((datum-b.datum)*24+hour-b.hour)*60+minute-b.minute+(b.minutes_from_gmt-minutes_from_gmt))*60+second-b.second)*1000000+millisecond-b.millisecond;
      default: assert(0);
   }
}

// not tested with different time zones
bool Zeitpunkt_new::operator<(const Zeitpunkt_new &b) const throw()
{  int prec2=prec<b.prec?prec:b.prec;
   int mdiff=b.minutes_from_gmt-minutes_from_gmt;
   int hdiff=mdiff/60;
   mdiff%=60;
   if (datum<b.datum) return true;
   if (datum!=b.datum || prec2==days) return false;
   if (hour<(b.hour-hdiff)) return true;
   if (hour>(b.hour-hdiff) || prec2==hours) return false;
   if (minute<(b.minute-mdiff)) return true;
   if (minute>(b.minute-mdiff) || prec2==minutes) return false;
   if (second<b.second) return true;
   if (second>b.second || prec2==seconds) return false;
   return millisecond<b.millisecond;
}

const Petig::Datum &Zeitpunkt_new::Datum() const throw()
{  return datum;
}

Zeitpunkt_new Zeitpunkt_new::operator+(const Zeitpunkt_new::Minutes &dist) const throw()
{  Zeitpunkt_new ret(*this);
   ret.minute+=int(dist);
   ret.hour+=ret.minute/60;
   ret.minute%=60;
   ret.datum+=ret.hour/24;
   ret.hour%=24;
   normalize_TZ();
   return ret;
}

// this is not the same as +Hours(24)
Zeitpunkt_new Zeitpunkt_new::operator+(const Zeitpunkt_new::Days &dist) const throw()
{  Zeitpunkt_new ret(*this);
   ret.datum+=int(dist);
   normalize_TZ();
   return ret;
}

// false if different time zones cross day/hour boundaries
bool Zeitpunkt_new::operator==(const Zeitpunkt_new &b) const throw()
{  int prec2=prec<b.prec?prec:b.prec;
   int mdiff=b.minutes_from_gmt-minutes_from_gmt;
   int hdiff=mdiff/60;
   mdiff%=60;
   if (datum!=b.datum) return false;
   if (prec2==days) return true;
   if (hour!=b.hour-hdiff) return false;
   if (prec2==hours) return true;
   if (minute!=b.minute-mdiff) return false;
   if (prec2==minutes) return true;
   if (second!=b.second) return false;
   if (prec2==seconds) return true;
   return millisecond==b.millisecond;
}

Zeitpunkt_new::operator time_t() throw()
{  struct tm tm;
   tm.tm_sec=prec>=seconds?second:0;
   tm.tm_min=prec>=minutes?minute:0;
   tm.tm_hour=prec>=hours?hour:0;
   tm.tm_mday=datum.Tag();
   tm.tm_mon=datum.Monat()-1;
   tm.tm_year=datum.Jahr()-1900;
#ifdef CENTRAL_EUROPE
   tm.tm_isdst=minutes_from_gmt>60;
#else
   tm.tm_isdst=-1; // FIXME: we have some information about time zones
#endif
   return mktime(&tm);
}

void Zeitpunkt_new::calculate_TZ(int isdst) const throw()
{  struct tm tm;
   tm.tm_sec=prec>=seconds?second:0;
   tm.tm_min=prec>=minutes?minute:0;
   tm.tm_hour=prec>=hours?hour:0;
   tm.tm_mday=datum.Tag();
   tm.tm_mon=datum.Monat()-1;
   tm.tm_year=datum.Jahr()-1900;
   tm.tm_isdst=isdst;
   mktime(&tm);
#ifdef __MINGW32__
   minutes_from_gmt=tm.tm_isdst?120:60;
#else
   minutes_from_gmt=tm.tm_gmtoff/60;
#endif
}

void Zeitpunkt_new::normalize_TZ() const throw()
{
#ifdef CENTRAL_EUROPE  
   calculate_TZ(minutes_from_gmt>60);
#else
   calculate_TZ();
#endif
}

Zeitpunkt_new::Zeitpunkt_new(time_t t) throw()
	: millisecond(0), prec(seconds)
{  struct tm *tm(localtime(&t));
   datum=Petig::Datum(tm->tm_mday,tm->tm_mon+1,tm->tm_year+1900);
   hour=tm->tm_hour;
   minute=tm->tm_min;
   second=tm->tm_sec;
#ifdef __MINGW32__
   minutes_from_gmt=tm->tm_isdst?120:60;
#else
   minutes_from_gmt=tm->tm_gmtoff/60;
#endif
}
