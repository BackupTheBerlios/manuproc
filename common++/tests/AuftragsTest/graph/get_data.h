// $Id: get_data.h,v 1.23 2003/02/12 13:54:33 christof Exp $
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

class graph_data_node
{
public:
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
      std::string graphtitle;

     void get_files(const std::string &mode); 
     void get_values_from_files();
     void get_values_from_files_Z();
     void fill_map();
     std::vector<st_files> filenames;
  public:
     graph_data_node(const std::string &mode);

     std::vector<st_files> get_filenames() const {return filenames;}
     const std::string &get_title() const { return graphtitle; }
     std::list<AufEintragBase> get_existing_aeb() const;
     struct st_node_strings{AufEintragBase auftrag; std::string mengen;
                            std::string zusatz;
            st_node_strings(AufEintragBase a,std::string m,std::string z) 
               : auftrag(a),mengen(m),zusatz(z) {} };
     st_node_strings get_mengen_for_node(AufEintragBase aeb);
     std::vector<std::pair<std::string,std::string> >  get_edges_for(AufEintragBase aeb);

// global parameters - evil ... but I don't want to rewrite the program
	static bool show_referenz;
	static bool dont_hide_empty;
	// limit number of files to display
	static unsigned limit,start;
};

#endif

