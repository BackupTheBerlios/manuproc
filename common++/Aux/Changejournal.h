// $Id: Changejournal.h,v 1.14 2006/06/20 13:35:27 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
 *  Copyright (C) 2006 Christof Petig
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

#include<Misc/Datum.h>
#include<Misc/SQLerror.h>
#include<Artikel/ArtikelBase.h>
#include<Auftrag/AufEintragBase.h>
#include<Instanzen/ppsInstanz.h>


class pps_ChJournalEntryBase
{
protected:
 cH_ppsInstanz instanz; 	//Instanz
 int chid;	//Ändrung
  
public:
 typedef int CID;
 
 pps_ChJournalEntryBase(ppsInstanz::ID i,int c)
 	: instanz(i), chid(c) {}
 pps_ChJournalEntryBase(cH_ppsInstanz const& i,CID c)
 	: instanz(i), chid(c) {}
 	
 CID CId() { return chid; } 
 ppsInstanz::ID IId() { return instanz->Id(); }

 pps_ChJournalEntryBase() : instanz(ppsInstanzID::None), chid(0) {}

};

class pps_ChJournalEntry : public pps_ChJournalEntryBase
{
public:
 typedef enum {NONE=-1,REQUEST=0,CONFIRM=1,CONFLIKT=2} ChJStatVal;
 typedef enum {CH_NONE=0,CH_MENGE='M',CH_LIEFDAT='L'} ChJType;

private:
 ManuProC::Datum datum;
 ChJStatVal status;
 ArtikelBase artikel;
 AufEintragBase auftragentry;
 AuftragBase::mengen_t newval;
 ManuProC::Datum newdate;
 AuftragBase::mengen_t delta; // Menge oder Tage
 ChJType type;   

public:
 static const __deprecated pps_ChJournalEntry newChange(cH_ppsInstanz inst,
 		const AufEintragBase &aufe, const ArtikelBase &art,
 		AuftragBase::mengen_t _newval, AuftragBase::mengen_t _delta, ChJType _type,
 		CID change=0) throw(SQLerror)
 { assert(inst==aufe.Instanz());
   return newChange(aufe,art,_newval,_delta,_type,change);
 }
 static const __deprecated pps_ChJournalEntry newChange(cH_ppsInstanz inst,
 		const AufEintragBase &aufe, const ArtikelBase &art,
 		const ManuProC::Datum &_newdat, int _deltatage, ChJType _type,
 		CID change=0) throw(SQLerror)
 { assert(inst==aufe.Instanz());
   return newChange(aufe,art,_newdat,_deltatage,_type,change);
 }

 static const pps_ChJournalEntry newChange(const AufEintragBase &aufe, 
                const ArtikelBase &art, AuftragBase::mengen_t _newval,
                AuftragBase::mengen_t _delta, ChJType _type=CH_MENGE, 
                CID change=0) throw(SQLerror);
 static const pps_ChJournalEntry newChange(const AufEintragBase &aufe, 
                const ArtikelBase &art, const ManuProC::Datum &_newdat, 
                int _deltatage, ChJType _type=CH_LIEFDAT,
 		CID change=0) throw(SQLerror);
 		
 pps_ChJournalEntry(cH_ppsInstanz instid, CID change) throw(SQLerror);
 pps_ChJournalEntry() : 
   pps_ChJournalEntryBase(ppsInstanzID::None,ManuProcEntity<>::none_id),
   status(NONE),type(CH_NONE) {}

};
