/* $Id: auftrag_enums.h,v 1.2 2002/12/17 13:55:32 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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


#ifndef AUFENUMS
#define AUFENUMS

namespace ManuProC 
{ 
 namespace Auftrag {
    enum Action {r_None,r_Anlegen,r_Planen,r_Produziert,r_Closed,
                 r_Abbestellt,r_Reparatur};
  }
}

#endif
