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
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Aux/Transaction.h>
#include "getopt.h"
#include <Auftrag/AufEintrag.h>
#include <unistd.h>


const static struct option options[]=   
{ { "aktion",     required_argument,      NULL, 'a' },
  { "Artikel_1",     required_argument,      NULL, 'l' }, 
  { "Artikel_2",     required_argument,      NULL, 'r' }, 
  { "menge",	required_argument, NULL, 'm'},
  { "prozess",	required_argument, NULL, 'p'},
  { "instanz",	required_argument, NULL, 'i'},
  { "help",	no_argument, NULL, '?'},
//  { "nur_auftrag", no_argument, NULL, 'A'},
 { NULL,      0,       NULL, 0 }
};

enum eaction {None, Delete,Create,BestellenBei};


int main(int argc,char *argv[])
{
// bool nur_auftrag=false;
 Petig::PrintUncaughtExceptions();
 try{
   Petig::dbconnect();

 eaction action=None;
 ArtikelBase AB1,AB2; 
 ArtikelBaum::faktor_t menge=0;
 ppsInstanz::ID newInstanz=ppsInstanzID::None;
 Prozess::ID proz=Prozess::default_id;
 int opt;
 
 if (string(argv[0]).find("delete")!=string::npos) action=Delete;
 else if (string(argv[0]).find("create")!=string::npos) action=Create;
 else if (string(argv[0]).find("bestellenbei")!=string::npos) action=BestellenBei;
 while ((opt=getopt_long(argc,argv,"a:l:r:m:i:p:?",options,NULL))!=EOF)
  { switch (opt)
    {  case 'a' : 
         { if(std::string("delete")==optarg) action=Delete;
           else if(std::string("create")==optarg) action=Create;
           else if(std::string("bestellenbei")==optarg) action=BestellenBei;
           else {cerr << "Option '-a' mit 'delete', 'create' oder 'bestellenbei' belegen.\n";}
           break;
         }
       case 'l' : AB1=ArtikelBase(atoi(optarg)); break;
       case 'r' : AB2=ArtikelBase(atoi(optarg)); break;
       case 'm' : menge=atof(optarg); break;
       case 'p' : proz=Prozess::ID(atoi(optarg)); break;
       case 'i' : newInstanz=ppsInstanz::ID(atoi(optarg)); break;
       USAGE:
       case '?' : std::cerr <<" USAGE: "<<argv[0];
          if (action==None) std::cerr <<" -a (delete|create|bestellenbei)";
          std::cerr << " -l<linker (Eltern-)Artikel> [-r<rechter (Kind-)Artikel> [-m<Menge> -p]|-i<Instanz>]\n";
          if (action==Delete || action==None)
             std::cerr<< " artikel_delete -l 219419 -r 219420\n";
          if (action==Create || action==None)
             std::cerr<< " artikel_create -l219419 -r219420 -p30 -m2\n";
          if (action==Create || action==None)
             std::cerr<< " artikel_bestellenbei -l 219419 -i 2\n";
          exit(1);
//       case 'A': nur_auftrag=true;
//          break;
    }
  }
 Transaction tr;
 switch(action) {
   case Delete : {
                   if(AB1.Id()==0 || AB2.Id()==0 ) {cerr << "AB1.Id()==0 || AB2.Id()==0\n"; exit(1);}
                   ArtikelBaum::delete_Artikel(getuid(),AB1,AB2);
                   cout << "Artikel "<<cH_ArtikelBezeichnung(AB1)->Bezeichnung()
                     << " stammt NICHT mehr von Artikel "
                     <<cH_ArtikelBezeichnung(AB2)->Bezeichnung()<<'\n';
                   break;
                 }
   case Create : { 
                   if(AB1.Id()==0 || AB2.Id()==0) {cerr << "AB1.Id()==0 || AB2.Id()==0\n"; exit(1);}
                   ArtikelBaum::RohArtikel ra(AB2,menge,proz);
                   try { ArtikelBaum::new_Artikel(getuid(),AB1,ra); }
                   catch (AufEintrag::NoAEB_Error &e)
                   {  cout << e.what() << '\n';
                   }
                   cout << "Artikel "<<cH_ArtikelBezeichnung(AB1)->Bezeichnung()
                     << " stammt von Artikel "
                     <<cH_ArtikelBezeichnung(AB2)->Bezeichnung()
                     << ". Menge: "<<menge<<'\n';
                   break;
                 }
   case BestellenBei : {
                   if(AB1.Id()==0 || newInstanz==ppsInstanzID::None ) {cerr << "AB1.Id()==0 || newInstanz==ppsInstanzID::None\n"; exit(1);}
                   ArtikelStamm::set_BestellenBei(AB1,newInstanz); 
                   cout << "Artikel "<<cH_ArtikelBezeichnung(AB1)->Bezeichnung()
                        <<" wird in der Instanz "<<cH_ppsInstanz(newInstanz)->Name()<<" bestellt\n";
                   break;
                   }
   default: goto USAGE;
  }
 tr.commit();

 }catch(SQLerror &e){std::cout << e<<'\n';}
 return 0;
}
