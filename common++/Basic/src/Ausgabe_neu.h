/* $Id: Ausgabe_neu.h,v 1.4 2001/06/27 08:04:09 christof Exp $ */
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
#include <Aux/fixedpoint.h>
#include <cmath>

const std::string Formatiere(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ');
#if 0                
extern inline const std::string Formatiere(double Zahl,
                unsigned int Nachkommastellen,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ')
{  return Formatiere((unsigned long)(Zahl*pow(10,Nachkommastellen)+.5),
		Nachkommastellen,Ziellaenge,
		TausenderTrennzeichen,Komma,fuehrendesZeichen);
}
#endif
extern inline const std::string FormatiereTeX(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,char fuehrendesZeichen=' ')
{  return Formatiere(Zahl,Nachkommastellen,Ziellaenge,"\\,", ",", fuehrendesZeichen);
}
#if 0
extern inline const std::string FormatiereTeX(double Zahl,
                unsigned int Nachkommastellen,
                unsigned int Ziellaenge=0,char fuehrendesZeichen=' ')
{  return FormatiereTeX((unsigned long)(Zahl*pow(10,Nachkommastellen)+.5),
		Nachkommastellen,Ziellaenge,fuehrendesZeichen);
}
#endif
                
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
{  return FormatiereTeX(Zahl.Scaled(),Zahl.Scale());
}

template <int decimals,class Ftype,class Itype>
 const std::string Formatiere(const fixedpoint<decimals,Ftype,Itype> &Zahl, unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ')
{  return Formatiere(Zahl.Scaled(),Zahl.Scale(),Ziellaenge,
		TausenderTrennzeichen,Komma,fuehrendesZeichen);
}

// unnoetige Nachkommastellen unterdruecken
template <int decimals,class Ftype,class Itype>
 const std::string FormatiereTeX_short(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  Itype val(Zahl.Scaled());
   unsigned int scale(Zahl.Scale());
   while (scale>0 && !(val%10)) { val/=10; --scale; }
   return FormatiereTeX(val,scale);
}

template <int decimals,class Ftype,class Itype>
 const std::string Formatiere_short(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  Itype val(Zahl.Scaled());
   unsigned int scale(Zahl.Scale());
   while (scale>0 && !(val%10)) { val/=10; --scale; }
   return Formatiere(val,scale);
}
#endif
