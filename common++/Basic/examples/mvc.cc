// $Id: mvc.cc,v 1.13 2003/09/04 07:43:34 christof Exp $
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
#include <BaseObjects/Model.h>
#ifndef SIGC1_2
#include <sigc++/signal_system.h>
#define SLOT_ARG(x) (x)
#else
//#include <sigc++/object.h>
#include <sigc++/object_slot.h> // sigc++.h> // hmm where do you get SigC::slot
#define SLOT_ARG(x) (*(x))
#endif
#include <utility>
#include <vector>

#include <BaseObjects/ModelPlex.h>

class View_int : public SigC::Object
{	Model_ref<int> model;
	SigC::Connection c;
	
	void refresh(void *x)
	{  if (model.matches(x))
	      std::cout << "View: value changed to " << model.get_value() << '\n';
	}
public:
	View_int(const Model_ref<int> &m) : model(m)
	{  c=m.signal_changed().connect(SigC::slot(SLOT_ARG(this),&View_int::refresh));
	   std::cout << "View: Initial value " << model.get_value() << '\n';
	}
	void operator=(const Model_ref<int> &m2)
	{  c.disconnect();
	   model=m2;
	   std::cout << "View: Model changed, value " << model.get_value() << '\n';
	   c=model.signal_changed().connect(SigC::slot(SLOT_ARG(this),&View_int::refresh));
	}
};

int main()
{  std::cout << "Model overhead " << sizeof(Model<int>)-sizeof(int) << " bytes\n";
   std::cout << "View overhead " << sizeof(View_int) << " bytes\n";

   { Model<int> model(2),model2(4);
     View_int view(model);
     model=3;
     view=model2;
     model=7;
     model2=8;
     view=model;
   }
   
   // now we test a structure with a shared signal (and use a Model_ref)
   { Model<std::pair<int,int> > model(std::pair<int,int>(1,2));
     Model_ref<int> a(model.Valueptr()->first, model.signal_changed()),
     		b(model.Valueptr()->second, model.signal_changed());
     View_int view(a);
     View_int view2(b);
     a=12;
     b=13;
   }

   // now we test a structure with a shared signal (complicated way)
   { Model<std::pair<int,int> > model(std::pair<int,int>(1,2));
     View_int view(Model_ref<int>(model.Valueptr()->first, model.signal_changed()));
     View_int view2(Model_ref<int>(model.Valueptr()->second, model.signal_changed()));
     model.Assign(model.Valueptr()->first, 10);
     model.Assign(model.Valueptr()->second, 11);
   }

   // perhaps the ease recommends the extra bytes for a signal per element
   { std::pair<Model<int>,Model<int> > model(std::pair<int,int>(1,2));
     View_int view(model.first);
     View_int view2(model.second);
     model.first=5;
     model.second=6;
   }
   
   {  std::vector<Model_copyable<int> > v;
      v.push_back(Model_copyable<int>(20));
      View_int view(v.back());
      v.back()=25;
   }
   
   { std::cout << "Multiplexer test\n";
     Model<int> model(2),model2(4);
     ModelPlex<int> plex;
     View_int view(plex); // ,view2(model);
     plex.set_model(model);
     plex=7;
     plex.set_model(model2);
     model2=1;
     plex.set_model(model);
     std::cout << "model=" << model.Value() << " model2="<<model2.Value() << '\n';
   }
   return 0;
}
