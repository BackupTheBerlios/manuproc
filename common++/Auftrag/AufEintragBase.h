// $Id: AufEintragBase.h,v 1.31 2002/06/26 09:04:27 christof Exp $
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
//#include<Instanzen/ppsInstanz.h>
#include <Auftrag/AuftragBase.h>
#include <Auftrag/auftrag_status.h>
//#include <list>
#include<BaseObjects/ManuProcEntity.h>
class cH_Kunde;


class AufEintragBase : public AuftragBase
{
public: 
  typedef AuftragBase::mengen_t mengen_t;
  static const int none_znr=0;
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

 // gibt danach gesamte gelieferte Menge zurück
 // USE: AufEintrag.abschreiben(menge)
// void abschreiben(mengen_t menge,ManuProcEntity::ID lfrsid=
// 			ManuProcEntity::none_id) const throw(SQLerror) 
// 			  {assert("!dont use me!");};
public:
//??? void abschreiben_fuer_Instanzen(mengen_t menge) const throw(SQLerror);
 bool deleteAuftragEntry() const throw(SQLerror);
 void setLetzteLieferung(const Petig::Datum &datum) const throw(SQLerror);

 // gibt die Menge zurück, die verändert wurde. Falls reduziert werden sollte
 // müssen die input/output menge nicht übereinstimmen, da keine negativen Mengen
 // bestellt werden können
 mengen_t updateStkDiffBase__(int uid,mengen_t menge) const throw(SQLerror);

 void setLetztePlanungFuer(cH_ppsInstanz planinstanz) const throw(SQLerror);
 void calculateProzessInstanz(); // private?
 void setMaxPlanInstanz(int anzahl_planinstanzen) const throw(SQLerror); // private?
 
 int ZNr() const { return zeilennr; }
 bool valid() const { return AuftragBase::valid(); }

 // Planen
 // *this ist der ZielAufEintragBase
 void PlanenDispo(int uid,const ArtikelBase& artikel,mengen_t menge,const Petig::Datum &datum);
 static void Planen(int uid,std::vector<AufEintrag> LAE,mengen_t menge,
      const AuftragBase &zielauftrag,const Petig::Datum &datum);

 int split_zuordnungen_to(mengen_t menge,Petig::Datum datum, 
                         ArtikelBase artikel,AufStatVal status,
                         int uid,bool dispoplanung);

void move_menge_to_dispo_zuordnung_or_lager(mengen_t menge,ArtikelBase artikel,int uid);


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
