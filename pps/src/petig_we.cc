// generated 2004/8/30 13:51:10 CEST by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// newer (non customized) versions of this file go to petig_we.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "petig_we.hh"
#include <Misc/dbconnect.h>
#include "MyMessage.h"

extern MyMessage *meldung;

void petig_we::on_petig_we_ok_clicked()
{ 
 ManuProC::Connection c_to_p;
 
 c_to_p.setHost("localhost");
 c_to_p.setDbase("petigdb");
 c_to_p.Port(25432);
 c_to_p.Name("remote_petig");
 c_to_p.User("mabella");

 try{ 
 ManuProC::dbconnect(c_to_p); 
 }
 catch(SQLerror &e)
  { meldung->Show(e);
    return;
  }

 ManuProC::dbdefault(c_to_p.Name());

 ManuProC::dbdisconnect_nt(c_to_p.Name());
}

void petig_we::on_petig_we_cancel_clicked()
{  
}
