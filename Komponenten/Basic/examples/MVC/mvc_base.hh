// generated 2003/2/26 13:34:55 CET by christof@puck.petig-baender.de
// using glademm V1.1.3e_cvs
//
// newer (non customized) versions of this file go to mvc_base.hh_new

// This file is for your program, I won't touch it again!


#if !defined(_MVCBASECLASSES_HH)
#define _MVCBASECLASSES_HH 

#include <gdkmm/pixbuf.h>
#include <BaseObjects/Model.h>

class window1_base
{protected:
        Model<int> myint;
        Model<std::string> mystring;
        Model<bool> mybool,mybool2,mybool3;
        Glib::RefPtr<Gdk::Pixbuf> auto_roll,hand_roll;
public: 
	window1_base();
};
#endif //
