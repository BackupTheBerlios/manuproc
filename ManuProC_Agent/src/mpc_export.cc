// generated 2004/5/17 17:02:24 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0.2
//
// newer (non customized) versions of this file go to mpc_export.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "mpc_export.hh"
#include <gtkmm/fileselection.h>
#include <MyMessage.h>
//#include <WinFileReq.hh>
#include <gtkmm/entry.h>
#include "mpc_export.hh"
#include "mpc_agent.hh"
#include <Misc/itos.h>

extern mpc_agent *mpca;

mpc_export::mpc_export()
{
 std::string fname("mpca_");
 fname=fname+itos(VERKNR)+"_"+itos(mpca->get_orderid())+".xml";
 export_file_name->set_text(fname);
}

void mpc_export::set_entry_text(const std::string &s)
{
export_file_name->set_text(s);
}

void mpc_export::on_file_select_clicked()
{  
 Gtk::FileSelection fs(export_file_name->get_text());
 int ret;
 fs.set_transient_for(*this);

 ret=fs.run();
 std::cout << "ret:" << ret << "\n";
 if(ret==Gtk::RESPONSE_OK)
   export_file_name->set_text(fs.get_filename());

}

void mpc_export::on_do_file_export_clicked()
{
 mpca->export_xml(export_file_name->get_text());
 export_close->activate(); 
}

void mpc_export::on_do_file_send_clicked()
{  
 export_close->activate(); 
}

