// $Id: WinFileReq.hh,v 1.10 2004/12/15 08:10:42 christof Exp $

#ifndef _FILESELECTION1_HH
#  define _FILESELECTION1_HH

#ifndef __MINGW32__
#  include "WinFileReq_glade.hh"
#else
#  include <gtkmm/window.h>
#endif
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/slot.h>
#endif

#ifdef __MINGW32__
#include <string>
#endif

class WinFileReq
#ifndef __MINGW32__  
			 : public WinFileReq_glade
#endif  
{
#ifndef __MINGW32__
	SigC::Slot1<void,const std::string &> slot;
	bool call_on_cancel;
	
        void on_ok_button1_clicked();
        void on_cancel();
#endif
public:
	WinFileReq(const SigC::Slot1<void,const std::string &> &sl, std::string file,
		std::string filter=std::string(), std::string extension=std::string(), std::string title=std::string(), bool load=true,
		Gtk::Window *parent=0,bool pass_cancel=false);
	// to abstract the new/automatic behaviour on win32/linux
	static void create(const SigC::Slot1<void,const std::string &> &sl,const std::string &file,
		std::string filter=std::string(), std::string extension=std::string(), std::string title=std::string(), bool load=true,
		Gtk::Window *parent=0,bool pass_cancel=false);
};
#endif
