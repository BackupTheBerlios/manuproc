// $Id: simplest.cc,v 1.9 2004/05/04 08:10:49 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
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

#include "config.h"
#include "simplest.hh"
#include "treebase_data.h"
#include <iostream>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

void simplest::on_Beenden_activate()
{   
}

void simplest::on_leaf_selected(cH_RowDataBase d)
{  std::cout << d->Value(0,0)->getStrVal() << ',' << d->Value(1,0)->getStrVal() 
	<< ',' << d->Value(2,0)->getStrVal() <<'\n';
}

simplest::simplest()
{  std::vector <std::string> v;
   v.push_back("Integer");
   v.push_back("String");
   v.push_back("something else");
   v.push_back("summe 1");
   v.push_back("summe 2");
   treebase->setTitles(v);
   std::vector <cH_RowDataBase> datavec;
   datavec.push_back(cH_RowDataStrings("1","C","<none>","8"));
   datavec.push_back(cH_RowDataStrings("2","A","<none>","2"));
   datavec.push_back(cH_RowDataStrings("10","B","<none>","3"));
   datavec.push_back(cH_RowDataStrings("10","B","<none>","3"));
   treebase->setDataVec(datavec);
   
   treebase->signal_leaf_selected().connect(SigC::slot(*this,&simplest::on_leaf_selected));
}

