/* $Id: AuftragsBaum.h,v 1.6 2002/07/05 12:35:01 christof Exp $ */
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

#ifndef CXX_AUFTRAG_AUFTRAGSBAUM_H
#define CXX_AUFTRAG_AUFTRAGSBAUM_H

#include <Auftrag/AufEintragBase.h>
#include <vector>


class AuftragsBaum 
{
 public:
        struct st_AuftragsBlatt
                { AufEintragBase AEB2; 
                  ArtikelBase AB;
                  AufEintragBase::mengen_t menge;
                  st_AuftragsBlatt(AufEintragBase a,ArtikelBase ab,AufEintragBase::mengen_t m)
                    : AEB2(a),AB(ab),menge(m) {} 
                };

 private:
        typedef std::vector<st_AuftragsBlatt> auftragsbaum_t;
 public:
        typedef auftragsbaum_t::const_iterator const_iterator;
        
 private:
        auftragsbaum_t auftragsbaum;
        
 public:
        AuftragsBaum(const AufEintragBase aeb,bool kinder=false); 
                //kinder=false: Alle Elternaufträge
                //kinder=true : Alle Kinderaufträge
                                                
         size_t size() const
          {  return auftragsbaum.size(); }
         const_iterator begin() const
          {  return auftragsbaum.begin(); }
         const_iterator end() const
          {  return auftragsbaum.end(); }
         bool empty() const
          {  return auftragsbaum.empty(); }
              
};                    
#endif
