/* $Id: ArtikelGang.cc,v 1.3 2002/10/04 08:23:21 thoma Exp $ */
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

#include <Ketten/ArtikelGang.h>
#include <Aux/FetchIStream.h>
#include <Aux/mystring.h>

void ArtikelGang::delete_Garn(const KettenGarn& garn) const  throw(SQLerror)
{
 std::string S="delete from ketten_garn where (artikel,gaenge,ketten,zeile,"
      "faeden,garn,ketten_index) = ("
      +itos(Id())+","
      +itos(Gaenge())+","
      +itos(garn.Kettenzahl())+","
      +itos(garn.Zeile())+","
      +itos(garn.Faeden())+","
      +itos(garn.Id())+","
      +itos(garn.Index())+")";
 Query::Execute(S);
 SQLerror::test("Kette::delete(): Garn schon weg");
}

void ArtikelGang::save_Garn(const KettenGarn& garn/*,std::list<ArtikelGang> KombiArtikel*/) const  throw(SQLerror)
{
 std::string S="insert into ketten_garn "
   " (artikel,gaenge,ketten,zeile,faeden,garn,ketten_index,laenge/*,kombinierte_kette*/,wiederholung) values "
   " ("+
   itos(Id())+","+
   itos(Gaenge())+","+
   itos(garn.Kettenzahl())+","+
   itos(garn.Zeile())+","+
   itos(garn.Faeden())+","+
   itos(garn.Id())+","+
   itos(garn.Index())+","+
   "nullif("+itos(garn.Laenge())+",0),'"
//   "nullif("+itos(garn.Kombiniert())+",false),'"
   +garn.Wiederholung()   
   +"')";
 Query::Execute(S); 
 SQLerror::test(__FILELINE__);

/*
 static bool recursion=true; // falls mehr als zwei ArtikelGang zu einer Kombikette gehören
 KombiArtikel.remove(*this);
 if(recursion)
   for(std::list<ArtikelGang>::const_iterator i=KombiArtikel.begin();i!=KombiArtikel.end();++i)
    {
      recursion=false;
//cout << cH_ArtikelBezeichnung(Artikel())->Bezeichnung()<<' '<<Gaenge()
// <<" wird kombiniert mit "
// <<cH_ArtikelBezeichnung(i->Artikel())->Bezeichnung()<<' '<<i->Gaenge()<<'\n';
       i->save_Garn(garn,KombiArtikel);
    }
 recursion=true;
*/
}


/*
void ArtikelGang::delete_kombi_kette(int kombiniert) const  throw(SQLerror)
{
 assert(kombiniert!=Kettscheibe::nicht_kombinierte_kette);
 std::string S="delete from ketten_garn where (kombinierte_kette)"
      "=("+itos(kombiniert)+")";
 Query::Execute(S);
 SQLerror::test("Kette::delete(): Kombikette konnte nicht gelöscht werden");
}
*/


std::string ArtikelGang::getWiederholung(const e_wiederholung &ew) const
{
  std::vector<KettenGarn> K=load_Garn(0);  
  char maxchar='A'-1;

  for(std::vector<KettenGarn>::const_iterator i=K.begin();i!=K.end();++i)
   {
     std::string s=i->Wiederholung();
     char c=s[0];
     if(c>maxchar) maxchar=c;
   }
  char B[2];
  if     (ew==Same) B[0]=maxchar;
  else if(ew==Next) B[0]=maxchar+1;
  if(B[0]<'A') B[0]='A';
  if(B[0]>'Z') B[0]='Z';
  B[1]=0;
  return B;
}

void ArtikelGang::setKombinierteKette(int index, Kettscheibe::st_kombi b) const
{
  for(std::list<Kettscheibe>::iterator i=kettscheiben.begin();i!=kettscheiben.end();++i)
   {
     if(i->Index() == index)
         i->setKombiniert(b);
   }
}

std::string ArtikelGang::KombiniertMit_c_str(const KettenGarn& garn) const
{
  std::string s;
  for(std::list<Kettscheibe>::const_iterator i=kettscheiben.begin();i!=kettscheiben.end();++i)
   {
     if(garn.Index() == i->Index())
      {
        for(std::vector<Kettscheibe::st_kombi>::const_iterator j=i->getKombiniert().begin();j!=i->getKombiniert().end();++j)
         s+= cH_ArtikelBezeichnung(j->artikel)->Bezeichnung()
               +" KetteNr."+itos(j->KettenIndex)+", ";
      }       
   }
  ManuProC::remove_last_from(s,", ");
  return s;
}

std::vector<Kettscheibe::st_kombi> ArtikelGang::KombiniertMit(const KettenGarn& garn) const
{
  for(std::list<Kettscheibe>::const_iterator i=kettscheiben.begin();i!=kettscheiben.end();++i)
   {
     if(garn.Index() == i->Index())
      {
        return i->getKombiniert();
      }       
   }
  assert(!"never get here\n");
  abort();
}

