// $Id: steuerprogramm.hh,v 1.31 2002/12/20 14:37:45 christof Exp $
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

#ifndef STEUERFILE_H
#define STEUERFILE_H

//#define REPARATUR_PROGRAMM_TESTEN

//#define MANU_PROC_TEST
//#define MABELLA_TEST
//#define PETIG_TEST

// automatically guess the correct test set (comment all above out!)

#include <ManuProCConfig.h>

#if !defined(PETIG_TEST) && !defined(MANU_PROC_TEST) && !defined(MABELLA_TEST)
# if defined(PETIG_EXTENSIONS)
#  define PETIG_TEST
# elif defined(MABELLA_EXTENSIONS)
#  define MABELLA_TEST
# else
#  define MANU_PROC_TEST
# endif
#endif

#ifdef MANU_PROC_TEST
  #define MANU_DATAPATH "~/ManuProC/Anleitung/datenbank"
#else if defined MABELLA_TEST
  #define MANU_DATAPATH "./database_tables_init_Mabella"
#endif


#include <Artikel/ArtikelBase.h>
#include <Auftrag/auftrag_status.h>
#include <Instanzen/ppsInstanz.h>

#ifdef PETIG_TEST

#define MIT_ROHWARENLAGER
#define MIT_BANDLAGER

#define KUNDENINSTANZ (ppsInstanz::ID(1))
#define ROLLEREI (ppsInstanz::ID(10))
#define SPRITZGIESSEREI (ppsInstanz::ID(7))

#define LagerPlatzKupfer (LagerPlatz(ppsInstanzID::Rohwarenlager,KUPFER_LAGERPLATZ))
#define LagerPlatzKupfer2 (LagerPlatz(ppsInstanzID::Rohwarenlager,KUPFER_LAGERPLATZ2))
#define LagerPlatzAcetat (LagerPlatz(ppsInstanzID::Rohwarenlager,ACETAT_LAGERPLATZ))
#define LagerPlatzJumbo (LagerPlatz(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ))

#define ARTIKEL_ROLLEREI ArtikelBase(124555)
#define ARTIKEL_ROLLEREIK ArtikelBase(124556)
#define ARTIKEL_FAERBEREI ArtikelBase(124551)
#define ARTIKEL_BANDLAGER ArtikelBase(123755)
#define ARTIKEL_RINGE ArtikelBase(210120)
#define ARTIKEL_ACETAT ArtikelBase(211007)
#define ARTIKEL_KUPFER ArtikelBase(209813)

#elif defined MANU_PROC_TEST 

#define ARTIKEL_SORTIMENT_SCHWARZ ArtikelBase(27)
#define ARTIKEL_SORTIMENT_BUNT ArtikelBase(28)
#define ARTIKEL_METALL ArtikelBase(1)
#define ARTIKEL_GRANULAT_GRUEN ArtikelBase(4)
#define ARTIKEL_GRIFF_ROT ArtikelBase(7)
#define ARTIKEL_SCHRAUBENZIEHER_GELB ArtikelBase(23)
#define ARTIKEL_SCHRAUBENZIEHER_ROT ArtikelBase(20)
#define ARTIKEL_SCHACHTEL ArtikelBase(26)


#define ROHWARENLAGER (ppsInstanz::ID(30))
#define EINKAUF (ppsInstanz::ID(35))
#define GIESSEREI (ppsInstanz::ID(20))
#define WERKSTATT (ppsInstanz::ID(10))
#define SCHRAUBENZIEHERLAGER (ppsInstanz::ID(5))
#define KUNDENINSTANZ (ppsInstanz::ID(1))

#elif defined MABELLA_TEST

#define ARTIKEL_TRIO ArtikelBase(44803)
#define ARTIKEL_ZWEI ArtikelBase(44800)

#define FERTIGWARENLAGER (ppsInstanz::ID(2))
#define EINKAUF (ppsInstanz::ID(4))
#define PRODPLANUNG (ppsInstanz::ID(3))
#define WEBEREI (ppsInstanz::ID(5))

#endif



#define DATUM ManuProC::Datum(31,12,2010)
#define DATUMP ManuProC::Datum(29,12,2010)
#define SPLITDATUM ManuProC::Datum(31,12,2011)
#define PLANDATUM4 ManuProC::Datum(31,12,2004)
#define PLANDATUM5 ManuProC::Datum(31,12,2005)
#define PLANDATUM6 ManuProC::Datum(31,12,2006)
#define DATUM9 ManuProC::Datum(31,12,2009)
#define NEWDATUM ManuProC::Datum(31,12,2020)

#define KUNDE 1000
#define KUNDE2 1111

#define UID 1001

#endif
