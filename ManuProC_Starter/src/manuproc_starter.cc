// generated 2004/5/7 15:30:30 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to manuproc_starter.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gtkmm/main.h>
#include <Aux/dbconnect.h>

#include "ManuProc_Starter.hh"

int main(int argc, char **argv)
{  
   
   Gtk::Main m(&argc, &argv);

 try {
      ManuProC::Connection conn;
      ManuProC::dbconnect(conn);

ManuProc_Starter *ManuProc_Starter = new class ManuProc_Starter();
   m.run(*ManuProc_Starter);
delete ManuProc_Starter;
         
      ManuProC::dbdisconnect();
   } catch (SQLerror &e)
   {  std::cerr << e << '\n';
      return 1;
   }

   return 0;
}
