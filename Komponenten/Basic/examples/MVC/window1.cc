// generated 2002/7/5 22:59:43 CEST by christof@puck.petig-baender.de
// using glademm V1.1.0d_cvs
//
// newer (non customized) versions of this file go to window1.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window1.hh"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <bool_properties.hh>

void window1::loeschen()
{  myint=0;
   mystring="";
}

window1::window1()
{  myint.signal_changed().connect(SigC::slot(*this,&window1::int_changed));
   myint.signal_changed().connect(SigC::slot(*this,&window1::someth_changed));
   mystring.signal_changed().connect(SigC::slot(*this,&window1::string_changed));
   mystring.signal_changed().connect(SigC::slot(*this,&window1::someth_changed));
   mybool.signal_changed().connect(SigC::slot(*this,&window1::bool_changed));
   mybool3.signal_changed().connect(SigC::slot(*this,&window1::someth_changed));
   Gtk::AssociateVisibility(stringwidget,mybool2);
   Gtk::AssociateSensitivity(boolwidget,mybool2);
   slconn.set(mystring,label1);
   isconn.set(myint,spinbutton1);
   srand(time(0));
}

void window1::int_changed(gpointer)
{  std::cout << "int changed to " << myint.get_value() << '\n';
}

void window1::bool_changed(gpointer)
{  std::cout << "bool changed to " << mybool.get_value() << '\n';
}

void window1::string_changed(gpointer)
{  std::cout << "string changed to " << mystring.get_value() << '\n';
}

void window1::someth_changed(gpointer x)
{  if (myint.matches(x)) std::cout << "int changed\n";
   else if (mystring.matches(x)) std::cout << "string changed\n";
   else if (mybool3.matches(x)) 
      std::cout << (mybool3.Value() ? "Würfeln von Hand" : "Magus würfelt") << '\n';
   else std::cout << "something else changed\n";
}

void window1::randomize()
{  mybool=rand()>=(RAND_MAX/2);
   mybool2=rand()>=(RAND_MAX/2);
   myint=int(101.0*rand()/(RAND_MAX+1.0));
   std::string x;
   for (int i=int(17.0*rand()/(RAND_MAX+1.0));i>0;--i)
      x=x+char(95.0*rand()/(RAND_MAX+1.0)+' ');
   mystring=x;
}
