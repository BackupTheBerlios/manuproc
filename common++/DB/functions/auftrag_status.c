#include <postgres.h>
#include "fmgr.h"
#include "auftrag_status.h"

PG_FUNCTION_INFO_V1(aufstat_2lab);
Datum aufstat_2lab(PG_FUNCTION_ARGS)
{  text *ret=0;
   const char *t;
   int len;
   
   if (PG_ARGISNULL(0)) PG_RETURN_NULL();
   
   switch (PG_GETARG_INT32(0))
   {
    case UNCOMMITED : 
    	t="unbestätigt";
    break;
    
    case OPEN :
    	t="offen";    
    break;
    
    case CLOSED :
    	t="fertig";    
    break;
    
    case STORNO :
    	t="storno";    
    break;
    
    default : PG_RETURN_NULL();
    }

 len = VARHDRSZ + strlen(t);
 ret = (text*)palloc(len);
 if (ret == NULL)
 { elog(ERROR, "unable to allocate memory in aufstat_2lab()");
   PG_RETURN_NULL();
 }
 VARATT_SIZEP(ret) = len;
 memcpy(VARDATA(ret), t,len-VARHDRSZ);
 PG_RETURN_TEXT_P(ret);
}


PG_FUNCTION_INFO_V1(aufstat_2id);
Datum aufstat_2id(PG_FUNCTION_ARGS)
{  text *t=0;
   int len;
   
   if (PG_ARGISNULL(0)) PG_RETURN_NULL();
   t=PG_GETARG_TEXT_P(0);
   len=VARSIZE(t)-VARHDRSZ;
 
 if(len==11 && memcmp(VARDATA(t),"unbestätigt",len)==0)
   PG_RETURN_INT32(UNCOMMITED);
 
 if(len==5 && memcmp(VARDATA(t),"offen",len)==0) 
   PG_RETURN_INT32(OPEN);
    
 if(len==6 && memcmp(VARDATA(t),"fertig",len)==0) 
   PG_RETURN_INT32(CLOSED);
    
 if(len==6 && memcmp(VARDATA(t),"storno",len)==0)     
   PG_RETURN_INT32(STORNO);

 PG_RETURN_NULL();    
}
