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
#include<Aux/ppsInstanz.h>
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
//        typedef Preis::rabatt_t rabatt_t;
	typedef int rabatt_t; // vorläufig ...

 protected:
        cH_ppsInstanz instanz; 	
 	int auftragid;	
 public:
	AuftragBase() 
	        : instanz(ppsInstanz::INST_NONE), auftragid(0) {}
	AuftragBase(cH_ppsInstanz _instanz, int aufid=0) throw() 
	        :instanz(_instanz), auftragid(aufid) 
	        {}
	AuftragBase(cH_ppsInstanz _instanz, int aufid,int kid) throw(SQLerror) ;
        
	int Id() const {return auftragid;}
   void set_Id(int i) {auftragid = i;}
   ppsInstanz::ID InstanzID() const {return instanz->Id(); }
   cH_ppsInstanz Instanz() const {return instanz; }
	bool valid() const { return auftragid!=0; }
   void setStatusAuftragBase(AufStatVal st) const throw(SQLerror);

   void create_if_not_exists(AufStatVal status,Kunde::ID kunde=Kunde::default_id) const;
   int insertNewEntry(const mengen_t bestellt, 
                const Petig::Datum lieferdatum, const ArtikelBase& artikel,
                const AufStatVal status,const bool setInstanzAuftraege,
                const Preis& preis=Preis(),const fixedpoint<2> rabatt=0
                ) const throw(SQLerror);
   void InstanzAuftraegeAnlegen(const ArtikelBase& art,const int altZnr,
               const Petig::Datum& lieferdatum, const AufStatVal status, 
                const mengen_t menge) const; 
   bool existEntry(const ArtikelBase& artid,
                        const Petig::Datum& lieferdatum,
                        int& znr,int &newznr, mengen_t& menge, const AufStatVal status
                        ) const throw(SQLerror);
   void tryUpdateEntry(mengen_t bestellt, 
               const Petig::Datum lieferdatum, const ArtikelBase& artikel,
                AufStatVal status,const AufEintragBase& altAEB) const throw(SQLerror);

	// wandelt enum in std::string um
	static const std::string getStatusStr(AufStatVal a);
};

#endif
