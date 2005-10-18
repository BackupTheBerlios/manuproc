// $Id: get_data.cc,v 1.57 2005/10/18 21:46:30 christof Exp $
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
#include <Misc/relops.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Misc/TraceNV.h>
#include <algorithm>
#include <Auftrag/SelectedFullAufList.h>

static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channele("DEBUG",trace_channel);

#ifdef  MANU_PROC_TEST
static std::string referenzdir="../database_tables_test_ManuProC";
#elif defined MABELLA_TEST
static std::string referenzdir="../database_tables_test_Mabella";
#elif defined PETIG_TEST
static std::string referenzdir="../database_tables_test";
#endif

bool graph_data_node::show_referenz;
bool graph_data_node::dont_hide_empty;
unsigned graph_data_node::limit=~0;
unsigned graph_data_node::start=0;
unsigned graph_data_node::article=0;

graph_data_node::graph_data_node(const std::string &mode)
{
  if (mode.empty())
  { graphtitle=getenv("PGDATABASE");
    filenames.push_back(st_files(ManuProC::Datum::today().c_str(),""));
    vec_files_auftragentry.push_back(st_files("",""));
    vec_files_auftragsentryzuordnung.push_back("");
    if (article)
    { SelectedFullAufList auftraglist=SelectedFullAufList(SQLFullAuftragSelector::
          sel_Artikel(ppsInstanzID::None,ArtikelBase(article)));
      for (SelectedFullAufList::iterator j=auftraglist.begin();j!=auftraglist.end();++j)
      { if (!dont_hide_empty && (j->getCombinedStatus()>OPEN || !j->getStueck())) 
          continue;
        list_auftrag.push_back(st_auftrag(*j,j->getStueck(),j->getGeliefert(),
            j->getCombinedStatus(),j->getLieferdatum(),"",j->Artikel()));
        AufEintragZu::list_t kinder=AufEintragZu::get_Referenz_list
            (*j,AufEintragZu::list_kinder,AufEintragZu::list_Artikel);
        for (AufEintragZu::list_t::const_iterator i=kinder.begin();i!=kinder.end();++i)
        { if (i->Art!=ArtikelBase(article)) continue; // oder hinzufÃ¼gen?
          list_auftragszuordnung.push_back(st_aebZ(*j,i->AEB,i->Menge,0));
        }
      }
      fill_map();
    }
    else
      std::cerr << "kein Artikel angegeben ... noch nicht implementiert\n";
  }
  else
  {get_files(mode);
   get_values_from_files();
   fill_map();
   get_values_from_files_Z();
  }
}

bool operator==(const graph_data_node::st_auftrag &a, const AufEintragBase &b)
{  return a.auftrag==b;
}

#if __GNUC__ < 3
template <class A,class B> bool operator!=(const A &a,const B &b) { return !(a==b); }
#endif

void graph_data_node::get_values_from_files()
{
  for(std::vector<st_files>::const_iterator i=vec_files_auftragentry.begin();i!=vec_files_auftragentry.end();++i)
   {
     std::ifstream F((i->filename).c_str());
     if(!F) {std::cout <<"FEHLER: "<< i->filename<<" kann nicht geöffnet werden: "<<F<<"\n"; exit(1);}
     std::string zeile;
     for(int j=0;j<2;++j) std::getline(F,zeile); // Kopfzeilen überlesen
     while(std::getline(F,zeile))
      {
        cH_ppsInstanz instanz(ppsInstanzID::None);
        int id,znr;
        AuftragBase::mengen_t bestellt,geliefert;
        ManuProC::Datum datum;
        AufStatVal status=NOSTAT;
        ArtikelBase artikel;
        std::string::size_type s1;
        for(int j=0;j<10;++j)
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
           else if (j==9) artikel=ArtikelBase(atoi(zeile.substr(0,s1).c_str()));
           zeile=zeile.substr(s1+1);
          }
        if(instanz==ppsInstanzID::None) continue;
        AufEintragBase aeb(instanz,id,znr);

	bool schon_drin=false;
	if (!dont_hide_empty && bestellt==geliefert)
	{  schon_drin=std::find(list_auftrag.begin(),list_auftrag.end(),aeb)!=list_auftrag.end();
	}
	if (dont_hide_empty || bestellt!=geliefert || schon_drin)
           list_auftrag.push_back(st_auftrag(aeb,bestellt,geliefert,status,datum,i->prefix,artikel));
      }
   }
}

namespace {
struct st_comp
{	AufEintragBase aebALT, aebNEU;
	st_comp(AufEintragBase a,AufEintragBase n) : aebALT(a), aebNEU(n) {}
};
 }

bool operator==(const graph_data_node::st_aebZ &a, const st_comp &b)
{  return a.aebALT==b.aebALT && a.aebNEU==b.aebNEU;
}

