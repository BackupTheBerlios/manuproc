// $Id: Zeitpunkt_new_ostr.cc,v 1.8 2005/09/08 10:08:18 christof Exp $
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

#include <Misc/Zeitpunkt_new.h>
#include <iomanip>

std::ostream &operator<<(std::ostream &o,const Zeitpunkt_new &t)
{  o << t.datum << " ";
   if (t.prec<=Zeitpunkt_new::days) return o;
   char oldfill=o.fill('0');
   o << t.hour << ":";
   int oldwidth=o.width(2);
   o.width(2);
   o << (t.prec>=Zeitpunkt_new::minutes ? t.minute : 0);
   if (t.prec>=Zeitpunkt_new::seconds)
   { o << ":";
     o.width(2);
     o << t.second;
     if (t.prec>=Zeitpunkt_new::milliseconds && t.microsecond) 
     {  o << '.';
        unsigned w=6,ms=t.microsecond;
        while (w>=2 && !(ms%10)) { ms/=10; --w; }
        o.width(w);
        o << ms;
     }
   }
   if (t.minutes_from_gmt)
   {  o << ' ';
      if (t.minutes_from_gmt>0) o << '+';
      o << (t.minutes_from_gmt/60.0);
   }
   o.width(oldwidth); o.fill(oldfill);
   return o;
}
