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
# include <objidl.h> // IMalloc
# include <shlobj.h>
#include <gdk/gdkwin32.h>

class TagStream { public: static void utf82iso(std::string &s); };
//extern std::string utf82iso(const std::string &s);
#endif

#ifndef __MINGW32__
void WinFileReq::on_ok_button1_clicked()
{  slot(get_filename());
   delete this;
}

void WinFileReq::on_cancel()
{  if (call_on_cancel) slot(std::string());
   delete this;
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

WinFileReq::WinFileReq(const SigC::Slot1<void,const std::string &> &sl,
                std::string file, std::string filter, 
                std::string extension, std::string title, bool load,
		Gtk::Window *parent, bool pass_cancel)
#ifndef __MINGW32__
	: slot(sl), call_on_cancel(pass_cancel)
#endif
{
#ifndef __MINGW32__
   set_filename(file);
   set_title(title);
   if (parent) set_transient_for(*parent);
#else
   // file is assumed to have UTF-8 encoding
   TagStream::utf82iso(file);
   TagStream::utf82iso(title);
   
   TagStream::utf82iso(filter);
   TagStream::utf82iso(extension);

   char buf[MAX_PATH];
   strncpy(buf,file.c_str(),sizeof buf);

   if (filter.empty() && extension.empty())
   // browse for a directory
   { BROWSEINFO bi;
     ZeroMemory(&bi, sizeof(BROWSEINFO));
     bi.hwndOwner = (HWND)GDK_WINDOW_HWND(parent->get_window()->gobj());
     bi.pszDisplayName = buf;
     bi.lpszTitle = title.c_str();
     bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
     
     LPITEMIDLIST pidl=SHBrowseForFolder(&bi);
     if (pidl) 
     { // get the name of the folder
       if ( SHGetPathFromIDList ( pidl, buf ) )
       { std::string result=buf;
         TagStream::utf82iso(result);
         const_cast<SigC::Slot1<void,const std::string &>&>(sl)(result);
       }
        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }     
     }
     else if (pass_cancel)
        const_cast<SigC::Slot1<void,const std::string &>&>(sl)(std::string());
     return;
   }
   
   OPENFILENAME ofn;

   ZeroMemory(&ofn, sizeof (OPENFILENAME));
   ofn.lStructSize = sizeof (OPENFILENAME);
   if (parent) ofn.hwndOwner = (HWND)GDK_WINDOW_HWND(parent->get_window()->gobj()); 
   ofn.lpstrFile = buf;
   ofn.nMaxFile = sizeof buf;
   if (filter.empty() && !extension.empty() && !title.empty())
   {  filter=title+" (*."+extension+")\0*."+extension+"\0"
           "Alle Dateien (*.*)\0*.*\0";
   }
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
   { std::string result=buf;
     TagStream::utf82iso(result);
     const_cast<SigC::Slot1<void,const std::string &>&>(sl)(result);
   }
   else if (pass_cancel)
      const_cast<SigC::Slot1<void,const std::string &>&>(sl)(std::string());
#endif
}

void WinFileReq::create(const SigC::Slot1<void,const std::string &> &sl,const std::string &file,
		std::string filter, std::string extension, std::string title, bool load,
		Gtk::Window *parent, bool pass_cancel)
{  (void)
#ifndef __MINGW32__
     new
#endif
       WinFileReq(sl,file,filter,extension,title,load,parent,pass_cancel);
}
