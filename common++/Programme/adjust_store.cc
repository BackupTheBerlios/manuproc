// $Id: adjust_store.cc,v 1.4 2002/10/04 13:57:49 thoma Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2002 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "adjust_store.h"
#include <Aux/dbconnect.h>
#include <Aux/exception.h>
#include <getopt.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <Lager/JumboLager.h>
#include <Lager/RohwarenLager.h>
#include <unistd.h>
//debug:
#include <Artikel/ArtikelBezeichnung.h>

const static struct option options[]=
{{ "instanz", required_argument, NULL, 'i' },
 { "database", required_argument,      NULL, 'd' },
 { "dbhost", required_argument,      NULL, 'h' },  
 { NULL,      0,       NULL, 0 }
};

int main(int argc,char *argv[])
{
  int opt;
  ppsInstanz::ID instanz= ppsInstanzID::None;
  std::string database="";
  std::string dbhost="";  

  if(argc==1) exit(1);
  while ((opt=getopt_long(argc,argv,"h:d:i:",options,NULL))!=EOF)
   {
    switch(opt)
     {
       case 'i' : instanz = ppsInstanz::ID(atoi(optarg));break;
       case 'd' : database=optarg;break;
       case 'h' : dbhost=optarg;break;  
       case '?' : cout << "USAGE: "<<argv[0]<<" -i <instanz> [-d <Datenbank>] [-h <dbhost>]\n";
                  exit(1);              
     }
   }

  ManuProC::PrintUncaughtExceptions();
  try{
    ManuProC::Connection conn;
    conn.setHost(dbhost);
    conn.setDbase(database);
    ManuProC::dbconnect(conn);

    adjust_store AS=adjust_store(cH_ppsInstanz(instanz));

    ManuProC::dbdisconnect();
  }catch(SQLerror &e){std::cout << e<<'\n'; return 1;}
  return 0;
}

adjust_store::adjust_store(cH_ppsInstanz _instanz)
: instanz(_instanz)
{
  if(!instanz->LagerInstanz())
   {
     cout << "Fehler: "<<instanz->Name() <<" ist keine Lagerinstanz\n";
     exit(1);
   }
  getLagerInhalt();  
  vormerkungen_subrahieren();
  FreieMengenAuftraege_anlegen();
}

void adjust_store::FreieMengenAuftraege_anlegen()
{
 AuftragBase da(instanz,AuftragBase::dispo_auftrag_id);
 da.create_if_not_exists(OPEN);
 for(std::vector<st_artmeng>::const_iterator i=UnverplanteArtikel.begin();i!=UnverplanteArtikel.end();++i)
  {
   if(!i->menge) continue;
   H_Lager L(instanz);
   bool alt=L->dispo_auftrag_aendern(i->artikel,i->menge);

   if(alt)   
     {
      cout <<"Update von "<<cH_ArtikelBezeichnung(i->artikel)->Bezeichnung()
           <<"\tum "<<i->menge<<'\n';
     }
   else
     {
      cout <<"Neuanlegen von "<<cH_ArtikelBezeichnung(i->artikel)->Bezeichnung()
           <<"\tmit "<<i->menge<<'\n';
     }
  }

}

void adjust_store::vormerkungen_subrahieren()
{
cout << "Anzahl der Artikel im Lager = "<<LI.size()<<'\n';
  for(std::vector<LagerInhalt>::const_iterator i=LI.begin();i!=LI.end();++i)
   {
     AuftragBase::mengen_t menge=i->GesamtMenge();
//cout <<i->Artikel().Id()<<' '<<cH_ArtikelBezeichnung(i->Artikel())->Bezeichnung()
//  <<'\t'<<menge<<'\n';

     // Vorgemerkte Menge (1er Aufträge) wieder abziehen
     // Schon eingetragene Menge wieder abziehen
     SelectedFullAufList auftraglist1=SelectedFullAufList(SQLFullAuftragSelector::
       sel_Artikel_Planung_id(instanz->Id(),ManuProC::DefaultValues::EigeneKundenId,i->Artikel(),AuftragBase::plan_auftrag_id));
     // der Selector holt nur die Aufträge mit dem Status OPEN
     for (SelectedFullAufList::const_iterator j=auftraglist1.begin();j!=auftraglist1.end();++j)
       {
         if(j->Id()!=AuftragBase::plan_auftrag_id) assert(!"never get here");
         menge-=j->getRestStk() ;
//cout << "Planung abziehne "<<AufEintragBase(*j)<<'\t'<<j->getRestStk()<<'\t'<<menge<<'\n';
         assert(menge>=0);
       }
     SelectedFullAufList auftraglist2=SelectedFullAufList(SQLFullAuftragSelector::
          sel_Artikel_Planung_id(instanz->Id(),ManuProC::DefaultValues::EigeneKundenId,i->Artikel(),AuftragBase::dispo_auftrag_id));
     // der Selector holt nur die Aufträge mit dem Status OPEN
     for (SelectedFullAufList::const_iterator j=auftraglist2.begin();j!=auftraglist2.end();++j)
      {
         if(j->Id()!=AuftragBase::dispo_auftrag_id) assert(!"never get here");
         assert(j->getStueck()==j->getRestStk());
         menge-=j->getRestStk();
//cout << "Dispo abziehne "<<AufEintragBase(*j)<<'\t'<<j->getRestStk()<<'\t'<<menge<<'\n';
      }
//cout << "Endmenge: "<<menge<<'\n';

     UnverplanteArtikel.push_back(st_artmeng(i->Artikel(),menge));
   }
}




void adjust_store::getLagerInhalt()
{
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
  if(instanz == ppsInstanzID::Rohwarenlager)
     LI=RohwarenLager().LagerInhalt();
  else if(instanz == ppsInstanzID::Bandlager)
     LI=JumboLager().LagerInhalt();
  else 
#endif
     assert (!"Lager nicht implementiert\n");
  Lager::LagerInhaltSum(LI);
}


