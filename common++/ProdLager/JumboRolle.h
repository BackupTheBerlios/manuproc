// $Id: JumboRolle.h,v 1.7 2002/01/21 17:01:50 cvs_christof Exp $

#ifndef _JUMBOROLLE_HH
#  define _JUMBOROLLE_HH


#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Artikel/ArtikelBezeichnung.h>
#include"CodeError.h"
#include <iostream>
#include <vector>
#include <Aux/Zeitpunkt_new.h>
#include <ProdLager/LagerPlatz.hh>


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
 
 LagerPlatz LagerPosition() const {return lagerposition;}
 void setLagerPosition(LagerPlatz LP) {lagerposition=LP;}

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
 void setInsLager(Zeitpunkt_new z) {wiederinslager=z;} 
 void setVerarbDatum(Zeitpunkt_new z) {verarbdatum=z;} 
 Zeitpunkt_new VerarbDatum() const { return verarbdatum;}
 Zeitpunkt_new InsLagerDatum() const { return wiederinslager;}
 Jumbo_Status getStatus() const { return status; } 
 void setStatus(Jumbo_Status s)  {status=s;}
 const ArtikelBase &Artikel() const { return artikel; }

 static bool isRollNrOK(int code);
 static int Pruefziffer_anhaengen(int nummer);
};

#endif
