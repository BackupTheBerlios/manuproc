// generated 2002/11/13 0:12:01 CET by christof@puck.petig-baender.de
// using glademm V1.1.2a_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/christof/localcvs/src/guenther/guenther.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/WinFileReq.cc

#include "config.h"
#include "WinFileReq_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtk--/accelgroup.h>
#include <gtk--/button.h>

WinFileReq_glade::WinFileReq_glade(
) : Gtk::FileSelection("Datei ausw�hlen")
{  
   
   Gtk::FileSelection *WinFileReq = this;
   Gtk::AccelGroup *WinFileReq_accgrp = Gtk::AccelGroup::create();
   _data = new GlademmData(WinFileReq_accgrp);
   WinFileReq->get_ok_button()->set_flags(GTK_CAN_FOCUS);
   WinFileReq->get_ok_button()->set_relief(GTK_RELIEF_NORMAL);
   WinFileReq->get_cancel_button()->set_flags(GTK_CAN_FOCUS);
   WinFileReq->get_cancel_button()->set_relief(GTK_RELIEF_NORMAL);
   WinFileReq->set_border_width(10);
   WinFileReq->set_title("Datei ausw�hlen");
   WinFileReq->set_modal(false);
   WinFileReq->set_position(GTK_WIN_POS_NONE);
   WinFileReq->add_accel_group(*(_data->getAccelGroup()));
   WinFileReq->show_fileop_buttons();
   WinFileReq->show();
   WinFileReq->get_ok_button()->clicked.connect(SigC::slot(this, &WinFileReq_glade::on_ok_button1_clicked));
   WinFileReq->get_cancel_button()->clicked.connect(WinFileReq->destroy.slot());
}

WinFileReq_glade::~WinFileReq_glade()
{  delete _data;
}
