/* $Id: JumboRolle.h,v 1.9 2002/09/02 13:04:04 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

// $Id: JumboRolle.h,v 1.9 2002/09/02 13:04:04 christof Exp $

#ifndef _JUMBOROLLE_HH
#  define _JUMBOROLLE_HH


//#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED


#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Artikel/ArtikelBezeichnung.h>
#include"CodeError.h"
#include <iostream>
#include <vector>
#include <Aux/Zeitpunkt_new.h>
#include <Lager/LagerPlatz.hh>

#define CODELENGTH	8

#define NOT_USED(x...) 

class KettplanKette;

class JumboRolle
{
public:
 typedef unsigned int ID;
            
  enum Jumbo_Status{Gedruckt=0, Gewebt, ImLager, Verarbeitet};
        
  // I do not like id in this struct too much CP            
 struct st_inventur
 { ID code; ArtikelBase::ID id; bool drin;
 
   st_inventur(ID c, ArtikelBase::ID i,bool d=false) 
   : code(c),id(i),drin(d) {} 
 };
 
private:
 ID code; // mit PrüfZiffer
 ArtikelBase artikel;
 int maschine;
 ManuProC::Datum plandatum;
 int webmaschine;
 int sollmeter;
 Zeitpunkt_new verarbdatum;
 Jumbo_Status status;
 int lauf;
 int gang;
 int barcoist;
 Zeitpunkt_new barcofertdatum;
 Zeitpunkt_new wiederinslager;
 bool rest;
 LagerPlatz lagerposition;
 
public:
 JumboRolle();
 JumboRolle(ID code) throw(SQLerror,CodeError);
 JumboRolle(ID _code,int _maschine,const ManuProC::Datum &datum,int _webmaschine,
                 int soll_meter,int _status,int _lauf,int _gang,
                 ArtikelBase::ID artikelid) throw();
 static vector<JumboRolle> create(const KettplanKette &k, bool rest=false);
 
 const std::vector<int> lagerPositionen() throw(SQLerror) { return lagerPositionen(artikel); }
 static const std::vector<int> lagerPositionen(const ArtikelBase &art) throw(SQLerror);
 
 ID Code() const {return Id();} // Mit Prüfziffer!
 ID Id() const {return code;} // Mit Prüfziffer!
 bool Rest() const {return rest;}
 std::string CodeStr() const;
 std::string artBezeichnung() const { return cH_ArtikelBezeichnung(artikel)->Bezeichnung();}
 std::string MeterStr() const;
 int Meter() const;
 std::string verarbDatum() const { return verarbdatum.Datum().c_str();}
 std::string inslagerDatum() const { return wiederinslager.Datum().c_str();}
 Zeitpunkt_new VerarbDatum() const { return verarbdatum;}
 Zeitpunkt_new InsLagerDatum() const { return wiederinslager;}
 Jumbo_Status getStatus() const { return status; } 
 const ArtikelBase &Artikel() const { return artikel; }
 LagerPlatz LagerPosition() const {return lagerposition;}

 // diese Funktionen haben ein _nodb Suffix, da sie die Daten in der
 // Datenbank nicht ändern, obwohl man das annehmen könnte
 void setInsLager_nodb(Zeitpunkt_new z) {wiederinslager=z;} 
 void setVerarbDatum_nodb(Zeitpunkt_new z) {verarbdatum=z;} 
 void setStatus_nodb(Jumbo_Status s)  {status=s;}
 void setLagerPosition_nodb(LagerPlatz LP) {lagerposition=LP;}

 static bool isRollNrOK(ID code);
 static int Pruefziffer_anhaengen(int nummer);
};

//#endif
#endif
