// generated 2004/3/15 11:10:58 CET by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to mpc_agent.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gtkmm/main.h>

#include "mpc_agent.hh"

int main(int argc, char **argv)
{  
   
   Gtk::Main m(&argc, &argv);

mpc_agent *mpc_agent = new class mpc_agent();
   m.run(*mpc_agent);
delete mpc_agent;
   return 0;
}
