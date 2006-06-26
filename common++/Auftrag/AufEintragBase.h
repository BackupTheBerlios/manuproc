// $Id: AufEintragBase.h,v 1.52 2006/06/26 07:53:01 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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
#include<Misc/SQLerror.h>
#include <Auftrag/AuftragBase.h>
#include <Auftrag/auftrag_status.h>
#include<BaseObjects/ManuProcEintrag.h>
#include<Auftrag/auftrag_enums.h>
class cH_Kunde;




class AufEintragBase : public AuftragBase
{
public: 
  typedef AuftragBase::mengen_t mengen_t;
  static const int none_znr=ManuProcEintrag::none_znr;

protected:
 int zeilennr;

private:

 void updateStk(mengen_t newstk,mengen_t diff,const ArtikelBase& artikel,AufStatVal status) const throw(SQLerror);


public:
 AufEintragBase() : zeilennr(none_znr) {}
 AufEintragBase(cH_ppsInstanz inst,int auftragid) 
 	: AuftragBase(inst,auftragid), zeilennr(none_znr) {}
 AufEintragBase(cH_ppsInstanz inst,int auftragid,int z) 
 	:  AuftragBase(inst,auftragid), zeilennr(z) {}
 AufEintragBase(const AuftragBase& a,int z) 
 	:  AuftragBase(a), zeilennr(z) {}

private:
 void setVerarbeitung(const cH_Prozess p) const throw(SQLerror);

public:
 bool deleteAuftragEntry() const throw(SQLerror);
 void setLetzteLieferung(const ManuProC::Datum &datum) const throw(SQLerror);

 // gibt die Menge zurück, die verändert wurde. Falls reduziert werden sollte
 // müssen die input/output menge nicht übereinstimmen, da keine negativen Mengen
 // bestellt werden können
 mengen_t updateStkDiffBase__(const mengen_t &menge) const throw(SQLerror);

 void setLetztePlanungFuer(cH_ppsInstanz planinstanz) const throw(SQLerror);
 void calculateProzessInstanz(); // private?
 void setMaxPlanInstanz(int anzahl_planinstanzen) const throw(SQLerror); // private?
 
 int ZNr() const { return zeilennr; }
 bool valid() const { return AuftragBase::valid() && zeilennr>0; }
 bool operator!() const { return !valid(); }

private:
// friend class LagerBase;
 friend class AuftragBase;

public:

// int split_zuordnungen_to(mengen_t menge,ManuProC::Datum datum, 
//                         ArtikelBase artikel,AufStatVal status,
//                         bool dispoplanung);

 bool operator<(const AufEintragBase& b) const 
       {return Instanz()<b.Instanz() 
       	|| (Instanz()==b.Instanz() && (Id()<b.Id()
    		|| (Id()==b.Id() && ZNr()<b.ZNr())));
        }
 bool operator==(const AufEintragBase& b) const 
        {return Instanz()==b.Instanz() && Id()==b.Id() && ZNr()==b.ZNr();}

 friend std::ostream &operator<<(std::ostream &o,const AufEintragBase &ae);
 std::string str() const;

  // Diese Funktion ist zum Debuggen sehr nützlich:
  void ExistMenge(const std::string &s="") const;
  
  // vereinfachte Variante für AuftragBase::PassendeZeile
  static AufEintragBase PassendeZeile(const AuftragBase &a,
          const ManuProC::Datum &lieferdatum, const ArtikelBase& artikel,
          AufStatVal status) throw(SQLerror);
//  static std::list<AufEintragBase> PassendeZeilen(ArtikelBase const& art,
//          ppsInstanz::ID instanz,AufStatVal status) throw(SQLerror);
};

std::ostream &operator<<(std::ostream &o,const AufEintragBase &ae);
Query::Row& operator>>(Query::Row& is,AufEintragBase &aeb);
ArgumentList &operator<<(ArgumentList&q, const AufEintragBase &aeb);

#endif
