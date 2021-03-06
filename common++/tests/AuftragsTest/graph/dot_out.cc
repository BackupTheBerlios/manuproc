// $Id: dot_out.cc,v 1.25 2005/10/18 21:46:25 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2005 Adolf Petig GmbH & Co. KG
 *  written by Malte Thoma and Christof Petig
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

#include <fstream>
#include "dot_out.h"
#include <Auftrag/AufEintragBase.h>

#define CUSTOM_ORDERNO 10

void dot_out::write()
{
  std::ofstream fout("dot.script");
  write_header(fout);

  if(mode=="X" || mode=="?") 
  {  write_legend(fout);
     write_footer(fout,"Legende");
  }
  else  
  {  
     write_node(fout);
     graph_data_node N(mode);
     write_footer(fout,N.get_title());
  }
  fout.close();
  system("sh dot.script");
}

void dot_out::write_legend(std::ofstream &fout)
{
  fout << "\tnode[shape=record]\n"
       << "\t\tfilenames [label=\"{Dateiname 1 | Dateiname 2} | {P1 | P2}\"];\n\n";
  fout << "\tnode[shape=Mrecord]\n"
       << "\t\tmy1 [label=\"{Instanz/Auftrag Id/Zeilennummer | P1:(Datum,Status)/P2:(Datum,Status) | P1:Menge1/P2:Menge2}\"]\n\n"; 
}


struct st_edge{Node node;std::string bez;std::vector<std::pair<std::string,std::string> > child;
       st_edge(const Node &n,const std::string &b,const std::vector<std::pair<std::string,std::string> > &c)
         :node(n),bez(b),child(c) {}};

std::string aeb_to_string(AufEintragBase aeb)
{
  return aeb.str();
}

void dot_out::write_node(std::ofstream &fout)
{
  graph_data_node N(mode);
  write_filenames(fout,N.get_filenames());
  std::list<AufEintragBase> L=N.get_existing_aeb();

  std::vector<st_edge> vec_edge;

  for(std::list<AufEintragBase>::const_iterator i=L.begin();i!=L.end();++i)
   {
     graph_data_node::st_node_strings M=N.get_mengen_for_node(*i);
     Node node("my_"+itos(++cc),cc,M.auftrag);
     node.write(fout,M.auftrag,M.mengen,M.zusatz,M.artikel);
#if 0     
     std::vector<std::pair<std::string,std::string> >  E=N.get_edges_for(*i);
     std::vector<std::pair<std::string,std::string> > Vchild;
     for(std::vector<std::pair<std::string,std::string> >::const_iterator j=E.begin();j!=E.end();++j)
      {
        Vchild.push_back(std::pair<std::string,std::string>(j->first,j->second));
      }
#endif      
     vec_edge.push_back(st_edge(node,aeb_to_string(M.auftrag),N.get_edges_for(*i)));
   }


  for(std::vector<st_edge>::const_iterator i=vec_edge.begin();i!=vec_edge.end();++i)
   {
     for(std::vector<std::pair<std::string,std::string> >::const_iterator j=i->child.begin();j!=i->child.end();++j)
      {
        for(std::vector<st_edge>::const_iterator k=vec_edge.begin();k!=vec_edge.end();++k)
         {
          if(j->first==k->bez)
            {
              erank rank=None;
              AufEintragBase a1=i->node.Auftrag();
              AufEintragBase a2=k->node.Auftrag();
              if(a1.Instanz()==a2.Instanz()) rank=same;
              Edge(fout,i->node,k->node,j->second,rank);
            }
         }
      }
   }
}

void Node::write(std::ofstream &fout,AufEintragBase auftrag,
		const std::string &menge,
		const std::string &zusatz,const std::string &artikel)
{
  std::string shape="Mrecord";
  if(Auftrag().Id()>=CUSTOM_ORDERNO || Auftrag().Id()==1) shape="record";
  fout << "\n\tsubgraph p_"<<ClusterCount()<<" {\n"
          "\t\tnode [shape="<<shape<<"];\n"
          "\t\t"<<bezeichner<<" [label=\"{"<<aeb_to_string(auftrag)<<" | "<<artikel<<" | "<<zusatz<<" | "<<menge<<"}\"];\n"
          "\t\tfontsize=12;}\n";
}

void dot_out::Edge(std::ofstream &fout,Node n1,Node n2, std::string s,erank rank)
{
  fout << "\n\t"<<n1.Name()<<" -> "<<n2.Name()
       << " [fontsize=12,";
  if(rank==same) fout << "labeldistance=3.5,labelangle=90,";
  else fout << "labeldistance=2,labelangle=0,";
  fout << " labelfontsize=14";
  switch (colour) {
    case Black : fout << ",labelfontcolor=black"; break;
    case Colour: fout << ",labelfontcolor=coral"; break;
    default : std::cerr<< "Undefined Colour\n"; abort();
   }
  fout << ",headlabel=\""<<s<<"\"";
  if(rank==same) fout << ", weight=-1";
  fout <<"];\n";
  if(rank==same) 
    fout << "\t{rank=same;"<< n1.Name()<<" "<<n2.Name()<<"}\n";
}

void dot_out::write_header(std::ofstream &fout)
{
  fout << "#!/bin/bash\n\n"
          "dot -Tps << END > G.ps \n"
          "digraph G {\n"
          "\t\tsize=\"10.5,7.9\";\n"
//          "\t\tratio=compress;\n"
          "\t\tratio=fill;\n"
          "\t\trotate=90;\n"
          "\t\tcenter=true;\n";
}

void dot_out::write_footer(std::ofstream &fout,std::string label)
{
  fout << "\t\tlabel=\""<<label<<"\"\n"
          "\t\tlabelloc=top\n"
          "}\n"
          "END\n\n"
          "gv G.ps &\n\n";
}

#include <Misc/mystring.h>
void dot_out::write_filenames(std::ofstream &fout,const std::vector<graph_data_node::st_files> &filenames)
{
  std::string F="{";
  for(std::vector<graph_data_node::st_files>::const_iterator i=filenames.begin();i!=filenames.end();++i)
   {
     F+=i->filename+" | ";     
   }
  ManuProC::remove_last_from(F," | ");
  F+="}|{";
  for(std::vector<graph_data_node::st_files>::const_iterator i=filenames.begin();i!=filenames.end();++i)
   {
     F+=i->prefix+" | ";     
   }
  ManuProC::remove_last_from(F," | ");
  F+="}";
  fout << "\n\tnode [shape=record];\n"
       << "\t\tfilenames [label=\"" << F << "\"];\n\n";
}

