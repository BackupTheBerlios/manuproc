// $Id: grafik_auftragsbaum.cc,v 1.2 2002/06/27 07:42:31 cvs_christof Exp $
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

#include <Aux/dbconnect.h>
#include <Aux/exception.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AufEintragZu.h>

void show_auftrag(const AufEintragBase &AEB,int ident);


void show_kinder(const AufEintragBase &aeb,int ident)
{
  AufEintragZu AEZ(aeb);
  std::list<AufEintragZu::st_reflist> LK=AEZ.get_Referenz_list(aeb,true);
//  for (int i=0;i<ident;++i) cout << "  ";
//  cout << "Kinder von "<< aeb<<":\n";
  ident+=1;
  vector<AufEintragBase> VAEB;
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=LK.begin();i!=LK.end();++i)
   {
     for (int j=0;j<ident;++j) cout << "  ";
     cout <<"Zu: "<<aeb<<" ==> "<<i->AEB <<" ("<<i->Menge<<")\n";
     show_auftrag(i->AEB,ident);
//     VAEB.push_back(i->AEB);
   }
//  for(vector<AufEintragBase>::const_iterator i= VAEB.begin();VAEB.end();++i)
//     show_auftrag(*i,ident);
//  cout << '\n';
}


void show_auftrag(const AufEintragBase &AEB,int ident)
{
  try{ 
   AufEintrag AE(AEB);
   for (int i=0;i<ident;++i) cout << "  ";
   cout << AE <<"\tMenge:"<<AE.getStueck()<<"  Rest:"<<AE.getRestStk()
        <<"  EntryStatus:"<<AE.getEntryStatus()<<'\n';
   show_kinder(AEB,ident);
   }catch(AufEintrag::NoAEB_Error &e)
    { cerr <<AEB<<" existiert nicht\n";
    }    
}


int main(int argc,char* argv[])
{  
   ManuProC::PrintUncaughtExceptions();
   try{
   ManuProC::dbconnect();
   if(argc!=2)
    {
      cerr << argv[0] << " muß mit Kundenauftrags ID aufgerufen werden.\n";
      exit(1);
    }
   cH_ppsInstanz I(ppsInstanzID::Kundenauftraege);
   AuftragBase AB(I,atoi(argv[1]));
   int znr=1;
   show_auftrag(AufEintragBase(AB,znr),0);

   }catch(SQLerror &e){std::cout << e<<'\n';}
   return 0;
}
