/* $Id: selAufEntry.h,v 1.15 2001/07/02 17:04:57 cvs_christof Exp $ */
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

#if 0
#ifndef SELECTEDAUFLIST
#define SELECTEDAUFLIST

#include"Auftrag/sqlAuftragSelector.h"
#include"Aux/SQLerror.h"
#include<vector>

class SelectedAufentry
{
 int instanz;
 int auftragid;
 int zeilennr;
 int kdnr;
 
public:
 SelectedAufentry(int _instanz, int aufid, int znr, int knr=1); 
	
 int getAuftragid() const { return auftragid;}
 int Instanz() const { return instanz;}
 int getZeilennr() const { return zeilennr;}
 int getKdNr() const { return kdnr; }
 inline int mapKey() { return auftragid*10000+zeilennr; }
};



class SelectedAufList
{
public:
 std::vector<SelectedAufentry> aufidliste;

 SelectedAufList(const SQLAuftragSelector &sel) throw(SQLerror);
 int getLen() { return aufidliste.size();}
 void insert(int instanz,int aid, int znr, int knr=1);
 void delEntry(int instanz,int aid, int znr);
 void delAuftrag(int instanz,int aid);
 void addList(const SQLAuftragSelector &sel) throw(SQLerror);
};

#endif
#endif
