// $Id: DataBase_init.hh,v 1.1 2002/06/20 06:43:31 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef DATAINIT_H
#define DATAINIT_H


#include "steuerprogramm.hh"
///////////////////////////////////////////////////////////////////////////
// Do not change anything below this line /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#include <Artikel/ArtikelBaum.h>
#include <Artikel/Prozess.h>
#include <Lager/LagerPlatz.hh>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>


#define PROZESS_ROLLEREI_FAERBEREI ProzessID::Rollen_25m
#define FAKTOR_FAERBEREI ArtikelBaum::faktor_t(25)
#define FAKTOR_BANDLAGER ArtikelBaum::faktor_t(1)
#define PROZESS_DEFAULT ProzessID::Verarbeitung
#define FAKTOR_ACETAT ArtikelBaum::faktor_t(0.006)
#define FAKTOR_KUPFER ArtikelBaum::faktor_t(0.01)
#define FAKTOR_RINGE ArtikelBaum::faktor_t(2)

#define ACETAT_LAGERPLATZ "07D2"
#define ACETAT_KARTONS 3
#define ACETAT_KG_PRO_KARTON 7
#define ACETAT_RESTE 2
#define ACETAT_RESTE_KG 3

#define KUPFER_LAGERPLATZ "07D3"
#define KUPFER_KARTONS 2
#define KUPFER_KG_PRO_KARTON 10
#define KUPFER_RESTE 3
#define KUPFER_RESTE_KG 2

#define GAENGE 1
#define KETTLAENGE 2000
#define STUECKLAENGE 2000
#define MASCHIENE 211
#define SCHAERDATUM Petig::Datum().today()
#define JUMBO_LAGERPLATZ 712


class DataBase_init
{
    void ArtikelBaum_anlegen();
    void ArtikelBaum_anlegen_execute(const ArtikelBase &art,
            const cH_Prozess &prozess,const ArtikelBase &art_von,
            const ArtikelBaum::faktor_t &faktor=ArtikelBaum::faktor_t(0));
    void JumboLager_initalisieren();
    void RohwarenLager_initalisieren();
    void RohwarenLager_initalisieren_execute(const ArtikelBase &artikel, 
       const LagerPlatz& LP,int kartons,int kg_pro_karton,int reste,int kg_reste);

  public:
    DataBase_init();
};

#endif
