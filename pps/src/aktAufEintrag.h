/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef AKTAUFEINTRAG_H
#define AKTAUFEINTRAG_H

/* Diese Klasse beinhaltet die Daten einer Auftragszeile
   Es erfolgen keinerlei Datenbankzugriffe
   Diese Klasse ist eine reine Datenabstraktion
*/

#include <cstdlib>
#include <string>
//#include<Aux/SQLerror.h>
//#include<gtk--/list.h> // weg !
//#include<SearchCombo.h> // weg !
#include <Artikel/ArtikelBase.h>
#include <Auftrag/auftrag_status.h>
//#include<ArtikelBox.hh> // weg !
#include <Aux/Kalenderwoche.h>
//#include <Auftrag/auftrag_status.h>
//#include <Auftrag/AufEintragBase.h>
#include <Aux/Preis.h>
#include <Aux/Datum.h>

class AufEintrag;

class aktAufEintrag
{
 int stueck;
 bool stk_ok;
 Petig::Datum lieferdatum;
 bool lief_ok;
 int zeile;
 AufStatVal status;
 int dispoentrynr;
 ArtikelBase artikel;
 Preis preis;
 int rabatt;
    
public:
 aktAufEintrag() : stueck(0), stk_ok(false), lief_ok(false), 
 	zeile(0), status(UNCOMMITED), dispoentrynr(0), rabatt(0) 
 // the only sensible value
 { lieferdatum=Petig::Datum::today(); } 
 
 void fill(const AufEintrag &aufentry);
 bool allesOK() const;

 Kalenderwoche getKW() const { return lieferdatum.KW();}
 const Petig::Datum getLieferdatum() const { return lieferdatum; }
 bool setLieferdatum(Kalenderwoche);
 bool setLieferdatum(const Petig::Datum);
 
 long getStk() const { return stueck;}
 int getZln() const { return zeile;}  
 AufStatVal getStatus() const { return status;}
 int getDispoENr() const { return dispoentrynr;}
 const ArtikelBase &Artikel() const { return artikel;}
 const Preis getPreis() const { return preis; }
 int Rabatt() const { return rabatt; }
 float Preismenge() const { return preis.PreisMenge(); }

 void clear() {*this=aktAufEintrag(); }
 bool setStk(int stk) { stueck=stk; return (stk_ok=(stk>0)); }
 void setArtikel(const ArtikelBase &a) { artikel=a; }	  
 void setPreis(const Preis &p) { preis = p; }
 void setRabatt(int r) { rabatt = r; }
};

#endif
