/* $Id: Ausgabe_neu.h,v 1.19 2005/07/21 08:48:28 christof Exp $ */
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
#include <Misc/fixedpoint.h>

const std::string FormatiereEmptyInt(long Zahl);

const std::string Formatiere(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ');
                
const std::string Formatiere(unsigned long long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ');                
                
// #warning TODO: für double sollte es einen Fehler geben ?
extern inline const std::string FormatiereTeX(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,char fuehrendesZeichen=' ')
{  return Formatiere(Zahl,Nachkommastellen,Ziellaenge,"\\,", ",", fuehrendesZeichen);
}

extern inline const std::string FormatiereTeX(unsigned long long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,char fuehrendesZeichen=' ')
{  return Formatiere(Zahl,Nachkommastellen,Ziellaenge,"\\,", ",", fuehrendesZeichen);
}

static const int NEEDCHAR=1;
static const int BARISNEWLINE=2;
// set this flag to false  if you have not yet changed TeX output to utf-8 
namespace Ausgabe_neu {
extern bool TeX_uses_UTF8;
}
std::string string2TeX(const std::string s, int flags=0) throw();

template <int decimals,class Ftype,class Itype>
 const std::string FormatiereTeX(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  return Zahl.String(false, 0, "\\,", ",");
}

template <int decimals,class Ftype,class Itype>
 const std::string Formatiere(const fixedpoint<decimals,Ftype,Itype> &Zahl, unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",",char fuehrendesZeichen=' ')
{  return Zahl.String(false, Ziellaenge, TausenderTrennzeichen, Komma,
	fuehrendesZeichen);
}

// unnötige Nachkommastellen unterdrücken
template <int decimals,class Ftype,class Itype>
 const std::string FormatiereTeX_short(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  return Zahl.String(true, 0, "\\,", ",");
}
template <int decimals,class Ftype,class Itype>
 const std::string Formatiere_short(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  return Zahl.String(true, 0, ".", ",");
}
template <int decimals,class Ftype,class Itype>
 const std::string FormatiereEmpty_short(const fixedpoint<decimals,Ftype,Itype> &Zahl)
{  if (!Zahl) return "";
   return Zahl.String(true, 0, ".", ",");
}

// um transparent zwischen fixedpoints und ints umschalten zu können
extern inline const std::string Formatiere_short(unsigned int i)
{  return Formatiere((unsigned long)i);
}

// als Preis (2 Nachkommastellen, alle weiteren als superscript (Potenzen))
// ausgeben
template <int decimals,class Ftype,class Itype>
 const std::string FormatiereTeX_Preis(const fixedpoint<decimals,Ftype,Itype> &Zahl);
#endif
