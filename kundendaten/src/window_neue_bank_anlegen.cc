// generated 2001/6/6 11:43:39 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to window_neue_bank_anlegen.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window_neue_bank_anlegen.hh"
#include "windowTop.hh"

void window_neue_bank_anlegen::on_entry_bankname_activate()
{   
 entry_blz->grab_focus();
}

void window_neue_bank_anlegen::on_entry_blz_activate()
{   
 on_button_uebernehmen_clicked(); 
}

void window_neue_bank_anlegen::on_button_uebernehmen_clicked()
{   
std::string name = entry_bankname->get_text();
 long unsigned int blz = strtol(entry_blz->get_text().c_str(),NULL,10);
 Kunde K;
 unsigned long int bank_index = K.neue_bank_anlegen(name,blz);
 hauptfenster->neue_bank_uebernehmen(bank_index);
 destroy();
}

void window_neue_bank_anlegen::on_button_abbrechen_clicked()
{   
  destroy();
}

window_neue_bank_anlegen::window_neue_bank_anlegen(windowTop* h,std::string st, long int blz)
{
  hauptfenster=h;
  entry_blz->set_text(itos(blz));
  label_text->set_text(st);
}
