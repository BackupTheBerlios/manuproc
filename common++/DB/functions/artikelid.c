// $Id: artikelid.c,v 1.4 2005/07/27 08:57:49 christof Exp $

#include "postgres.h"
#include "executor/spi.h"	/* this is what you need to work with SPI */
#include "fmgr.h"
#include <catalog/pg_type.h> /* for various oids */

#define MAXSPALTEN 40

#define DEBUG_IT(x...) 
// elog(NOTICE,x)

#ifdef DEBUG
#define DEBUG1 DEBUG
#endif

// sep (if set) overrides the separator if space
static long try_to_find(char *buf,int artikeltyp,int kunde,int signifikanz,char sep)
{  const char *komponenten[MAXSPALTEN];
   int qresult;
   char query[1024];
   char query2[1024];
   static char spaltennamen[MAXSPALTEN][40];
   static char separatoren[MAXSPALTEN];
//   static int spaltennummer[MAXSPALTEN];
   int spalten;
   int len,len2,i;
   char tableoid[256];

   snprintf(query, sizeof query, "select oid from pg_class where relname='artbez_%d_%d'",
   		artikeltyp,kunde);
   qresult = SPI_exec(query, 1);
   if (qresult != SPI_OK_SELECT || !SPI_processed) 
   {  DEBUG_IT("SPI_exec returned %d, %d lines", qresult,SPI_processed);
      return 0;
   }
   strncpy(tableoid,SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1),sizeof tableoid);
                        
   snprintf(query, sizeof query, 
      		"select separator, coalesce(spaltenname, bezkomptext), bezkomptype "
      		"from extbezschema "
      		"where (artikeltyp,extartbezid,signifikanz)=(%d,%d,%d) "
      		"order by bezkomptype",
      	    artikeltyp,kunde,signifikanz);
   DEBUG_IT(query);
   qresult = SPI_exec(query, MAXSPALTEN);
      if (qresult != SPI_OK_SELECT)
      {  elog(ERROR, "SPI_exec returned %d", qresult);
         return 0;
      }
      spalten=SPI_processed;
      DEBUG_IT("spalten %d",spalten);
      for (i=0;i<spalten && i<MAXSPALTEN;++i)
      {  char *res,*res1;
         res=SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,1);
         res1=SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,2);
         DEBUG_IT("%d: %s %s %s",i,res,res1,
         	SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,3));
         // try to replace space by different separator
         separatoren[i]=res?((*res!=' ')?*res:(sep?sep:' ')):0;
         if (!res1) spaltennamen[i][0]=0;
         else strncpy(spaltennamen[i],res1,sizeof spaltennamen[0]);
      }
      // zerlegen ... this code relies on separator being char not string!
      for (i=0;i<spalten && i<MAXSPALTEN;++i)
      {  if (!separatoren[i])
         {  if (i+1<spalten) 
            {  DEBUG_IT("%d: kein separator mittendrin",i);
               return 0;
            }
            komponenten[i]=buf;
         }
         else
         {  char *next=strchr(buf,separatoren[i]);
            if (!next) 
            {  DEBUG_IT("%d: separator %c nicht in %s",i,separatoren[i],buf);
               return 0;
            }
            komponenten[i]=buf;
            *next=0;
            buf=next+1;
         }
      }
      for (i=0;i<spalten && i<MAXSPALTEN;++i)
      {  if (strspn(komponenten[i],"0123456789")!=strlen(komponenten[i]))
         {  int type;
            snprintf(query,sizeof query,"select atttypid from pg_attribute "
         		"where attrelid=%s and attname='%s'",
         		tableoid,spaltennamen[i]);
            DEBUG_IT(query);
            qresult = SPI_exec(query, 1);
            if (qresult != SPI_OK_SELECT || !SPI_processed) 
            {  DEBUG_IT("SPI_exec returned %d, %d lines", qresult,SPI_processed);
               return 0;
	    }
	    type=atoi(SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1));
            switch(type)
            {  case INT8OID: case INT2OID: case INT4OID:
               case NUMERICOID: // FIXME: accept '.'
               case FLOAT4OID: case FLOAT8OID: // FIXME: accept '.e' etc.
                  DEBUG_IT("%s not suitable as %s (type %d)",
                  	komponenten[i],spaltennamen[i],type);
                  return 0;
               default:
                  elog(NOTICE,"Is %s suitable for artbez_%d_%d.%s? (unknown type %d)",
                  	komponenten[i],artikeltyp,kunde,spaltennamen[i],type);
                  return 0;
               
               case VARCHAROID: case BPCHAROID: case TEXTOID:
                  break;
            }
         }
      }
      // anfragen
      len=snprintf(query,sizeof query,"select id from artbez_%d_%d where (%s",
      		artikeltyp,kunde,spaltennamen[0]);
      len2=snprintf(query2,sizeof query2,")=('%s'",komponenten[0]);
      for (i=1;i<spalten && i<MAXSPALTEN;++i)
      {  len+=snprintf(query+len,sizeof(query)-len,",%s",spaltennamen[i]);
         len2+=snprintf(query2+len2,sizeof(query2)-len2,",'%s'",komponenten[i]);
      }
      len+=snprintf(query+len,sizeof(query)-len,"%s)",query2);

      DEBUG_IT(query);
      qresult=SPI_exec(query,1);
      if (qresult != SPI_OK_SELECT || SPI_processed!=1) return 0;
      return atoi(SPI_getvalue(SPI_tuptable->vals[0],SPI_tuptable->tupdesc,1));
}

PG_FUNCTION_INFO_V1(artikelid);
Datum artikelid(PG_FUNCTION_ARGS)
{  text *t=0;
   int len;
   int qresult;
   int spalten;
   char buf[1024];
   int i;
   int artikeltyp[MAXSPALTEN],kunde[MAXSPALTEN],signifikanz[MAXSPALTEN];
   
   if (PG_ARGISNULL(0)) PG_RETURN_NULL();
   t=PG_GETARG_TEXT_P(0);
   len=VARSIZE(t)-VARHDRSZ;
   if (len>=sizeof buf) len=sizeof(buf)-1;
   
   if ((qresult = SPI_connect()) < 0)
   {  elog(ERROR,"%s: SPI_connect returned %d", __FUNCTION__,qresult);
   }
   qresult = SPI_exec("select distinct artikeltyp,extartbezid,signifikanz "
   	"from extbezschema where signifikanz>0",MAXSPALTEN);
   if (qresult != SPI_OK_SELECT)
   {  elog(ERROR, "SPI_exec returned %d", qresult);
   }
   spalten=SPI_processed;
   for (i=0;i<spalten && i<MAXSPALTEN;++i)
   {  artikeltyp[i]=atoi(SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,1));
      kunde[i]=atoi(SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,2));
      signifikanz[i]=atoi(SPI_getvalue(SPI_tuptable->vals[i],SPI_tuptable->tupdesc,3));
   }
   for (i=0;i<spalten && i<MAXSPALTEN;++i)
   {  char buf[1024];
      int id;
      memcpy(buf,VARDATA(t),len);
      buf[len]=0;
      id=try_to_find(buf,artikeltyp[i],kunde[i],signifikanz[i],0);
      if (!id)
        id=try_to_find(buf,artikeltyp[i],kunde[i],signifikanz[i],'_');
      if (!id)
        id=try_to_find(buf,artikeltyp[i],kunde[i],signifikanz[i],'|');
      if (id) 
      {  SPI_finish();
         PG_RETURN_INT32(id);
      }
   }
   SPI_finish();
   PG_RETURN_NULL();
}
