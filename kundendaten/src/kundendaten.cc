// generated 2001/2/22 9:59:33 CET by thoma@ig23.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to kundendaten.cc_new

// This file is for your program, I won't touch it again!

#include<config.h>
#include <gtkmm/main.h>

#include "windowTop.hh"
#include <Aux/dbconnect.h>
#include<Aux/exception.h>
#include "MyMessage.h"

MyMessage *Message;

int main(int argc, char **argv)
{   
   Petig::PrintUncaughtExceptions();
   Petig::Connection conn;
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
   Petig::PrintUncaughtExceptions();

   try {

      Petig::dbconnect(conn);  
      Query ("set names 'utf-8'");

      class windowTop *w=new class windowTop();

      Message=new MyMessage();
      m.run(*w);
      delete w;
      delete Message;
      Petig::dbdisconnect();
   } catch (SQLerror &e)
   {  std::cerr << e << '\n';
      return 1;
   }
   
   catch (Petig::Datumsfehler &df)
   {  std::cerr << df << '\n';
      return 1;
   }   

   return 0;

}
