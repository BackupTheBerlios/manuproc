/* $Id: LieferscheinList.h,v 1.8 2002/02/28 15:19:29 christof Exp $ */
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


#include"Lieferschein.h"
#include<Aux/SQLerror.h>
#include<vector>

#ifndef LIEFERLIST
#define LIEFERLIST


class LieferscheinList
{
 std::vector<cH_Lieferschein> sellist;
 cH_ppsInstanz instanz;

public:
 typedef std::vector<cH_Lieferschein>::const_iterator const_iterator;
 const_iterator begin()
   {return sellist.begin();}
 const_iterator end()
   {return sellist.end();}


 struct sel_KundenId 
 { unsigned long int id;
   sel_KundenId(unsigned long int k):id(k){}
 }; 
 struct sel_ArtikelId 
 { unsigned long int id;
   sel_ArtikelId(unsigned long int k):id(k){}
 }; 
 
 LieferscheinList(const cH_ppsInstanz instanz) throw(SQLerror);
 LieferscheinList(const cH_ppsInstanz instanz,const sel_KundenId &selparam) throw(SQLerror);
 LieferscheinList(const cH_ppsInstanz instanz,const sel_ArtikelId &selparam) throw(SQLerror);

#ifdef MABELLA_EXTENSIONS
 struct sel_GeliefStat 
 { bool geliefert;
   sel_GeliefStat(bool g):geliefert(g){}
 }; 
 
 struct sel_DPDLNr 
 { int dpdlnr;
   sel_DPDLNr(int d):dpdlnr(d){}
 }; 


 LieferscheinList(const cH_ppsInstanz instanz,const sel_GeliefStat &selparam) throw(SQLerror);
 LieferscheinList(const cH_ppsInstanz _instanz,const sel_DPDLNr &selparam) throw(SQLerror);
 bool setDPDData(int dpdlnr) throw(SQLerror);
#endif

 void fuelle(const std::string &query) throw(SQLerror);
 std::string SQuery();
 unsigned int Size() {return sellist.size();}
 
 const cH_Lieferschein operator[](unsigned int i)
 	{ return sellist[i]; }
// friend std::ostream &operator<<(std::ostream &o,const LieferscheinList &l);
};

#endif
