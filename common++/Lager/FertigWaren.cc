// $Id: FertigWaren.cc,v 1.6 2004/01/13 22:11:36 jacek Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include "FertigWaren.h"
#include <Misc/Zeitpunkt_new.h>
#include <Misc/Transaction.h>

std::map<int,std::string> FertigWaren::usermap;


FetchIStream &operator>>(FetchIStream &is, FertigWaren &fw)
 { std::string zeit;
   char a;
  
   is >> fw.stk >> fw.bestand >>
	zeit >> 
   	a >>
//   	fw.uname >> 
 	fw.uid >> 
 	fw.lieferschein >> 
 	fw.artikel;
 	
   fw.zeit=Zeitpunkt_new(zeit.c_str());
   
   switch(a)
     {
	case 'L' : fw.aktion=FertigWaren::eLieferschein; break;
	case 'M' : fw.aktion=FertigWaren::eManuell; break;
	case 'I' : fw.aktion=FertigWaren::eInventur; break;
        default : fw.aktion=FertigWaren::eUnknown;
     }
	
   return is;	
}
  
  
