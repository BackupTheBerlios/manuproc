// $Id: mvc.cc,v 1.5 2002/11/06 12:02:34 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
#include <BaseObjects/MVC.h>
#include <sigc++/signal_system.h>
#include <utility>

class View_int : public SigC::Object
{	Model_ref<int> model;
	
	void refresh(void *x)
	{  if (&model.get_value()==x)
	      std::cout << "View: value changed to " << model.get_value() << '\n';
	}
public:
	View_int(const Model_ref<int> &m) : model(m)
	{  m.changed.connect(SigC::slot(this,&View_int::refresh));
	   std::cout << "View: Initial value " << model.get_value() << '\n';
	}
};

int main()
{  std::cout << "Model overhead " << sizeof(MVC<int>)-sizeof(int) << " bytes\n";
   std::cout << "View overhead " << sizeof(View_int) << " bytes\n";

   { MVC<int> model(2);
     View_int view(model);
     model=3;
   }
   
   // now we test a structure with a shared signal
   { MVC<std::pair<int,int> > model(std::pair<int,int>(1,2));
     View_int view(Model_ref<int>(model.Value().first, model.changed));
     View_int view2(Model_ref<int>(model.Value().second, model.changed));
     model.Assign(model.Value().first, 5);
     model.Assign(model.Value().second, 6);
   }

   // perhaps the ease recommends the extra bytes for a signal per element
   { std::pair<MVC<int>,MVC<int> > model(std::pair<int,int>(1,2));
     View_int view(model.first);
     View_int view2(model.second);
     model.first=5;
     model.second=6;
   }
   return 0;
}
