// $Id: AufEintragBase.h,v 1.26 2002/05/03 10:22:54 christof Exp $
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

#ifndef AUFEINTRAGBASE
#define AUFEINTRAGBASE
#include<Artikel/Prozess.h>
#include<Aux/SQLerror.h>
//#include<Aux/ppsInstanz.h>
#include <Auftrag/AuftragBase.h>
#include <Auftrag/auftrag_status.h>
//#include <list>
class cH_Kunde;


class AufEintragBase : public AuftragBase
{
public: 
  typedef AuftragBase::mengen_t mengen_t;
protected:
 int zeilennr;
  
private:
 void updateStk(mengen_t newstk,mengen_t diff,const ArtikelBase& artikel,AufStatVal status) const throw(SQLerror);

public:
 AufEintragBase() : zeilennr(0) {}
 AufEintragBase(cH_ppsInstanz inst,int auftragid) 
 	: AuftragBase(inst,auftragid), zeilennr(0) {}
 AufEintragBase(cH_ppsInstanz inst,int auftragid,int z) 
 	:  AuftragBase(inst,auftragid), zeilennr(z) {}
 AufEintragBase(const AuftragBase& a,int z) 
 	:  AuftragBase(a), zeilennr(z) {}

private:
 void setVerarbeitung(const cH_Prozess p) const throw(SQLerror);
public:
 // gibt danach gesamte gelieferte Menge zurück
 mengen_t abschreiben(mengen_t menge) const throw(SQLerror);
 bool deleteAuftragEntry() const throw(SQLerror);

 // gibt die Menge zurück, die verändert wurde. Falls reduziert werden sollte
 // müssen die input/output menge nicht übereinstimmen, da keine negativen Mengen
 // bestellt werden können
 mengen_t updateStkDiff__(mengen_t menge) const throw(SQLerror);

 void setLetztePlanungFuer(cH_ppsInstanz planinstanz) const throw(SQLerror);
 void calculateProzessInstanz(); // private?
 void setMaxPlanInstanz(int anzahl_planinstanzen) const throw(SQLerror); // private?
 
 int ZNr() const { return zeilennr; }
 bool valid() const { return AuftragBase::valid(); }

 bool operator<(const AufEintragBase& b) const 
       {return Instanz()<b.Instanz() 
       	|| (Instanz()==b.Instanz() && (Id()<b.Id()
    		|| (Id()==b.Id() && ZNr()<b.ZNr())));
        }
 bool operator==(const AufEintragBase& b) const 
        {return Instanz()==b.Instanz() && Id()==b.Id() && ZNr()==b.ZNr();}

 friend std::ostream &operator<<(std::ostream &o,const AufEintragBase &ae);
};

std::ostream &operator<<(std::ostream &o,const AufEintragBase &ae);

#endif
