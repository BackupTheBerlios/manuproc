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

#ifndef AUFTRAGBASE_H
#define AUFTRAGBASE_H
#include<Instanzen/ppsInstanz.h>
#include <Aux/Datum.h>
#include <Artikel/ArtikelBase.h>
#include <Aux/Preis.h>
#include <Auftrag/auftrag_status.h>
//#include <Aux/Datum.h>
class AufEintrag;
#include <Kunde/Kunde.h>
// hier sollten noch viel mehr Funktionen aus Auftrag rein !!!
class AufEintragBase;

class AuftragBase
{
        bool Exists() const throw(SQLerror) ;
 public:
   typedef fixedpoint<0> mengen_t;
   typedef Preis::rabatt_t rabatt_t;
   static const int handplan_auftrag_id =  20000; // gemeint sind alle Aufträge, die NICHT 0,1,2 sind
   static const int dispo_auftrag_id =  2;
   static const int plan_auftrag_id  =  1;
   static const int invalid_id       = -1;
   static const int ungeplante_id    =  0;

   static const int PlanId_for(const cH_ppsInstanz &instanz);

 protected:
   cH_ppsInstanz instanz; 	
 	int auftragid;	
 public:
	AuftragBase() 
	        : instanz(ppsInstanzID::None), auftragid(invalid_id) {}
	AuftragBase(cH_ppsInstanz _instanz, int aufid) throw() 
	        :instanz(_instanz), auftragid(aufid) 
	        {}
	AuftragBase(cH_ppsInstanz _instanz, int aufid,int kid) throw(SQLerror) ;
        
	int Id() const {return auftragid;}
   void set_Id(int i) {auftragid = i;}
   ppsInstanz::ID InstanzID() const {return instanz->Id(); }
   cH_ppsInstanz Instanz() const {return instanz; }
	bool valid() const { return auftragid!=invalid_id; }
   void setStatusAuftragBase(AufStatVal st) const throw(SQLerror);
   void setRabatt(const rabatt_t auftragsrabatt) const throw(SQLerror);

   void create_if_not_exists(AufStatVal status,Kunde::ID kunde=Kunde::default_id) const;
   int insertNewEntry(const mengen_t bestellt, 
                const Petig::Datum lieferdatum, const ArtikelBase& artikel,
                const AufStatVal status,int uid,const bool setInstanzAuftraege,
                const Preis& preis=Preis(),const rabatt_t aufeintragsrabatt=0,
                const cH_PreisListe &preisliste=PreisListe::none_id) const throw(SQLerror);
   void InstanzAuftraegeAnlegen(const ArtikelBase& art,const int altZnr,
               const Petig::Datum& lieferdatum, const AufStatVal status, int uid,
                const mengen_t menge) const; 
   bool existEntry(const ArtikelBase& artid,
                        const Petig::Datum& lieferdatum,
                        int& znr,int &newznr, mengen_t& menge, const AufStatVal status
                        ) const throw(SQLerror);
   // gibt Zeilennummer zurück
   int tryUpdateEntry(mengen_t bestellt, 
               const Petig::Datum lieferdatum, const ArtikelBase& artikel,
               AufStatVal status,int uid,const AufEintragBase& altAEB,bool force_new=false) 
               const throw(SQLerror);

	// wandelt enum in std::string um
	static const std::string getStatusStr(AufStatVal a);

   bool operator==(const AuftragBase &b) const 
      {return instanz==b.instanz && auftragid==b.auftragid;}
};

#endif
