// $Id: FertigWaren.cc,v 1.4 2003/07/15 13:18:25 jacek Exp $
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

FetchIStream &operator>>(FetchIStream &is, FertigWaren &fw)
 { std::string zeit;
  
   is >> fw.stk >> 
   	zeit >> 
   	(char)fw.aktion >>
   	fw.uname >> 
 	fw.uid >> 
 	fw.lieferschein >> 
 	fw.artikel;
 	
   fw.zeit=Zeitpunkt_new(zeit.c_str());
   	
   return is;	
}
  
  
