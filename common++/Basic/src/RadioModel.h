/* $Id: RadioModel.h,v 1.2 2004/04/29 13:17:23 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Christof Petig
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

#ifndef MANUPROC_RADIOMODEL_H
#define MANUPROC_RADIOMODEL_H

#include <BaseObjects/Model.h>
#if MPC_SIGC_VERSION<0x120
#error SigC 1.2 needed
#endif
#include <sigc++/object_slot.h>
#include <vector>

// excluding options
class RadioModel : public SigC::Object
{	typedef std::vector<Model_ref<bool> > v_t;
	v_t v;

	void callback(void *p)
	{  bool off=false;
	   for (v_t::iterator i=v.begin();i!=v.end();++i)
	   {  if ((*i).matches(p) && i->Value()) 
	      { off=true; break; }
	   }
	   if (off)
	      for (v_t::iterator i=v.begin();i!=v.end();++i)
	         if (!(*i).matches(p)) *i=false;
	}
	void connect()
	{  for (v_t::iterator i=v.begin();i!=v.end();++i)
	      i->signal_changed().connect(SigC::slot(*this,&RadioModel::callback));
	}
public:
	RadioModel(Model_ref<bool> a,Model_ref<bool> b)
	{  v.push_back(a); v.push_back(b);
	   connect();
	}
	RadioModel(Model_ref<bool> a,Model_ref<bool> b,Model_ref<bool> c)
	{  v.push_back(a); v.push_back(b); v.push_back(c);
	   connect();
	}
	RadioModel(const v_t &b) // = vector<Model_ref<bool> >
	 : v(b)
	{  connect();
	}
	RadioModel(const RadioModel &b)
	 : v(b.v)
	{  connect();
	}
};

#endif
