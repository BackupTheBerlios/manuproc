// $Id: AufEintragBase2.h,v 1.6 2001/06/22 09:38:43 cvs_malte Exp $
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

#ifndef AUFEINTRAGBASE2
#define AUFEINTRAGBASE2
#include<Artikel/Prozess.h>
#include<Aux/SQLerror.h>
#include<Aux/ppsInstanz.h>


class AufEintragBase2
{
protected:
 int auftragid;
 int zeilennr;
 ppsInstanz instanz;
  
public:
 AufEintragBase2() :auftragid(0), zeilennr(0), 
			instanz(ppsInstanz::INST_KNDAUF) {}
 AufEintragBase2(int a,int z, ppsInstanz::ppsInstId inst) 
 	: auftragid(a), zeilennr(z), instanz(inst) {}
 
 void setVerarbeitung(const cH_Prozess p) throw(SQLerror);
 inline int mapKey() const { return auftragid*10000+zeilennr; } 

// deprecatetd
 int AufId() const { return auftragid; }
 
 int Id() const { return auftragid; }
 int ZNr() const { return zeilennr; }
 ppsInstanz::ppsInstId InstId() const { return instanz.Id(); }
// int Instanz() const { return instanz; }
 
};

#endif
