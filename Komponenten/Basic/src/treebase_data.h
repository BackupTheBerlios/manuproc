/* $Id: treebase_data.h,v 1.9 2003/10/07 06:49:57 christof Exp $ */
/*  libKomponenten: GUI components for ManuProC's libcommon++
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

#ifndef KOMPONENTEN_TREEBASE_DATA_H
#define KOMPONENTEN_TREEBASE_DATA_H

// you can use this example class to pass up to six strings
// feel free to enlarge it if you need
// it's not designed for efficiency!

#include <Misc/EntryValueIntString.h>
#include <RowDataBase.h>

class RowDataStrings : public RowDataBase
{static const guint max_arguments=6;
 
 std::string data[max_arguments];
public:

 RowDataStrings(const std::string &a, const std::string &b="", const std::string &c="", 
 	   const std::string &d="", const std::string &e="", const std::string &f="")
 { data[0]=a;
   data[1]=b;
   data[2]=c;
   data[3]=d;
   data[4]=e;
   data[5]=f;
 }

// warum so kompliziert ...
 const std::string getStrAt(guint i) const 
   {return operator[](i);
   }
// ... wenns auch einfach geht
 const std::string operator[](guint i) const
   {if (i>=0 && i<max_arguments) return data[i];
    return "";
   }
   
 virtual const cH_EntryValue Value(guint _seqnr, gpointer gp) const
 {if (_seqnr>=0 && _seqnr<max_arguments) return cH_EntryValueIntString(data[_seqnr]);
  return cH_EntryValueIntString("");
 }
};

class cH_RowDataStrings : public cH_RowDataBase
{
 cH_RowDataStrings(const RowDataStrings *r) : cH_RowDataBase(r) {}
public:
 cH_RowDataStrings(const std::string &a, const std::string &b="", const std::string &c="", 
 	   const std::string &d="", const std::string &e="", const std::string &f="")
	: cH_RowDataBase(new RowDataStrings(a,b,c,d,e,f)) {}
	
 		
};



#endif
