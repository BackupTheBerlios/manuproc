// $Id: Faden.cc,v 1.25 2005/09/05 15:30:38 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002-2003 Adolf Petig GmbH & Co. KG
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

#include "Faden.hh"
#include "Materialeigenschaft.hh"
#include <Misc/relops.h>
#include <Misc/compiler_ports.h>
#include <Misc/FetchIStream.h>
#include <Faeden/Webangaben.hh>
#include <cassert>


std::ostream& operator<< (std::ostream& os, const Faden& f)
{
   os << f.anzahl  << ", " << f.material << ", " << f.bindung;
   return os;
}


int Faden::displayBreite() const
{
 const std::string tmp=bindung.extraFunctionData(Bindung::EFFBREITE);
 if(bindung.haveExtraFunction(Bindung::EFFBREITE))
   return atoi(tmp.c_str());
 else
   return getAnzahl();
}

Faden::Faden (int znr, unsigned z, ArtikelBase::ID s, Bindung b) 
: zeilennummer(znr), anzahl(z), material(s), bindung(b), kettscheibe(-1),
	ausn_gaenge(), ausn_faeden()
{
}

bool Faden::operator== (const Faden& f) const
{
   return ((material == f.material) && (bindung == f.bindung));
}

bool Faden::operator!= (const Faden& f) const
{
   return ((material != f.material) || (bindung != f.bindung));
}

Wiederholung::Wiederholung (const unsigned s, const unsigned e, const unsigned a)
{
   if (s < e)
   {
      start = s;
      end = e;
   }
   else
   {
      start = e;
      end = s;
   }
   anzahl = a;
}

bool Wiederholung::operator== (const Wiederholung& w) const
{
   return ((start == w.start) && (end == w.end));
}

bool Wiederholung::operator< (const Wiederholung& w) const
{
   return ((end < w.end) || ((end == w.end) && (start > w.start)));
}

// war mal const Faden
int Fadenliste::add (Faden f, const unsigned r)
{
   unsigned index = 0, row = r;

   if (row < liste.size())
   {
      std::vector<Faden>::iterator li = liste.begin();
      liste.insert (li+row, f);
      std::vector<unsigned>::iterator ri = repnumliste.begin();
      repnumliste.insert (ri+row, repnumliste [row]);
   }
   else
   {
      liste.push_back (f);
      repnumliste.push_back (1);
      row = liste.size() - 1;
   }

   std::vector<Faden>::iterator si = sumliste.begin();
   std::vector<Faden>::iterator sie = sumliste.end();
   while (std_neq(si,sie) && *si!=f)
   {
      si++;
      index++;
   }
   if (si == sie)
   {  // const Faden scheitert hier ... merkwürdig!
      // warum denn mit repnumliste malnehmen ???
      // f.setAnzahl (f.getAnzahl() * repnumliste [row]);
      sumliste.push_back (f);
      index = sumliste.size()-1;
      // index = (unsigned)-1; // designfehler !
      // index_max+1 muesste ok sein!
   }
   else
      si->setAnzahl (si->getAnzahl() + f.getAnzahl() * repnumliste [row]);
   sort_sumliste();

   std::vector<Wiederholung>::reverse_iterator i = repliste.rbegin();
   std::vector<Wiederholung>::reverse_iterator ie = repliste.rend();
   while (std_neq(i,ie) && (i->getEnd() >= row))
   {
      unsigned s = i->getStart();
      if (s > row)
         i->setStart (s+1);
      unsigned e = i->getEnd();
      if (e >= row)
         i->setEnd (e+1);
      i++;
   }
   return index;
}

