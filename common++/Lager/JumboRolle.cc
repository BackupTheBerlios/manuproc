// $Id: JumboRolle.cc,v 1.5 2004/03/11 15:53:40 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include"JumboRolle.h"
#include <cstdio>

bool JumboRolle::isRollNrOK(ID eancode)
{
 int i=0;
 int code=eancode/10;
 int check=eancode%10;
 int hilf,ziffer,sum=0;

 while(i<(CODELENGTH-1))
   {
    ziffer=code%10;
    code/=10;
    hilf= i%2 ? ziffer : 3*ziffer;
    sum+=hilf;
    i++;
   } 

 sum%=10;
 sum = sum ? 10-sum : 0;
 if(sum==check) return true;
 return false;
}

int JumboRolle::Pruefziffer_anhaengen(int nummer)
{
 int code=nummer;
 int sum=0;

 for (int i=0;i<(CODELENGTH-1);++i)
   {
    int ziffer=code%10;
    code/=10;
    int hilf= i%2 ? ziffer : 3*ziffer;
    sum+=hilf;
   } 
 sum%=10;
 return nummer*10+(10-sum)%10;
}

std::string JumboRolle::CodeStr() const 
{ char ctxt[10];
  snprintf(ctxt,10,"%04d %04d",code/10000,code%10000);
  return ctxt;
}


std::string JumboRolle::MeterStr() const 
{
 char mtxt[20];
 if(barcoist == sollmeter)
   {if(!barcoist) return "--";
    else
      {snprintf(mtxt,20,"%4d",barcoist); return mtxt;}
   }
 
 if(!barcoist){snprintf(mtxt,20,"[%4d]",sollmeter); return mtxt;}
 if(!sollmeter){snprintf(mtxt,20,"%4d",barcoist); return mtxt;}

 snprintf(mtxt,20,"%4d[%4d]",barcoist,sollmeter);
 return mtxt;
}

int JumboRolle::Meter() const 
{
 if(barcoist) return barcoist;
 return sollmeter;
}


void JumboRolle::ausArchivHolen()
{  if (!im_archiv) return;
   Query q("insert into rohjumbo "
            "(code,maschine,webmaschine,soll_meter,plan_datum,verarb_datum,"
            "status,lauf,gang,barcoist,barcofert_datum,wiederinslager,"
            "artikelid) "
      "select code,maschine,webmaschine,soll_meter,plan_datum,verarb_datum,"
            "status,lauf,gang,barcoist,barcofert_datum,wiederinslager,"
            "artikelid "
            "from rohjumbo_archiv where code=?");
   q << code;
   if (q.Result()==100)
         throw SQLerror("JumboRolle::ausArchivHolen",100,"Rolle nicht im Archiv");
   Query("delete from rohjumbo_archiv where code=?") << code;
   SQLerror::test("JumboRolle::buchen archiv-löschen");
   im_archiv=false;
}
