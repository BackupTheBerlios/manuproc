// $Id: Artikelpreis.h,v 1.19 2003/07/30 18:32:18 jacek Exp $
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


#ifndef ARTIKELPREIS_HH
#define ARTIKELPREIS_HH
#include <Artikel/Preis.h>
#include <Kunde/Kunde.h>
#include <Artikel/ArtikelBase.h>
#include <Misc/FetchIStream.h>
#include <Misc/UniqueValue.h>

//namespace ArtikelMisc { class ProzessListe; };

class Artikelpreis : public Preis
{	bool errechnet:1;
        bool gefunden:1;
        PreisListe::ID gefunden_in;
        ArtikelBase::ID artikel;
        int mindestmenge;
        
	Preis &getPreis()
	{  return *(Preis*)this; }
	void setPreis(const ArtikelBase::ID art, const Preis &p, 
		PreisListe::ID wo=ManuProcEntity<>::none_id, bool e=false)
	{  artikel=art; getPreis()=p; errechnet=e; gefunden=true; gefunden_in=wo; 
	}
	Artikelpreis()
	: errechnet(false), gefunden(false), gefunden_in(ManuProcEntity<>::none_id),
	  mindestmenge(1) 
	{}

	friend FetchIStream &operator>>(FetchIStream &is,std::pair<int, float> &kg);  
public:
	static const UniqueValue::value_t trace_channel;
	 		
	const Preis &getPreis() const
	{  return (const Preis &)*this; }
	Artikelpreis(const cH_Kunde &k,const ArtikelBase &a, int bestellmenge=1);
	Artikelpreis(const PreisListe::ID liste,const ArtikelBase &a, int bestellmenge=1);

	bool istErrechnet() const { return errechnet; }
	bool Gefunden() const { return gefunden; }
	int MindMenge() const { return mindestmenge; }
	PreisListe::ID GefundenIn() const { return gefunden_in; }
	const ArtikelBase::ID Artikel() const { return artikel; }
	
	static void UnCache(const PreisListe::ID liste,const ArtikelBase &a);
	static const Artikelpreis create(const PreisListe::ID liste,
               	const Preis &p, const ArtikelBase &a,
		int mindmenge,               	
               	std::vector<std::string> ins_all_komp=std::vector<std::string>()) throw(SQLerror);


	void changePreis(const Preis &p, int newmindmenge=0) throw(SQLerror);
	void changeMindMenge(int mindmenge) throw(SQLerror) {}	
   static void remove(const PreisListe::ID liste,const ArtikelBase &a,
   		std::vector<std::string> del_all_komp=std::vector<std::string>(),
   		int mindmenge=0) throw(SQLerror);

};
 
#endif
