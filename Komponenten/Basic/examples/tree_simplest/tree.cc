// generated 2001/6/15 9:52:50 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to marc.cc_new

// This file is for your program, I won't touch it again!

#include <gtk--/main.h>

#include "simplest.hh"
#include "with_class.hh"

int main(int argc, char **argv)
{   
   
   Gtk::Main m(&argc, &argv);
//manage(new class simplest());
manage(new class with_class());
   m.run();
   return 0;
}
