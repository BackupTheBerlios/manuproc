/* $Id: ArtikelGang_Malte.h,v 1.2 2003/10/06 10:08:10 christof Exp $ */
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

#ifndef ARTIKELGANG_MALTE_HH
#define ARTIKELGANG_MALTE_HH

#include <Ketten/ArtikelGang.h>
#include <Ketten/Kettscheibe.h>
#include <Ketten/KettenGarn.h>

// mal sehen, wozu das gut sein soll
class ArtikelGang_Malte : public ArtikelGang
{
   mutable std::list<Kettscheibe> kettscheiben;
//   mutable int index; // Kettenindex für die Kombinierte Kette

public:
	ArtikelGang_Malte(int g,ArtikelBase id,bool load_garn=false,int kettlaenge=0) : ArtikelGang(g,id)
	      { if(load_garn) load_Garn(kettlaenge);  }
	ArtikelGang_Malte(const ArtikelGang &ag,bool load_garn=false,int kettlaenge=0) : ArtikelGang(ag)
	      { if(load_garn) load_Garn(kettlaenge);  }
	ArtikelGang_Malte() {}
   void setKombinierteKette(int index,Kettscheibe::st_kombi b) const ;
   std::list<Kettscheibe>& getKettscheiben() {return kettscheiben;}
   std::string KombiniertMit_c_str(const KettenGarn& garn) const;
   std::vector<Kettscheibe::st_kombi> KombiniertMit(const KettenGarn& garn) const;

   void save_Garn(const KettenGarn& garn/*,std::list<ArtikelGang> KombiArtikel*/) const  throw(SQLerror);
   void delete_Garn(const KettenGarn& garn) const  throw(SQLerror);
   std::vector<KettenGarn> load_Garn(int kettlaenge) const;

   enum e_wiederholung{Next,Same};
   std::string getWiederholung(const e_wiederholung &ew) const;
};

#endif
