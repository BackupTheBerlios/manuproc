/* $Id: Kettscheibe.h,v 1.4 2002/10/04 13:57:48 thoma Exp $ */
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


#ifndef KETTSCHEIBE_HH
#define KETTSCHEIBE_HH

#include <vector>
#include <Artikel/ArtikelBase.h>

class Kettscheibe //: Kette
{
      int index;
   public:
      struct st_kombi{ArtikelBase artikel; int gang; int KettenIndex; 
            st_kombi() : gang(-1),KettenIndex(-1) {}
            st_kombi(ArtikelBase a,int g,int k)
               :artikel(a),gang(g),KettenIndex(k){}
            bool operator<(const st_kombi &b) const
                  {return artikel<b.artikel || 
                    (artikel==b.artikel && gang<b.gang) ||
                    (artikel==b.artikel && gang==b.gang 
                        && KettenIndex<b.KettenIndex) ; }
               };
   private:
      std::vector<st_kombi> kombiniert_mit;
      
   public:

      Kettscheibe(int i) : index(i) {}

      int Index() const {return index;}
      void setKombiniert(st_kombi b) {kombiniert_mit.push_back(b);}
      const std::vector<st_kombi>& getKombiniert() const {return kombiniert_mit;}

      bool operator==(const Kettscheibe& b) const {return Index()==b.Index();}
      bool operator<(const Kettscheibe& b) const {return Index()<b.Index();}
};

#endif
