// generated 2002/11/13 0:12:01 CET by christof@puck.petig-baender.de
// using glademm V1.1.2a_cvs
//
// newer (non customized) versions of this file go to WinFileReq.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "WinFileReq.hh"

#ifdef __MINGW32__
# include <windows.h>
# include <commdlg.h>
#include <gdk/gdkwin32.h>

extern std::string utf82iso(const std::string &s);
#endif

#ifndef __MINGW32__
void WinFileReq::on_ok_button1_clicked()
{  slot(get_filename());
   delete this;
}

void WinFileReq::on_cancel()
{  delete this;
}

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/bind.h>
// don't ask me why this does not work:
// #  include <sigc++/compatibility.h>
// /usr/include/sigc++-2.0/sigc++/object_slot.h: In function `
//    SigC::Slot0<T_return> SigC::slot(T_obj1&, T_return (T_obj2::*)()) [with 
//    T_return = void, T_obj1 = WinFileReq_glade, T_obj2 = WinFileReq_glade]':
// WinFileReq_glade.cc:35:   instantiated from here
// /usr/include/sigc++-2.0/sigc++/object_slot.h:32: error: cannot convert from 
//    base `sigc::trackable' to derived type `WinFileReq_glade' via virtual base `
//    sigc::trackable'
      
namespace SigC
{  template <class T> 
    Slot0<void> slot(T &t, void (T::*p)()) { return sigc::mem_fun(t,p); }
}
#endif

#include "WinFileReq_glade.cc"
#endif

WinFileReq::WinFileReq(const SigC::Slot1<void,const std::string &> &sl,const std::string &file,
		std::string filter, std::string extension, std::string title, bool load,
		Gtk::Window *parent)
#ifndef __MINGW32__
	: slot(sl)
#endif
{
#ifndef __MINGW32__
   set_filename(file);
   set_title(title);
   if (parent) set_transient_for(*parent);
#else
   filter=utf82iso(filter);
   extension=utf82iso(extension);
   title=utf82iso(title);
   // file is assumed to have windows encoding
   OPENFILENAME ofn;
   char buf[10240];

   strncpy(buf,file.c_str(),sizeof buf);

   ZeroMemory(&ofn, sizeof (OPENFILENAME));
   ofn.lStructSize = sizeof (OPENFILENAME);
   if (parent) ofn.hwndOwner = (HWND)GDK_WINDOW_HWND(parent->get_window()->gobj()); 
   ofn.lpstrFile = buf;
   ofn.nMaxFile = sizeof buf;
   if (filter.empty()) ofn.lpstrFilter = "Alle Dateien (*.*)\0*.*\0";
   else ofn.lpstrFilter=filter.c_str();
   ofn.nFilterIndex = 1;
   ofn.lpstrDefExt= extension.c_str();
   ofn.lpstrTitle = title.c_str();
   ofn.Flags = OFN_PATHMUSTEXIST 
   		| (load ? OFN_FILEMUSTEXIST : 0);
   if (!file.empty() && file[file.size()-1]=='\\')
   {  *buf=0;
      ofn.lpstrInitialDir = file.c_str();
   }

   bool res=false;
   if (load) res=GetOpenFileName(&ofn);
   else res=GetSaveFileName(&ofn);

   if (res) 
   {  const_cast<SigC::Slot1<void,const std::string &>&>(sl)(buf);
   }
#endif
}
