// generated 2001/6/5 15:00:29 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to window_neue_Kundennummer.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window_neue_Kundennummer.hh"
#include <Kunde/Kunde.h>

void window_neue_Kundennummer::on_kundennr_activate()
{   
 entry_firma->grab_focus();
}

void window_neue_Kundennummer::on_firma_activate()
{   
 entry_name2->grab_focus();
}

void window_neue_Kundennummer::on_name2_activate()
{  
 button_ok->grab_focus();
}

void window_neue_Kundennummer::on_button_ok_clicked()
{   
 int kid = atoi(entry_kundennr->get_text().c_str());
 K.newKunde(kid,entry_firma->get_text(),entry_name2->get_text(),
 		gruppe->get_value()); 
 hauptfenster->neue_kundennmmer(kid);
 destroy();
}

void window_neue_Kundennummer::on_button_abbrechen_clicked()
{   
  destroy();
}

window_neue_Kundennummer::window_neue_Kundennummer(windowTop* h,
		Kundengruppe::ID kid)
{
 hauptfenster=h;
 entry_kundennr->set_text(itos(Kunde::nextval()));
}
