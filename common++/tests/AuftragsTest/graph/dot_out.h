// $Id: dot_out.h,v 1.2 2002/09/26 15:58:50 thoma Exp $
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
      const std::string bezeichner;
      const unsigned int cluster_count;
      const AufEintragBase auftrag;
   public:
      Node(std::string s,unsigned int cc,AufEintragBase a) : 
            bezeichner(s),cluster_count(cc),auftrag(a) {};

      void write(ofstream &fout,AufEintragBase auftrag,std::string menge,std::string zusatz);
      std::string Name() const {return bezeichner;}
      unsigned int ClusterCount() const {return cluster_count;}

      const AufEintragBase Auftrag() const{return auftrag;}
      Node operator=(const Node &b)
         {return Node(b.bezeichner,b.cluster_count,b.auftrag); }
      bool operator==(const Node &b)
         {return bezeichner==b.bezeichner && cluster_count==b.cluster_count;}

};


class dot_out 
{
  public:
      enum e_colour{Black,Colour};

  private:
      emode mode;
      e_colour colour;
      enum erank{None,same,sink};
      unsigned int cc; // cc=cluster_count
      
      void write_header(ofstream &fout);
      void write_legend(ofstream &fout);
      void write_footer(ofstream &fout,std::string label);
      
      void write_filenames(ofstream &fout,const std::vector<graph_data_node::st_files>& filenames);
      void write_node(ofstream &fout);

      void Edge(ofstream &fout,Node n1,Node n2, std::string s,erank rank);

  public:
      dot_out(emode m,e_colour c) : mode(m),colour(c),cc(0) {};
      void write();
};

#endif
