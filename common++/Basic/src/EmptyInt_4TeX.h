/* $Id: EmptyInt_4TeX.h,v 1.6 2003/07/23 06:59:51 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include <string>
#include <Misc/itos.h>

std::string EmptyInt_4TeX(int i,int breite=0)
    {if (i==0) 
       { std::string s;
         for(int i=0;i<=breite;++i) s+="~";
         return s; 
       }
     else return itos(i);
    }
/*
std::string EmptyInt_4TeX(double i)
    {if (i==0.0) return ""; 
     else return dtos(i);
    }
*/
