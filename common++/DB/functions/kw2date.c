/* $Id: kw2date.c,v 1.1 2004/10/13 10:02:50 jacek Exp $ */

/*
 * to_date('4-2003','IW-YYYY') works the same way and is standard
 *
 */

#include "postgres.h"
#include <sys/time.h>
#include <time.h>
#include "fmgr.h"

#define seconds_per_day (60*60*24)
#define seconds_per_week (7*seconds_per_day)

PG_FUNCTION_INFO_V1(kw2date);
Datum kw2date(PG_FUNCTION_ARGS)
{  text *ret=0;
   int len;
   int yyyyww=0;
   struct tm tm,*tm2;
   char buf[20];
   time_t t;

   if (PG_ARGISNULL(0)) PG_RETURN_NULL();
   yyyyww=PG_GETARG_INT32(0);
   
   memset(&tm,0,sizeof tm);
   tm.tm_mday=1;
//   tm.tm_mon=0;
   tm.tm_year=(yyyyww/100)-1900;
   tm.tm_hour=12;
   tm.tm_isdst=-1;
   t=mktime(&tm);
   if (!tm.tm_wday) tm.tm_wday=7;
   if (tm.tm_wday<5) // <Friday  1.1. is KW1
      t=t+(5-tm.tm_wday)*seconds_per_day;
   else // >=Friday 1.1. is KW52
      t=t+(12-tm.tm_wday)*seconds_per_day;
   t=t+((yyyyww%100)-1)*seconds_per_week;
   tm2=localtime(&t);
   snprintf(buf,sizeof buf,"%d-%d-%d",tm2->tm_year+1900,
	tm2->tm_mon+1,tm2->tm_mday);

 len = VARHDRSZ + strlen(buf);
 ret = (text*)palloc(len);
 if (ret == NULL)
 {  elog(ERROR, "unable to allocate memory in aufstat_2lab()");
    PG_RETURN_NULL();
 }
 VARATT_SIZEP(ret) = len;
 memcpy(VARDATA(ret), buf,len-VARHDRSZ);
 PG_RETURN_TEXT_P(ret);
}
