/* $Id: EntryValue.h,v 1.2 2001/06/22 07:29:02 christof Exp $ */
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

#ifndef ENTRYVALUE_H
#define ENTRYVALUE_H

#include<stdio.h>
#include"Datum.h"
#include"Artikel/Prozess.h"

#define SUBID_SPAWN     1
#define SUBID_WORK      2
#define SUBID_DONE      3


class EntryValue
{
 string strval;
 int intval;
 bool infinity;
// this does not belong here !!! put into a child class!!!
 bool prozess;
 Petig::Datum datum;

 struct {
  int p_id;
  string p_label;
  string p_text;
  Petig::Datum p_datum;
  int p_subid;
  } pval;
// end child class
 
public:

 EntryValue()
   {intval=-1;infinity=true;strval="-";prozess=false;}
 EntryValue(int v);
 EntryValue(const string &s);
 EntryValue(const char x[])
 { *this=EntryValue(string(x)); }
 EntryValue(const cH_Prozess &p, int subid, const Petig::Datum &d);
 EntryValue(const Petig::Datum &d);
   
 operator const string() const { return strval;}
 operator int() const { return infinity ? -1 : intval;}
 operator bool() const;

// das Alles sollte glaube ich weg 
 int getIntVal() const { return infinity ? -1:intval;}
 const string getStrVal() const { return strval;}
 const string getStrProzVal() const { return strval; }
// bis hier
 int getSubIdVal() const { return prozess ? pval.p_subid:-1;}
 int getProzIdVal() const { return prozess ? pval.p_id:-1;} 
 const Petig::Datum Datum() const { return datum; }
  
 bool operator==(const EntryValue &v) const;
 bool operator<(const EntryValue &v) const;
};

#endif // ENTRYVALH

