#include "postgres.h"
#include "fmgr.h"

PG_FUNCTION_INFO_V1(myatoi);
Datum myatoi(PG_FUNCTION_ARGS)
{  text *t=0;
   char buf[30];
   int len;
   
   if (PG_ARGISNULL(0)) PG_RETURN_NULL();
   t=PG_GETARG_TEXT_P(0);
   len=VARSIZE(t)-VARHDRSZ;
   if (len>=sizeof buf) len=sizeof(buf)-1;
   memcpy(buf,VARDATA(t),len);
   buf[len]=0;
   PG_RETURN_INT32(atoi(buf));
}
