// $Id: JumboRolle.h,v 1.1 2002/02/08 21:51:26 christof Exp $

#ifndef _JUMBOROLLE_HH
#  define _JUMBOROLLE_HH


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

class JumboRolle
{
public:
            
  enum Jumbo_Status{Gedruckt=0, Gewebt, ImLager, Verarbeitet};
  
        
  // I do not like id in this struct too much CP            
 struct st_inventur
 { int code;unsigned int id; bool drin;
 
   st_inventur(int c, unsigned int i,bool d=false) 
   : code(c),id(i),drin(d) {} 
 };
private:
 int code; // mit PruefZiffer
 ArtikelBase artikel;
 int maschine;
 Petig::Datum plandatum;
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
 JumboRolle() {}
 JumboRolle(int code) throw(SQLerror,CodeError);
 
 const std::vector<int> lagerPositionen() throw(SQLerror) { return lagerPositionen(artikel); }
 static const std::vector<int> lagerPositionen(const ArtikelBase &art) throw(SQLerror);
 
 int Code() const {return code;} // Mit Prüfziffer!
 bool Rest() const {return rest;}
 std::string CodeStr() const;
 std::string artBezeichnung() const { return ArtikelBezeichnung(artikel).Bezeichnung();}
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

 static bool isRollNrOK(int code);
 static int Pruefziffer_anhaengen(int nummer);
};

#endif
