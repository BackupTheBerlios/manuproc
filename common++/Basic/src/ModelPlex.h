// $Id: ModelPlex.h,v 1.8 2004/11/15 08:51:37 christof Exp $
/*  libcommon++: ManuProC's OO library
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

#ifndef MODELPLEX_H
#define MODELPLEX_H

// Model multiplexer

/*******************************
  M1               M2
  changed() val    changed() val
   
   
   P
   changed()  value
   
   changed muss feuern, wenn zugrundeliegende Variable geändert wird
   oder ein Wechsel des Models den Wert ändert

   changed muss changed der zugrundeliegenden Variable aufrufen
***********************************/

#include <BaseObjects/Model.h>

#if MPC_SIGC_VERSION<0x120
#error this class needs SigC 1.2+
#endif
#include <sigc++/object_slot.h>
#if MPC_SIGC_VERSION > 0x120
#  include <sigc++/compatibility.h>
#endif

#if 0
#include <Misc/Trace.h>
extern ManuProC::Tracer::Channel modelplex_chan;
#define MODELPLEX_DEBUG(msg,arg...) ManuProC::Trace _mpx(modelplex_chan,msg,##arg)
#define MODELPLEX_DEBUG_ON
#else
#define MODELPLEX_DEBUG(msg,arg...)
#endif

template <class T>
 class ModelPlex : public SigC::Object, public Model_copyable<T>
{	typedef ModelPlex<T> this_t;
	
	SigC::Connection mv_con, cm_con;
	Model_ref<T> actmodel;

	// model ist actmodel, we+we ist this
	void we2actmodel(void *x)
	{  MODELPLEX_DEBUG(__FUNCTION__,Id(),actmodel.Id());
	   mv_con.block();
	   if (actmodel.valid()) actmodel=this->Value();
           mv_con.unblock();
	}
	void actmodel2us()
	{  MODELPLEX_DEBUG(__FUNCTION__,Id(),actmodel.Id());
	   cm_con.block();
	   if (actmodel.valid() && this->Value()!=actmodel.Value()) // do not fire if unchanged
	      *this=actmodel.Value(); 
	   cm_con.unblock();
	}
	void actmodel_value_changed(void *x)
	{  MODELPLEX_DEBUG(__FUNCTION__,Id(),actmodel.Id());
	   if (actmodel.matches(x)) actmodel2us();
	}

	void operator=(const this_t &b);
public:
	ModelPlex(const Model_ref<T> &m=Model_ref<T>())
	{ MODELPLEX_DEBUG(__FUNCTION__,Id(),m.Id());
	  cm_con=this->signal_changed().connect(
			SigC::slot(*this,&this_t::we2actmodel));
	  if (m.valid()) set_model(m); 
	}
//#if defined(MODELPLEX_DEBUG_ON)	
	ModelPlex(const ModelPlex<T> &a)
	: SigC::Object(a), Model_copyable<T>(a)
	{  MODELPLEX_DEBUG(__PRETTY_FUNCTION__,Id(),a.Id());
	   cm_con=this->signal_changed().connect(
			SigC::slot(*this,&this_t::we2actmodel));
	   if (a.actmodel.valid()) set_model(a.actmodel);
	}
//#endif

	void set_model(const Model_ref<T> &m)
	{  MODELPLEX_DEBUG(__FUNCTION__,Id(),m.Id());
	   mv_con.disconnect();
	   actmodel=m;
	   actmodel2us();
	   if (actmodel.valid())
	      mv_con=actmodel.signal_changed().connect(
	      		SigC::slot(*this,&this_t::actmodel_value_changed));
	}
	
	const T &operator=(const T &v)
	{  MODELPLEX_DEBUG(__PRETTY_FUNCTION__,Id(),v);
	   return Model<T>::operator=(v); }
	void operator=(const Model_ref<T> &m)
	{  set_model(m); }
};

#endif
