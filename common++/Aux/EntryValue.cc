/* $Id: EntryValue.cc,v 1.16 2001/06/20 11:24:29 cvs_malte Exp $ */
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

#include"Aux/EntryValue.h"
#include <Aux/string0.h>

EntryValue::EntryValue(int v)
   {intval=v; char s[50]; snprintf0(s,50,"%d",v); strval=s;
    infinity=false;prozess=false;}


EntryValue::EntryValue(const string &s)
   {intval=atoi(s.c_str()); 
    strval=s;
    unsigned char *x=(unsigned char*)s.c_str();
    while (isspace(*x)) ++x;
    infinity=!isdigit(*x);
    prozess=false;
   }

EntryValue::EntryValue(const cH_Prozess &p, int subid=0, const Petig::Datum &d=Petig::Datum())
   {infinity=true;
    prozess=true; 
    pval.p_id=p->getProzessID();
    pval.p_label=subid?p->getTyp():"-";
    pval.p_text=subid?p->getText():"-";
    pval.p_datum=d;
    pval.p_subid=subid;
    strval=pval.p_label+" "+
           pval.p_text+" "+
           (pval.p_datum.valid()?pval.p_datum.c_str():"-");
    }

EntryValue::EntryValue(const Petig::Datum &d)
{
 strval=d.c_str();
 intval=d.Jahr()*10000+d.Monat()*100+d.Tag();
 datum=d;
 prozess=false; 
 infinity=false;
}



 bool EntryValue::operator==(const EntryValue &v) const
   {if((!infinity)&&(!v.infinity))
      {if(v.intval==intval) return true;
       return false;}
    if(prozess&&v.prozess)
      {
      if((!pval.p_id) && (!v.pval.p_id)) return true;
      if(strval==v.strval) return true; 
      return false;
      }
    else return false;}
    
 bool EntryValue::operator<(const EntryValue &v) const
   {if((!infinity)&&(!v.infinity))
    { if(intval<v.intval) return true;
      if(intval>v.intval) return false;
    }
    if(prozess&&v.prozess)
    {if(strval<v.strval) return true;
     if(strval>v.strval) return false;
     if(pval.p_subid<v.pval.p_subid) return true;
     return false;
    }
    if(strval<v.strval) return true; 
    else return false;}
 
 EntryValue::operator bool() const
   { if(!infinity)
       if(intval) return true;
     return false;}  
