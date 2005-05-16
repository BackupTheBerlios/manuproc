// generated 2004/5/7 15:30:30 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to ManuProc_Starter.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "ManuProc_Starter.hh"
#include <gtkmm/main.h>



ManuProc_Starter::ManuProc_Starter(void) throw(SQLerror)
{
 dbcapability=new DBCapability();
 
 DBCapability::WhiteColumn wc("fibu","");
 fibu_start->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::EXECUTE)));
     
 DBCapability::WhiteColumn wc1("vertrieb","");
 vertrieb_start->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc1,DBCapability::EXECUTE)));     

 DBCapability::WhiteColumn wc2("OOo","");
 ooo->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc2,DBCapability::EXECUTE)));  
}


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

void ManuProc_Starter::on_artikel_start_clicked()
{  
 std::string cmd("/bin/sh -c artikeleingabe &");
 system(cmd.c_str());
}

void ManuProc_Starter::on_quit_clicked()
{
 Gtk::Main::instance()->quit();
}


void ManuProc_Starter::on_preise_start_clicked()
{  
 std::string cmd("/bin/sh -c preise &");
 system(cmd.c_str());
}


void ManuProc_Starter::on_fibu_start_clicked()
{  
 std::string cmd("/bin/sh -c fibu &");
 system(cmd.c_str());

}

void ManuProc_Starter::on_vertrieb_start_clicked()
{
 std::string cmd("/bin/sh -c vertrieb &");
 system(cmd.c_str());
  
}

void ManuProc_Starter::on_ooo_clicked()
{
 std::string cmd("/bin/sh -c openoffice &");
 system(cmd.c_str());
  
}


