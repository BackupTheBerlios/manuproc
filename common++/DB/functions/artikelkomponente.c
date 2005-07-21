// $Id: artikelkomponente.c,v 1.2 2005/07/21 08:48:25 christof Exp $

#include "postgres.h"
#include "executor/spi.h"	/* this is what you need to work with SPI */
#include "fmgr.h"
#if defined(__GNUC__) && __GNUC__>3
#define __FUNC_OLD__
#else
#define __FUNC_OLD__ __FUNCTION__
#endif

#define MAXSPALTEN 20

#define DEBUG_IT(x...) 
//elog(NOTICE,x)

#ifdef DEBUG
#define DEBUG1 DEBUG
#endif

// if it wants NULL this function returns Datum(0) aka PG_RETURN_VOID();
static Datum artikelkomponente5_int(int4 artikelid,int4 artikeltyp,int4 schema,int4 signifikanz,int4 nummer)
{  char query[256];
   int qresult;
   char *res;
   char buf[128]="";
   char *result=buf;
   text *ret;
   int len;
   char spaltennamen[40];

   if ((qresult = SPI_connect()) < 0)
   {  elog(ERROR, __FUNC_OLD__": SPI_connect returned %d", qresult);
      return (Datum) 0;
   }
      snprintf(query, sizeof query, 
      		"select coalesce(spaltenname, bezkomptext) " //", bezkomptype "
      		"from extbezschema "
      		"where (extartbezid,artikeltyp,signifikanz)=(%d,%d,%d) "
      		"order by bezkomptype limit 1 offset %d",
      	    schema,artikeltyp,signifikanz,nummer-1);
      DEBUG_IT(query);
      qresult = SPI_exec(query, MAXSPALTEN);
      if (qresult != SPI_OK_SELECT)
      {  elog(ERROR, "SPI_exec returned %d", qresult);
      }
      if (!SPI_processed) { SPI_finish(); return (Datum) 0; }
      
      DEBUG_IT("spalten %d",SPI_processed);
      res=SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1);
      DEBUG_IT("%s",res);
      strncpy(spaltennamen,res,sizeof spaltennamen);

      snprintf(query,sizeof query,"select %s from artbez_%d_%d where id=%d"
         		,spaltennamen,artikeltyp,schema,artikelid);
      DEBUG_IT(query);
      qresult=SPI_exec(query,1);
      len=0;
      if (qresult != SPI_OK_SELECT || SPI_processed!=1 || 
          !SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1))
         {  SPI_finish(); return (Datum)0; }
      strncpy(buf,SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1),sizeof(buf));
      result=buf;
   SPI_finish();
   len = VARHDRSZ + strlen(result) + 1;
   ret = (text*)palloc(len);
   if (ret == NULL)
      elog(ERROR, "unable to allocate memory in "__FUNC_OLD__);
   VARATT_SIZEP(ret) = len-1;
   strncpy(VARDATA(ret), result, len-VARHDRSZ);
   PG_RETURN_TEXT_P(ret);
}

static int warengruppe(int artikelid, int *schema)
{  char query[256];
   int artikeltyp=-1;
   int qresult;

      if ((qresult = SPI_connect()) < 0)
      {  elog(ERROR, __FUNC_OLD__": SPI_connect returned %d", qresult);
         return -1;
      }
      snprintf(query, sizeof query, "select %s,defaultschema from artikelstamm where id=%d",
      		*schema==1?"coalesce(internwarengruppe,warengruppe)"
      			:"warengruppe",artikelid);
      qresult = SPI_exec(query, 1);
      if (qresult != SPI_OK_SELECT)
         elog(ERROR, __FUNC_OLD__": SPI_exec returned %d", qresult);
      if (SPI_processed<1) 
      {  elog(NOTICE, "Artikel %d nicht in artikelstamm",artikelid);
      }
      else
      {  const char *res2=SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1);
         if (!res2) elog(NOTICE, "artikeltyp(%d)=NULL",artikelid);
         else {  artikeltyp=atoi(res2); /*elog(DEBUG1, "artikeltyp=%s",res2);*/ }
         res2=SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,2);
         if (!res2) elog(NOTICE, "defaultschema(%d)=NULL",artikelid);
         else {  *schema=atoi(res2); /*elog(DEBUG1, "artikeltyp=%s",res2);*/ }
      }
      SPI_finish();
   return artikeltyp;
}

PG_FUNCTION_INFO_V1(artikelkomponente);

Datum artikelkomponente(PG_FUNCTION_ARGS)
// text * () int4 artikelid,int4 schema,int4 signifikanz,int4 index)
{  int artikelid,schema,signifikanz,index,defschema;
   Datum res=(Datum)0;
   int artikeltyp=-1;
   
   if (PG_ARGISNULL(0) || PG_ARGISNULL(1) ||PG_ARGISNULL(2) ||PG_ARGISNULL(3)) 
      PG_RETURN_NULL();
   artikelid=PG_GETARG_INT32(0);
   schema=PG_GETARG_INT32(1);
   signifikanz=PG_GETARG_INT32(2);
   index=PG_GETARG_INT32(3);
   if (!artikelid) PG_RETURN_NULL();

   defschema=schema;
   artikeltyp=warengruppe(artikelid,&schema);
   if (artikeltyp==-1) PG_RETURN_NULL();
   
   res=artikelkomponente5_int(artikelid,artikeltyp,schema,signifikanz,index);
   if (res == (Datum) 0) 
      res=artikelkomponente5_int(artikelid,artikeltyp,defschema,signifikanz,index);

   if (res == (Datum) 0) PG_RETURN_NULL();
   return res;
}

