// $Id: ModelWidgetConnection.h,v 1.10 2004/07/15 08:10:00 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#ifndef MODELWIDGETCONNECTION_H
#define MODELWIDGETCONNECTION_H

#include <BaseObjects/Model.h>
#include <sigc++/object.h>
#include <glib/gtypes.h>
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

template <typename T,typename W,typename ID=gpointer>
 class ModelWidgetConnection : public SigC::Object
{public: // protected:  work around a g++ 3.3.0 bug
	typedef ModelWidgetConnection<T,W,ID> this_t;
	typedef W widget_t;
	
protected:
	SigC::Connection mv_con, cm_con;
	Model_ref<T> model;
	widget_t *widget;

	// these default actions fit for views, override them for controllers
	virtual void model2widget()=0;
	virtual void widget2model() {}
	virtual SigC::Connection connect() { return SigC::Connection(); }

	virtual void disconnect() { cm_con.disconnect(); }

private:
	void refresh(ID x)
	{  if (model.matches(x)) model2view();
	}
protected:
	void model2view()
	{  if (!widget) return;
	   cm_con.block();
	   model2widget();
	   cm_con.unblock();
	}
	void controller2model()
	{  if (!model) return;
	   mv_con.block();
	   widget2model();
           mv_con.unblock();
	}
	// I cannot call set_widget since model2view is pure
	// virtual at this point, please do it in your ctor
	ModelWidgetConnection(const Model_ref<T> &m=Model_ref<T>())
		: widget(0)
	{ if (m.valid()) set_model(m); }

public:
	virtual void set_widget(widget_t *w)
	{  disconnect();
	   widget=w;
	   if (widget)
	   {  if (model.valid()) model2view();
	      cm_con=connect();
	   }
	}
	void set_model(const Model_ref<T> &m)
	{  mv_con.disconnect();
	   model=m;
	   if (widget) model2view();
	   mv_con=model.signal_changed().connect(SigC::slot(*this,&this_t::refresh));
	}
	void set(const Model_ref<T> &m,widget_t *w)
	{  set_widget(w);
	   set_model(m);
	}

	// silence gcc	
	virtual ~ModelWidgetConnection() {}
};

#endif
