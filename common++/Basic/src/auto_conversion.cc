// $Id: auto_conversion.cc,v 1.1 2001/04/23 08:11:59 christof Exp $
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


#include <Aux/Datum.h>
#include <stdio.h>
#include <cctype>
#include <cassert>

void Petig::Datum::from_auto(const char *datum) throw(Datumsfehler,Formatfehler)
{   int numlen(0);
    for (const char *s=datum;isdigit(*s);s++,numlen++);
    if (numlen>=6) // Adabas internal
    {  tag = getnum((unsigned char*)datum+6, 2);
	monat = getnum((unsigned char*)datum+4, 2);
	jahr = getnum((unsigned char*)datum, 4);
    }
    else if (numlen==4) // ISO
    {  if (datum[4]!='-' || datum[7]!=datum[4]) throw Formatfehler();
       jahr = getnum((unsigned char*)datum, 4);
       monat = getnum((unsigned char*)datum+5, 2);
       tag = getnum((unsigned char*)datum+8, 2);
    }
    else if (numlen==2 && datum[2]=='.') // European/German
    {  if (datum[2]!='.' || datum[5]!=datum[2]) throw Formatfehler();
       jahr = getnum((unsigned char*)datum+6, 4);
       monat = getnum((unsigned char*)datum+3, 2);
       tag = getnum((unsigned char*)datum, 2);
    }
    else if (numlen==2 && datum[2]=='/') // European/SQL
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
    else throw Formatfehler();
    teste();
}

#include <Aux/Zeitpunkt_new.h>

Zeitpunkt_new::Zeitpunkt_new(const char *stamp)
	: hour(0), minute(0), second(0), millisecond(0), 
	  minutes_from_gmt(0), prec(days)
{  const char *payload=stamp;
   int len=strlen(payload);
   datum.from_auto(payload);
   
   if (!isdigit(stamp[2]) || !isdigit(stamp[4])) 
  {if (len>10) assert(payload[10]==' ');
   if (len>=13)
   {  hour=(payload[11]-'0')*10+(payload[12]-'0');
      prec=hours;
   }
   if (hour>23 || hour<0) hour=0;

   if (len>=16)
   {  assert(payload[13]==':');
      minute=(payload[14]-'0')*10+(payload[15]-'0');
      prec=minutes;
   }
   if (minute>59 || minute<0) minute=0;

   if (len>=19)
   {  assert(payload[16]==':');
      second=(payload[17]-'0')*10+(payload[18]-'0');
      prec=seconds;
   }
   if (second>59 || second<0) second=0;

   assert (len==22);
   assert(payload[19]=='+');
   {  minutes_from_gmt=Petig::Datum::getnum((const unsigned char*)payload+20,2)*60;
   }
   if (minutes_from_gmt>12*60 || minutes_from_gmt<-12*60) minutes_from_gmt=0;
  }
  else
  {if (len>=12)
   {  hour=(payload[8]-'0')*10+(payload[9]-'0');
      prec=hours;
   }
   if (hour>23 || hour<0) hour=0;

   if (len>=14)
   {  minute=(payload[10]-'0')*10+(payload[11]-'0');
      prec=minutes;
   }
   if (minute>59 || minute<0) minute=0;

   if (len>=16)
   {  second=(payload[12]-'0')*10+(payload[13]-'0');
      prec=seconds;
   }
   if (second>59 || second<0) second=0;

   if (len>16)
   {  millisecond=Petig::Datum::getnum((const unsigned char*)payload+14,6);
      for (int i=len;i<22;i++) millisecond*=10;
      prec=milliseconds;
   }
   if (millisecond>999999 || millisecond<0) millisecond=0;
   calculate_TZ();
  }
}

