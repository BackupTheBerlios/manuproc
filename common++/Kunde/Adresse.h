/* $Id: Adresse.h,v 1.1 2001/04/23 08:11:59 christof Exp $ */
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

#include "Aux/SQLerror.h"
#define MYKUNDENNR 1

#ifdef __cplusplus
class Adresse
{  int kundennr;
   char IDnr[21];
   string firma;
   char postanwvor[41];
   string strasse;
   char postanwnach[41];
   string hsnr;
   int PLZ;
   long postfach;
   int postfPLZ;
   string ort;
   bool rng_an_postfach;
   char landname[41];
   char postfachname[41];
   bool lieferadresse;

 public:
   const char *getFirma() { return firma.c_str();}
   const char *getPostAnwVor() { return postanwvor;}
   const char *getStrasse() { return strasse.c_str();}
   const char *getHsnr() { return hsnr.c_str();}
   int getPlz() { return PLZ;}
   const char *getOrt() { return ort.c_str();}
   int putIntoNLines(char **buf,int num,int len,bool TeX=false) const throw();
   Adresse(int kdnr) throw(SQLerror);
   int Kundennr() const throw();
   void mark_Lieferanschrift() throw() { lieferadresse=true; }
};

#define NULL_VALUE -1

extern "C" {
#else
typedef void Adresse;
typedef int bool;
#define throw()
#endif
Adresse *Adresse_by_kdnr(int kdnr) throw();
void free_Adresse(Adresse *a) throw();
int putIntoNLines(Adresse *a,char **buf,int num,int len,bool TeX) throw();
void mark_Lieferanschrift(Adresse *a) throw();

#ifdef __cplusplus
}
#endif
