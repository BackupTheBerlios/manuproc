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
  on_button_ok_clicked();
}

void window_neue_Kundennummer::on_firma_activate()
{   
  on_button_ok_clicked();
}

void window_neue_Kundennummer::on_button_ok_clicked()
{   
 Kunde K(Kunde::none_id);
 int kid = atoi(entry_kundennr->get_text().c_str());
 K.newKunde(kid,entry_firma->get_text()); 
 hauptfenster->neue_kundennmmer(kid);
 destroy();
}

void window_neue_Kundennummer::on_button_abbrechen_clicked()
{   
  destroy();
}

window_neue_Kundennummer::window_neue_Kundennummer(windowTop* h)
{
 hauptfenster=h;
 Kunde K(Kunde::none_id);
 unsigned int nextval = K.nextval();
 entry_kundennr->set_text(itos(nextval));
}
