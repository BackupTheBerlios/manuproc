// $Id: AufEintragBase2.h,v 1.22 2001/11/06 14:40:34 cvs_malte Exp $
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
#include <Auftrag/AuftragBase.h>

// eigentlich AufEintragBaseBase

class AufEintragBase2 : public AuftragBase
{
protected:
 int zeilennr;
  
 // noch benötigt? CP 10'01
private:
 inline int mapKey() const { return auftragid*10000+zeilennr; } 

public:
 AufEintragBase2() : zeilennr(0) {}
 AufEintragBase2(cH_ppsInstanz inst) 
 	: AuftragBase(inst), zeilennr(0) {}
 AufEintragBase2(cH_ppsInstanz inst,int a,int z) 
 	:  AuftragBase(inst,a), zeilennr(z) {}
 
 void setVerarbeitung(const cH_Prozess p) const throw(SQLerror);
 // gibt gelieferte Menge zurück
 int abschreiben(int menge) const throw(SQLerror);
 bool deleteAuftragEntry() const throw(SQLerror);
 
 int ZNr() const { return zeilennr; }
 bool valid() const { return AuftragBase::valid(); }

 bool operator<(const AufEintragBase2& b) const 
        {return Id()<b.Id();}
};

#endif
