// $Id: EAN.h,v 1.14 2006/10/31 16:04:21 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2001 Adolf Petig GmbH & Co. KG
 *                             written by Christof Petig and Malte Thoma
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
#include <Artikel/ArtikelTyp.h>


class EAN
{
      static std::string add_Pruefsumme(std::string ean,int unsigned length=12);
   public:
//      static std::string next_value(const ArtikelTyp at);
      static std::string next_value();
};


class EAN_Range
{
 int id;
 int fixed;
 int von;
 int bis;
 
 EAN_Range(){};
public:

 EAN_Range(int _id) throw(SQLerror);
 int Von() const { return von; }
 int Bis() const { return bis; }
 int Fixed() const { return fixed; }

 static int NextRangeID() throw(SQLerror);
 unsigned long long ReserveNext() throw(SQLerror);
 
};





