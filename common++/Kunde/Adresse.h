/* $Id: Adresse.h,v 1.7 2003/01/08 09:46:57 christof Exp $ */
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

#include <Misc/SQLerror.h>
#define MYKUNDENNR 1

#ifdef __cplusplus
class Adresse
{  int kundennr;
   char IDnr[21];
   std::string firma;
   char postanwvor[41];
   std::string strasse;
   char postanwnach[41];
   std::string hsnr;
   std::string PLZ;
   std::string postfach;
   std::string postfPLZ;
   std::string ort;
   bool rng_an_postfach;
   char landname[41];
   char postfachname[41];
   bool lieferadresse;

 public:
   const std::string Strasse() const { return strasse;}
   const std::string Firma() const { return firma;}
   const std::string Hsnr() const { return hsnr;}
   const std::string Ort() const { return ort;}
   const std::string Plz() const { return PLZ;}
 
#warning alte Methoden; bitte ersetzen
// sollen weg
   const char *getFirma() { return firma.c_str();}
   const char *getPostAnwVor() { return postanwvor;}
   const char *getStrasse() { return strasse.c_str();}
   const char *getHsnr() { return hsnr.c_str();}
   const char *getOrt() { return ort.c_str();}
//   bis hierher



//   int getPlz() const { return PLZ;}
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
