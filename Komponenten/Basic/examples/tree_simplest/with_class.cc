// generated 2001/6/15 9:52:50 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to with_class.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "with_class.hh"

void with_class::on_Beenden_activate()
{   
}

#include <Aux/EntryValueIntString.h>

class MyRowData : public RowDataBase
{
// your data 
 int intval;
 string stringval;
 
public:

 MyRowData(int i,const string &s)
	: intval(i),stringval(s) {}
	
 virtual const cH_EntryValue Value(int _seqnr) const
	{switch(_seqnr)
		{case 0 : return cH_EntryValueIntString(intval); break;
		 case 1 : return cH_EntryValueIntString(stringval); break;
		 case 2 : return cH_EntryValueIntString("<none>"); break;
 		 default : return cH_EntryValueIntString("?"); break;
		}
	}
 int DataI() const
 {  return intval; }
};

#if 0
// this creates a nice Handle class for convenience
// not strictly needed
class cH_MyRowData : public cH_RowDataBase
{
 cH_MyRowData(const MyRowData *r) : cH_RowDataBase(r) {}
public:
//  cH_MyRowData() {}  // not recommended
 cH_MyRowData(int i,const string &s)
	: cH_RowDataBase(new MyRowData(i,s)) {}
 cH_MyRowData(const cH_RowDataBase &d)
// better check here ...
 	: cH_RowDataBase(d) {}
 const MyRowData *operator*()
 	{  return dynamic_cast<MyRowData*>(&**this); }
};
#endif

void with_class::on_leaf_selected(cH_RowDataBase d)
{  const MyRowData *dt=dynamic_cast<const MyRowData*>(&*d);
// cH_MyRowData dt(d); // looks better, eh?
   cout << "Data " << dt->DataI() << '\n';
}

with_class::with_class()
{  vector <string> v;
   v.push_back("Integer");
   v.push_back("String");
   v.push_back("something else");
   v.push_back("summe 1");
   v.push_back("summe 2");
   treebase->setTitles(v);
   vector <cH_RowDataBase> datavec;
//   datavec.push_back(cH_MyRowData(1,"X"));
   datavec.push_back(new MyRowData(1,"X"));
   datavec.push_back(new MyRowData(2,"Y"));
   datavec.push_back(new MyRowData(10,"Z"));
   treebase->setDataVec(datavec);
   
   treebase->leaf_selected.connect(SigC::slot(this,&with_class::on_leaf_selected));
}
