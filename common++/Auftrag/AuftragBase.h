/* $Id: AuftragBase.h,v 1.45 2003/05/22 12:50:48 christof Exp $ */
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
#include <Misc/Datum.h>
#include <Artikel/ArtikelBase.h>
#include <Misc/Preis.h>
#include <Auftrag/auftrag_status.h>
class AufEintrag;
#include <Kunde/Kunde.h>
// hier sollten noch viel mehr Funktionen aus Auftrag rein !!!
class AufEintragBase;
#include <BaseObjects/ManuProcEntity.h>
class FetchIStream;
#include <Misc/UniqueValue.h>
#include <Misc/compiler_ports.h>

class AuftragBase
{
   bool Exists() const throw(SQLerror) ;
 public:
   typedef fixedpoint<ManuProC::Precision::AuftragsMenge> mengen_t;
   typedef Preis::rabatt_t rabatt_t;
   typedef ManuProcEntity<>::ID ID;
   static const ID handplan_auftrag_id =  3; // gemeint sind alle Aufträge, die NICHT 0,1,2 sind
   static const ID dispo_auftrag_id =  2;
   static const ID plan_auftrag_id  =  1;
   static const ID invalid_id       = ManuProcEntity<>::none_id ;
   // warum umbenennen?
   static const ID none_id       = ManuProcEntity<>::none_id ;
   static const ID ungeplante_id    =  0;

   static const ID PlanId_for(const cH_ppsInstanz &instanz);

 protected:
   cH_ppsInstanz instanz; 	
 	ID auftragid;	
 public:
	AuftragBase() 
	        : instanz(ppsInstanzID::None), auftragid(invalid_id) {}
	AuftragBase(cH_ppsInstanz _instanz, ID aufid) throw() 
	        :instanz(_instanz), auftragid(aufid) 
	        {}
	AuftragBase(cH_ppsInstanz _instanz, ID aufid,Kunde::ID kid) throw(SQLerror) ;

   std::string str() const;        
	ID Id() const {return auftragid;}
   void set_Id(ID i) {auftragid = i;}
   ppsInstanz::ID InstanzID() const {return instanz->Id(); }
   cH_ppsInstanz Instanz() const {return instanz; }
	bool valid() const { return auftragid!=invalid_id; }
   bool editierbar() const;
   void setStatusAuftragBase(AufStatVal st) const throw(SQLerror);
   void setRabatt(const rabatt_t auftragsrabatt) const throw(SQLerror);
   
   struct st_BestellmengeAendern{bool automatisch_geplant;bool force_new;bool dispoplanung;
          explicit st_BestellmengeAendern() : automatisch_geplant(false),force_new(false),dispoplanung(false){}
          explicit st_BestellmengeAendern(bool a) : automatisch_geplant(a),force_new(false),dispoplanung(false){}
          explicit st_BestellmengeAendern(bool a,bool b,bool c) 
            : automatisch_geplant(a),force_new(b),dispoplanung(c){}
         };
   // gibt Zeilennummer zurück
   int PassendeZeile(const ManuProC::Datum lieferdatum, const ArtikelBase& artikel,
               AufStatVal status, unsigned uid) const throw(SQLerror);
   // gibt Zeilennummer zurück
   int BestellmengeAendern(mengen_t bestellt, 
               const ManuProC::Datum lieferdatum, const ArtikelBase& artikel,
               AufStatVal status,int uid,const AufEintragBase& altAEB,
               st_BestellmengeAendern st_bool=st_BestellmengeAendern()) const throw(SQLerror);

	// wandelt enum in std::string um
	static const std::string getStatusStr(AufStatVal a);

   bool operator==(const AuftragBase &b) const 
      {return instanz==b.instanz && auftragid==b.auftragid;}

   // Varianten für negative Zahlen??? kommt häufiger vor?
   static mengen_t min(const mengen_t &x,const mengen_t &y);
   static mengen_t max(const mengen_t &x,const mengen_t &y);

   static const UniqueValue::value_t trace_channel;

public:
// wird in Instanzen.cc verwendet
   void create_if_not_exists(AufStatVal status,Kunde::ID kunde=Kunde::default_id) const;
// wird in AufEintrag_sql.pgcc verwendet
   // könnte eigentlich durch 
   // AuftragBase(instanz,AB::dispo_auftrag_id).BestellmengeAendern 
   // ersetzt werden (menge,datum,artikel,OPEN,uid,AufEintragBase())
   static __deprecated void dispo_auftrag_aendern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,
      const mengen_t &menge,const ManuProC::Datum &datum,const AufEintragBase &kindAEB) ;
// Iiii bah - überprüfen!
// zu viele merkwürdige Argumente
   // -> BestellmengeAendern ??? PassendeZeile?
   __deprecated bool existEntry(const ArtikelBase& artid,
                        const ManuProC::Datum& lieferdatum,
                        int& znr,int &newznr, mengen_t& menge, const AufStatVal status
                        ) const throw(SQLerror);
   // gibt Zeilennr zurück
   int existEntry(const ArtikelBase& artid, const ManuProC::Datum& lieferdatum,
                  const AufStatVal status, mengen_t& menge_out) const throw();
};

std::ostream &operator<<(std::ostream &o,const AuftragBase &a);
class ArgumentList;
ArgumentList &operator<<(ArgumentList &q, const AuftragBase &ab);
//FetchIStream& operator>>(FetchIStream& is,AuftragBase::mengen_t &menge);


#endif
