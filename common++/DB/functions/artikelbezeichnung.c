// $Id: artikelbezeichnung.c,v 1.3 2005/10/12 08:59:39 christof Exp $

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
static Datum artikelbezeichnung4_int(int4 artikelid,int4 artikeltyp,int4 schema,int4 signifikanz)
{  char query[256];
   char sign[40];
   int qresult;
   char *res;
   char buf[128]="";
   char *result=buf;
   text *ret;
   int len;
   static int spalten;
   static int oldschema=-1,oldsign=-1,oldtyp=-1;
   static char spaltennamen[MAXSPALTEN][40];
   static char separatoren[MAXSPALTEN];
   static int spaltennummer[MAXSPALTEN];
   int i;

   if ((qresult = SPI_connect()) < 0)
   {  elog(ERROR, __FUNC_OLD__": SPI_connect returned %d", qresult);
      return (Datum) 0;
   }
   if (signifikanz!=oldsign || oldschema!=schema || oldtyp!=artikeltyp)
   {  oldsign=signifikanz;
      oldschema=schema;
      oldtyp=artikeltyp;
      
      if (!signifikanz) *sign=0;
      else snprintf(sign,sizeof sign, "and signifikanz=%d", signifikanz);
      // noch 
      snprintf(query, sizeof query, 
      		"select separator, coalesce(spaltenname, bezkomptext), bezkomptype "
      		"from extbezschema "
      		"where extartbezid=%d %s and artikeltyp=%d "
      		"order by bezkomptype",
      	    schema,sign,artikeltyp);
      DEBUG_IT(query);
      qresult = SPI_exec(query, MAXSPALTEN);
      if (qresult != SPI_OK_SELECT)
      {  elog(ERROR, "SPI_exec returned %d", qresult);
      }
      spalten=SPI_processed;
      DEBUG_IT("spalten %d",spalten);
      for (i=0;i<spalten;++i)
      {  char *res1;
         res=SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,1);
         res1=SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,2);
         DEBUG_IT("%d: %s %s",i,res,res1);
         separatoren[i]=res?*res:0;
         if (!res1) spaltennamen[i][0]=0;
         else strncpy(spaltennamen[i],res1,sizeof spaltennamen[0]);
         res=SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,3);
         if (!res) spaltennummer[i]=0;
         else spaltennummer[i]=atoi(res);
         DEBUG_IT("%d",spaltennummer[i]);
      }
   }
   if (!spalten) { SPI_finish(); return (Datum) 0; }

         len=snprintf(query,sizeof query,"select %s",spaltennamen[0]);
         for (i=1;i<spalten;++i)
            len+=snprintf(query+len,sizeof(query)-len,",%s",spaltennamen[i]);
         len+=snprintf(query+len,sizeof(query)-len," from artbez_%d_%d where id=%d"
         		,artikeltyp,schema,artikelid);
         DEBUG_IT(query);
         qresult=SPI_exec(query,1);
         len=0;
         if (qresult != SPI_OK_SELECT || SPI_processed!=1)
         {  SPI_finish(); return (Datum)0; }
         else for (i=0;i<spalten;++i)
         {  res=SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,i+1);
            if (!res) res="";
            len+=snprintf(buf+len,sizeof(buf)-len,separatoren[i]?"%s%c":"%s",
            		res,separatoren[i]);
         }
         DEBUG_IT( "%d %d",len,(int)(res));
   SPI_finish();
   len = VARHDRSZ + strlen(result) + 1;
   ret = (text*)palloc(len);
   if (ret == NULL)
      elog(ERROR, "unable to allocate memory in "__FUNC_OLD__);
   VARATT_SIZEP(ret) = len-1;
   strncpy(VARDATA(ret), result, len-VARHDRSZ);
   PG_RETURN_TEXT_P(ret);
}

static int warengruppe(int artikelid, int schema)
{  char query[256];
   int artikeltyp=-1;
   int qresult;

      if ((qresult = SPI_connect()) < 0)
      {  elog(ERROR, __FUNC_OLD__": SPI_connect returned %d", qresult);
         return -1;
      }
      snprintf(query, sizeof query, "select %s from artikelstamm where id=%d",
      		schema==1?"coalesce(internwarengruppe,warengruppe)"
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
      }
      SPI_finish();
   return artikeltyp;
}

PG_FUNCTION_INFO_V1(artikelbezeichnung4);

Datum artikelbezeichnung4(PG_FUNCTION_ARGS)
// text * () int4 artikelid,int4 artikeltyp,int4 schema,int4 signifikanz)
{  int artikelid,artikeltyp,schema,signifikanz;
   Datum res=(Datum)0;
   
   if (PG_ARGISNULL(0) || PG_ARGISNULL(1) ||PG_ARGISNULL(2) ||PG_ARGISNULL(3))
      PG_RETURN_NULL();
   artikelid=PG_GETARG_INT32(0);
   artikeltyp=PG_GETARG_INT32(1);
   schema=PG_GETARG_INT32(2);
   signifikanz=PG_GETARG_INT32(3);
   if (!artikelid) PG_RETURN_NULL();

   res=artikelbezeichnung4_int(artikelid,artikeltyp,schema,signifikanz);
   if (res == (Datum) 0) PG_RETURN_NULL();
   return res;
}

// durch Bin�zugriffe kann hier noch Geschwindigkeit herausgeholt werden
PG_FUNCTION_INFO_V1(artikelbezeichnung3);

