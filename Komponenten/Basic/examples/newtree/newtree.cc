// generated 2002/11/7 16:42:28 CET by christof@puck.petig-baender.de
// using glademm V1.1.2a_cvs
//
// newer (non customized) versions of this file go to newtree.cc_new

// This file is for your program, I won't touch it again!

#include <gtkmm/main.h>
#include "window1.hh"
#include <Misc/dbconnect.h>

int main(int argc, char **argv)
{  
   
   Gtk::Main m(&argc, &argv);
   ManuProC::dbconnect();

window1 *window1 = new class window1();
   m.run(*window1);
delete window1;
   ManuProC::dbdisconnect();
   return 0;
}
