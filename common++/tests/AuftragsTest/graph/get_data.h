// $Id: get_data.h,v 1.10 2002/11/22 15:19:37 thoma Exp $
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

#ifndef GET_DATA_H  
#define GET_DATA_H

#include <vector>
#include <string>
#include <map>
#include <list>
#include <Auftrag/AufEintragBase.h>


enum emode {None,Legende,Menge,Planung,Split,Lager,ZweiAuftraege,
   ZweiterAuftrag_frueheresDatum,Lieferschein,LieferscheinMenge,
   LieferscheinZusatz,LieferscheinZweiAuftraege,LieferscheinJacek,
   ZweiKunden,ZweiKundenMengeFreigeben,
   ManuProCTest};

class graph_data_node
{
      struct st_auftrag{AufEintragBase auftrag; AuftragBase::mengen_t bestellt;
                        AuftragBase::mengen_t geliefert;AufStatVal status;
                        Petig::Datum datum;std::string prefix;
             st_auftrag(AufEintragBase a, AuftragBase::mengen_t b,
                         AuftragBase::mengen_t g,AufStatVal s,Petig::Datum d,
                         std::string p)
                :auftrag(a),bestellt(b),geliefert(g),status(s),datum(d),prefix(p) {}
             bool operator<(const st_auftrag &b) const {return auftrag<b.auftrag;}
             };

      struct st_aebZ{AufEintragBase aebALT;AufEintragBase aebNEU; AuftragBase::mengen_t menge;
            st_aebZ(AufEintragBase a,AufEintragBase n,AuftragBase::mengen_t m)
            : aebALT(a),aebNEU(n),menge(m) {}
             bool operator<(const st_aebZ &b) const {return aebALT<b.aebALT;}
            };
  public:
      struct st_child{AufEintragBase aeb;AuftragBase::mengen_t menge;
              st_child(AufEintragBase a,AuftragBase::mengen_t m) 
               : aeb(a),menge(m){} 
              bool operator<(const st_child &b) const {return aeb<b.aeb;}
               };
      struct st_files{std::string filename;std::string prefix;
             st_files(std::string f) : filename(f) {}
             st_files(std::string f,std::string p) : filename(f),prefix(p) {}
             };
  private:
      std::vector<st_files> vec_files_auftragentry;
      std::vector<std::string> vec_files_auftragsentryzuordnung;
      std::list<st_auftrag> list_auftrag;
      std::map<AufEintragBase,std::vector<st_auftrag> > map_aeb;
      std::list<st_aebZ> list_auftragszuordnung;

     void get_files(emode); 
     void get_values_from_files();
     void get_values_from_files_Z();
     void fill_map();
     std::vector<st_files> Mfiles();
     std::vector<st_files> Pfiles();
     std::vector<st_files> Sfiles();
     std::vector<st_files> Lfiles();
     std::vector<st_files> Zfiles();
     std::vector<st_files> Dfiles();
     std::vector<st_files> Lsfiles();
     std::vector<st_files> Lmfiles();
     std::vector<st_files> LZfiles();
     std::vector<st_files> LAfiles();
     std::vector<st_files> LSJfiles();
     std::vector<st_files> ZKfiles();
     std::vector<st_files> ZKMfiles();
     std::vector<st_files> ManuProCfiles();
     std::vector<st_files> filenames;
  public:
     graph_data_node(emode);

     std::vector<st_files> get_filenames() const {return filenames;}
     std::list<AufEintragBase> get_existing_aeb() const;
     struct st_node_strings{AufEintragBase auftrag; std::string mengen;
                            std::string zusatz;
            st_node_strings(AufEintragBase a,std::string m,std::string z) 
               : auftrag(a),mengen(m),zusatz(z) {} };
     st_node_strings get_mengen_for_node(AufEintragBase aeb);
     std::vector<pair<std::string,std::string> >  get_edges_for(AufEintragBase aeb);

};

#endif

