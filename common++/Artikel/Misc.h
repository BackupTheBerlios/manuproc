// $Id: Misc.h,v 1.2 2001/05/10 16:31:37 christof Exp $
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


#ifndef _ARTIKEL_MISC_H
#define _ARTIKEL_MISC_H
#include <Artikel/ArtikelBaum.h>
#include <Kunde/Kunde.h>
#include <Artikel/Preis.h>

namespace ArtikelMisc
{  

struct ProzessSchritt
{  cH_Prozess prozess;
   string prozessname;
   Preis preis; // Vorsicht: eventuell Stückgröße
   float stueckgroesse;
   
   ProzessSchritt() : prozess(Prozess::default_id), stueckgroesse(0) {}
};

struct ProzessListe;

void LeseProzesse(const ArtikelBase &ab,ProzessListe &out);

struct ProzessListe
{  typedef ArtikelBase::ID ID;
   typedef vector<ProzessSchritt>::iterator iterator;
   typedef vector<ProzessSchritt>::const_iterator const_iterator;
   typedef vector<ProzessSchritt>::const_reverse_iterator preis_iterator;
   
public:   // members
   ID rohartikel;
   ID endartikel;
   Kunde::ID liste;

   unsigned int rohartikel_preis_index;
   ID rohartikel_preis;
   Preis rohpreis;
   /** zeitlich umgekehrte Reihenfolge, d.h. frühe Prozesse zuletzt */
   vector<ProzessSchritt> schritte; 

public:   // methods
   ProzessListe() : liste(Kunde::default_id) {}
   ProzessListe(const Kunde::ID l,const ArtikelBase &ab) : liste(l)
   {  LeseProzesse(ab,*this); }
   void ErzeugePreisArtikelDummy(void); // this is nearly const
   float BezugsMenge() const
   {  return schritte.size() ? schritte[0].stueckgroesse : 0; }
   iterator begin() { return schritte.begin(); }
   iterator end() { return schritte.end(); }
   const_iterator begin() const { return schritte.begin(); }
   const_iterator end() const { return schritte.end(); }
   /** zeitlich korrekte Reihenfolge */
   preis_iterator preis_begin() const
   { return schritte.rbegin()+(schritte.size()-rohartikel_preis_index); }
   preis_iterator preis_end() const { return schritte.rend(); }
   const Kunde::ID getListe() const
   {  return liste; }
   void setListe(const Kunde::ID l)
   {  liste=l; }
};
};
#endif