Datum artikelbezeichnung3(PG_FUNCTION_ARGS)
// text * () int4 artikelid,int4 schema,int4 signifikanz)
{  int artikelid,schema,signifikanz;
   Datum res=(Datum)0;
   int artikeltyp=-1;
   
   if (PG_ARGISNULL(0) || PG_ARGISNULL(1) ||PG_ARGISNULL(2))
      PG_RETURN_NULL();
   artikelid=PG_GETARG_INT32(0);
   schema=PG_GETARG_INT32(1);
   signifikanz=PG_GETARG_INT32(2);
   if (!artikelid) PG_RETURN_NULL();

   artikeltyp=warengruppe(artikelid,schema);
   if (artikeltyp==-1) PG_RETURN_NULL();
   
   res=artikelbezeichnung4_int(artikelid,artikeltyp,schema,signifikanz);
   if (res == (Datum) 0) PG_RETURN_NULL();
   return res;
}

// durch Bin�zugriffe kann hier noch Geschwindigkeit herausgeholt werden
//
// diese Funktion gibt bevorzugt die interne Bezeichnung zurck
//
// ID, signigikanz
static Datum artikelbezeichnung2_int(int4 artikelid,int4 signifikanz)
{  int artikeltyp;
   int schema;
   int interntyp;
   Datum result= (Datum) 0;
   
   if (!artikelid) return (Datum)0;
   {  int qresult;
      char query[256];
      if ((qresult = SPI_connect()) < 0)
      {  elog(ERROR, __FUNC_OLD__": SPI_connect returned %d", qresult);
         SPI_finish();   
         return (Datum)0;
      }
      snprintf(query, sizeof query, 
      		"select warengruppe,defaultschema,internwarengruppe"
      		" from artikelstamm "
      		"where id=%d",artikelid);
      qresult = SPI_exec(query, 1);
      if (qresult != SPI_OK_SELECT)
      {  elog(ERROR, __FUNC_OLD__": SPI_exec returned %d", qresult);
         SPI_finish();
         return (Datum)0;
      }
      if (SPI_processed<1) 
      {  elog(NOTICE, "Artikel %d nicht existent",artikelid);
         SPI_finish();
         return (Datum)0;
      }
      else
      {  char *res;
         res=SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1);
         if (!res) {  artikeltyp=0; /*elog(DEBUG1, "artikeltyp=NULL");*/ }
         else {  artikeltyp=atoi(res); /*elog(DEBUG1, "artikeltyp=%s",res);*/ }
         res=SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,2);
         if (!res) {  schema=0; elog(DEBUG1, "schema(%d)=NULL",artikelid); }
         else {  schema=atoi(res); /*elog(DEBUG1, "artikeltyp=%s",res);*/ }
         res=SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,3);
         if (!res) interntyp=artikeltyp;
         else interntyp=atoi(res);
      }
      SPI_finish();
   }
   result=artikelbezeichnung4_int(artikelid,interntyp,1,signifikanz);
   if (result == (Datum)0)
      result=artikelbezeichnung4_int(artikelid,interntyp,schema,signifikanz);
   if (result == (Datum)0)
      result=artikelbezeichnung4_int(artikelid,artikeltyp,schema,signifikanz);
   return result;
}

PG_FUNCTION_INFO_V1(artikelbezeichnung2);

Datum artikelbezeichnung2(PG_FUNCTION_ARGS)
// text * () int4 artikelid, int4 signifikanz)
{  int artikelid,signifikanz;
   Datum res=(Datum)0;
   
   if (PG_ARGISNULL(0) || PG_ARGISNULL(1))
      PG_RETURN_NULL();
   artikelid=PG_GETARG_INT32(0);
   signifikanz=PG_GETARG_INT32(1);
   if (!artikelid) PG_RETURN_NULL();

   res=artikelbezeichnung2_int(artikelid,signifikanz);
   if (res == (Datum) 0) PG_RETURN_NULL();
   return res;
}

PG_FUNCTION_INFO_V1(artikelbezeichnung1);

Datum artikelbezeichnung1(PG_FUNCTION_ARGS)
// text * () int4 artikelid)
{  int artikelid;
   Datum res=(Datum)0;
   
   if (PG_ARGISNULL(0)) PG_RETURN_NULL();
   artikelid=PG_GETARG_INT32(0);
   if (!artikelid) PG_RETURN_NULL();

   res=artikelbezeichnung2_int(artikelid,1);
   if (res == (Datum) 0) PG_RETURN_NULL();
   return res;
}

PG_FUNCTION_INFO_V1(artikelbezeichnung_schema);

Datum artikelbezeichnung_schema(PG_FUNCTION_ARGS)
// text * () int4 artikelid,int4 schema
{  int artikelid,schema;
   Datum res=(Datum)0;
   int artikeltyp=-1;
   
   if (PG_ARGISNULL(0) || PG_ARGISNULL(1))
      PG_RETURN_NULL();
   artikelid=PG_GETARG_INT32(0);
   schema=PG_GETARG_INT32(1);
   if (!artikelid) PG_RETURN_NULL();

   artikeltyp=warengruppe(artikelid,schema);
   if (artikeltyp==-1) PG_RETURN_NULL();
   
   res=artikelbezeichnung4_int(artikelid,artikeltyp,schema,1);
   if (res == (Datum) 0) res=artikelbezeichnung2_int(artikelid,1);
   if (res == (Datum) 0) PG_RETURN_NULL();
   return res;
}
