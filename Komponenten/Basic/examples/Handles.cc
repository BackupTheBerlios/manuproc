// $Id: Handles.cc,v 1.4 2003/05/11 22:09:28 christof Exp $
/*  ManuProcWidgets: ManuProC's GUI element library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include <Misc/Handles.h>
#include <iostream>
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/clist.h>
#include <gtkmm/box.h>
#include <gtkmm/optionmenu.h>
#include <gtkmm/scrolledwindow.h>
#include <Gtk_OStream.h>

class A : public HandleContent
{  	int val;
	friend class Handle<A>;
public:
	A(int x) : val(x) 
	{  std::cerr << "ctor " << val << '/' << this << '\n';
	}
	~A()
	{  std::cerr << "dtor " << val << '/' << this << '\n';
	}
	bool operator==(const A &b)
	{  return val==b.val;
	}
	friend std::ostream &operator<<(std::ostream &o,const A &a);
};

std::ostream &operator<<(std::ostream &o,const A &a)
{  return o << a.val;
}

class Ah : public Handle<A>
{public:
	Ah(int x=-1) : Handle<A>(new A(x)) {};
	Ah(A *a) : Handle<A>(a) {}; // dangerous, you pass ownership
};

class MeinFenster : public Gtk::Window
{  Gtk::CList *clist1;
   Gtk::OptionMenu *optionmenu1;
	
   void on_clist1_select_row(gint row, gint column, GdkEvent *event);
   void om_clicked();
public:
   MeinFenster();
};

gint on_window1_delete_event(GdkEventAny *ev)
{ Gtk::Main::instance()->quit();
  return 0;
}

void MeinFenster::om_clicked()
{  gpointer user_data=optionmenu1->get_menu()->get_active()->get_user_data();
   Ah handle=static_cast<A*>(user_data);
   cout << *handle << '\n';  // oder handle->Value()
}

void MeinFenster::on_clist1_select_row(gint row, gint column, GdkEvent *event)
{  gpointer user_data=clist1->row(row)->get_data();
   Ah handle=static_cast<A*>(user_data);
   cout << *handle << '\n';  // oder handle->Value()
}

MeinFenster::MeinFenster()
  : Gtk::Window(Gtk::WINDOW_TOPLEVEL), clist1(0), optionmenu1(0)
{
   Gtk::Window *window1=this;
   clist1 = manage(new class Gtk::CList(2));
   Gtk::ScrolledWindow *scrolledwindow1 = manage(new class Gtk::ScrolledWindow());
   optionmenu1 = manage(new class Gtk::OptionMenu());
   Gtk::VBox *vbox1 = manage(new Gtk::VBox(false, 0));
   scrolledwindow1->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow1->add(*clist1);
   vbox1->pack_start(*scrolledwindow1);
   vbox1->pack_start(*optionmenu1, false, false, 0);
   window1->add(*vbox1);
   clist1->show();
   scrolledwindow1->show();
   optionmenu1->show();
   vbox1->show();
   window1->show();
   window1->delete_event.connect(SigC::slot(&on_window1_delete_event));
   
   // allocate quark (without calling this first, get_user_data cannot return
   //			the same value as get_data("user_data"))
   window1->set_user_data(0);
   // fill it
   window1->set_data_full("user_data",Ah(17)->ref(),&A::unref);

   {Gtk::OStream os(clist1);
    os << "@\t0"; os.flush(Ah(0)->ref(),&A::unref);
    os << "A\t1"; os.flush(Ah(1)->ref(),&A::unref);
    os << "B\t2"; os.flush(Ah(2)->ref(),&A::unref);
   }
   {Gtk::OStream os(optionmenu1);
    os << "-1"; os.flush(Ah(-1)->ref(),&A::unref);
   }
   {Gtk::OStream os(optionmenu1);
    os << "3"; os.flush(Ah(3)->ref(),&A::unref);
    os << "4"; os.flush(Ah(4)->ref(),&A::unref);
   }
   clist1->signal_select_row().connect(SigC::slot(*this, &MeinFenster::on_clist1_select_row));
   optionmenu1->get_menu()->signal_deactivate().connect(SigC::slot(*this, &MeinFenster::om_clicked));
}

int main(int argc, char **argv)
{  Gtk::Main m(&argc, &argv);
   m.run(*new MeinFenster());
   return 0;   
}
