/* $Id: Ausgabe_neu.h,v 1.13 2002/06/24 07:35:40 christof Exp $ */
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

#ifndef AUSGABE_NEW_H
#define AUSGABE_NEW_H
#include <string>
#include <iostream>
#include <Misc/fixedpoint.h>
#include <cmath>

const std::string FormatiereEmptyInt(long Zahl);

const std::string Formatiere(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ');
// #warning TODO: f¨r double sollte es einen Fehler geben ?
extern inline const std::string FormatiereTeX(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,char fuehrendesZeichen=' ')
{  return Formatiere(Zahl,Nachkommastellen,Ziellaenge,"\\,", ",", fuehrendesZeichen);
}
                
std::ostream &Formatiere(std::ostream &os,unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ');
extern inline std::ostream &FormatiereTeX(std::ostream &os,unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0)
{  return Formatiere(os,Zahl,Nachkommastellen,Ziellaenge,"\\,", ",", '~');
}

static const int NEEDCHAR=1;
static const int BARISNEWLINE=2;
std::string string2TeX(const std::string s, int flags=0) throw();

template <int decimals,class Ftype,class Itype>
 const std::string FormatiereTeX(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  if (Zahl.Scaled()<0) return "-"+FormatiereTeX(-Zahl.Scaled(),Zahl.Scale());
   return FormatiereTeX(Zahl.Scaled(),Zahl.Scale());
}

template <int decimals,class Ftype,class Itype>
 const std::string Formatiere(const fixedpoint<decimals,Ftype,Itype> &Zahl, unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ')
{  if (Zahl.Scaled()<0) return "-"+Formatiere(-Zahl.Scaled(),Zahl.Scale(),
		Ziellaenge?Ziellaenge-1:0,TausenderTrennzeichen,Komma,
		fuehrendesZeichen);
   return Formatiere(Zahl.Scaled(),Zahl.Scale(),Ziellaenge,
		TausenderTrennzeichen,Komma,fuehrendesZeichen);
}

// unnoetige Nachkommastellen unterdruecken
template <int decimals,class Ftype,class Itype>
 const std::string FormatiereTeX_short(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  Itype val(Zahl.Scaled());
   if (val<0) val=-val;
   unsigned int scale(Zahl.Scale());
   while (scale>0 && !(val%10)) { val/=10; --scale; }
   return (Zahl.Scaled()<0?"-":"")+FormatiereTeX(val,scale);
}

// um transparent zwischen fixedpoints und ints umschalten zu können
static inline const std::string Formatiere_short(unsigned int i)
{  return Formatiere(i);
}
static inline const std::string FormatiereEmpty_short(unsigned int i)
{  return FormatiereEmptyInt(i);
}

template <int decimals,class Ftype,class Itype>
 const std::string Formatiere_short(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  Itype val(Zahl.Scaled());
   if (val<0) val=-val;
   unsigned int scale(Zahl.Scale());
   while (scale>0 && !(val%10)) { val/=10; --scale; }
   return (Zahl.Scaled()<0?"-":"")+Formatiere(val,scale);
}

// als Preis (2 Nachkommastellen, alle weiteren als superscript (Potenzen))
// ausgeben

template <int decimals,class Ftype,class Itype>
 const std::string FormatiereTeX_Preis(const fixedpoint<decimals,Ftype,Itype> &Zahl);

#endif
