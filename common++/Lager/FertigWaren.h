// $Id: FertigWaren.h,v 1.5 2002/06/20 06:29:53 christof Exp $
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
                
#ifndef _FERTIGWAREN_HH
#  define _FERTIGWAREN_HH


#include<Aux/SQLerror.h>
//#include<Aux/Datum.h>
#include<Artikel/ArtikelBezeichnung.h>
//#include <iostream>
//#include <vector>
#include <Aux/Zeitpunkt_new.h>
//#include <Lager/LagerPlatz.hh>
#include <BaseObjects/ManuProcEntity.h>


class FertigWaren
{
public:
  enum enum_Aktion{eLieferschein='L', eManuell='M', eInventur='I'};
  
private:
  ArtikelBase artikel;
  Zeitpunkt_new zeit; // wird erst nach einer Aktion gesetzt;
  enum_Aktion aktion;
  int stk;
  ManuProcEntity::ID lieferschein;
//  LagerPlatz lagerposition;

  enum e_buchen{Rein,Raus};
  void Buchen(int pid,e_buchen e); 
public:
 FertigWaren(ArtikelBase a,enum_Aktion a2,int s,
 		ManuProcEntity::ID lfrsid=ManuProcEntity::none_id)
   : artikel(a)/*,zeit()*/,aktion(a2),stk(s),lieferschein(lfrsid)  {};
// FertigWaren(ArtikelBase a,Zeitpunkt_new z,enum_Aktion a2,int s)
//   : artikel(a),zeit(z),aktion(a2),stk(s) {} throw(SQLerror);
 
 std::string artBezeichnung() const { return cH_ArtikelBezeichnung(artikel)->Bezeichnung();}
 int Stk() const {return stk;};
 ManuProcEntity::ID Lfrsid() const { return lieferschein;}
 Zeitpunkt_new Zeit() const { return zeit;}
 enum_Aktion Aktion() const {return aktion;}
 const ArtikelBase &Artikel() const { return artikel; }
// LagerPlatz LagerPosition() const {return lagerposition;}

 
 // Die unterschiedlichen Funktionen garantieren ein korrektes Vorzeichen.
 void Einlagern(int pid) {Buchen(pid,Rein);}
 void Auslagern(int pid) {Buchen(pid,Raus);}
 void Inventur(int pid);

};

#endif
