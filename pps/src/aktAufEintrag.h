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

#include<stdlib.h>
#include<string>
#include<Aux/SQLerror.h>
#include<gtk--/list.h>
#include<SearchCombo.h>
//#include<more_param.h>
#include<Artikel/ArtikelBase.h>
#include<ArtikelBox.hh>
#include <Aux/Kalenderwoche.h>
#include<Auftrag/auftrag_status.h>
#include<Auftrag/AufEintragBase.h>

class AufEintrag;

#ifndef AKTAUFEINTRAG
#define AKTAUFEINTRAG

class aktAufEintrag
{
 int stueck;
 bool stk_ok;
// Kalenderwoche lieferwoche;
 Petig::Datum lieferdatum;
 bool lief_ok;
 int zeile;
 AufStatVal status;
 int dispoentrynr;
 ArtikelBase artikel;
 Preis preis;
 int rabatt;
// float preismenge;
    
public:


void fill(AufEintrag &aufentry);
bool allesOK();

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

 aktAufEintrag() { zeile=0; }
 	
 void clear()
 {
  stueck=zeile=dispoentrynr=0;
  stk_ok=lief_ok=false; 
  preis=Preis();
  rabatt=0;
 }

 bool setStk(int stk)
 {
  stueck=stk;
  return (stk_ok=(stk>0));
 }

 void setArtikel(const ArtikelBase &a) { artikel=a; }	  

 void setPreis(const Preis &p) { preis = p; }
 void setRabatt(int r) { rabatt = r; }
// void setPreismenge(float _pm){preismenge=_pm;}
};

#endif
