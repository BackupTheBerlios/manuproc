#include "postgres.h"
#include "fmgr.h"

PG_FUNCTION_INFO_V1(ean_check);
Datum ean_check(PG_FUNCTION_ARGS)
{  text *t=0;
   char buf[30];
   int len,sum=0,i;

   if (PG_ARGISNULL(0)) PG_RETURN_NULL();
   t=PG_GETARG_TEXT_P(0);
   len=VARSIZE(t)-VARHDRSZ;
   if (len>=sizeof buf) len=sizeof(buf)-1;
   memcpy(buf,VARDATA(t),len);
   buf[len]=0;

   for(i=len-1;i>=0;i--)
    {
      sum+= (buf[i]-'0') * (i & 1 ? 3 : 1);
    }
   if (!(sum %= 10)) sum = 10;

   PG_RETURN_INT32(10-sum);
}

