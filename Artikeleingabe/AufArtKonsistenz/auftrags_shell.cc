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

#include <Aux/exception.h>
#include <Aux/dbconnect.h>
#include <Aux/Transaction.h>
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AufEintragZu.h>
#include <Artikel/ArtikelStamm.h>
#include "getopt.h"
#include <unistd.h>


static const int auftargid_for_geplant=1;

const static struct option options[]=   
{ { "aktion",        required_argument,      NULL, 'x' },   
  { "Auftrag_1_ID",     required_argument,      NULL, 'A' },
  { "Auftrag_2_ID",     required_argument,      NULL, 'a' },
  { "Auftrag_1_Instanz",     required_argument,      NULL, 'I' },
  { "Auftrag_2_Instanz",     required_argument,      NULL, 'i' },
  { "Auftrag_1_ZNr",     required_argument,      NULL, 'Z' },
  { "Auftrag_2_ZNr",     required_argument,      NULL, 'z' },
  { "Artikel",   required_argument,      NULL, 'r' },
  { "Menge",     required_argument,      NULL, 'm' },
  { "Planmenge",     required_argument,      NULL, 'p' },
  { "Rekursiv",     no_argument,      NULL, 'R' },
 { NULL,      0,       NULL, 0 }
};

enum eaction {None, Delete,Create,Change};

void Problems(void *,AufEintrag::st_problems e)
{
  switch (e.art) {
     case AufEintrag::Geplant :
        std::cout << "WARNUNG: es werden "<<e.menge_input<<" in Auftrag "<<e.AEB
             << " geändert, aber es sind nur noch "<<e.menge_output
             << " nicht verplant\n\n";
        break;          
     case AufEintrag::Geliefert :
        std::cout << "WARNUNG: Auftrag "<<e.AEB<<" ist schon ausgeliefert worden, reduzieren nicht möglich\n";
        break;
     case AufEintrag::GeliefertFatal :
        std::cout << "ACHTUNG: Zuviel ausgeliefert, vollständige Änderung nicht möglich:\n"
             << "Auftrag "<<e.AEB<<"\tgewünschte Menge: "<<e.menge_input<<
             "\tgeänderte Menge: "<<e.menge_output<<'\n';
        break;
     case AufEintrag::Lager :
        std::cout << "Im Lager ist von Artikel "
             <<cH_ArtikelBezeichnung(AufEintrag(e.AEB).ArtId())->Bezeichnung()
             << " "<<abs(e.menge_input-e.menge_output)<<" freigeworden\n";
         break;
     case AufEintrag::Geplant_nolager :
       {
         AufEintrag AE(e.AEB);
         std::cout << "Artikel "<<cH_ArtikelBezeichnung(AE.ArtId())->Bezeichnung()
             << " ist in Instanz "<<e.AEB.Instanz()->Name()
             << " am "<<AE.LastEditDate().c_str() <<" geplant worden"
             << " Mengenreduzierung nicht möglich.\n";
         break;
       }
   }
}


