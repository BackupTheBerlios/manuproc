// generated 2002/3/25 15:42:25 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/christof/kundendaten/kundendaten.glade
// for gtk 1.2.10 and gtkmm 1.2.8
//
// Please modify the corresponding derived classes in ./src/dialogInfo.cc

#include "config.h"
#include "dialogInfo.hh"
#include <vector>
#include <libgnomeui/gnome-stock.h>
#include <gtk--/button.h>
#include <gtk--/buttonbox.h>
#include <gtk--/box.h>

dialogInfo_glade::dialogInfo_glade(
)
{  dialogInfo = this;
   dialogInfo->append_button(GNOME_STOCK_BUTTON_OK);
   dialogInfo->get_button(0)->set_flags(GTK_CAN_FOCUS);
   dialogInfo->get_vbox()->set_spacing(8);
   dialogInfo->set_title("Information");
   dialogInfo->set_modal(true);
   dialogInfo->set_position(GTK_WIN_POS_CENTER);
   dialogInfo->set_policy(false, false, false);
   
   Gtk::HButtonBox *dialog_action_area1 = Gtk::wrap((GtkHButtonBox*)GNOME_DIALOG(dialogInfo->gtkobj())->action_area);
   dialog_action_area1->set_spacing(8);
   dialogInfo->show();
}

dialogInfo_glade::~dialogInfo_glade()
{  
}
