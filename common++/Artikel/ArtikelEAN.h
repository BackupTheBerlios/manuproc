/* $Id: ArtikelEAN.h,v 1.1 2002/09/26 15:08:32 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
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

#ifndef ARTIKELEAN
#define ARTIKELEAN

#include<string>
#include"Aux/SQLerror.h"
#include"Artikel/ArtikelBase.h"

class ArtikelEAN : public ArtikelBase
{
 std::string ean;
 
public:

 ArtikelEAN() {}
 ArtikelEAN(const std::string _ean) throw(SQLerror); 

 const std::string getEAN() const {return ean;}

};



#endif





