/* Processed by ecpg (2.9.0) */
/* These three include files are added by the preprocessor */
#include <ecpgtype.h>
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
#line 1 "Adresse.pgcc"
/* $Id: Adresse.cc,v 1.1 2002/09/26 15:32:50 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "Adresse.h"
#include "Aux/Ausgabe_neu.h"
#include "Aux/string0.h"


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

#line 28 "Adresse.pgcc"


Adresse::Adresse(int kdnr) throw(SQLerror)
{  /* exec sql begin declare section */
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

         
         
    
   
#line 32 "Adresse.pgcc"
   int  KUNDENNR  = kdnr ;
 
#line 33 "Adresse.pgcc"
   char  IDNR [ 21 ]   ;
 
#line 34 "Adresse.pgcc"
   char  FIRMA [ 61 ]   ;
 
#line 35 "Adresse.pgcc"
   char  POSTANWVOR [ 41 ]   ;
 
#line 36 "Adresse.pgcc"
   char  STRASSE [ 36 ]   ;
 
#line 37 "Adresse.pgcc"
   char  POSTANWNACH [ 41 ]   ;
 
#line 38 "Adresse.pgcc"
   char  HSNR [ 8 ]   ;
 
#line 39 "Adresse.pgcc"
   char  _PLZ [ 11 ]   ;
 
#line 40 "Adresse.pgcc"
   char  POSTFACH [ 21 ]   ;
 
#line 41 "Adresse.pgcc"
   char  POSTFPLZ [ 11 ]   ;
 
#line 42 "Adresse.pgcc"
   char  ORT [ 31 ]   ;
 
#line 43 "Adresse.pgcc"
   bool  RNG_AN_POSTFACH   ;
 
#line 44 "Adresse.pgcc"
   char  LANDNAME [ 41 ]   ;
 
#line 45 "Adresse.pgcc"
   char  POSTFACHNAME [ 41 ]   ;
 
#line 46 "Adresse.pgcc"
   char  ISOCODE [ 4 ]   ;
 
#line 48 "Adresse.pgcc"
   int  IIDNR   ,  IPOSTANWVOR   ,  ISTRASSE   ,  IPOSTANWNACH   ,  IHSNR   ,  IPLZ   ,  IPOSTFACH   ,  IPOSTFPLZ   ,  IORT   ,  IRNG_AN_POSTFACH   ;
 
#line 50 "Adresse.pgcc"
   int  ILANDNAME   ,  IPOSTFACHNAME   ;
/* exec sql end declare section */
#line 51 "Adresse.pgcc"


   { ECPGdo(__LINE__, NULL, "select  firma  , postanwvor  , strasse  , hsnr  , postanwnach  , plz  , ort  , postfach  , postfplz  , rng_an_postfach  , idnr   from kunden where kundennr  = ?  ", 
	ECPGt_int,&(KUNDENNR),1L,1L,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, 
	ECPGt_char,(FIRMA),61L,1L,61*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(POSTANWVOR),41L,1L,41*sizeof(char), 
	ECPGt_int,&(IPOSTANWVOR),1L,1L,sizeof(int), 
	ECPGt_char,(STRASSE),36L,1L,36*sizeof(char), 
	ECPGt_int,&(ISTRASSE),1L,1L,sizeof(int), 
	ECPGt_char,(HSNR),8L,1L,8*sizeof(char), 
	ECPGt_int,&(IHSNR),1L,1L,sizeof(int), 
	ECPGt_char,(POSTANWNACH),41L,1L,41*sizeof(char), 
	ECPGt_int,&(IPOSTANWNACH),1L,1L,sizeof(int), 
	ECPGt_char,(_PLZ),11L,1L,11*sizeof(char), 
	ECPGt_int,&(IPLZ),1L,1L,sizeof(int), 
	ECPGt_char,(ORT),31L,1L,31*sizeof(char), 
	ECPGt_int,&(IORT),1L,1L,sizeof(int), 
	ECPGt_char,(POSTFACH),21L,1L,21*sizeof(char), 
	ECPGt_int,&(IPOSTFACH),1L,1L,sizeof(int), 
	ECPGt_char,(POSTFPLZ),11L,1L,11*sizeof(char), 
	ECPGt_int,&(IPOSTFPLZ),1L,1L,sizeof(int), 
	ECPGt_bool,&(RNG_AN_POSTFACH),1L,1L,sizeof(bool), 
	ECPGt_int,&(IRNG_AN_POSTFACH),1L,1L,sizeof(int), 
	ECPGt_char,(IDNR),21L,1L,21*sizeof(char), 
	ECPGt_int,&(IIDNR),1L,1L,sizeof(int), ECPGt_EORT);}
