// $Id: FertigWarenLager.h,v 1.11 2003/07/25 16:26:36 jacek Exp $
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
#include "Lager.h"

class FertigWarenLager : public LagerBase
{
private:
   FertigWaren fw;

   void Buchen(FertigWaren::e_buchen e);     
   
public:
#if defined MABELLA_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
      FertigWarenLager(const FertigWaren _fw) :
      		LagerBase(ppsInstanzID::Fertigwarenlager),
      		fw(_fw)  
      {}
#else
      FertigWarenLager(const FertigWaren _fw) :
      		LagerBase(ppsInstanzID::None),
      		fw(_fw)  
      {}
#endif

   ArtikelBase Artikel() const {return fw.Artikel();}
   std::pair<Zeitpunkt_new,int> letzteInventur();
   int Bestand(ManuProC::Datum date);
   int Bestand() { return Bestand(ManuProC::Datum::today()) ;}
   
  void Einlagern() {Buchen(FertigWaren::Rein);}
  void Auslagern() {Buchen(FertigWaren::Raus);}
  void WiederEinlagern() {Buchen(FertigWaren::WiederRein);}
  void Inventur();
  const FertigWaren &getFertigWaren() const { return fw; }
  virtual std::vector<class LagerInhalt> LagerInhalt_
  		(const ArtikelBase& artikel) const;
};

#endif
