// $Id: auto_conversion.cc,v 1.13 2004/05/07 12:37:22 christof Exp $
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


#include <Misc/Datum.h>
#include <stdio.h>
#include <cctype>
#include <cassert>
#include <cstdlib>

void ManuProC::Datum::from_auto(const char *datum,const char **endptr) throw(Datumsfehler,Formatfehler)
{   
  while(isspace(*datum)) ++datum; 
  if (!*datum) 
    {  if (endptr) *endptr=datum;
       *this=Datum();
       return; 
    }

    int numlen(0);
    const char *ptr=0;
    for (const char *s=datum;isdigit(*s);s++,numlen++) ;
    if (numlen>=8) // numerical presentation 20040101...
    {  tag = getnum((const unsigned char*)datum+6, 2);
	monat = getnum((const unsigned char*)datum+4, 2);
	jahr = getnum((const unsigned char*)datum, 4);
	if (endptr) *endptr=datum+8;
    }
    else if (numlen==6) // compacted presentation
    {  tag = getnum((const unsigned char*)datum, 2);
	monat = getnum((const unsigned char*)datum+2, 2);
	jahr = getnum((const unsigned char*)datum+4, 2);
	if (endptr) *endptr=datum+6;
    }
    else if (numlen==4) // ISO
    {  
       if (datum[numlen]!='-') throw Formatfehler();
       jahr = getnum((const unsigned char*)datum, numlen);
       ptr=const_cast<char*>(datum+numlen+1);
       monat = strtol(ptr,const_cast<char**>(&ptr),10);
       if (*ptr!=datum[numlen]) throw Formatfehler();
       ++ptr;
       tag = strtol(ptr,const_cast<char**>(&ptr),10);
       if (endptr) *endptr=ptr;
    }
    else if (numlen<=2 && datum[numlen]=='.') // European/German
    {  tag=getnum((unsigned char*)datum, numlen);
       ptr=const_cast<char*>(datum+numlen+1);
       monat = strtol(ptr,const_cast<char**>(&ptr),10);
       if (*ptr!=datum[numlen]) throw Formatfehler();
       ++ptr;
       jahr = strtol(ptr,const_cast<char**>(&ptr),10);
       if (endptr) *endptr=ptr;
    }
#if 0    
    else if (numlen<=2 && datum[2]=='/') // European/SQL
    {  if (datum[2]!='/' || datum[5]!=datum[2]) throw Formatfehler();
       jahr = getnum((unsigned char*)datum+6, 4);
       monat = getnum((unsigned char*)datum+3, 2);
       tag = getnum((unsigned char*)datum, 2);
    }
    else if (numlen==2 && datum[2]=='-') // US/Postgres
    {  if (datum[2]!='-' || datum[5]!=datum[2]) throw Formatfehler();
       jahr = getnum((unsigned char*)datum+6, 4);
       tag = getnum((unsigned char*)datum+3, 2);
       monat = getnum((unsigned char*)datum, 2);
    }
#endif    
    else throw Formatfehler();
    // conditional?
    if (jahr<100) { jahr+=1900; if (jahr<1950) jahr+=100; }
    teste();
}

#include <Misc/Zeitpunkt_new.h>

Zeitpunkt_new::Zeitpunkt_new(const char *stamp)
	: hour(0), minute(0), second(0), millisecond(0), 
	  minutes_from_gmt(0), prec(days)
{  if (!*stamp) return;
   const char *payload=stamp;
   datum.from_auto(payload,&payload);
   int len=strlen(payload);

   if (!isdigit(payload[1]) || !isdigit(payload[2])|| !isdigit(payload[3])) 
  {if (len>=1) { assert(*payload==' '); ++payload; --len; }
   if (len>=1)
   {  hour=strtol(payload,const_cast<char**>(&payload),10);
      if (hour>23 || hour<0) hour=0;
      prec=hours;
      len=strlen(payload);
   }
   if (len>=1)
   {  assert(*payload==':'); ++payload;
      minute=strtol(payload,const_cast<char**>(&payload),10);
      if (minute>59 || minute<0) minute=0;
      prec=minutes;
      len=strlen(payload);
   }
   if (len>=1)
   {  assert(*payload==':'); ++payload;
      second=strtol(payload,const_cast<char**>(&payload),10);
      if (second>59 || second<0) second=0;
      prec=seconds;
      len=strlen(payload);
   }
   if (len>=1 && *payload=='.') // second fractions
   {  ++payload;
      char *endptr;
      millisecond=strtol(payload,&endptr,10);
      for (int i=0;i<6-(endptr-payload);++i) millisecond*=10;
      if (millisecond>999999 || millisecond<0) millisecond=0;
      prec=milliseconds;
      payload=endptr;
      len=strlen(payload);
   }
   if (len>=1) // timezone
   {  assert(*payload=='+' || *payload=='-');
      minutes_from_gmt=strtol(payload,const_cast<char**>(&payload),10)*60;
      if (minutes_from_gmt>12*60 || minutes_from_gmt<-12*60) minutes_from_gmt=0;
   }
   else calculate_TZ(); // kills milliseconds?
  }
  else // fixed format, obsolete?
  {if (len>=2)
   {  hour=(payload[0]-'0')*10+(payload[1]-'0');
      prec=hours;
   }
   if (hour>23 || hour<0) hour=0;

   if (len>=4)
   {  minute=(payload[2]-'0')*10+(payload[3]-'0');
      prec=minutes;
   }
   if (minute>59 || minute<0) minute=0;

   if (len>=6)
   {  second=(payload[4]-'0')*10+(payload[5]-'0');
      prec=seconds;
   }
   if (second>59 || second<0) second=0;

   if (len>=8)
   {  millisecond=ManuProC::Datum::getnum((const unsigned char*)payload+6,6);
      for (int i=len;i<12;i++) millisecond*=10;
      prec=milliseconds;
   }
   if (millisecond>999999 || millisecond<0) millisecond=0;
   calculate_TZ(); // will kill milliseconds?
  }
}

