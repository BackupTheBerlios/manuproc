// $Id: Preis.h,v 1.7 2002/01/22 09:15:55 christof Exp $
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

class Preis
{
public:
	typedef fixedpoint<2> preismenge_t;
//	typedef unsigned int preismenge_t;
	static const unsigned int nachkommastellen=2;
	static const unsigned int rabattnachkommastellen=2;
	typedef fixedpoint<nachkommastellen> pfennig_cent_t;

private:
	pfennig_cent_t pfennig_cent;
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
	Preis(pfennig_cent_t dm_euro, cP_Waehrung w,preismenge_t pmenge=1)
		: pfennig_cent(dm_euro), 
		  preismenge(pmenge), waehrung(w), 
		  short_shl(false) {}
	Preis() : pfennig_cent(0), preismenge(1), waehrung(Waehrung::default_id), 
	          short_shl(false) {}
	
	int Wert_i(cP_Waehrung tp,preismenge_t pmenge=0) const throw()
	{  return Wert_fr(tp,pmenge).Scaled(); }
	
	pfennig_cent_t Wert_fr(cP_Waehrung tp,preismenge_t pmenge=0) const throw();
	pfennig_cent_t Wert(cP_Waehrung tp,preismenge_t pmenge=0) const throw()
	{ return Wert_fr(tp,pmenge); }
	pfennig_cent_t Wert() const { return pfennig_cent; }
	
	/// Vorsicht: rundet auf linken Typ
	Preis operator+(const Preis &b) const;
	/// Vorsicht: rundet auf linken Typ
	Preis operator-(const Preis &b) const;
	/// nach runden auf linken Typ
	bool operator!=(const Preis &b) const;
	/// exakter Vergleich
	bool operator<(const Preis &b) const;
	
	const Preis operator+=(const Preis &b)
	{  pfennig_cent+=b.Wert(waehrung,preismenge);
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

	Preis In(cP_Waehrung tp,preismenge_t stkgr=0) const
	{  if (!stkgr) stkgr=preismenge;
	   Preis ret(Wert(tp,stkgr),tp,stkgr);
	   ret.short_format(short_shl);
	   return ret;
	}
	// Preis Gesamtpreis(cP_Waehrung tp,preismenge_t stkgr=0) const;
	
	pfennig_cent_t Gesamtpreis(cP_Waehrung w,int anzahl,float menge,const fixedpoint<rabattnachkommastellen> &rabatt=fixedpoint<rabattnachkommastellen>(0.0)) const;
	const Preis Gesamtpreis(int anzahl,float menge,const fixedpoint<rabattnachkommastellen> &rabatt=fixedpoint<rabattnachkommastellen>(0.0)) const;
	
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
