// generated 2001/9/18 12:19:57 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to artikeleingabe.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gnome--/main.h>

#include "Artikeleingabe.hh"
#include <Aux/dbconnect.h> 
#include <Aux/exception.h>
#include "MyMessage.h"

MyMessage *mess;

int main(int argc, char **argv)
{
 Petig::PrintUncaughtExceptions();
 Petig::Connection conn;

#ifdef PETIG_EXTENSIONS
// conn.setDbase("petigdb");
#elif defined MABELLA_EXTENSIONS
// conn.setDbase("mabelladb");
#endif
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

     
 Gnome::Main m(PACKAGE, VERSION, 1, argv);
 try{ Petig::dbconnect(conn); 
 manage(new class Artikeleingabe(argc,argv));
 mess = manage(new MyMessage());
 m.run();
 }
 catch(SQLerror &e)
   { std::cerr<< e << '\n';
   }

 Petig::dbdisconnect_nt(conn.Dbase());
 return 0;
}