void graph_data_node::get_values_from_files_Z()
{ unsigned fileindex=0;
  for(std::vector<std::string>::const_iterator i=vec_files_auftragsentryzuordnung.begin();
  		i!=vec_files_auftragsentryzuordnung.end();++i,++fileindex)
   { 
     std::ifstream F((*i).c_str());
     if(!F) {std::cout <<"FEHLER: "<< *i<<" kann nicht geöffnet werden\n"; exit(1);}
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
           if     (j==0) { int i=atoi(zeile.substr(0,s1).c_str());
                           if(i!=0) instanzALT=cH_ppsInstanz(ppsInstanz::ID(i));
                  }
           else if(j==1) idALT=atoi(zeile.substr(0,s1).c_str());
           else if(j==2) znrALT=atoi(zeile.substr(0,s1).c_str());         
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

	bool schon_drin=false;
	if (!dont_hide_empty && !menge)
	{  schon_drin=std::find(list_auftragszuordnung.begin(),list_auftragszuordnung.end(),st_comp(aebALT,aebNEU))
		!=list_auftragszuordnung.end();
	}
	if (dont_hide_empty || !!menge || schon_drin)
          list_auftragszuordnung.push_back(st_aebZ(aebALT,aebNEU,menge,fileindex));
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
  std::string M,Mmem,Z,Zmem,artikel_str;
  ArtikelBase artikel;
  std::vector<st_auftrag> &V=map_aeb[aeb];
  for(std::vector<st_auftrag>::const_iterator j=V.begin();j!=V.end();++j)
    { std::string prefix=j->prefix;
      if (!prefix.empty()) prefix+=':';
      
      std::string m=j->bestellt.String();
      if(j->geliefert!=AuftragBase::mengen_t(0)) m+="("+j->geliefert.String()+")";
      if (!M.empty()) M+="/";
      if(Mmem != m) M+=prefix+m;
      Mmem=m;

      std::string z="("+std::string(j->datum.c_str())+","+itos(j->status)+")";
      if (!Z.empty()) Z+="/";
      if(Zmem != z )  Z+=prefix+z ;
//      Z+="/";
      Zmem=z;
      artikel=j->artikel;
    }
  if (!artikel) artikel_str="?";
  else artikel_str=cH_ArtikelBezeichnung(artikel)->Bezeichnung();
  return st_node_strings(aeb,M,Z,artikel_str);
}

std::vector<std::pair<std::string,std::string> > graph_data_node::get_edges_for(AufEintragBase aeb)
{
  std::list<st_child> list_child;
  for(std::list<st_aebZ>::const_iterator i=list_auftragszuordnung.begin();i!=list_auftragszuordnung.end();++i)
   {
     if(i->aebALT==aeb) list_child.push_back(st_child(i->aebNEU,i->menge,i->fileindex));
   }

  std::vector<std::pair<std::string,std::string> > V;
  if(!list_child.empty()) 
 {// nach ziel sortieren
  list_child.sort(); // spannend, dass dies die fileindex Reihenfolge nicht stört ... CP
  AufEintragBase aeb_mem=list_child.front().aeb;
  std::vector<std::string> S(1);
  std::vector<AuftragBase::mengen_t> Mmem(1);
  std::vector<unsigned> FImem(1);
  unsigned fileindex_mem=unsigned(-1);
  unsigned index=0;
  FImem[0]=unsigned(-3);
  ManuProC::Trace _t(trace_channel, __FUNCTION__,aeb);
  for(std::list<st_child>::const_iterator i=list_child.begin();i!=list_child.end();++i)
   { ManuProC::Trace _t(trace_channel, "for",NV("fileindex",i->fileindex),
   		NV("menge",i->menge),NV("aeb",i->aeb));
     if(aeb_mem==i->aeb) 
      { if (fileindex_mem==i->fileindex) 
        { ++index; 
          if (S.size()<index+1) S.resize(index+1);
          if (Mmem.size()<index+1) Mmem.resize(index+1);
          if (FImem.size()<index+1) { FImem.resize(index+1); FImem[index]=-3; }
        }
        else
        { index=0; fileindex_mem=i->fileindex; }
        
        bool need_value=false;
        if (FImem[index]+1!=i->fileindex)
        {  if (!S[index].empty()) S[index]+=' ';
           if (!vec_files_auftragentry[i->fileindex].prefix.empty())
              S[index]+=vec_files_auftragentry[i->fileindex].prefix+":";
           need_value=true;
        }
        else
        {  S[index]+="/";
        }
        FImem[index]=i->fileindex;
        if(need_value || Mmem[index] != i->menge)
        {  S[index]+=i->menge.String();
           Mmem[index]=i->menge;
        }
      }
     else
      { std::string S2;
        for (std::vector<std::string>::const_iterator j=S.begin();j!=S.end();++j)
        {  if (!S2.empty()) S2+="\\n";
           S2+=*j;
        }
        V.push_back(std::pair<std::string,std::string>(aeb_mem.str(),S2));

	S.clear();
	std::string n=i->menge.String();
	if (!vec_files_auftragentry[i->fileindex].prefix.empty())
	   n=vec_files_auftragentry[i->fileindex].prefix+":"+n;
        S.push_back(n);
        Mmem.clear();
        Mmem.push_back(i->menge);
        FImem.clear();
        FImem.push_back(i->fileindex);
        aeb_mem=i->aeb;
        fileindex_mem=i->fileindex;
        index=0;
      }
   }
   std::string S2;
        for (std::vector<std::string>::const_iterator i=S.begin();i!=S.end();++i)
        {  if (!S2.empty()) S2+="\\n";
           S2+=*i;
        }
        V.push_back(std::pair<std::string,std::string>(aeb_mem.str(),S2));
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
{ std::ifstream i(("../files.log/"+mode).c_str());
  if (!i.good()) 
  {  std::cerr << "../files.log/"<<mode<<": konnte Datei nicht öffnen\n";
     exit(1);
  }
  char buf[1024];
  unsigned int linenum=0;
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
     if (linenum>=start && linenum<limit)
     {  filenames.push_back(st_files(line.substr(space1+1,space2-space1-1),
     		line.substr(space2+1,space3-space2-1)));
     }
     ++linenum;
  }
  std::string dir=referenzdir;
  if (!show_referenz) dir="../results";
  for(std::vector<st_files>::const_iterator i=filenames.begin();i!=filenames.end();++i)
   {
     vec_files_auftragentry.push_back(st_files(dir+"/auftragentry_"+i->filename,i->prefix));
     vec_files_auftragsentryzuordnung.push_back(dir+"/auftragsentryzuordnung_"+i->filename);
   }
}

