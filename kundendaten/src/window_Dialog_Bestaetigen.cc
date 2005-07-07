// generated 2001/6/7 12:35:12 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to window_Dialog_Bestaetigen.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window_Dialog_Bestaetigen.hh"
#include "windowTop.hh"


void window_Dialog_Bestaetigen::on_button_ok_clicked()
{   
//  hauptfenster->kunde_loeschen();
  delete this;
}

void window_Dialog_Bestaetigen::on_button_cancel_clicked()
{   
  delete this;
}

window_Dialog_Bestaetigen::window_Dialog_Bestaetigen(windowTop *h,std::string st)
{
  hauptfenster=h;
  label_text->set_text(st);
}