int main(int argc,char *argv[])
{
 Petig::PrintUncaughtExceptions();
 try{
   Petig::dbconnect();

 bool rekursiv=false;
 eaction action=None;
 if (std::string(argv[0]).find("delete")!=std::string::npos) action=Delete;
 else if (std::string(argv[0]).find("create")!=std::string::npos) action=Create;
 else if (std::string(argv[0]).find("change")!=std::string::npos) action=Change;
 int Z=-1,z=-1,A=-1,a=-1;
 ppsInstanz::ID I=ppsInstanzID::None,i=ppsInstanzID::None;
 AuftragBase::mengen_t menge=0,planmenge=0;
 ArtikelBase ArtBase;
 int opt;
 while ((opt=getopt_long(argc,argv,"x:A:a:I:i:Z:z:r:m:p:?R",options,NULL))!=EOF)
  { switch (opt)
    {  case 'x' :
         { if(std::string("delete")==optarg) action=Delete;
           else if(std::string("create")==optarg) action=Create;
           else if(std::string("change")==optarg) action=Change;
           else {std::cerr << "Option '-x' mit 'delete', 'create' oder 'change' belegen.\n";}
           break;
         }
       case 'A' : A=atoi(optarg); break;
       case 'a' : a=atoi(optarg); break;
       case 'I' : I=ppsInstanz::ID(atoi(optarg)); break;
       case 'i' : i=ppsInstanz::ID(atoi(optarg)); break;
       case 'Z' : Z=atoi(optarg); break;
       case 'z' : z=atoi(optarg); break;
       case 'r' : ArtBase=ArtikelBase(atoi(optarg)); break;
       case 'm' : menge=atoi(optarg); break;
       case 'p' : planmenge=atoi(optarg); break;
       case 'R' : rekursiv=true; break;
       USAGE:
       case '?' : std::cerr <<" USAGE: "<<argv[0];
          if (action==None) std::cerr <<" -a (delete|create|change)";
          std::cerr << " -A<(Eltern-)Auftrag Nr> -I<(Eltern-)Auftrag Instanz> -Z<(Eltern-)Auftrag Zeilennr> -a<(Kind-)Auftrag Id> -i<(Kind-)Auftrag Instanz> -z<(Kind-)Auftrag Zeilennr> [-r<Artikel>] [-m<Menge> ] [-R(rekursiv)]\n";
          if (action==Delete || action==None)
             std::cerr<< " auftrags_shell -x delete -A21001 -I1 -Z15 -a20001 -i10 -z137\n";
          if (action==Create || action==None)
             std::cerr<< " auftrags_shell -x create -A21001 -I1 -Z15 -r22341 -m1000 -p700\n";
          if (action==Change || action==None)
             std::cerr<< " auftrags_shell -x change -A21001 -I1 -Z15 -a20001 -i10 -z137 -m1000 -R\n";
          exit(1);
     }
  }
 AufEintragBase AEB1(I,A,Z);
 AufEintragBase AEB2(i,a,z);
 Transaction tr;  
 bool ok=false;
 switch(action) { 
   case Delete : ok=AufEintragZu::remove(AEB1,AEB2); break;
   case Create : try { AufEintrag AE1(AEB1);
                       AufEintragBase Planauftrag;
                       int znr;
                       if(AEB1.Instanz()==ppsInstanzID::Kundenauftraege)
                        {
//                          AuftragBase AB(ArtikelStamm(ArtBase).BestellenBei(),AuftragBase::ungeplante_id);
                          AuftragBase AB(ppsInstanz::getBestellInstanz(ArtBase),AuftragBase::ungeplante_id);
                          znr=AB.tryUpdateEntry(menge,AE1.getLieferdatum(),ArtBase,AE1.getEntryStatus(),getuid(),AEB1,true);
                          Planauftrag=AufEintragBase(AB,znr);
                        }
                       else 
                        {
                          znr=AEB1.tryUpdateEntry(menge,AE1.getLieferdatum(),ArtBase,AE1.getEntryStatus(),getuid(),AEB1,true);
                          Planauftrag=AufEintragBase(AEB1,znr);
                        }
                    if(planmenge)
                     {
                       AuftragBase ab(Planauftrag.Instanz(),AuftragBase::plan_auftrag_id);
#warning ungetetstet
//                       AufEintrag(Planauftrag).Planen(getuid(),planmenge,true,AE1.getLieferdatum(),true);
                     }
                    ok=true;
                    break;
                   }catch(AufEintrag::NoAEB_Error &e){std::cout << e.what()<<"\nUngültiger Elternauftrag\n\nIst das Programm 'c++/Programme/Instanzen' vorher ausgeführt worden?\n\n"; goto USAGE;}
   case Change : try{ 
                      AufEintrag AE2(AEB2);
                 AufEintrag AE1(AEB1);
                 AuftragBase::mengen_t M=AufEintragZu(AEB1).getMenge(AEB2); 
                 AuftragBase::mengen_t mt=AE2.updateStkDiff__(getuid(),menge-M,true,AufEintragBase::r_Standard);
                 ok=AufEintragZu(AEB1).setMenge(AEB2,menge); 
//cout <<"shell: "<< mt<<' '<<AE2.getStueck()<<' '<<menge<<'\t'<<ok<<'\n';
//                 ok=true;
                 break;
                   }catch(AufEintrag::NoAEB_Error &e){std::cout << e.what()<<"\nUngültiger Elternauftrag\n"; goto USAGE;}
   default: goto USAGE;
  }
 if(!ok) std::cerr << "Fehlgeschlagen\n";
 tr.commit();
 }catch(SQLerror &e){std::cout << e<<'\n';}
 return 0;
}

/*
void rekursiv(AufEintrag &AE,AuftragBase::mengen_t menge)
{
 std::list<AufEintragZu::st_reflist> LU=AufEintragZu(AE).get_Referenz_list_ungeplant();
 std::list<AufEintragZu::st_reflist> LG=AufEintragZu(AE).get_Referenz_list_geplant();
 AuftragBase::mengen_t verplante_menge=0;
 if(!LG.empty) 
  {
   std::cerr << "WARNUNG: Für "<<AE.Instanz()->Name()<<' '<<AE.Id()<<' '<<AE.ZNr()<<'\n'
        << " wurde schon etwas geplant (daran wird nichts geändert):"
   for(std::list<AufEintragZu::st_reflist>::const_iterator i=LG.begin();i!=LG.end();++i)
    {
      std::cerr <<" Auftrag: " <<i->AEB.Instanz()->Name()<<'/'<<i->AEB.Id()<<'/'<<i->AEB.ZNr()
           <<" Menge:<<i->menge<<'\n';
      AuftragBase::mengen_t M=AufEintrag(i->AEB).getStueck();
      assert(i->menge==M)
      verplante_menge+=M;
    }
  } 
 for(std::list<AufEintragZu::st_reflist>::const_iterator i=LU.begin();i!=LU.end();++i)
  {
   i->AEB
  }
}
*/