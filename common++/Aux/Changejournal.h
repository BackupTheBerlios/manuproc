// $Id: Changejournal.h,v 1.6 2002/05/06 13:41:22 christof Exp $
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

#include<Aux/Datum.h>
#include<Aux/SQLerror.h>
#include<Artikel/ArtikelBase.h>
#include<Auftrag/AufEintragBase.h>
#include<Aux/ppsInstanz.h>


class pps_ChJournalEntryBase
{
protected:
 cH_ppsInstanz instanz; 	//Instanz
 int chid;	//Ändrung
  
public:
 typedef int CID;
 
 pps_ChJournalEntryBase(ppsInstanz::ID i,int c)
 	: instanz(i), chid(c) {}
 	
 CID CId() { return chid; } 
 ppsInstanz::ID IId() { return instanz->Id(); }

 pps_ChJournalEntryBase() : instanz(ppsInstanz::default_id), chid(0) {}
};

class pps_ChJournalEntry : public pps_ChJournalEntryBase
{
public:
 typedef enum {REQUEST=0,CONFIRM=1,CONFLIKT=2} ChJStatVal;
 typedef enum {CH_MENGE='M',CH_LIEFDAT='L'} ChJType;

private:
 Petig::Datum datum;
 ChJStatVal status;
 ArtikelBase artikel;
 AufEintragBase auftragentry;
 double newval;
 Petig::Datum newdate;
 double delta;
 ChJType type;   

public:
 static const pps_ChJournalEntry newChange(cH_ppsInstanz inst,
 		const AufEintragBase &aufe, const ArtikelBase &art,
 		double _newval, double _delta, ChJType _type,
 		CID change=0) throw(SQLerror);
 	
 static const pps_ChJournalEntry newChange(cH_ppsInstanz inst,
 		const AufEintragBase &aufe, const ArtikelBase &art,
 		const Petig::Datum &_newdat, double _delta, ChJType _type,
 		CID change=0) throw(SQLerror); 	
 		
 pps_ChJournalEntry(cH_ppsInstanz instid, CID change) throw(SQLerror);

};
