// $Id: Check.hh,v 1.29 2003/08/07 09:48:29 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG, 
 *  written by Malte Thoma + Christof Petig
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

#ifndef MPC_CHECK_HH
#define MPC_CHECK_HH
#include <Artikel/ArtikelBaum.h>
#include <Artikel/Prozess.h>
#include <Lager/LagerPlatz.hh>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>

class Check
{
   enum was_checken_p { b_Jumbo, b_Lieferschein, b_Menge, b_RohLager };
 public:
   enum was_checken { Nix,
   		Jumbo=1<<b_Jumbo,
   		Lieferschein=1<<b_Lieferschein,
   		Menge=1<<b_Menge,
   		RohLager=1<<b_RohLager };
   		
   enum e_check {Jumbo_richtig,Jumbo_falsch,Jumbo_doppelt};
 private:
   void dump(was_checken,const std::string &name);
   bool vergleich(was_checken,const std::string &name);
 public:
   Check(){}; 
   // Erfolgreich: true; sonst false
   bool teste(was_checken was, const std::string &name,bool vor_dem_test_reparieren=false);
   static bool analyse,reparieren,overwrite,resort;
};

Check::was_checken operator|(Check::was_checken a, Check::was_checken b);
bool operator&(Check::was_checken a, Check::was_checken b);

#endif
