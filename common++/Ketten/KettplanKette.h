// $Id: KettplanKette.h,v 1.10 2002/09/02 13:04:03 christof Exp $
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


#ifndef KETTPLANKETTE_HH
#define KETTPLANKETTE_HH

#include "Kette.h"
#include "ArtikelGang.h"
#include "KettenGarn.h"
#include <Aux/SQLerror.h>

class KettplanKette : public Kette {
public:
	static const int VA_PLAN=1;
	static const int VA_KETTLEN=2;
	static const int VA_STUECKLEN=4;
	static const int VA_SCHUSS=8;
	static const int VA_WEBMASCH=16;
	static const int VA_ARTIKEL=0x20;

private:
	mutable int planmasch; // Maschine für die die Kette momentan geplant ist
	mutable int kettlaenge;
	mutable int stuecklaenge;
	mutable float schussdichte;
	mutable int webmasch; // Maschine auf der sie gewebt wurde
        ManuProC::Datum abgzuletz;
        ManuProC::Datum maschinen_start;
        Kette aktuelle_kette; // na ob das Sinn macht ??? CP
        unsigned int abgeschnitten;
	mutable std::vector <ArtikelGang> artikel; // sortiert nach Namen (?)
	mutable int valid;

#define USUAL_INIT planmasch(0), kettlaenge(0), stuecklaenge(0), \
		schussdichte(0), webmasch(0), abgeschnitten(0), valid(0)

public:
	typedef std::vector <ArtikelGang>::const_iterator const_iterator;
	const_iterator begin()
	{  if (!(valid&VA_ARTIKEL)) get_artikel();
	   return artikel.begin(); }
	const_iterator end()
	{ if (!(valid&VA_ARTIKEL)) get_artikel();
	   return artikel.end(); }

	KettplanKette()
		: Kette(), USUAL_INIT
	{}

	KettplanKette(const Kette &k)
		: Kette(k), USUAL_INIT
	{}


	static KettplanKette create(const Kette &k,const std::vector <ArtikelGang> &artikel, 
		int kettlaenge, int stuecklaenge, int planmaschine=0);
//private: Ketteneingabe braucht das
        void push_back(const ArtikelGang& artikel) const;
public:
	
	int Kettlaenge() const throw();
	int Stuecklaenge() const throw();
	int WebMaschine() const { return barcoMaschine(); }
	int defaultKettlaenge() const throw(SQLerror);
	int defaultStuecklaenge() const throw(SQLerror);
	float defaultSchussdichte() const throw(SQLerror);
	void Schussdichte(float sd) throw(SQLerror);
	void BarcoMasch(int m) throw(SQLerror)
	{  setzeBarcoMasch(m); }
	void WebMaschine(int m) throw(SQLerror)
	{  setzeBarcoMasch(m); }
	void FrageNachSchussdichte() const throw(SQLerror);
	float Schussdichte() const throw();
	// wo in Barco verzeichnet
	int barcoMaschine() const throw();
	// für welche Maschine geplant
	int planMaschine() const throw();
        ManuProC::Datum Abgzuletzt() const throw(SQLerror);
        ManuProC::Datum Maschinen_start() const throw(SQLerror);
        unsigned int Abgeschnitten() const throw(SQLerror);
        static Kette Aktuelle_Kette(int webmasch) throw(SQLerror);
        Kette Folge_Kette() const throw(SQLerror);
//        ManuProC::Datum Fertig_am() const throw(SQLerror);
	void set_info(int pm,int kl,int sl,int sd) throw();
	const std::vector <ArtikelGang> &get_artikel() const
	{  return get_artikel_sorted(); }
	const std::vector <ArtikelGang> &get_artikel_sorted() const;

        void delete_Artikel(const ArtikelGang& artikel) const;
        int get_aktual_Index(const ArtikelGang& artikel) const;
        
// warum hier ? CP
//        void save_Garn(const ArtikelGang& artikel, const KettenGarn& garn) const;
//        void delete_Garn(const ArtikelGang& artikel, const KettenGarn& garn) const;
//        std::vector<KettenGarn> load_Garn(const ArtikelGang& artikel) const;

public:
	void UnCache(int what) const
	{  valid&=~what;
	}
	
private:
	int holePlanMaschine() const throw(SQLerror);
	void LaengenArtikel(int &art,int &br);
	/// deprecated, use Schussdichte(int sd)
	void setzeSchussdichte(float sd) throw(SQLerror)
	{  Schussdichte(sd); }
	/// deprecate[Ad, use BarcoMasch(int m)
	void setzeBarcoMasch(int m) throw(SQLerror);
	
public: // deprecated!!!
	

#if 0
private:
	// should kill this
	KettplanKette(int m,const ManuProC::Datum _schaerdatum)
		: Kette(m,_schaerdatum), USUAL_INIT
	{}

	// an this strange beast ...
	KettplanKette(int m,const ManuProC::Datum _schaerdatum,int l)
		: Kette(m,_schaerdatum), USUAL_INIT
	{ kettlaenge=l; valid|=VA_KETTLEN; }
	// kill this
	KettplanKette(int m,const char *adabasDatum)
		: Kette(m,adabasDatum), planmasch(0), kettlaenge(0), stuecklaenge(0), 
		  schussdichte(0), webmasch(0),
		  valid(0)
	{}
	KettplanKette(const char *barcoformat)
		: Kette(barcoformat), planmasch(0), kettlaenge(0), stuecklaenge(0), 
		  schussdichte(0), webmasch(0),
		  valid(0)
	{}
#endif	
#undef USUAL_INIT
};
#endif
