// generated 2004/5/7 15:30:30 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to ManuProc_Starter.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "ManuProc_Starter.hh"
#include <gtkmm/main.h>
#include <unistd.h>

#define DEFAULT_DB	"sdodb"
#define OLD_DB		"old_sdodb"

ManuProc_Starter::ManuProc_Starter(void) throw(SQLerror)
{
 dbcapability=new DBCapability();
 
 DBCapability::WhiteColumn wc("fibu","");
 fibu_start->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::EXECUTE)));
 fibu_alt_start->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::EXECUTE)));     

 DBCapability::WhiteColumn taxbird("taxbird","");
 taxbird_start->set_sensitive(
	dbcapability->isWhite(DBCapability::ColAct(taxbird,DBCapability::EXECUTE)));
     
 DBCapability::WhiteColumn wc1("vertrieb","");
 vertrieb_start->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc1,DBCapability::EXECUTE)));     

 DBCapability::WhiteColumn wc2("OOo","");
 ooo->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc2,DBCapability::EXECUTE)));
     
 DBCapability::WhiteColumn wc3("lagerwert","");
 lagerwert->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc3,DBCapability::EXECUTE)));     
     
 DBCapability::WhiteColumn wc4("offpos","");
 offeneposten->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(wc4,DBCapability::EXECUTE)));          

 DBCapability::WhiteColumn old_db("old_sdodb","");
 db_frame->set_sensitive(
     dbcapability->isWhite(DBCapability::ColAct(old_db,DBCapability::EXECUTE)));          
     
 if (access("/usr/local/bin/pps.preview",X_OK))
     preview->hide();
}


void ManuProc_Starter::on_pps_start_clicked()
{  
 std::string cmd("/bin/sh -c 'pps ");
 if(database_OLD->get_active())
   cmd+=" -d "+std::string(OLD_DB);
 cmd+="' &";
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
 std::string cmd("/bin/sh -c 'fibu ");
 if(database_OLD->get_active())
   cmd+=" -d "+std::string(OLD_DB);
 cmd+="' &";
 system(cmd.c_str());

}

void ManuProc_Starter::on_fibu_alt_start_clicked()
{
 std::string cmd("/bin/sh -c 'fibu -d old_fibu' &");
 system(cmd.c_str());
  
}

void ManuProc_Starter::on_vertrieb_start_clicked()
{
 std::string cmd("/bin/sh -c 'vertrieb ");
 if(database_OLD->get_active())
   cmd+=" -d "+std::string(OLD_DB);
 cmd+="' &";
 system(cmd.c_str());
  
}

void ManuProc_Starter::on_ooo_clicked()
{
 std::string cmd("/bin/sh -l -c openoffice &");
 system(cmd.c_str());
  
}


void ManuProc_Starter::on_preview_clicked()
{
 std::string cmd("/bin/sh -l -c pps.preview &");
 system(cmd.c_str());
  
}

void ManuProc_Starter::on_lagerwert_clicked()
{  
 std::string cmd("/bin/sh -l -c lagerwert_bis &");
 system(cmd.c_str());
}

void ManuProc_Starter::on_offeneposten_clicked()
{  
 std::string cmd("/bin/sh -l -c mk_offene_rngs &");
 system(cmd.c_str());
}

void ManuProc_Starter::on_taxbird_start_clicked()
{  
 std::string cmd("/bin/sh -l -c taxbird &");
 system(cmd.c_str());
} 


void ManuProc_Starter::on_translate_clicked()
{  
 std::string cmd("/bin/sh -l -c poedit &");
 system(cmd.c_str());
}

