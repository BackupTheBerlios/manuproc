// $Id: FertigWarenLager.h,v 1.7 2003/07/15 13:18:25 jacek Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
                
#ifndef _FERTIGWARENLAGER_HH
#  define _FERTIGWARENLAGER_HH

#include "FertigWaren.h"

class FertigWarenLager 
{
private:
   FertigWaren fw;

   void Buchen(FertigWaren::e_buchen e);     
   
public:
   FertigWarenLager(const FertigWaren _fw) : fw(_fw) {}

   ArtikelBase Artikel() const {return fw.Artikel();}
   std::pair<Zeitpunkt_new,int> letzteInventur();
   int Bestand(ManuProC::Datum date);
   int Bestand() { return Bestand(ManuProC::Datum::today()) ;}
   
  void Einlagern() {Buchen(FertigWaren::Rein);}
  void Auslagern() {Buchen(FertigWaren::Raus);}
  void Inventur();
  
};

#endif