bool Fadenliste::del (const unsigned row, unsigned& index)
{
   bool erased = false;
   Faden* f;

   if (row > liste.size()-1)
   {
      return false;
   }
   f = &(liste [row]);

   std::vector<Faden>::iterator si = sumliste.begin();
   std::vector<Faden>::iterator sie = sumliste.end();
   index = 0;
   while (std_neq(si, sie) && (*si!=*f))
   {
      si++;
      index++;
   }
   if (si == sie)
   {
      return false;
   }

   int anzahl_new = si->getAnzahl() - (f->getAnzahl() * repnumliste [row]);
   if (anzahl_new > 0)
      si->setAnzahl (anzahl_new);
   else
   {
      sumliste.erase (si);
      erased = true;
   }
   sort_sumliste();

   std::vector<Wiederholung>::reverse_iterator i = repliste.rbegin();
   std::vector<Wiederholung>::reverse_iterator ie = repliste.rend();
   while (std_neq(i,ie) && (i->getEnd() >= row))
   {
      unsigned s = i->getStart();
      if (s > row)
         i->setStart (s-1);
      unsigned e = i->getEnd();
      if (e >= row)
         i->setEnd (e-1);
      if ((s == row) && (e == row))
      {
         std::vector<Wiederholung>::iterator ib = i.base();
         repliste.erase (--ib);
      }
      i++;
   }

   std::vector<Faden>::iterator li = liste.begin();
   liste.erase (li+row);
   std::vector<unsigned>::iterator ri = repnumliste.begin();
   repnumliste.erase (ri+row);
   return erased;
}

bool Fadenliste::rep_add (const unsigned s, const unsigned e, const unsigned a)
{
   if ((s < 0) || (s > liste.size()-1) || (e < 0) || (e > liste.size()-1))
      return false;
   Wiederholung w (s,e,a);

   std::vector<Wiederholung>::iterator i = repliste.begin();
   std::vector<Wiederholung>::iterator ie = repliste.end();
   while (std_neq(i, ie) && (*i < w))
      i++;
   if (i == ie)
      repliste.push_back (w);
   else if (*i == w)
      i->setAnzahl (i->getAnzahl() * a);
   else
      repliste.insert (i, w);

   std::vector<unsigned>::iterator ri = repnumliste.begin();
   std::vector<Faden>::iterator fi = liste.begin();
   for (unsigned i = w.getStart(); i <= w.getEnd(); i++)
   {
      Faden f = *(fi+i);
      std::vector<Faden>::iterator si = sumliste.begin();
      std::vector<Faden>::iterator sie = sumliste.end();
      while (std_neq(si,sie) && (*si != f))
         si++;
      if (si == sie)
        return false;
      si->setAnzahl (si->getAnzahl() + *(ri+i) * (a-1) * f.getAnzahl());

      *(ri+i) *= a;
   }
   sort_sumliste();
   return true;
}

bool Fadenliste::rep_del (const unsigned s, const unsigned e)
{
    if ((s < 0) || (s > liste.size()-1) || (e < 0) || (e > liste.size()-1))
      return false;
   Wiederholung w (s,e,1);

   std::vector<Wiederholung>::iterator i = repliste.begin();
   std::vector<Wiederholung>::iterator ie = repliste.end();
   while (std_neq(i,ie) && (*i!=w))
      i++;
   if (i == ie)
      return false;

   int anzahl = i->getAnzahl();
   repliste.erase (i);

   std::vector<unsigned>::iterator ri = repnumliste.begin();
   std::vector<Faden>::iterator fi = liste.begin();
   for (unsigned i = w.getStart(); i <= w.getEnd(); i++)
   {
      *(ri+i) /= anzahl;

      Faden f = *(fi+i);
      std::vector<Faden>::iterator si = sumliste.begin();
      std::vector<Faden>::iterator sie = sumliste.end();
      while (std_neq(si,sie) && (*si != f))
         si++;
      if (si == sie)
        return false;
      si->setAnzahl (si->getAnzahl() - *(ri+i) * (anzahl-1) * f.getAnzahl());
   }
   sort_sumliste();
   return true;
}

void Fadenliste::erase()
{
   if (! liste.empty())
      liste.erase (liste.begin(), liste.end());
   if (! sumliste.empty())
      sumliste.erase (sumliste.begin(), sumliste.end());
   if (! repliste.empty())
      repliste.erase (repliste.begin(), repliste.end());
   if (! repnumliste.empty())
      repnumliste.erase (repnumliste.begin(), repnumliste.end());
}

void Fadenliste::print_out() const
{
   ostream_iterator<Faden> oit(std::cout, "\n");
   copy (liste.begin(), liste.end(), oit);
}

class Anzahl_Descending
{public:
	bool operator()(const Faden &a,const Faden &b) const
	{  return a.getAnzahl()>b.getAnzahl();
	}
};

void Fadenliste::sort_sumliste()
{  sort(sumliste.begin(),sumliste.end(),Anzahl_Descending());
}

