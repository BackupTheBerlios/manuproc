// $Id: dot_out.h,v 1.15 2003/08/06 09:17:53 christof Exp $
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

#ifndef DOTOUT_H
#define DOTOUT_H

#include <fstream>
#include <string>
#include "get_data.h"

class Node
{
      std::string bezeichner;
      unsigned int cluster_count;
      AufEintragBase auftrag;
   public:
      Node(const std::string &s,unsigned int cc,AufEintragBase a) : 
            bezeichner(s),cluster_count(cc),auftrag(a) {};

      void write(std::ofstream &fout,AufEintragBase auftrag,
      		const std::string &menge,const std::string &zusatz,
      		const std::string &artikel);
      std::string Name() const {return bezeichner;}
      unsigned int ClusterCount() const {return cluster_count;}

      const AufEintragBase Auftrag() const{return auftrag;}
      bool operator==(const Node &b)
         {return bezeichner==b.bezeichner && cluster_count==b.cluster_count;}

};


class dot_out 
{
  public:
      enum e_colour{Black,Colour};

  private:
      std::string mode;
      e_colour colour;
      enum erank{None,same,sink};
      unsigned int cc; // cc=cluster_count
      
      void write_header(std::ofstream &fout);
      void write_legend(std::ofstream &fout);
      void write_footer(std::ofstream &fout,std::string label);
      
      void write_filenames(std::ofstream &fout,const std::vector<graph_data_node::st_files>& filenames);
      void write_node(std::ofstream &fout);

      void Edge(std::ofstream &fout,Node n1,Node n2, std::string s,erank rank);

  public:
      dot_out(const std::string &m,e_colour c) : mode(m),colour(c),cc(0) {};
      void write();
};

#endif
