// generated 2000/1/28 11:31:10 MET by jacek@mimi.
// using glademm V0.5.5
//
// newer (non customized) versions of this file go to fertigwlager.cc_glade

// This file is for your program, I won't touch it again!

#include <gtk--/main.h>

#include "wlagermain.hh"
#include "MyMessage.h"
#include <config.h>
#include <gnome--/main.h>
#include <Aux/dbconnect.h>
#include <Aux/exception.h>


MyMessage *message;

int main(int argc, char **argv)
{   
 try {
      Petig::dbconnect();
      Gnome::Main m(PACKAGE, VERSION, argc, argv);
      Petig::PrintUncaughtExceptions();
      message=new MyMessage();
      manage(new class wlagermain());
      m.run();
      delete message;
     }
 catch (SQLerror &e)
  {message->Show(e);}
   return 0;
}

