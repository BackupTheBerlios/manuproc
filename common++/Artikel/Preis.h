// $Id: Preis.h,v 1.2 2001/07/05 09:23:02 christof Exp $
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
private:
	typedef fixedpoint<2> pfennig_cent_t;
	pfennig_cent_t pfennig_cent;
    	float preismenge; // 100 == /100m
    	cP_Waehrung waehrung;
    	bool short_shl:1;

    	const static float DM_EURO=1.95583;
    	const static float EURO_DM=1.0/DM_EURO;
public:
	Preis(float dm_euro, cP_Waehrung w,float pmenge=1)
		: pfennig_cent(dm_euro), 
		  preismenge(pmenge), waehrung(w), 
		  short_shl(false) {}
	Preis(double dm_euro, cP_Waehrung w,float pmenge=1)
		: pfennig_cent(dm_euro), 
		  preismenge(pmenge), waehrung(w), 
		  short_shl(false) {}
	Preis(pfennig_cent_t dm_euro, cP_Waehrung w,float pmenge=1)
		: pfennig_cent(dm_euro), 
		  preismenge(pmenge), waehrung(w), 
		  short_shl(false) {}
	Preis() : pfennig_cent(0), preismenge(1), waehrung(0), 
	          short_shl(false) {}
	
	int Wert_i(cP_Waehrung tp,float pmenge=0) const throw()
	{  return Wert_fr(tp,pmenge).Scaled(); }
	
	/// fixedpoint<2>
	pfennig_cent_t Wert_fr(cP_Waehrung tp,float pmenge=0) const throw();
	pfennig_cent_t Wert(cP_Waehrung tp,float pmenge=0) const throw()
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
	
//	void write(float &dm_euro, char *waehrung, int size_w, char *einheit, int size_e) const;
	float BezugsMenge() const
	{  return preismenge; }
	float PreisMenge() const
	{  return preismenge; }
	void BezugsMenge(float pmenge)
// umrechnen ?	
	{  preismenge=pmenge; }
//	bool isStueckpreis() const 
//	{  return preismenge==1; }
	cP_Waehrung getWaehrung() const
	{  return waehrung; }
	const std::string Typtext() const;

	Preis In(cP_Waehrung tp,float stkgr=0) const
	{  if (!stkgr) stkgr=preismenge;
	   Preis ret(Wert(tp,stkgr),tp,stkgr);
	   ret.short_format(short_shl);
	   return ret;
	}
	// Preis Gesamtpreis(cP_Waehrung tp,float stkgr=0) const;
	
	pfennig_cent_t Gesamtpreis(cP_Waehrung w,int anzahl,float menge,const fixedpoint<2> &rabatt=fixedpoint<2>(0.0)) const;
	const Preis Gesamtpreis(int anzahl,float menge,const fixedpoint<2> &rabatt=fixedpoint<2>(0.0)) const;
	
	void short_format(bool b=true)
	{  short_shl=b; }
	
	friend std::ostream &operator<<(std::ostream &,const Preis &p);
	
	bool operator!() const
	{  return !pfennig_cent;
	}
};

std::ostream &operator<<(std::ostream &,const Preis &p);
#endif
