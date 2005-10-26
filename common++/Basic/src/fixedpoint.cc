// $Id: fixedpoint.h,v 1.25 2005/10/05 08:39:23 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include "fixedpoint.h"
#include <Misc/Ausgabe_neu.h>
#define _GNU_SOURCE
#include <math.h>
#include <cassert>

#if defined(__GNUC__) && __GNUC__ >=4
# define TEMPLATEltgt template<>
#else
# define TEMPLATEltgt 
#endif

#define FP_STR(F,I) TEMPLATEltgt \
std::string fixedpoint_dyn<F,I>::String(bool _short, unsigned int Ziellaenge, \
		const char *TausenderTrennzeichen,const char *Komma,\
		char fuehrendesZeichen) const\
{  const char *sign="";\
   unsigned I val=Scaled();\
   if (Scaled()<0) \
   {  sign="-"; \
      val=-Scaled(); \
      if (Ziellaenge) --Ziellaenge;\
   }\
   unsigned scale=Scale();\
   if (_short) while (scale>0 && !(val%10)) { val/=10; --scale; }\
   return sign+Formatiere(val,scale,Ziellaenge,TausenderTrennzeichen,Komma,' ');\
}

FP_STR(double,long)
FP_STR(double,long long)

TEMPLATEltgt double fixedpoint_dyn<double,long>::as_float() const
{ // I don't know why *pow(10,-Scale()) does not work ...
#if !defined(__GNUC__ ) || __GNUC__<3
  return scaled/pow(10,Scale());
#else
  return scaled/exp10(Scale());
#endif
}

// template <class Ftype,class Itype>
TEMPLATEltgt Query::Row &operator>>(Query::Row &is, fixedpoint_dyn<double,long> &v)
{  std::string s;
   is >> s;
   v=s;
   return is;
}

TEMPLATEltgt
fixedpoint_dyn<double,long>::fixedpoint_dyn(std::string const &s,const char *TausenderTrennzeichen,const char *Komma)
 : scaled(), scale()
{ bool komma=false;
  bool negative=false;
//  assert(!TausenderTrennzeichen || strlen(TausenderTrennzeichen)<=1);
//  assert(!Komma || strlen(Komma)<=1);
  for (std::string::const_iterator i=s.begin();i!=s.end();++i)
  { if (isspace((unsigned char)(*i))) continue;
    if (*i=='-') { negative=true; continue; }
    if (TausenderTrennzeichen && *TausenderTrennzeichen && s.substr(i-s.begin(),strlen(TausenderTrennzeichen))==TausenderTrennzeichen)
    { i+=strlen(TausenderTrennzeichen)-1;
      continue;
    }
    if (Komma && *Komma && s.substr(i-s.begin(),strlen(Komma))==Komma)
    { i+=strlen(Komma)-1;
      komma=true;
      continue;
    }
    assert(isdigit((unsigned char)(*i)));
    scaled=scaled*10+(*i-'0');
    if (komma) scale++;
  }
  if (negative) scaled=-scaled;
}
