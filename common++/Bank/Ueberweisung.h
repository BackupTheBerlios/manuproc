/* $Id: Ueberweisung.h,v 1.8 2005/09/05 16:28:37 christof Exp $ */
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

#ifndef UEBERWEISUNG_H
#define UEBERWEISUNG_H
#include <BaseObjects/ManuProcEntity.h>
#include <Misc/fixedpoint.h>
#include <Misc/Datum.h>
#include <Kunde/Kunde.h>
#include <vector>
#include <string>

class Ueberweisung : public ManuProcEntity<>
{
   long blz;
   long long konto;
   long ownblz;
   long long ownkonto;   
   fixedpoint<2> betrag;
   fixedpoint<2> belegbetrag;
   bool euro;
   ManuProC::Datum faelligam;
   ManuProC::Datum erfasstam;
   ManuProC::Datum erfolgtam;
   Kunde::ID kundennr;
   int erfasstdurch;
   std::string erfasstdurch_name;
   int erfolgtdurch;
   std::string erfolgtdurch_name;   
   ManuProcEntity<>::ID belegnr;
   ManuProC::Datum belegdatum;
   
   std::string auftraggeber;
   std::string empfaenger;
   std::vector<std::string> vzweck;
   
   int buchungskonto;
   int buchungsjournal;

public:

   typedef struct {
      long blz;
      long long konto;
      long ownblz;
      long long ownkonto;   
      fixedpoint<2> betrag;
      ManuProC::Datum erfasstam;
      ManuProC::Datum faelligam;
      int erfasstdurch;
      std::string auftraggeber;
      std::string empfaenger;
      std::vector<std::string> vzweck;
      } InsertStr;


  Ueberweisung(ManuProcEntity<>::ID uid) throw(SQLerror);
  Ueberweisung();
  virtual ~Ueberweisung() {}

  static Ueberweisung *newUeberweisung(const InsertStr &is) throw(SQLerror);

  ID Id() const { return ID(entityid); }

  std::string Empfaenger() const { return empfaenger; }
  std::string Auftraggeber() const { return auftraggeber; }  
  fixedpoint<2> Betrag() const { return betrag; }
  ManuProC::Datum Erfasstam() const { return erfasstam; }
  ManuProC::Datum Faelligam() const { return faelligam; }
  ManuProC::Datum Erfolgtam() const { return erfolgtam; }
  ManuProC::Datum Belegdatum() const { return belegdatum; }
  long long Konto() const { return konto; }
  long BLZ() const { return blz; }  
  long OwnBLZ() const { return ownblz; }
  long long OwnKonto() const { return ownkonto; }
  std::string Vzweck(int feldnr=-1);
  Kunde::ID Kundennr() const { return kundennr; }
  std::string ErfasstDurchName() const { return erfasstdurch_name; }
  std::string ErfolgtDurchName() const { return erfolgtdurch_name; }  

  void Storno() throw(SQLerror);
  void setErfolgt(int diskettennr) throw(SQLerror);
  void Update(const InsertStr &is) throw(SQLerror);      
};
#endif


