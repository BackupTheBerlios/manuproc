/* $Id: AuftragBase.h,v 1.34 2002/11/07 07:48:30 christof Exp $ */
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
class AufEintrag;
#include <Kunde/Kunde.h>
// hier sollten noch viel mehr Funktionen aus Auftrag rein !!!
class AufEintragBase;
#include <BaseObjects/ManuProcEntity.h>
class FetchIStream;


class AuftragBase
{
        bool Exists() const throw(SQLerror) ;
 public:
   typedef fixedpoint<ManuProC::Precision::AuftragsMenge> mengen_t;
   typedef Preis::rabatt_t rabatt_t;
   static const int handplan_auftrag_id =  20000; // gemeint sind alle Aufträge, die NICHT 0,1,2 sind
   static const int dispo_auftrag_id =  2;
   static const int plan_auftrag_id  =  1;
   static const int invalid_id       = ManuProcEntity<>::none_id ;
   // warum umbenennen?
   static const int none_id       = ManuProcEntity<>::none_id ;
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

   std::string str() const;        
	int Id() const {return auftragid;}
   void set_Id(int i) {auftragid = i;}
   ppsInstanz::ID InstanzID() const {return instanz->Id(); }
   cH_ppsInstanz Instanz() const {return instanz; }
	bool valid() const { return auftragid!=invalid_id; }
   void setStatusAuftragBase(AufStatVal st) const throw(SQLerror);
   void setRabatt(const rabatt_t auftragsrabatt) const throw(SQLerror);

   void create_if_not_exists(AufStatVal status,Kunde::ID kunde=Kunde::default_id) const;
   void BaumAnlegen(const AufEintrag& AE,int uid,bool setInstanzAuftraege=true) const;
   void InstanzAuftraegeAnlegen(const AufEintrag& AE,mengen_t menge,int uid,bool automatisch_geplant=false) const;
   bool existEntry(const ArtikelBase& artid,
                        const ManuProC::Datum& lieferdatum,
                        int& znr,int &newznr, mengen_t& menge, const AufStatVal status
                        ) const throw(SQLerror);
   // gibt Zeilennummer zurück
   struct st_tryUpdateEntry{bool automatisch_geplant;bool force_new;bool dispoplanung;
          explicit st_tryUpdateEntry() : automatisch_geplant(false),force_new(false),dispoplanung(false){}
          explicit st_tryUpdateEntry(bool a) : automatisch_geplant(a),force_new(false),dispoplanung(false){}
          explicit st_tryUpdateEntry(bool a,bool b,bool c) 
            : automatisch_geplant(a),force_new(b),dispoplanung(c){}
         };
   int tryUpdateEntry(mengen_t bestellt, 
               const ManuProC::Datum lieferdatum, const ArtikelBase& artikel,
               AufStatVal status,int uid,const AufEintragBase& altAEB,
               st_tryUpdateEntry st_bool=st_tryUpdateEntry()) const throw(SQLerror);


	// wandelt enum in std::string um
	static const std::string getStatusStr(AufStatVal a);

   bool operator==(const AuftragBase &b) const 
      {return instanz==b.instanz && auftragid==b.auftragid;}

};

std::ostream &operator<<(std::ostream &o,const AuftragBase &a);

//FetchIStream& operator>>(FetchIStream& is,AuftragBase::mengen_t &menge);


#endif
