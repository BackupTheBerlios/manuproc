/* $Id: KettenGarn.cc,v 1.1 2002/09/26 15:17:07 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <Aux/FetchIStream.h>  
#include <Ketten/KettenGarn.h>
#include <Ketten/ArtikelGang.h>
#include <sqlca.h>

void KettenGarn::setWiederholung(const ArtikelGang &AG,std::string s)  throw(SQLerror)
{
  std::string S="update ketten_garn set wiederholung=nullif('"+s+
   "','') where (artikel,gaenge,zeile,ketten_index) = ("
   +itos(AG.Id())+","
   +itos(AG.Gaenge())+","
   +itos(Zeile())+","
   +itos(Index())
   +")";
  Query::Execute(S); 
  SQLerror::test(__FILELINE__);
  wiederholung=s;
}

void KettenGarn::setKombiniert(const Kettscheibe::st_kombi &AG1,const ArtikelGang &AG2,int Index2)  throw(SQLerror)
{
  std::string S="insert into ketten_kombiniert "
      "(artikel1,gang1,kettenindex1,artikel2,gang2,kettenindex2) "
      " values ("
      +itos(AG1.artikel.Id())+","
      +itos(AG1.gang)+","
      +itos(AG1.KettenIndex)+","
      +itos(AG2.Artikel().Id())+","
      +itos(AG2.Gaenge())+","
      +itos(Index2)+")";
  Query::Execute(S); 
  SQLerror::test(__FILELINE__);
}

void KettenGarn::deleteKombiniert(const Kettscheibe::st_kombi &AG1,const ArtikelGang &AG2,int Index2)  throw(SQLerror)
{
  std::string S="delete from ketten_kombiniert where "
      "(artikel1,gang1,kettenindex1,artikel2,gang2,kettenindex2) "
      " = ("
      +itos(AG1.artikel.Id())+","
      +itos(AG1.gang)+","
      +itos(AG1.KettenIndex)+","
      +itos(AG2.Artikel().Id())+","
      +itos(AG2.Gaenge())+","
      +itos(Index2)+")";
  Query::Execute(S); 
  SQLerror::test(__FILELINE__,100);
  if(sqlca.sqlcode)
   {
    S="delete from ketten_kombiniert where "
      "(artikel1,gang1,kettenindex1,artikel2,gang2,kettenindex2) "
      " = ("
      +itos(AG2.Artikel().Id())+","
      +itos(AG2.Gaenge())+","
      +itos(Index2)+","
      +itos(AG1.artikel.Id())+","
      +itos(AG1.gang)+","
      +itos(AG1.KettenIndex)+")";
    Query::Execute(S); 
    SQLerror::test(__FILELINE__);
   }
}

void KettenGarn::modify(const ArtikelGang & AG,int kettenzahl_neu,int faeden_neu,int faden_laenge,ArtikelBase garn) 
{
  std::string S="update ketten_garn set garn="+itos(garn.Id())+","
                                       "ketten="+itos(kettenzahl_neu)+","
                                       "faeden="+itos(faeden_neu)+","
                                       "laenge="+itos(faden_laenge)+                                     
     " where (artikel,gaenge,ketten_index,zeile) = ("
     +itos(AG.Artikel().Id())+","
     +itos(AG.Gaenge())+","
     +itos(Index())+","
     +itos(Zeile())+")";
  Query::Execute(S);
  SQLerror::test(__FILELINE__);
  art=garn;
  kettenzahl=kettenzahl_neu;
  faeden=faeden_neu;
  laenge=faden_laenge;
}


unsigned int KettenGarn::Wiederholung_anzahl() const
{
  if(Wiederholung().empty()) return 1;
  std::string w(Wiederholung(),1,Wiederholung().size());
  return (unsigned int)atoi(w.c_str());
}
