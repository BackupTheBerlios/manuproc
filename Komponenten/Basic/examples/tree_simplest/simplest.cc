// $Id: simplest.cc,v 1.6 2002/07/05 12:36:56 christof Exp $
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

// generated 2001/6/15 9:52:50 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to simplest.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "simplest.hh"
#include "treebase_data.h"


void simplest::on_Beenden_activate()
{   
}

void simplest::on_leaf_selected(cH_RowDataBase d)
{  std::cout << d->Value(0,0)->getStrVal() << ',' << d->Value(1,0)->getStrVal() 
	<< ',' << d->Value(2,0)->getStrVal() <<'\n';
}

simplest::simplest()
{  std::vector <string> v;
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
   
   treebase->leaf_selected.connect(SigC::slot(this,&simplest::on_leaf_selected));
}

