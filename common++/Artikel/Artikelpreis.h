// $Id: Artikelpreis.h,v 1.5 2001/10/23 08:45:18 christof Exp $
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
{	bool errechnet;
	Preis &getPreis()
	{  return *(Preis*)this; }
	void setPreis(const Preis &p, bool e=false)
	{  getPreis()=p; errechnet=e; }
public:
	const Preis &getPreis() const
	{  return (const Preis &)*this; }
	Artikelpreis(const Kunde::ID liste,const ArtikelBase &a);
	bool istErrechnet() const { return errechnet; }
	
	static void UnCache(const ArtikelBase &a);
};
#endif
