// $Id: steuerprogramm.hh,v 1.1 2002/06/20 06:43:31 christof Exp $
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


#define DATUM Petig::Datum(1,1,2010)
#define SPLITDATUM Petig::Datum(3,3,2003)
#define PLANDATUM Petig::Datum(5,5,2005)
#define NEWDATUM Petig::Datum(2,2,2020)

#define KUNDE 1000

#define UID 1000


#endif
