// $Id: ArtikelStamm2.cc,v 1.1 2006/06/12 14:20:36 christof Exp $
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

#include <Artikel/ArtikelStamm.h>
#include <Artikel/Artikelpreis.h>
#include <Kunde/PreisListe.h>

ManuProcEntity<>::ID ArtikelStamm::Bezugsquelle() const
{ if (payload.bezugsquelle) return payload.bezugsquelle;
  std::vector<Artikelpreis> bezug=Artikelpreis::Bezugspreise(art);
  if (bezug.empty()) // gibt es nirgendwo
    payload.bezugsquelle=ManuProcEntity<>::none_id;
  else
  { cH_PreisListe prsl(bezug[0].GefundenIn());
    std::vector<ManuProcHandle<>::ID> kunden=prsl->KundenMitDieserListe();
    if (kunden.empty()) payload.bezugsquelle=ManuProcEntity<>::none_id;
    else payload.bezugsquelle=kunden[0];
  }
  return payload.bezugsquelle;
}
