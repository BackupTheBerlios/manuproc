// $Id: mvc2.cc,v 1.1 2003/09/12 07:29:44 christof Exp $
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

#include <iostream>
#include <BaseObjects/RadioModel.h>

#ifndef SIGC1_2
#error you need sigc1.2 for this
#endif

#include <vector>

class View_bool : public SigC::Object
{	Model_ref<bool> model;
	SigC::Connection c;
	std::string name;
	
	void refresh(void *x)
	{  if (model.matches(x))
	      std::cout << name << ": value changed to " << model.get_value() << '\n';
	}
public:
	View_bool(const Model_ref<bool> &m,const std::string &n="View") : model(m), name(n)
	{  c=m.signal_changed().connect(SigC::slot(*this,&View_bool::refresh));
	   std::cout << name << ": Initial value " << model.get_value() << '\n';
	}
	void operator=(const Model_ref<bool> &m2)
	{  c.disconnect();
	   model=m2;
	   std::cout << name << ": Model changed, value " << model.get_value() << '\n';
	   c=model.signal_changed().connect(SigC::slot(*this,&View_bool::refresh));
	}
};

int main()
{  { Model<bool> a,b;
     View_bool v(a,"a"),v2(b,"b");
//     RadioModel r(a,b);
     std::vector<RadioModel> vec;
     {vec.push_back(RadioModel(a,b));}
     a=true;
     b=true;
   }
   return 0;
}
