/* $Id: Kettscheibe.cc,v 1.1 2002/09/26 15:12:31 thoma Exp $ */
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

/*
#include <Aux/SQLerror.h>
#include <Ketten/Kettscheibe.h>
#include <Ketten/ArtikelGang.h>
#include <Aux/FetchIStream.h>
#include <Aux/itos.h>
*/
/*
void Kettscheibe::saveKombinierteKette(const ArtikelGang& AG) throw() 
{
  if(kombinierte_kette==nicht_kombinierte_kette) return;
  std::string S="update ketten_garn set kombinierte_kette="
      " nullif("+itos(kombinierte_kette)+",nicht_kombinierte_kette) "
      " where (artikel,gaenge,ketten_index)=("
      +itos(AG.Id())+","+itos(AG.Gaenge())+","+itos(Index())+")";
  Query::Execute(S);
  SQLerror::test(__FILELINE__);
}


void Kettscheibe::push_back_kombiniert(const ArtikelGang& AG)  throw(SQLerror)
{
   if(kombinierte_kette==nicht_kombinierte_kette) return;
   std::string S="insert into ketten_kombinierte (id,artikel,gang,ketten_index) "
         " values ("+itos(kombinierte_kette)+","+itos(AG.Id())+","+
         itos(AG.Gaenge())+","+itos(Index())+")";
   Query::Execute(S);
   SQLerror::test(__FILELINE__);
}

*/
