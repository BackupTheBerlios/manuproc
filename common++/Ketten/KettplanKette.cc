// $Id: KettplanKette.cc,v 1.5 2004/02/25 11:35:49 christof Exp $
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


#include "KettplanKette.h"
//#include <Ketten/ArtikelGang_Malte.h>

void KettplanKette::set_info(int pm,int kl,int sl,int sd) throw()
{  planmasch=pm;
   kettlaenge=kl;
   stuecklaenge=sl;
   schussdichte=sd;
   valid|=VA_PLAN|VA_SCHUSS;
   if (stuecklaenge) valid|=VA_STUECKLEN;
   if (kettlaenge) valid|=VA_KETTLEN;
}

int KettplanKette::planMaschine() const throw()
{  return (valid&VA_PLAN)?planmasch:holePlanMaschine();
}

void KettplanKette::setStuecklaenge(const int wert)  const throw(SQLerror)
{
  std::string q="update ketten set stucklaenge="+itos(wert)+" where "
   " (maschine,schaerdatum) = ("+itos(Maschine())+","
      +Schaerdatum().postgres_null_if_invalid()+")";
  Query::Execute(q);
  SQLerror::test(__FILELINE__);
  stuecklaenge=wert;
}
 
void KettplanKette::setKettlaenge(const int wert)  const throw(SQLerror)
{
  std::string q="update ketten set laenge="+itos(wert)+" where "
   " (maschine,schaerdatum) = ("+itos(Maschine())+","
      +Schaerdatum().postgres_null_if_invalid()+")";
  Query::Execute(q);
  SQLerror::test(__FILELINE__);
  kettlaenge=wert;
}
