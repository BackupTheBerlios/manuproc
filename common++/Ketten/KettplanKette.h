// $Id: KettplanKette.h,v 1.4 2001/11/05 08:58:29 christof Exp $
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

/*#warning this class is obselete (barco will vanish!) */

class KettplanKette : public Kette {
public:
	static const int VA_PLAN=1;
	static const int VA_KETTLEN=2;
	static const int VA_STUECKLEN=4;
	static const int VA_SCHUSS=8;
	static const int VA_WEBMASCH=16;
	static const int VA_ARTIKEL=0x20;

private:
	int planmasch;
	mutable int kettlaenge;
	int stuecklaenge;
	float schussdichte;
	int webmasch;
        Petig::Datum abgzuletz;
        Petig::Datum maschinen_start;
        Kette aktuelle_kette;
        unsigned int abgeschnitten;
	std::vector <ArtikelGang> artikel; // sortiert ??
	mutable int valid;

public:
	typedef std::vector <ArtikelGang>::const_iterator const_iterator;
	const_iterator begin()
	{  if (!(valid&VA_ARTIKEL)) get_artikel();
	   return artikel.begin(); }
	const_iterator end()
	{ if (!(valid&VA_ARTIKEL)) get_artikel();
	   return artikel.end(); }
	
	KettplanKette()
		: Kette(), planmasch(0), kettlaenge(0), stuecklaenge(0), 
		  schussdichte(0), webmasch(0),
		  valid(0)
	{}

	KettplanKette(int m,const Petig::Datum _schaerdatum)
		: Kette(m,_schaerdatum), planmasch(0), kettlaenge(0), stuecklaenge(0), 
		  schussdichte(0), webmasch(0),
		  valid(0)
	{}
	KettplanKette(int m,const Petig::Datum _schaerdatum,int l)
		: Kette(m,_schaerdatum), planmasch(0), kettlaenge(l), stuecklaenge(0), 
		  schussdichte(0), webmasch(0),
		  valid(0)
	{}
	KettplanKette(const Kette &k)
		: Kette(k), planmasch(0), kettlaenge(0), stuecklaenge(0), 
		  schussdichte(0), webmasch(0),
		  valid(0)
	{}
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
	
	int Kettlaenge() const throw();
	int Stuecklaenge() throw();
	int defaultKettlaenge() const throw(SQLerror);
	int defaultStuecklaenge() throw(SQLerror);
	float defaultSchussdichte() throw(SQLerror);
	void Schussdichte(float sd) throw(SQLerror);
	void BarcoMasch(int m) throw(SQLerror)
	{  setzeBarcoMasch(m); }
	void WebMaschine(int m) throw(SQLerror)
	{  setzeBarcoMasch(m); }
	void FrageNachSchussdichte() throw(SQLerror);
	float Schussdichte() throw();
	// wo in Barco verzeichnet
	int barcoMaschine() throw();
	// für welche Maschine geplant
	int planMaschine() const throw();
        Petig::Datum Abgzuletzt() const throw(SQLerror);
        Petig::Datum Maschinen_start() const throw(SQLerror);
        unsigned int Abgeschnitten() const throw(SQLerror);
        static Kette Aktuelle_Kette(int webmasch) throw(SQLerror);
        Kette Folge_Kette() throw(SQLerror);
//        Petig::Datum Fertig_am() const throw(SQLerror);
	void set_info(int pm,int kl,int sl,int sd) throw();
	const std::vector <ArtikelGang> &get_artikel()
	{  return get_artikel_sorted(); }
	const std::vector <ArtikelGang> &get_artikel_sorted();

        void save_Kette() const;
        void save_Artikel(const ArtikelGang& artikel) const;
        void delete_Artikel(const ArtikelGang& artikel) const;
        int get_aktual_Index(const ArtikelGang& artikel) const;
        void save_Garn(const ArtikelGang& artikel, const KettenGarn& garn) const;
        void delete_Garn(const ArtikelGang& artikel, const KettenGarn& garn) const;
        vector<KettenGarn> load_Garn(const ArtikelGang& artikel) const;

private:
	int holePlanMaschine() const throw(SQLerror);
	void LaengenArtikel(int &art,int &br);
	
public: // deprecated!!!
	/// deprecated, use Schussdichte(int sd)
	void setzeSchussdichte(float sd) throw(SQLerror)
	{  Schussdichte(sd); }
	/// deprecate[Ad, use BarcoMasch(int m)
	void setzeBarcoMasch(int m) throw(SQLerror);
	
	void UnCache(int what) const
	{  valid&=~what;
	}
};
#endif