void Fadenliste::EntfalteWiederholungen_recurse(std::vector<Faden> &liste_out,
	std::vector<Wiederholung>::const_iterator ri,std::vector<Faden>::const_iterator i,
	std::vector<Faden>::const_iterator e,unsigned index) const
{reloop:
   if (i==e) return;
   // Zeilen vor der aktuellen Wiederholung
   while (std_neq(i,e) && (ri==repend() || index<ri->getStart()))
   {  liste_out.push_back(*i);
      ++i; ++index;
   }
   if (ri==repend() || i==e) return;
   // Wiederholung
   std::vector<Faden>::const_iterator new_end(i);
   ++new_end; // Muss ja eine Zeile nach der Wiederholung stehen
   for (unsigned j=ri->getStart();j<ri->getEnd();++j) 
   {  if (new_end==e) return; // da ist was schiefgelaufen !!!
      ++new_end;
   }
   std::vector<Wiederholung>::const_iterator next_ri(ri);
   ++next_ri;
   for (unsigned j=0;j<ri->getAnzahl();++j)
      EntfalteWiederholungen_recurse(liste_out,next_ri,i,new_end,index);
   // Zeilen nach der Wiederholung (von vorne beginnen)
   i=new_end;
   index+=ri->getEnd()-ri->getStart()+1;
   while (ri->getStart()<index) ++ri;
   goto reloop;
}

void Fadenliste::EntfalteWiederholungen(std::vector<Faden> &liste_out) const
{  int index=0;
   liste_out.clear();
   EntfalteWiederholungen_recurse(liste_out,repbegin(),begin(),end(),index);
}

fixedpoint<5> Faden::get_Gewicht_kg_pro_km_Faden() const
{
  Materialeigenschaft M(getMaterial());
  // Anzahl * g/10km * kg/1000g ==> Gewicht in kg ///////////
  return getAnzahl() * M.Gewicht_g_10km() * 0.0001 ;
}

fixedpoint<5> Faden::get_Gewicht_kg_mal_anzschfaeden_pro_km_und_breite_mm_und_schussdichte_cm() const
{
  Materialeigenschaft M(getMaterial());
  //  1/cm*(1cm/10mm) * g/10km*(kg/1000g) ==> Gewicht in kg ///////////
  return   2 * getAnzahl() * M.Gewicht_g_10km() * 0.00001 ;
  //usage:
//  for (Schussfaeden::const_iterator i=schussfaeden.begin();i!=schussfaeden.end();++i)
//         anzschfaed+=i->getAnzahl();  
//  this /(float)anzschfaed *breite_mm*schussfaeden.Schussdichte_cm();
         
}


#include <Misc/FetchIStream.h>
#include <Misc/itos.h>
void Faden::create(const ArtikelBase &artikel,const int znr,const int anzahl,const ArtikelBase &material,Bindung bindung) throw(SQLerror)
{
  std::string S="insert into webang_faeden (artikel,zeilennummer,"
      "anzahl,material,bindung) values ("
      +itos(artikel.Id())+","
      +itos(znr)+","
      +itos(anzahl)+","
      +itos(material.Id())+","
      +itos(bindung.getId())+")";
  Query::Execute(S);
  SQLerror::test(__FILELINE__);
}

void Faden::create_wiederholung(const ArtikelBase &artikel, const Wiederholung &W) throw(SQLerror)
{
  std::string S="insert into webang_wiederhol (artikel,anfangszeile,"
      "endzeile,wiederholungen) values ("
      +itos(artikel.Id())+","
      +itos(W.getStart())+","
      +itos(W.getEnd())+","
      +itos(W.getAnzahl())+")";
  Query::Execute(S);
  SQLerror::test(__FILELINE__);
}

static FetchIStream &operator>>(FetchIStream &is, Bindung &b)
{  int bnr;
   is >> FetchIStream::MapNull(bnr,Bindung::standard_id);
   b=Bindung::getById(bnr);
   return is;
}

static FetchIStream &operator>>(FetchIStream &is, Fd_Kettscheibe &f)
{  is >> f.nr
   	>> FetchIStream::MapNull(f.max_kettlaenge)
   	>> FetchIStream::MapNull(f.max_fadenzahl)
   	>> FetchIStream::MapNull(f.verlaengern)
   	>> FetchIStream::MapNull(f.ausn_gaenge)
   	>> FetchIStream::MapNull(f.ausn_maxfd)
   	>> FetchIStream::MapNull(f.ausn_gaenge2)
   	>> FetchIStream::MapNull(f.ausn_maxfd2);
   return is;
}

