// $Id: get_data.cc,v 1.39 2003/01/06 17:26:44 christof Exp $
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

#include "get_data.h"
#include <fstream>
#include "../steuerprogramm.hh"

#ifdef  MANU_PROC_TEST
static std::string referenzdir="../database_tables_test_ManuProC";
#elif defined MABELLA_TEST
static std::string referenzdir="../database_tables_test_Mabella";
#elif defined PETIG_TEST
static std::string referenzdir="../database_tables_test";
#endif

bool graph_data_node::show_referenz;

graph_data_node::graph_data_node(const std::string &mode)
{
   get_files(mode);
   get_values_from_files();
   fill_map();
   get_values_from_files_Z();
}

void graph_data_node::get_values_from_files()
{
  for(std::vector<st_files>::const_iterator i=vec_files_auftragentry.begin();i!=vec_files_auftragentry.end();++i)
   {
     std::ifstream F((i->filename).c_str());
     if(!F) {cout <<"FEHLER: "<< i->filename<<" kann nicht geöffnet werden: "<<F<<"\n"; exit(1);}
     std::string zeile;
     for(int j=0;j<2;++j) std::getline(F,zeile); // Kopfzeilen überlesen
     while(std::getline(F,zeile))
      {
        cH_ppsInstanz instanz(ppsInstanzID::None);
        int id,znr;
        AuftragBase::mengen_t bestellt,geliefert;
        Petig::Datum datum;
        AufStatVal status=NOSTAT;
        std::string::size_type s1;
        for(int j=0;j<8;++j)
          {
           std::string trenner;
           if(j<2) trenner="-";
           else trenner ="|";
           s1=zeile.find(trenner);
           if     (j==0) { int i=atoi(zeile.substr(0,s1).c_str());
                           if(i!=0) instanz=cH_ppsInstanz(ppsInstanz::ID(i));
                  }
           else if(j==1) id=atoi(zeile.substr(0,s1).c_str());
           else if(j==2) znr=atoi(zeile.substr(0,s1).c_str());         
           else if(j==3) bestellt=atof(zeile.substr(0,s1).c_str());         
           else if(j==4) geliefert=atof(zeile.substr(0,s1).c_str());         
           else if(j==5) try{  datum.from_postgres(zeile.substr(0,s1).c_str());         
               }catch(ManuProC::Datum::Formatfehler &e) {}
           else if(j==6) status=AufStatVal(atoi(zeile.substr(0,s1).c_str()));         
           zeile=zeile.substr(s1+1,std::string::npos);
          }
        if(instanz==ppsInstanzID::None) continue;
        AufEintragBase aeb(instanz,id,znr);

        list_auftrag.push_back(st_auftrag(aeb,bestellt,geliefert,status,datum,i->prefix));
      }
   }
}

void graph_data_node::get_values_from_files_Z()
{
  for(std::vector<std::string>::const_iterator i=vec_files_auftragsentryzuordnung.begin();i!=vec_files_auftragsentryzuordnung.end();++i)
   {
     std::ifstream F((*i).c_str());
     if(!F) {cout <<"FEHLER: "<< *i<<" kann nicht geöffnet werden\n"; exit(1);}
     std::string zeile;
     for(int j=0;j<2;++j) std::getline(F,zeile); // Kopfzeilen überlesen
     while(std::getline(F,zeile))
      {
        cH_ppsInstanz instanzALT(ppsInstanzID::None);
        cH_ppsInstanz instanzNEU(ppsInstanzID::None);
        int idALT,znrALT,idNEU,znrNEU;
        AuftragBase::mengen_t menge;
        std::string::size_type s1;
        for(int j=0;j<6;++j)
         {
           std::string trenner;
           if(j==2||j==5) trenner="|";
           else trenner ="-";
           s1=zeile.find(trenner);
//           if     (j==0) instanzALT=cH_ppsInstanz(ppsInstanz::ID(atoi(zeile.substr(0,s1).c_str())));
           if     (j==0) { int i=atoi(zeile.substr(0,s1).c_str());
                           if(i!=0) instanzALT=cH_ppsInstanz(ppsInstanz::ID(i));
                  }
           else if(j==1) idALT=atoi(zeile.substr(0,s1).c_str());
           else if(j==2) znrALT=atoi(zeile.substr(0,s1).c_str());         
//           else if(j==3) instanzNEU=cH_ppsInstanz(ppsInstanz::ID(atoi(zeile.substr(0,s1).c_str())));
           if     (j==3) { int i=atoi(zeile.substr(0,s1).c_str());
                           if(i!=0) instanzNEU=cH_ppsInstanz(ppsInstanz::ID(i));
                  }
           else if(j==4) idNEU=atoi(zeile.substr(0,s1).c_str());
           else if(j==5) znrNEU=atoi(zeile.substr(0,s1).c_str());         
           zeile=zeile.substr(s1+1,std::string::npos);
         }
        menge=atof(zeile.c_str()); 
        if(instanzALT==ppsInstanzID::None) continue;
        AufEintragBase aebALT(instanzALT,idALT,znrALT);
        AufEintragBase aebNEU(instanzNEU,idNEU,znrNEU);
//cout <<"XX\t"<< aebALT<<'\t'<<aebNEU<<'\t'<<menge<<'\n';
        list_auftragszuordnung.push_back(st_aebZ(aebALT,aebNEU,menge));
      }
   }
}




