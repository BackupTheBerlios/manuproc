/* Processed by ecpg (2.9.0) */
/* These three include files are added by the preprocessor */
#include <ecpgtype.h>
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
#line 1 "AufEintragBase.pgcc"
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

#include <Auftrag/AufEintragBase.h>
#include "AufEintrag.h"
#include <Aux/Transaction.h>
#include <Auftrag/auftrag_status.h>
#include <Auftrag/AuftragsBaum.h> 
#include <Aux/Changejournal.h>


#line 1 "/usr/include/postgresql/sqlca.h"
#ifndef POSTGRES_SQLCA_H
#define POSTGRES_SQLCA_H

#ifndef DLLIMPORT
#ifdef __CYGWIN__
#define DLLIMPORT __declspec (dllimport)
#else
#define DLLIMPORT
#endif   /* __CYGWIN__ */
#endif   /* DLLIMPORT */

#define SQLERRMC_LEN	70

#ifdef __cplusplus
extern		"C"
{
#endif

struct sqlca
{
	char		sqlcaid[8];
	long		sqlabc;
	long		sqlcode;
	struct
	{
		int			sqlerrml;
		char		sqlerrmc[SQLERRMC_LEN];
	}			sqlerrm;
	char		sqlerrp[8];
	long		sqlerrd[6];
	/* Element 0: empty						*/
	/* 1: OID of processed tuple if applicable			*/
	/* 2: number of rows processed				*/
	/* after an INSERT, UPDATE or				*/
	/* DELETE statement					*/
	/* 3: empty						*/
	/* 4: empty						*/
	/* 5: empty						*/
	char		sqlwarn[8];
	/* Element 0: set to 'W' if at least one other is 'W'	*/
	/* 1: if 'W' at least one character string		*/
	/* value was truncated when it was			*/
	/* stored into a host variable.				*/

	/*
	 * 2: if 'W' a (hopefully) non-fatal notice occured
	 */	/* 3: empty */
	/* 4: empty						*/
	/* 5: empty						*/
	/* 6: empty						*/
	/* 7: empty						*/

	char		sqlext[8];
};

extern DLLIMPORT struct sqlca sqlca;


#ifdef __cplusplus
}
#endif

#endif

#line 27 "AufEintragBase.pgcc"