#line 61 "Adresse.pgcc"

   SQLerror::test(__FILELINE__" select from kunden");
   
   if (IRNG_AN_POSTFACH) RNG_AN_POSTFACH=false;
   if (IPOSTANWVOR) POSTANWVOR[0]=0;
   if (IPOSTANWNACH) POSTANWNACH[0]=0;
   if (ISTRASSE) STRASSE[0]=0;
   if (IHSNR) HSNR[0]=0;
   if (IORT) ORT[0]=0;
   if (IPLZ) _PLZ[0]=0;
   if (IPOSTFPLZ) POSTFPLZ[0]=0;
   if (IPOSTFACH) POSTFACH[0]=0;
   if (!IIDNR && IDNR[0])
    { int i;
      for (i=0;i<3 && isalpha(IDNR[i]);i++) 
         ISOCODE[i]=tolower((unsigned char)IDNR[i]);
      ISOCODE[i]=0;

/*      exec sql SELECT landname, postfachname 
      	into :LANDNAME :ILANDNAME, :POSTFACHNAME :IPOSTFACHNAME
      	FROM AUSLAND  where isocode= :ISOCODE;
      SQLerror::test(__FILELINE__"SELECT DIRECT FROM AUSLAND",100);
      if(sqlca.sqlcode) IPOSTFACHNAME=ILANDNAME=-1;
*/
    }
   else 
     IPOSTFACHNAME=ILANDNAME=-1;
   if (IPOSTFACHNAME) strncpy0(POSTFACHNAME,"Postfach",sizeof POSTFACHNAME);
   if (ILANDNAME) LANDNAME[0]=0;
   
   kundennr=KUNDENNR;
   strcpyx(IDnr,IDNR);
   firma=FIRMA;
   strcpyx(postanwvor,POSTANWVOR);
   strasse=STRASSE;
   strcpyx(postanwnach,POSTANWNACH);
   hsnr=HSNR;
   ort=ORT;
   PLZ=_PLZ;
   postfach=POSTFACH;
   postfPLZ=POSTFPLZ;
   rng_an_postfach=RNG_AN_POSTFACH;
   strcpyx(landname,LANDNAME);
   strcpyx(postfachname,POSTFACHNAME);
   lieferadresse=false;
}

static const std::string Dform(unsigned int i,bool TeX=false) throw()
{  return TeX?FormatiereTeX(i,2):Formatiere(i,2);
}

int Adresse::putIntoNLines(char **buf,int num,int len,bool TeX) const throw()
{  int lines=0,line=0;
   bool postfachflag=false;
   bool postfach_valid = postfach!="" && postfPLZ!="";
   
   for (int i=0;i<num;i++) buf[i][0]=0;
   if (firma!="") lines++;
   if (postanwvor[0]) lines++;
   if (strasse!="" && postfach_valid)
   		postfachflag=true;
   else if (rng_an_postfach && !lieferadresse && postfach_valid)
           	postfachflag=true;
   if (postfachflag || strasse[0] || hsnr[0]) lines++;
   if (postanwnach[0]) lines++;
   if (ort!="") lines++;
   
   if (lines<num && num>4) 
   { strncpy0(buf[line++],"Firma",len); lines++; }
   if (firma!="") strncpy0(buf[line++],TeX?string2TeX(firma).c_str():firma.c_str(),len);
   if (postanwvor[0] && lines<=num) strncpy0(buf[line++],TeX?string2TeX(postanwvor).c_str():postanwvor,len);
   if (postfachflag) 
   {  if (TeX) snprintf0(buf[line++],len,"%s~%s",string2TeX(postfachname).c_str(),
			Dform(atoi(postfach.c_str()),true).c_str());
      else snprintf0(buf[line++],len,"%s %s",postfachname,
			Dform(atoi(postfach.c_str())).c_str());
   }
   else if (strasse!="" || hsnr!="")
   {  if (TeX) snprintf0(buf[line++],len,"%s~%s",string2TeX(strasse).c_str(),
	string2TeX(hsnr).c_str());
      else snprintf0(buf[line++],len,"%s %s",strasse.c_str(),hsnr.c_str());
   }
   if (postanwnach[0] && line+1<num) strncpy0(buf[line++],TeX?string2TeX(postanwnach).c_str():postanwnach,len);
   if (line+1<num) { buf[line++][0]=0; lines++; }
   if (ort!="" && line<num) 
   {  if (PLZ!="") 
         snprintf0(buf[line++],len,"%05d%c%s",
         	postfachflag ? atoi(postfPLZ.c_str()) : atoi(PLZ.c_str()),
         	TeX?'~':' ',TeX?string2TeX(ort).c_str():ort.c_str());
      else 
         strncpy0(buf[line++],TeX?string2TeX(ort).c_str():ort.c_str(),len);
   }
/*
   if (ausland)
   {  if (line<num) strncpy0(buf[line++],TeX?string2TeX(landname).c_str():landname,len);
      else 
      {  int ortlen=strlen(buf[line-1]);
      
         if (ortlen+strlen(landname)+1<len)
         {  buf[line-1][ortlen]=' ';
            strncpy0(buf[line-1]+ortlen+1,TeX?string2TeX(landname).c_str():landname,len-ortlen-1);
         }
         else ; // geht halt nicht :-(
      }
   }
*/

   return line;
}

extern "C" {
Adresse *Adresse_by_kdnr(int kdnr) throw()
{  try 
   {  return new Adresse(kdnr);
   } 
   catch (SQLerror& e)
   {  std::cerr << "Datenbankfehler "<< e << '\n';
      if (e.Code()<0) { std::cerr << "Programm beendet sich\n"; abort(); }
      return NULL;
   }
}
void free_Adresse(Adresse *a) throw()
{  delete a;
}
int putIntoNLines(Adresse *a,char **buf,int num,int len,bool TeX) throw()
{  return a->putIntoNLines(buf,num,len,TeX);
}
void mark_Lieferanschrift(Adresse *a) throw()
{  a->mark_Lieferanschrift();
}
	   }