void graph_data_node::fill_map()
{
  list_auftrag.sort();
  AufEintragBase aeb_mem;
  if(!list_auftrag.empty()) aeb_mem=list_auftrag.front().auftrag;
  std::vector<st_auftrag> V;
  for(std::list<st_auftrag>::const_iterator i=list_auftrag.begin();i!=list_auftrag.end();++i)
   {
     if(aeb_mem!=i->auftrag) 
      {
        map_aeb[aeb_mem]=V;
        aeb_mem=i->auftrag;
        V.clear();
      }
     V.push_back(*i);
   }
  if(!list_auftrag.empty())
   {
     map_aeb[aeb_mem]=V;
   }
}

graph_data_node::st_node_strings graph_data_node::get_mengen_for_node(AufEintragBase aeb)
{
//  std::string A=aeb.Instanz()->Name()+"/"+itos(aeb.Id())+"/"+itos(aeb.ZNr());
  std::string M,Mmem,Z,Zmem;
  std::vector<st_auftrag> V=map_aeb[aeb];
  for(std::vector<st_auftrag>::const_iterator j=V.begin();j!=V.end();++j)
    {
      if(j->prefix!="") {M+=j->prefix+":"; Z+=j->prefix+":";}
      
      std::string m=j->bestellt.String();
      if(j->geliefert!=AuftragBase::mengen_t(0)) m+="("+j->geliefert.String()+")";
      if(Mmem != m) M+=m;
      M+=+"/";
      Mmem=m;

      std::string z="("+string(j->datum.c_str())+","+itos(j->status)+")";
      if(Zmem != z )  Z+=z ;
      Z+="/";
      Zmem=z;
    }
  std::string::size_type st1=M.find_last_of("/");
  if(st1!=std::string::npos) M.erase(st1,1);
  std::string::size_type st2=Z.find_last_of("/");
  if(st2!=std::string::npos) Z.erase(st2,1);
  return st_node_strings(aeb,M,Z);
}

std::vector<pair<std::string,std::string> > graph_data_node::get_edges_for(AufEintragBase aeb)
{
  std::list<st_child> list_child;
  for(std::list<st_aebZ>::const_iterator i=list_auftragszuordnung.begin();i!=list_auftragszuordnung.end();++i)
   {
     if(i->aebALT==aeb) list_child.push_back(st_child(i->aebNEU,i->menge));
   }

  list_child.sort();
  std::vector<pair<std::string,std::string> > V;
  AufEintragBase aeb_mem;
  if(!list_child.empty()) aeb_mem=list_child.front().aeb;
  std::string S,Mmem;
  for(std::list<st_child>::const_iterator i=list_child.begin();i!=list_child.end();++i)
   {
//cout <<"X\t" <<aeb<<'\t'<<i->aeb<<'\t'<<i->menge<<'\n';
     if(aeb_mem==i->aeb) 
      {
        std::string m=i->menge.String();
        if(Mmem != m) S+=m;
        S+="/";
        Mmem=m;
      }
     else
      {
        std::string A=aeb_mem.Instanz()->Name()+"/"+itos(aeb_mem.Id())+"/"+itos(aeb_mem.ZNr());
        std::string::size_type st1=S.find_last_of("/");
        if(st1!=std::string::npos) S.erase(st1,1);
        V.push_back(pair<std::string,std::string>(A,S));

        S=i->menge.String()+"/";
        Mmem=i->menge.String();
        aeb_mem=i->aeb;
      }
   }
 if(!list_child.empty())
  {
    std::string A=aeb_mem.Instanz()->Name()+"/"+itos(aeb_mem.Id())+"/"+itos(aeb_mem.ZNr());
    std::string::size_type st1=S.find_last_of("/");
    if(st1!=std::string::npos) S.erase(st1,1);
    V.push_back(pair<std::string,std::string>(A,S));
  }
 return V;
}

std::list<AufEintragBase> graph_data_node::get_existing_aeb() const
{
  std::list<AufEintragBase> LA;
  std::list<st_auftrag> L=list_auftrag;
  L.sort();
  for(std::list<st_auftrag>::const_iterator i=L.begin();i!=L.end();++i)
   {
     LA.push_back(i->auftrag);
   }
  LA.unique();
  return LA;
}



void graph_data_node::get_files(const std::string &mode)
{ ifstream i(("../files.log/"+mode).c_str());
  if (!i.good()) 
  {  std::cerr << "../files.log/"<<mode<<": konnte Datei nicht öffnen\n";
     exit(1);
  }
  char buf[1024];
  while (!i.eof())
  {  i.getline(buf,sizeof buf);
     std::string line=buf;
     if (line.empty()) continue;
     if (line[0]=='#' && line.size()>=2)
     {  graphtitle=line.substr(2);
        continue;
     }
     std::string::size_type space1,space2,space3;
     space1=line.find(' ');
     if (space1==std::string::npos) continue;
     space2=line.find(' ',space1+1);
     if (space2==std::string::npos) continue;
     space3=line.find(' ',space2+1);
     if (space3==std::string::npos) continue;
     filenames.push_back(st_files(line.substr(space1+1,space2-space1-1),
     		line.substr(space2+1,space3-space2-1)));
//std::cerr << line.substr(space1+1,space2-space1-1) << ',' << line.substr(space2+1) << ",\n";
  }
  std::string dir=referenzdir;
  if (!use_referenz) dir="../results";
  for(std::vector<st_files>::const_iterator i=filenames.begin();i!=filenames.end();++i)
   {
     vec_files_auftragentry.push_back(st_files(dir+"/auftragentry_"+i->filename,i->prefix));
     vec_files_auftragsentryzuordnung.push_back(dir+"/auftragsentryzuordnung_"+i->filename);
   }
}