void AufEintragBase::setVerarbeitung(const cH_Prozess p) const throw(SQLerror)
{
 assert (Instanz()==ppsInstanz::Kundenauftraege);
 /* exec sql begin declare section */
   
   
   
   
 
#line 33 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 34 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 35 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 36 "AufEintragBase.pgcc"
   int  PROZESSID   ;
/* exec sql end declare section */
#line 37 "AufEintragBase.pgcc"

 PROZESSID=p->getProzessID();
 
 Transaction tr;
 
 { ECPGdo(__LINE__, NULL, "update auftrag_prozess set prozessid  = ? , datum  = now ()  where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )", 
	ECPGt_int,&(PROZESSID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 44 "AufEintragBase.pgcc"

 SQLerror::test("Prozessaktualisierung (1) ",100);
 
 if((sqlca.sqlcode==100) || sqlca.sqlerrd[2]==0)
   {
    { ECPGdo(__LINE__, NULL, "insert into auftrag_prozess ( instanz  , auftragid  , zeilennr  , prozessid  , datum  ) values ( ? , ? , ? , ? , now () )", 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(PROZESSID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 52 "AufEintragBase.pgcc"

    SQLerror::test("Prozessaktualisierung (2)");
   }
 tr.commit();
}


bool AufEintragBase::deleteAuftragEntry() const throw(SQLerror)
{
  assert (InstanzID()!=ppsInstanz::INST_KNDAUF) ;

   AuftragsBaum AB(*this,true);
 if(!AB.empty()) return false;

 /* exec sql begin declare section */
  
  
  
 
#line 70 "AufEintragBase.pgcc"
   int  AUFTRAGID  = auftragid ;
 
#line 71 "AufEintragBase.pgcc"
   int  ZEILENNR  = zeilennr ;
 
#line 72 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
/* exec sql end declare section */
#line 73 "AufEintragBase.pgcc"

  { ECPGdo(__LINE__, NULL, "delete from auftragsentryzuordnung where ( neuinstanz  , neuauftragid  , neuzeilennr  ) = ( ? , ? , ? )", 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 77 "AufEintragBase.pgcc"

 SQLerror::test(__FILELINE__+(std::string)" DELETE AUFTRAG 1");

  { ECPGdo(__LINE__, NULL, "delete from auftragentry where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )", 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 82 "AufEintragBase.pgcc"

 SQLerror::test(__FILELINE__+(std::string)" DELETE AUFTRAG 2");
 return true;
}

AuftragBase::mengen_t AufEintragBase::abschreiben(mengen_t menge) const throw(SQLerror)
{
 /* exec sql begin declare section */
  
  
  
   
  
  
 
#line 91 "AufEintragBase.pgcc"
   int  AUFID  = Id () ;
 
#line 92 "AufEintragBase.pgcc"
   int  AUFZNR  = ZNr () ;
 
#line 93 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 94 "AufEintragBase.pgcc"
   int  STATUS   ;
 
#line 95 "AufEintragBase.pgcc"
   int  BESTELLT   ;
 
#line 96 "AufEintragBase.pgcc"
   int  GELIEFERT   ;
/* exec sql end declare section */
#line 97 "AufEintragBase.pgcc"

 
 Transaction tr;
 { ECPGdo(__LINE__, NULL, "lock table auftragentry in exclusive mode", ECPGt_EOIT, ECPGt_EORT);}
#line 100 "AufEintragBase.pgcc"

 { ECPGdo(__LINE__, NULL, "select  bestellt  , geliefert  , status   from auftragentry where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )  ", 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFZNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, 
	ECPGt_int,&(BESTELLT),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(GELIEFERT),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(STATUS),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 104 "AufEintragBase.pgcc"

 SQLerror::test(__FILELINE__);
 AufStatVal oldstatus=AufStatVal(STATUS);

 if(menge>=0 && STATUS!=(AufStatVal)OPEN) 
    throw(SQLerror(__FILELINE__,-1,"Auftragszeile ist bereits geschlossen"));
 if(menge<0 && STATUS==(AufStatVal)UNCOMMITED) 
    throw(SQLerror(__FILELINE__,-1,"Auftragszeile ist nicht bestätigt"));
 GELIEFERT+=menge;
 if(GELIEFERT>=BESTELLT) STATUS=(AufStatVal)CLOSED;
 else if(menge<0 && GELIEFERT<BESTELLT) STATUS=(AufStatVal)OPEN;

 if(STATUS!=oldstatus)
   AufEintrag(*this).setStatus(AufStatVal(STATUS),true);

 { ECPGdo(__LINE__, NULL, "update auftragentry set geliefert  = ? , status  = ?  where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )", 
	ECPGt_int,&(GELIEFERT),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(STATUS),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFZNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 120 "AufEintragBase.pgcc"

 SQLerror::test(__FILELINE__);
 tr.commit();


 return GELIEFERT;
}



void AufEintragBase::setLetztePlanungFuer(cH_ppsInstanz planinstanz) const throw(SQLerror)
{
 assert (Instanz()==ppsInstanz::Kundenauftraege);

 if(planinstanz->LagerInstanz()) return;

 /* exec sql begin declare section */
   
   
   
   
   
 
#line 139 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 140 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 141 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 142 "AufEintragBase.pgcc"
   int  PLANUNG  = planinstanz -> Id () ;
 
#line 143 "AufEintragBase.pgcc"
   int  PROZESSID  = planinstanz -> get_Prozess () -> Id () ;
/* exec sql end declare section */
#line 144 "AufEintragBase.pgcc"

 Transaction tr;
 
 { ECPGdo(__LINE__, NULL, "update auftrag_prozess set letztePlanInstanz  = ? , maxPlanInstanz  = null , datum  = now ()  where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )", 
	ECPGt_int,&(PLANUNG),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 149 "AufEintragBase.pgcc"

 SQLerror::test("Prozessaktualisierung",100);

 if((sqlca.sqlcode==100) || sqlca.sqlerrd[2]==0)
   {
    { ECPGdo(__LINE__, NULL, "insert into auftrag_prozess ( instanz  , auftragid  , zeilennr  , prozessid  , letztePlanInstanz  ) values ( ? , ? , ? , ? , ? )", 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(PROZESSID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(PLANUNG),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 157 "AufEintragBase.pgcc"

    SQLerror::test("Prozessaktualisierung");
   }
 tr.commit();
}

void AufEintragBase::setMaxPlanInstanz(int anzahl_maxplaninstanzen) const throw(SQLerror)
{
 /* exec sql begin declare section */
   
   
   
   
 
#line 166 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 167 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 168 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 169 "AufEintragBase.pgcc"
   int  PLANUNG  = anzahl_maxplaninstanzen ;
/* exec sql end declare section */
#line 170 "AufEintragBase.pgcc"


 Transaction tr;
 
 { ECPGdo(__LINE__, NULL, "update auftrag_prozess set maxPlanInstanz  = ? , datum  = now ()  where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )", 
	ECPGt_int,&(PLANUNG),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 176 "AufEintragBase.pgcc"

 SQLerror::test("Prozessaktualisierung",100);
 
 tr.commit();
}


void AufEintragBase::calculateProzessInstanz()
{
  assert (Instanz()==ppsInstanz::Kundenauftraege);
  AuftragsBaum AB(*this,true);
  int anz=0;
  for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
     if(i->AEB2.Id()==0) continue;      if(AufEintrag(i->AEB2).getStueck() == i->menge)
       ++anz;
   }
  setMaxPlanInstanz(anz);
}   



void AufEintragBase::updateStkDiff(mengen_t menge) const throw(SQLerror)
{
 /* exec sql begin declare section */
    
  
      
    
 
#line 207 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 208 "AufEintragBase.pgcc"
   int  INSTANZ  = instanz -> Id () ;
 
#line 209 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 210 "AufEintragBase.pgcc"
   double  MENGE  = menge ;
/* exec sql end declare section */
#line 211 "AufEintragBase.pgcc"


 Transaction tr;

 { ECPGdo(__LINE__, NULL, "update auftragentry set bestellt  = bestellt  + ?  where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )", 
	ECPGt_double,&(MENGE),1L,1L,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 216 "AufEintragBase.pgcc"

 SQLerror::test("updateStkDiff: update stk in auftragentry");

#warning updateStk im Moment FALSCHE Einträge in ProdChageJournal!!! 
  {
     pps_ChJournalEntry::newChange(
         instanz, *this, 0,0,MENGE,pps_ChJournalEntry::CH_MENGE);
  }
 tr.commit(); 
}
