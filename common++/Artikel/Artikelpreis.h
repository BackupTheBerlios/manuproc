// $Id: Artikelpreis.h,v 1.9 2002/06/20 06:29:52 christof Exp $
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

//namespace ArtikelMisc { class ProzessListe; };

class Artikelpreis : public Preis
{	bool errechnet:1;
        bool gefunden:1;
        PreisListe::ID gefunden_in;
        ArtikelBase::ID artikel;
        
	Preis &getPreis()
	{  return *(Preis*)this; }
	void setPreis(const ArtikelBase::ID art, const Preis &p, PreisListe::ID wo=ManuProcEntity::none_id, bool e=false)
	{  artikel=art, getPreis()=p; errechnet=e; gefunden=true; gefunden_in=wo; }
	Artikelpreis()
	: errechnet(false), gefunden(false), gefunden_in(ManuProcEntity::none_id) {}
public:
	const Preis &getPreis() const
	{  return (const Preis &)*this; }
	Artikelpreis(const cH_Kunde &k,const ArtikelBase &a);
	Artikelpreis(const PreisListe::ID liste,const ArtikelBase &a);

	bool istErrechnet() const { return errechnet; }
	bool Gefunden() const { return gefunden; }
	PreisListe::ID GefundenIn() const { return gefunden_in; }
	const ArtikelBase::ID Artikel() const { return artikel; }
	
	static void UnCache(const PreisListe::ID liste,const ArtikelBase &a);
	static const Artikelpreis create(const PreisListe::ID liste,
		const Preis &p, const ArtikelBase &a) throw(SQLerror);
// tut nix sinnvolles, oder? MAT	void updatePreis(const Preis &p) throw(SQLerror);
	void changePreis(const Preis &p) throw(SQLerror);
   static void remove(const PreisListe::ID liste,const ArtikelBase &a) throw(SQLerror);
};
#endif
