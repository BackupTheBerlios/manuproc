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


#line 1 "/usr/lib/postgresql/include/sqlca.h"
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
 
 { ECPGdo(__LINE__, NULL, "update auftrag_prozess set prozessid  = ? , datum  = now ()  where auftragid  = ? and zeilennr  = ?", 
	ECPGt_int,&(PROZESSID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 43 "AufEintragBase.pgcc"

 SQLerror::test("Prozessaktualisierung",100);
 
 if((sqlca.sqlcode==100) || sqlca.sqlerrd[2]==0)
   {
    { ECPGdo(__LINE__, NULL, "insert into auftrag_prozess ( auftragid  , instanz  , zeilennr  , prozessid  , datum  ) values ( ? , ? , ? , ? , now () )", 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(PROZESSID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 51 "AufEintragBase.pgcc"

    SQLerror::test("Prozessaktualisierung");
   }
 tr.commit();
}


bool AufEintragBase::deleteAuftragEntry() const throw(SQLerror)
{
  assert (InstanzID()!=ppsInstanz::INST_KNDAUF) ;

   AuftragsBaum AB(*this,true);
 if(!AB.empty()) return false;

 /* exec sql begin declare section */
  
  
  
 
#line 69 "AufEintragBase.pgcc"
   int  AUFTRAGID  = auftragid ;
 
#line 70 "AufEintragBase.pgcc"
   int  ZEILENNR  = zeilennr ;
 
#line 71 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
/* exec sql end declare section */
#line 72 "AufEintragBase.pgcc"

  { ECPGdo(__LINE__, NULL, "delete from auftragsentryzuordnung where neuauftragid  = ? and neuinstanz  = ? and neuzeilennr  = ?", 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 75 "AufEintragBase.pgcc"

 SQLerror::test(__FILELINE__+(std::string)" DELETE AUFTRAG 1");

  { ECPGdo(__LINE__, NULL, "delete from auftragentry where auftragid  = ? and zeilennr  = ? and instanz  = ?", 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 80 "AufEintragBase.pgcc"

 SQLerror::test(__FILELINE__+(std::string)" DELETE AUFTRAG 2");
 return true;
}

AuftragBase::mengen_t AufEintragBase::abschreiben(mengen_t menge) const throw(SQLerror)
{
 /* exec sql begin declare section */
  
  
  
   
  
  
 
#line 89 "AufEintragBase.pgcc"
   int  AUFID  = Id () ;
 
#line 90 "AufEintragBase.pgcc"
   int  AUFZNR  = ZNr () ;
 
#line 91 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 92 "AufEintragBase.pgcc"
   int  STATUS   ;
 
#line 93 "AufEintragBase.pgcc"
   int  BESTELLT   ;
 
#line 94 "AufEintragBase.pgcc"
   int  GELIEFERT   ;
/* exec sql end declare section */
#line 95 "AufEintragBase.pgcc"

 
 Transaction tr;
 { ECPGdo(__LINE__, NULL, "lock table auftragentry in exclusive mode", ECPGt_EOIT, ECPGt_EORT);}
#line 98 "AufEintragBase.pgcc"

 { ECPGdo(__LINE__, NULL, "select  bestellt  , geliefert  , status   from auftragentry where auftragid  = ? and zeilennr  = ? and instanz  = ?  ", 
	ECPGt_int,&(AUFID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFZNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, 
	ECPGt_int,&(BESTELLT),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(GELIEFERT),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(STATUS),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 102 "AufEintragBase.pgcc"

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

 { ECPGdo(__LINE__, NULL, "update auftragentry set geliefert  = ? , status  = ?  where auftragid  = ? and zeilennr  = ? and instanz  = ?", 
	ECPGt_int,&(GELIEFERT),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(STATUS),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFZNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 118 "AufEintragBase.pgcc"
  
 SQLerror::test(__FILELINE__);
 tr.commit();


 return GELIEFERT;
}



void AufEintragBase::setLetztePlanungFuer(ppsInstanz::ID planinstanz) const throw(SQLerror)
{
 assert (Instanz()==ppsInstanz::Kundenauftraege);
 /* exec sql begin declare section */
   
   
   
   
 
#line 134 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 135 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 136 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 137 "AufEintragBase.pgcc"
   int  PLANUNG  = planinstanz ;
/* exec sql end declare section */
#line 138 "AufEintragBase.pgcc"

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
#line 143 "AufEintragBase.pgcc"

 SQLerror::test("Prozessaktualisierung",100);
 
 if((sqlca.sqlcode==100) || sqlca.sqlerrd[2]==0)
   {
    { ECPGdo(__LINE__, NULL, "insert into auftrag_prozess ( instanz  , auftragid  , zeilennr  , letztePlanInstanz  ) values ( ? , ? , ? , ? )", 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(PLANUNG),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 151 "AufEintragBase.pgcc"

    SQLerror::test("Prozessaktualisierung");
   }
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


void AufEintragBase::setMaxPlanInstanz(int maxplaninstanz) const throw(SQLerror)
{
 /* exec sql begin declare section */
   
   
   
   
 
#line 179 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 180 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 181 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 182 "AufEintragBase.pgcc"
   int  PLANUNG  = maxplaninstanz ;
/* exec sql end declare section */
#line 183 "AufEintragBase.pgcc"


 Transaction tr;
 
 { ECPGdo(__LINE__, NULL, "update auftrag_prozess set maxPlanInstanz  = ? , datum  = now ()  where auftragid  = ? and instanz  = ? and zeilennr  = ?", 
	ECPGt_int,&(PLANUNG),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 189 "AufEintragBase.pgcc"

 SQLerror::test("Prozessaktualisierung");
 
 tr.commit();
}


void AufEintragBase::updateStkDiff(mengen_t menge) const throw(SQLerror)
{
 /* exec sql begin declare section */
    
  
      
    
 
#line 199 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 200 "AufEintragBase.pgcc"
   int  INSTANZ  = instanz -> Id () ;
 
#line 201 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 202 "AufEintragBase.pgcc"
   double  MENGE  = menge ;
/* exec sql end declare section */
#line 203 "AufEintragBase.pgcc"


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
#line 209 "AufEintragBase.pgcc"

 SQLerror::test("updateStkDiff: update stk in auftragentry");

#warning updateStk im Moment FALSCHE Einträge in ProdChageJournal!!! 
  {
     pps_ChJournalEntry::newChange(
         instanz, *this,
         0,0,MENGE,pps_ChJournalEntry::CH_MENGE);
  }
 tr.commit(); 
}


void AufEintragBase::setStatus_raw(AufStatVal status)  const throw(SQLerror)
{
 /* exec sql begin declare section */
    
  
      
  
 
#line 228 "AufEintragBase.pgcc"
   int  AUFTRAGID  = Id () ;
 
#line 229 "AufEintragBase.pgcc"
   int  INSTANZ  = InstanzID () ;
 
#line 230 "AufEintragBase.pgcc"
   int  ZEILENNR  = ZNr () ;
 
#line 231 "AufEintragBase.pgcc"
   int  STATUS  = status ;
/* exec sql end declare section */
#line 232 "AufEintragBase.pgcc"


 { ECPGdo(__LINE__, NULL, "update auftragentry set status  = STATUS   where ( instanz  , auftragid  , zeilennr  ) = ( ? , ? , ? )", 
	ECPGt_int,&(INSTANZ),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(AUFTRAGID),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(ZEILENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 235 "AufEintragBase.pgcc"

 SQLerror::test("updateStatus_raw: update status in auftragentry");
}
