// $Id: ModelPlex.h,v 1.2 2003/09/03 08:03:27 christof Exp $
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

#ifndef SIGC1_2
#error this class needs SigC 1.2+
#endif

template <class T>
 class ModelPlex : public SigC::Object, public Model<T>
{	typedef ModelPlex<T> this_t;
	
	SigC::Connection mv_con, cm_con;
	Model_ref<T> actmodel;

	// model ist actmodel, we+we ist this
	void we2actmodel(void *x)
	{  mv_con.block();
	   if (actmodel.valid()) actmodel=Value();
           mv_con.unblock();
	}
	void actmodel2us()
	{  cm_con.block();
	   if (Value()!=actmodel.Value()) // do not fire if unchanged
	      *this=actmodel.Value(); 
	   cm_con.unblock();
	}
	void actmodel_value_changed(void *x)
	{  if (actmodel.matches(x)) actmodel2us();
	}

public:
	ModelPlex(const Model_ref<T> &m=Model_ref<T>())
	{ cm_con=signal_changed().connect(
			SigC::slot(*this,&this_t::we2actmodel));
	  if (m.valid()) set_model(m); 
	}

	void set_model(const Model_ref<T> &m)
	{  mv_con.disconnect();
	   actmodel=m;
	   actmodel2us();
	   if (actmodel.valid())
	      mv_con=actmodel.signal_changed().connect(
	      		SigC::slot(*this,&this_t::actmodel_value_changed));
	}
	
	const T &operator=(const T &v)
	{  return Model<T>::operator=(v); }
	void operator=(const Model_ref<T> &m)
	{  set_model(m); }
};

#endif
