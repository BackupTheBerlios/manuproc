/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
// $Id: with_class.cc,v 1.9 2001/10/23 08:56:40 christof Exp $

#include "config.h"
#include "with_class.hh"
#include <gtk--/main.h>
#include <Aux/EntryValueIntString.h>
#include <Aux/EntryValueEmptyInt.h>
#include <tclistnode.h>

void with_class::on_Beenden_activate()
{   Gtk::Main::instance()->quit();
}

enum Spalten
//{  SP_ATT0, SP_ATT1, SP_ATT2, SP_SUM0, SP_SUM1, SP_SUM2 };
{  SP_ATT0, SP_ATT1, SP_ATT2, SP_ATT3, SP_ATT4, SP_SUM0 };

class MyRowData : public RowDataBase
{
// your data 
 int intval,i2,i3;
 std::string stringval,s1;
 
public:

 MyRowData(int i,const std::string &s,int _i2,int _i3,const std::string _s1)
	: intval(i),i2(_i2),i3(_i3),stringval(s),s1(_s1) {}
	
 virtual const cH_EntryValue Value(guint _seqnr,gpointer gp) const
	{	
	 switch((Spalten)_seqnr)
		{case SP_ATT0 : return cH_EntryValueIntString(intval);
		 case SP_ATT1 : return cH_EntryValueIntString(stringval);
		 case SP_ATT2 : return cH_EntryValueIntString(i2);
		 case SP_ATT3 : return cH_EntryValueIntString(i3);
		 case SP_ATT4 : return cH_EntryValueIntString(s1);
// 		 case SP_SUM0 ... SP_SUM2 : return cH_EntryValueIntString(Data(_seqnr-SP_SUM0));
 		 case SP_SUM0 : return cH_EntryValueIntString(Data(_seqnr));
 		 default : return cH_EntryValueIntString("?");
		}
	}
 int Data(int i) const
 {  if (!i) return intval; 
    if (i==1) return intval*2;
    return intval*intval;
 }

#if 0
 std::vector<cH_EntryValueIntString> get_orderd(guint _seqnr) const
   {
     
   }
#endif   
};

class SumNode : public TCListNode
{  int sum0;//,sum1,sum2;
public:
	// const for historical reasons
 virtual void cumulate(const cH_RowDataBase &rd)
   {
    sum0 += (dynamic_cast<const MyRowData &>(*rd)).Data(0);
//    sum1 += (dynamic_cast<const MyRowData &>(*rd)).Data(1);
//    sum2 += (dynamic_cast<const MyRowData &>(*rd)).Data(2);
   }

   virtual const cH_EntryValue Value(guint col,gpointer gp) const
   {
    switch(col) 
      { case SP_SUM0 : return cH_EntryValueEmptyInt(sum0);
//        case SP_SUM1 : return cH_EntryValueEmptyInt(sum1);
//        case SP_SUM2 : return cH_EntryValueEmptyInt(sum2);
        default : return cH_EntryValueIntString("?");
      }
   }

 SumNode(guint col, const cH_EntryValue &v, bool expand)
//   : TCListNode(col, v, expand), sum0(0),sum1(0),sum2(0) {}
   : TCListNode(col, v, expand), sum0(0) {}
 static TCListNode *create(guint col, const cH_EntryValue &v, bool expand)
    {  return new SumNode(col,v,expand); }
};

#if 1
// this creates a nice Handle class for convenience
// not strictly needed
class cH_MyRowData : public cH_RowDataBase
{
public:
 typedef const MyRowData ContentType;
 cH_MyRowData(ContentType *r) : cH_RowDataBase(r) {}
//  cH_MyRowData() {}  // not recommended
 cH_MyRowData(int i,const std::string &s,int _i2,int _i3,const std::string _s1)
//	: cH_RowDataBase(new MyRowData(i,s)) {}
	: cH_RowDataBase(new MyRowData(i,s,_i2,_i3,_s1)) {}
 cH_MyRowData(const cH_RowDataBase &d)
// better check here ...
 	: cH_RowDataBase(d) {}
 ContentType *operator*() const
 	{  return &((Handle<const ContentType>*)this)->operator*(); }
 ContentType *operator->() const
 	{  return &((Handle<const ContentType>*)this)->operator*(); }
};
#endif

void with_class::on_leaf_selected(cH_RowDataBase d)
{  //const MyRowData *dt=dynamic_cast<const MyRowData*>(&*d);
// cH_MyRowData dt(d); // looks better, eh?
   try
   {  // test this variant, too
//      cH_MyRowData dt=treebase->getSelectedRowDataBase_as<cH_MyRowData,const MyRowData>();
      cH_MyRowData dt=treebase->getSelectedRowDataBase_as<cH_MyRowData>();
      std::cout << "Data " << dt->Data(0) << ',' << dt->Data(1) << ',' << dt->Data(2) << '\n';
   }
   catch (const std::exception &e)
   {  std::cerr << e.what() << '\n';
   }
}

void with_class::on_button_data_clicked()
{
try{
// geht beides
//  cH_MyRowData dt=treebase->getSelectedRowDataBase_as<cH_MyRowData>();
  cH_MyRowData dt(treebase->getSelectedRowDataBase_as<cH_MyRowData>());
  std::cout << "Data " << dt->Data(0) << ',' << dt->Data(1) << ',' << dt->Data(2) << '\n';
}catch(std::exception &e){cerr << e.what();}
}

with_class::with_class()
{  std::vector <std::string> v(treebase->Cols());
   v[SP_ATT0]="Integer";
   v[SP_ATT1]="String";
   v[SP_ATT2]="something else";
   v[SP_ATT3]="something";
   v[SP_ATT4]="else";
   v[SP_SUM0]="summe 1";
//   v[SP_SUM1]="summe 2";
//   v[SP_SUM2]="summe 3";
   treebase->setTitles(v);
   std::vector <cH_RowDataBase> datavec;
   datavec.push_back(new MyRowData(1,"X",2,3,"A"));
   datavec.push_back(new MyRowData(2,"Y",2,3,"A"));
   datavec.push_back(new MyRowData(10,"Z",2,3,"A"));
   treebase->setDataVec(datavec);
   
   treebase->leaf_selected.connect(SigC::slot(this,&with_class::on_leaf_selected));
   treebase->set_NewNode(&SumNode::create);
}