static FetchIStream &operator>>(FetchIStream &is, Faden &f)
{  is >> f.zeilennummer >> f.anzahl >> f.material >> f.bindung
   	>> FetchIStream::MapNull(f.kettscheibe,-1)
   	>> FetchIStream::MapNull(f.ausn_gaenge)
   	>> FetchIStream::MapNull(f.ausn_faeden);
   return is;
}

void Fadenliste::Load(const ArtikelBase &ab,const Bindungsliste &bindungsliste)
{  assert(&bindungsliste==&Bindung::GlobaleListe());
   Webangaben wa(ab);
   wa.Load();
   Load(wa);
}

void Fadenliste::LoadRecurse(const Webangaben &wa)
{ assert(!!wa.VarianteVon());
  assert(liste.empty());
  assert(sumliste.empty());
  assert(repliste.empty());
  assert(kettscheiben.empty());
  const Webangaben::ersetzen_t &ersetzen=wa.Ersetzen();
  Webangaben parent(wa.VarianteVon());
  parent.Load();
  Fadenliste pfaeden;
  pfaeden.Load(parent);
  for (const_iterator i=pfaeden.begin();i!=pfaeden.end();++i)
  {  Faden f=*i;
     std::map<ArtikelBase,ArtikelBase>::const_iterator found=ersetzen.find(f.material);
     if (found!=ersetzen.end()) f.material=found->second;
     add(f,unsigned(-1));
  }
  for (const_repiterator i=pfaeden.repbegin();i!=pfaeden.repend();++i)
     rep_add(i->getStart(),i->getEnd(),i->getAnzahl());
  for (const_kettiterator i=pfaeden.kettbegin();i!=pfaeden.kettend();++i)
  { if (i->nr>=kettscheiben.size()) kettscheiben.resize(i->nr+1);
    kettscheiben[i->nr]=*i;
  }
}

void Fadenliste::Load(const Webangaben &wa)
{  erase();
   FetchIStream is;
   const Webangaben::ersetzen_t &ersetzen=wa.Ersetzen();
   ArtikelBase ab=wa.Artikel();
   if (!!wa.VarianteVon()) ab=wa.VarianteVon();
      
      Query q("select zeilennummer, anzahl, material, bindung, kettscheibe, "
      	"ausn_gaenge,ausn_faeden "
      	"from webang_faeden where artikel=? order by zeilennummer");
      q << ab;
      while ((q>>is).good())
      {  Faden f;
         is >> f >> Query::check_eol();
         if (!ersetzen.empty())
         {  std::map<ArtikelBase,ArtikelBase>::const_iterator found=ersetzen.find(f.material);
            if (found!=ersetzen.end()) f.material=found->second;
         }
         add(f,unsigned(-1));
      }

      Query q2("select anfangszeile, endzeile, wiederholungen "
		"from webang_wiederhol where artikel=? "
		// request small ones first so we print it right
		"order by endzeile-anfangszeile");
      q2 << ab;
      while ((q2>>is).good())
      {  int anfangszeile, endzeile, wiederholungen;
         is >> anfangszeile >> endzeile >> wiederholungen >> Query::check_eol();
         rep_add(anfangszeile, endzeile, wiederholungen);
      }

      Query q3("select kettscheibe, "
      	"max_kettlaenge,max_fadenzahl,verlaengern,"
      	"ausn_gaenge1,ausn_maxfd1,ausn_gaenge2,ausn_maxfd2 "
		"from webang_ketten where artikel=? "
		// request last one first so that we resize efficiently
		"order by kettscheibe desc");
      q3 << ab;
      while ((q3>>is).good())
      {  Fd_Kettscheibe ks;
         is >> ks >> Query::check_eol();
         if (ks.nr>=kettscheiben.size()) kettscheiben.resize(ks.nr+1);
         kettscheiben[ks.nr]=ks;
      }
  if (!!wa.VarianteVon() && liste.empty() && kettscheiben.empty())
  // try the slower recursive way
    LoadRecurse(wa);
}

const Fd_Kettscheibe &Fadenliste::Kettscheibe(unsigned nr) const
{  assert(nr<10000);
   if (nr>=kettscheiben.size()) 
      const_cast<Fadenliste*>(this)->kettscheiben.resize(nr+1);
   return kettscheiben[nr];
}
