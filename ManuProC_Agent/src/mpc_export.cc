// generated 2004/5/17 17:02:24 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0.2
//
// newer (non customized) versions of this file go to mpc_export.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "mpc_export.hh"
#include <gtkmm/fileselection.h>
#include <MyMessage.h>
#include <WinFileReq.hh>
#include <gtkmm/entry.h>

void mpc_export::on_file_select_clicked()
{  
// Gtk::FileSelection fs;
// int ret;
// fs.set_transient_for(*this);

// ret=fs.run();
// fs.get_filename();

  	SigC::Slot1<void,const std::string &> s(*export_file_name,&Gtk::Entry::set_text);

 WinFileReq(
	s,
 	"",
 	"",
 	"",
 	"Export to file",
 	false,
 	*this);
}

void mpc_export::on_do_file_export_clicked()
{  
}

void mpc_export::on_do_file_send_clicked()
{  
}
