// $Id: artikeleingabe.cc,v 1.7 2004/11/29 10:43:40 christof Exp $
/*  Artikeleingabe: ManuProC's article management program
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#include <config.h>
#include <gtkmm/main.h>

#include "Artikeleingabe.hh"
#include <Aux/dbconnect.h> 
#include <Aux/exception.h>
#include "MyMessage.h"

MyMessage *mess;

int main(int argc, char **argv)
{
 ManuProC::PrintUncaughtExceptions();
 ManuProC::Connection conn;

 int i;

 while ((i = getopt(argc, argv, "h:d:")) != EOF)
 switch (i)
   {  case 'h':
         conn.setHost(optarg);
         break;
      case 'd':
         conn.setDbase(optarg);
         break;
      default : break;
   }

     
 Gtk::Main m(argc, argv);
 try{ Petig::dbconnect(conn); 
 Query ("set names 'utf-8'");
 mess = new MyMessage();
 Artikeleingabe *ae=new class Artikeleingabe(argc,argv);
 m.run(*ae);
 delete ae;
 delete mess;
 }
 catch(SQLerror &e)
   { std::cerr<< e << '\n';
   }

 Petig::dbdisconnect_nt(conn.Dbase());
 return 0;
}
