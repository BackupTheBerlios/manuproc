// generated 2002/7/5 22:59:43 CEST by christof@puck.petig-baender.de
// using glademm V1.1.0d_cvs
//
// newer (non customized) versions of this file go to window1.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window1.hh"

void window1::loeschen()
{  myint=0;
   mystring="";
}

window1::window1()
{  myint.changed.connect(SigC::slot(this,&window1::int_changed));
   myint.changed.connect(SigC::slot(this,&window1::someth_changed));
   mystring.changed.connect(SigC::slot(this,&window1::string_changed));
   mystring.changed.connect(SigC::slot(this,&window1::someth_changed));
}

void window1::int_changed(gpointer)
{  std::cout << "int changed to " << myint.get_value() << '\n';
}


void window1::string_changed(gpointer)
{  std::cout << "string changed to " << mystring.get_value() << '\n';
}

void window1::someth_changed(gpointer x)
{  if (x==&myint.Value()) std::cout << "int changed\n";
   else if (x==&mystring.Value()) std::cout << "string changed\n";
   else std::cout << "something else changed\n";
}
