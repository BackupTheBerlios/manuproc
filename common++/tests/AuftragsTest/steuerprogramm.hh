// $Id: steuerprogramm.hh,v 1.6 2002/07/05 12:35:02 christof Exp $
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


#define MIT_ROHWARENLAGER
#define MIT_BANDLAGER

#include <Artikel/ArtikelBase.h>
#include <Auftrag/auftrag_status.h>

#define ARTIKEL_ROLLEREI ArtikelBase(124555)
#define ARTIKEL_FAERBEREI ArtikelBase(124551)
#define ARTIKEL_BANDLAGER ArtikelBase(123755)
#define ARTIKEL_RINGE ArtikelBase(210120)
#define ARTIKEL_ACETAT ArtikelBase(211007)
#define ARTIKEL_KUPFER ArtikelBase(209813)


#define DATUM ManuProC::Datum(31,12,2010)
#define SPLITDATUM ManuProC::Datum(31,12,2011)
#define PLANDATUM4 ManuProC::Datum(31,12,2004)
#define PLANDATUM5 ManuProC::Datum(31,12,2005)
#define PLANDATUM6 ManuProC::Datum(31,12,2006)
#define DATUM9 ManuProC::Datum(31,12,2009)
#define NEWDATUM ManuProC::Datum(31,12,2020)

#define KUNDE 1000

#define UID 1000


#endif
