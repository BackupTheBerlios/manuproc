// $Id: Check.hh,v 1.2 2002/06/20 06:54:51 christof Exp $
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

///////////////////////////////////////////////////////////////////////////
// Do not change anything below this line /////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include <Artikel/ArtikelBaum.h>
#include <Artikel/Prozess.h>
#include <Lager/LagerPlatz.hh>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>

class Check
{
 public:
   enum e_check {Open,Menge_Plus,Menge_Minus,Menge_MinusMinus,StatusClosed,
                 Datumsaenderung,Rohwarenlager_einlagern,Bandlager_einlagern,
                 Rohwarenlager_auslagern,Bandlager_auslagern,
                 Planen_Kupfer,Planen_Weberei,Planen_Faerberei_teil,Planen_Faerberei,
                 Split,Split_Rohwarenlager_einlagern,Split_Rohwarenlager_auslagern,
                 ZweiAuftraege_anlegen,ZweiterAuftrag_frueheresDatum,
                 Jumbo_raus,Jumbo_rein};
 private:
   void dump();
   void vergleich(e_check);
 public:
   Check(){}; 
   void teste(e_check check);

};
