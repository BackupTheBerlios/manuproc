// generated 2004/5/7 15:30:30 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to ManuProc_Starter.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "ManuProc_Starter.hh"
#include <gtkmm/main.h>

void ManuProc_Starter::on_pps_start_clicked()
{  
 std::string cmd("/bin/sh -c pps &");
 system(cmd.c_str());
}

void ManuProc_Starter::on_kunden_start_enter()
{
 std::string cmd("/bin/sh -c kundendaten &");
 system(cmd.c_str());
}

void ManuProc_Starter::on_lager_start_clicked()
{  
 std::string cmd("/bin/sh -c fertigwlager &");
 system(cmd.c_str());
}

void ManuProc_Starter::on_quit_clicked()
{
 Gtk::Main::instance()->quit();
}
