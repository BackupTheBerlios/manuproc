// $Id: ArtikelBaum.cc,v 1.6 2003/02/03 12:57:04 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Misc/FetchIStream_fixedpoint.h>

#ifdef PETIG_EXTENSIONS
#include <Artikel/ArtikelBezeichnung.h>
#include <Faeden/Schussfaeden.hh>
#include <Faeden/Webangaben.hh>
#include <Faeden/Faden.hh>
#include <Faeden/Materialeigenschaft.hh>
#endif

ArtikelBaum::cache_t ArtikelBaum::cache;

FetchIStream &operator>>(FetchIStream &is, ArtikelBaum::RohArtikel &ra)
{  is >> ra.rohartikel >> ra.erzeugung;
   is >> FetchIStream::MapNull<ArtikelBaum::faktor_t>(ra.menge,ra.erzeugung->getMtrProStk());
   if (!ra.menge) ra.menge=1; // 0 ist Unsinn!?
   return is;
}

void ArtikelBaum::setID(const ID &stamp) throw(SQLerror)
{ArtikelBase::setID(stamp);
 zusammensetzung_t *cached=cache.lookup(Id());
 if (cached) zusammensetzung=*cached;
 else
 {  zusammensetzung.clear();
    (Query("select az.altartikelid, az.prozessid, az.menge "
 	"from artikelzusammensetzung az "
	"where id=? order by az.altartikelid")
	<< Id()).FetchArray(zusammensetzung);
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
    try {  //ArtikelStamm AS(*this);
    if(ppsInstanz::getProduktionsInstanz(*this)==ppsInstanzID::Weberei
     	&& zusammensetzung.empty())
     {
       Schussfaeden schussfaeden;
       Webangaben webangaben(Id());
       Fadenliste fadenliste;
       static Bindungsliste bindungsliste;
       try {                  
          schussfaeden.Load(Id());
          bool w=webangaben.Load();
          if (bindungsliste.empty()) bindungsliste.Load();
          if (w) fadenliste.Load(Id(),bindungsliste);
         }
       catch(SQLerror &e) {std::cerr << e << '\n';}
       for (Fadenliste::const_iterator i=fadenliste.sumbegin();i!=fadenliste.sumend();++i)
        { const double km_m= 1/1000.0;
          double gewicht_kg_m = i->get_Gewicht_kg_pro_km_Faden() * km_m;
          RohArtikel ra;
          ra.rohartikel=i->getMaterial();
          ra.menge= gewicht_kg_m;
          ra.erzeugung=cH_Prozess(ProzessID::Weben);
          zusammensetzung.push_back(ra);
        }
       int breite_mm=cH_ArtikelBezeichnung(*this)->Breite();
       int anzschfaed=0;
       for (Schussfaeden::const_iterator i=schussfaeden.begin();i!=schussfaeden.end();++i)
            anzschfaed+=i->getAnzahl();  
       for (Schussfaeden::const_iterator i=schussfaeden.begin();i!=schussfaeden.end();++i)
        {
          Materialeigenschaft M(i->getMaterial());
          // Gewicht in kg/m ==> 1/cm*mm*(1cm/10mm) * g/10km*(kg/1000g)*(km/1000m)
          const double cm_m = 100.0;
          const double m_mm = 1/1000.0;
          const double _10km_m = 1/10000.0;
          const double kg_g = 1/1000.0;
          double gewicht_kg_m = 
          	2 * webangaben.Schussdichte_cm() * cm_m // Schuss per m
          	* (i->getAnzahl() / (double)anzschfaed)	// Anteil des Mat.
          	* breite_mm*m_mm	// m per Schuss
          	* M.Gewicht_g_10km() * _10km_m * kg_g; // kg per m Mat.
          RohArtikel ra;
          ra.rohartikel=i->getMaterial();
          ra.menge= gewicht_kg_m;
          ra.erzeugung=cH_Prozess(ProzessID::Verarbeitung);
          zusammensetzung.push_back(ra);
        }
      gleiche_Faeden_summieren();
     }          
    }
    catch (SQLerror &e)
    {  std::cerr << "Kein ArtikelStamm zu " << Id() << '\n';
    }
#endif

   cache.Register(Id(),zusammensetzung);
 }
}

#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
void ArtikelBaum::gleiche_Faeden_summieren()
{typedef fixedpoint<3> menge_t;
 std::map<unsigned int,menge_t > mmap;
 for(std::vector<RohArtikel>::iterator i=zusammensetzung.begin();i!=zusammensetzung.end();++i)
   mmap[(*i).rohartikel.Id()] += (*i).menge;
 for (std::map<unsigned int,menge_t >::iterator m=mmap.begin();m!=mmap.end();++m)
   for(std::vector<RohArtikel>::iterator i=zusammensetzung.begin();i!=zusammensetzung.end();++i)
      if ((*i).rohartikel.Id() == m->first ) (*i).menge=m->second;
 // Duplikate entfernen
 sort(zusammensetzung.begin(),zusammensetzung.end());
 zusammensetzung.erase(unique(zusammensetzung.begin(),zusammensetzung.end()),zusammensetzung.end());
}
#endif


int ArtikelBaum::Tiefe() const
{
  if(tiefe!=NoDepth) return tiefe;

  tiefe=1;
  for (const_iterator i=begin();i!=end();++i)
   { int t=ArtikelBaum(i->rohartikel).Tiefe();
     if (t>tiefe) tiefe=t;
   }
//sich selber nicht mitzählen daher weg:  ++tiefe;
//  if (ArtikelStamm(Id()).BestellenBei()->LagerInstanz())
//      ++tiefe;
#warning das muß noch getestet/überarbeitet werden
  if(ppsInstanz::getBestellInstanz(*this)->LagerInstanz())
    ++tiefe;
  return tiefe;
}

#ifdef PETIG_EXTENSIONS
void ArtikelBaum::Sort()
{
 std::sort(zusammensetzung.begin(),zusammensetzung.end());
}
#endif

void ArtikelBaum::UnCache(const ArtikelBase &stamp)
{  cache.deregister(stamp.Id());
}

ArtikelBaum::faktor_t ArtikelBaum::Faktor(const ArtikelBase &kind) throw(SQLerror)
{  if (kind==static_cast<const ArtikelBase &>(*this)) return 1;
   faktor_t result=0;
   for (const_iterator i=begin();i!=end();++i)
   {  if (i->rohartikel==kind) result+=i->menge;
      else result+=i->menge*ArtikelBaum(i->rohartikel).Faktor(kind);
   }
   return result;
}

