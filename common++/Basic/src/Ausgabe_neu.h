/* $Id: Ausgabe_neu.h,v 1.1 2001/04/23 08:11:59 christof Exp $ */
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

const string Formatiere(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",");
extern inline const string FormatiereTeX(unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0)
{  return Formatiere(Zahl,Nachkommastellen,Ziellaenge,"\\,");
}
                
ostream &Formatiere(ostream &os,unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",");
extern inline ostream &FormatiereTeX(ostream &os,unsigned long Zahl,
                unsigned int Nachkommastellen=0,
                unsigned int Ziellaenge=0)
{  return Formatiere(os,Zahl,Nachkommastellen,Ziellaenge,"\\,");
}
static const int NEEDCHAR=1;
static const int BARISNEWLINE=2;
string string2TeX(const string s, int flags=0) throw();

template <class F>
 const string FormatiereTeX2(const F &Zahl)
{  return FormatiereTeX(Zahl.Scaled(),Zahl.Scale());
}

template <class F>
 const string Formatiere2(const F &Zahl, unsigned int Ziellaenge=0,
                const char *TausenderTrennzeichen=".",
                const char *Komma=",")
{  return Formatiere(Zahl.Scaled(),Zahl.Scale(),Ziellaenge,TausenderTrennzeichen,Komma);
}
#endif
