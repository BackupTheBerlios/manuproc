// $Id: FertigWaren.h,v 1.8 2003/01/08 09:46:57 christof Exp $
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


#include<Misc/SQLerror.h>
#include<Artikel/ArtikelBezeichnung.h>
#include <Misc/Zeitpunkt_new.h>
#include <BaseObjects/ManuProcEntity.h>
#include <Misc/FetchIStream.h>

class FertigWaren
{
public:
  enum enum_Aktion{eLieferschein='L', eManuell='M', eInventur='I'};
  
private:
  ArtikelBase artikel;
  Zeitpunkt_new zeit; // wird erst nach einer Aktion gesetzt;
  enum_Aktion aktion;
  int stk;
  ManuProcEntity<>::ID lieferschein;
  int uid;
  std::string uname;
  
  enum e_buchen{Rein,Raus};
  void Buchen(int pid,e_buchen e); 
public:
 FertigWaren(ArtikelBase a,enum_Aktion a2,int s,
 		ManuProcEntity<>::ID lfrsid=ManuProcEntity<>::none_id)
   : artikel(a)/*,zeit()*/,aktion(a2),stk(s),lieferschein(lfrsid),uid(0)  {};
 
 FertigWaren() : aktion(eLieferschein),stk(0),lieferschein(ManuProcEntity<>::none_id),
 	uid(0)  {};
 		
 std::string artBezeichnung() const { return cH_ArtikelBezeichnung(artikel)->Bezeichnung();}
 int Stk() const {return stk;};
 ManuProcEntity<>::ID Lfrsid() const { return lieferschein;}
 Zeitpunkt_new Zeit() const { return zeit;}
 enum_Aktion Aktion() const {return aktion;}
 const ArtikelBase &Artikel() const { return artikel; }
 const std::string User() const { return uname;}
 void setUser(const std::string user) { uname=user;}

 
 // Die unterschiedlichen Funktionen garantieren ein korrektes Vorzeichen.
 void Einlagern(int pid) {Buchen(pid,Rein);}
 void Auslagern(int pid) {Buchen(pid,Raus);}
 void Inventur(int pid);

 friend FetchIStream &operator>>(FetchIStream &is, FertigWaren &fw);
};

FetchIStream &operator>>(FetchIStream &is, FertigWaren &fw);

#endif
