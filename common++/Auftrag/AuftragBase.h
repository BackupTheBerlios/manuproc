/* $Id: AuftragBase.h,v 1.62 2004/10/21 11:02:19 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2004 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski
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
   static const ID handplan_auftrag_id =  3; // gemeint sind alle Auftr�ge, die NICHT 0,1,2 sind
   static const ID dispo_auftrag_id =  2;
   static const ID plan_auftrag_id  =  1;
   static const ID ungeplante_auftrag_id    =  0;
   static const ID invalid_id       = ManuProcEntity<>::none_id ;
   // warum umbenennen?
   static const ID none_id       = ManuProcEntity<>::none_id ;
   static const ID ungeplante_id    =  ungeplante_auftrag_id;
   static const ID handplan_id =  handplan_auftrag_id;
   static const ID dispo_id =  dispo_auftrag_id;
   static const ID plan_id  =  plan_auftrag_id;
//   static const ID auffuellen_id =  3; // Mindestmengenbestellung im Lager
   
   static const int jahresmultiplikator = 10000;

   static const ID PlanId_for(const cH_ppsInstanz &instanz);

 protected:
   cH_ppsInstanz instanz; 	
   ID auftragid;	
   mutable Kunde::ID verknr;	
   
 public:
	AuftragBase() 
	        : instanz(ppsInstanzID::None), auftragid(invalid_id),
	        verknr(Kunde::none_id) {}
	AuftragBase(cH_ppsInstanz _instanz, ID aufid) throw() 
	        :instanz(_instanz), auftragid(aufid),verknr(Kunde::none_id) 
	        {}
	AuftragBase(cH_ppsInstanz _instanz, ID aufid,Kunde::ID kid) throw(SQLerror) ;

   std::string str() const;        
	ID Id() const {return auftragid;}
   void set_Id(ID i) {auftragid = i;}
   ppsInstanz::ID InstanzID() const {return instanz->Id(); }
   cH_ppsInstanz Instanz() const {return instanz; }
	bool valid() const { return auftragid!=invalid_id; }
	bool operator!() const { return !valid(); }
   bool editierbar() const;
   void setStatusAuftragBase(AufStatVal st) const throw(SQLerror);
   void setRabatt(const rabatt_t auftragsrabatt) const throw(SQLerror);
   
   // gibt Zeilennummer zur�ck
   int PassendeZeile(const ManuProC::Datum lieferdatum, const ArtikelBase& artikel,
               AufStatVal status) const throw(SQLerror);
   // gibt Zeilennummer zur�ck
   int BestellmengeAendern(mengen_t bestellt, 
               const ManuProC::Datum lieferdatum, const ArtikelBase& artikel,
               AufStatVal status,const AufEintragBase& altAEB) const throw(SQLerror);

	// wandelt enum in std::string um
	static const std::string getStatusStr(AufStatVal a);

   bool operator==(const AuftragBase &b) const 
      {return instanz==b.instanz && auftragid==b.auftragid;}

   // Varianten f�r negative Zahlen??? kommt h�ufiger vor?
   static mengen_t min(const mengen_t &x,const mengen_t &y)
   { mengen_t m; if(x<=y) m=x; else m=y; if(m<0) m=0; return m;}
   static mengen_t max(const mengen_t &x,const mengen_t &y)
   { if(x>=y) return x; return y; }
   static mengen_t Gesamtmenge(int stueck,const mengen_t &x);

   static const UniqueValue::value_t trace_channel;
   static bool tolerate_inconsistency;

public:
   Kunde::ID getVerknr() const throw(SQLerror);	

// wird in Instanzen.cc verwendet
   bool create_if_not_exists(AufStatVal status,Kunde::ID kunde=Kunde::default_id) const;
// wird in AufEintrag_sql.pgcc verwendet
   // k�nnte eigentlich durch 
   // AuftragBase(instanz,AB::dispo_auftrag_id).BestellmengeAendern 
   // ersetzt werden (menge,datum,artikel,OPEN,AufEintragBase())
   static __deprecated void dispo_auftrag_aendern(cH_ppsInstanz instanz,const ArtikelBase artikel,
      const mengen_t &menge,const ManuProC::Datum &datum,const AufEintragBase &kindAEB) ;
// Iiii bah - �berpr�fen!
// zu viele merkw�rdige Argumente
   // -> BestellmengeAendern ??? PassendeZeile?
   __deprecated bool existEntry(const ArtikelBase& artid,
                        const ManuProC::Datum& lieferdatum,
                        int& znr,int &newznr, mengen_t& menge, const AufStatVal status
                        ) const throw(SQLerror);
   // gibt Zeilennr zur�ck
   int existEntry(const ArtikelBase& artid, const ManuProC::Datum& lieferdatum,
                  const AufStatVal status, mengen_t& menge_out) const throw();

   static int aktuellesJahr();
   static void setzeAktuellesJahr(int j);
   
   static std::string ID2string(ID id);
};

std::ostream &operator<<(std::ostream &o,const AuftragBase &a);
class ArgumentList;
ArgumentList &operator<<(ArgumentList &q, const AuftragBase &ab);
//FetchIStream& operator>>(FetchIStream& is,AuftragBase::mengen_t &menge);

// f�r map<AuftragBase,...>
bool operator<(const AuftragBase &a, const AuftragBase &b);

#endif
