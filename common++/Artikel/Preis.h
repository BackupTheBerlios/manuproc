// $Id: Preis.h,v 1.15 2002/11/07 07:48:23 christof Exp $
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

#ifndef PREIS_HH
#define PREIS_HH
#include <Aux/fixedpoint.h>
#include <iostream>
#include <string>
#include <Aux/Waehrung.h>
#include <DynamicEnums/DefaultValues.h>

class Preis
{
public:
	typedef fixedpoint<ManuProC::Precision::LieferscheinMenge> preismenge_t;
   
	typedef fixedpoint<ManuProC::Precision::GeldBetrag> geldbetrag_t;
	typedef geldbetrag_t pfennig_cent_t;
	typedef fixedpoint<ManuProC::Precision::Rabatt> rabatt_t;
	
private:
      geldbetrag_t pfennig_cent;
    	preismenge_t preismenge; // 100 == /100m
    	cP_Waehrung waehrung;
    	bool short_shl:1;

    	const static float DM_EURO=1.95583;
    	const static float EURO_DM=1.0/DM_EURO;
    	
public:
	Preis(float dm_euro, cP_Waehrung w,preismenge_t pmenge=1)
		: pfennig_cent(dm_euro), 
		  preismenge(pmenge), waehrung(w), 
		  short_shl(false) {}
	Preis(double dm_euro, cP_Waehrung w,preismenge_t pmenge=1)
		: pfennig_cent(dm_euro), 
		  preismenge(pmenge), waehrung(w), 
		  short_shl(false) {}
	Preis(geldbetrag_t dm_euro, cP_Waehrung w,preismenge_t pmenge=1)
		: pfennig_cent(dm_euro), 
		  preismenge(pmenge), waehrung(w), 
		  short_shl(false) {}
	Preis() : pfennig_cent(0), preismenge(1), waehrung(Waehrung::default_id), 
	          short_shl(false) {}
	
	int Wert_i(const cP_Waehrung tp,preismenge_t pmenge=0) const throw()
	{  return Wert_fr(tp,pmenge).Scaled(); }
	
	geldbetrag_t Wert_fr(const cP_Waehrung tp,preismenge_t pmenge=0) const throw();
	geldbetrag_t Wert(const cP_Waehrung tp,preismenge_t pmenge=0) const throw()
	{ return Wert_fr(tp,pmenge); }
	geldbetrag_t Wert() const { return pfennig_cent; }
	
	/// Vorsicht: rundet auf linken Typ
	Preis operator+(const Preis &b) const;
	/// Vorsicht: rundet auf linken Typ
	Preis operator-(const Preis &b) const;
	/// nach runden auf linken Typ
	bool operator!=(const Preis &b) const;
	/// exakter Vergleich
	bool operator<(const Preis &b) const;
	
	const Preis operator+=(const Preis &b)
	{  *this=*this+b;
	   return *this;
	}
	
	Preis operator*(double b) const
	{  return Preis(pfennig_cent*b,waehrung,preismenge); }
	
	preismenge_t BezugsMenge() const
	{  return preismenge; }
	preismenge_t PreisMenge() const
	{  return preismenge; }
	void BezugsMenge(preismenge_t pmenge)
// umrechnen ?	
	{  preismenge=pmenge; }
//	bool isStueckpreis() const 
//	{  return preismenge==1; }
	cP_Waehrung getWaehrung() const
	{  return waehrung; }
	const std::string Typtext() const;

	Preis In(cP_Waehrung w,preismenge_t stkgr=0) const
	{  if (!stkgr) stkgr=preismenge;
	   Preis ret(Wert_fr(w,stkgr),w,stkgr);
	   ret.short_format(short_shl);
	   return ret;
	}

	geldbetrag_t Gesamtpreis(cP_Waehrung w,int anzahl,preismenge_t menge,const rabatt_t &rabatt=rabatt_t(0.0)) const;
	// braucht man diese Routine wirklich? ich würde den anderen Gesamtpreis empfehlen
	const Preis Gesamtpreis(int anzahl,float menge,const rabatt_t &rabatt=rabatt_t(0.0)) const;
	
	void short_format(bool b=true)
	{  short_shl=b; }
	
	friend std::ostream &operator<<(std::ostream &,const Preis &p);
	
	bool operator!() const
	{  return !pfennig_cent;
	}
};

std::ostream &operator<<(std::ostream &,const Preis &p);
// template?
const Preis operator*(fixedpoint<5> f, const Preis &p);

#endif
