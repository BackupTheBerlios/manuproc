// $Id: fixp_SpinButton.hh,v 1.1 2006/08/03 11:57:22 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef MANUPROC_WIDGETS_MVC_FIXP_H
#define MANUPROC_WIDGETS_MVC_FIXP_H

#include <gtkmm/spinbutton.h>
#include <ModelWidgetConnection.h>
#include <Misc/fixedpoint.h>

template <int digits>
 class fixp_SpinButton : public Gtk::SpinButton
{	typedef fixedpoint<digits> T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::SpinButton>
	{	bool any_change;
		SigC::Connection cm_con2[3];
		typedef Gtk::SpinButton widget_t;
		typedef ModelWidgetConnection<T,Gtk::SpinButton> this_t;

		bool on_focus_out(GdkEventFocus *ev)
		{  if (any_change) this->controller2model();
		//   select_region(0,0); // needed ?
		   return false;
		}
		bool on_focus_in(GdkEventFocus *ev)
		{  this->widget->select_region(0,-1);
		   return false;
		}
		void keypress()
		{ any_change=true; }
		void on_activate()
		{ on_focus_out(0); }
		
		void model2widget()
		{  this->widget->set_value(this->model.get_value().as_float());
		   any_change=false;
		}
		void widget2model()
		{  this->widget->update();
		   this->model=this->widget->get_value();
		   any_change=false;
		}
		SigC::Connection connect()
		{  cm_con2[0]=this->widget->signal_focus_out_event().connect(SigC::slot(*this,&fixp_SpinButton::Connection::on_focus_out),true);
		   cm_con2[1]=this->widget->signal_focus_in_event().connect(SigC::slot(*this,&fixp_SpinButton::Connection::on_focus_in),true);
		   // I'm not quite sure whether this is needed at all
		   cm_con2[2]=this->widget->signal_activate().connect(SigC::slot(*this,&fixp_SpinButton::Connection::on_activate),true);
		   return this->widget->signal_changed().connect(SigC::slot(*this,&fixp_SpinButton::Connection::keypress));
		}
		void disconnect()
		{  cm_con2[0].disconnect();
		   cm_con2[1].disconnect();
		   cm_con2[2].disconnect();
		   this_t::disconnect();
		}

	public:
		Connection(widget_t *w=0) : any_change(false) 
		{ this->set_widget(w); }
		Connection(const Model_ref<T> &m, widget_t *w=0) 
		: this_t(m), any_change(false) { this->set_widget(w); }
	};
private:
	Connection conn;
	
	// explicitely forbid to use these (make them private)
	gfloat get_value_as_float() const;
	gint get_value_as_int() const;
	void set_value(gfloat);
public:
	fixp_SpinButton(const Model_ref<T> &model,T min,T max)
		: conn(model)
	{  set_update_policy(Gtk::UPDATE_ALWAYS);
	   set_numeric(true);
	   set_digits(digits);
	   get_adjustment()->set_lower(min.as_float());
	   get_adjustment()->set_upper(max.as_float());
	   get_adjustment()->set_step_increment(1);
	   conn.set_widget(this);
	};
};

#endif

