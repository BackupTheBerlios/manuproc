// generated 2002/11/7 16:33:48 CET by christof@puck.petig-baender.de
// using glademm V1.1.2a_cvs
//
// newer (non customized) versions of this file go to window1.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window1.hh"
#include <Misc/EntryValueIntString.h>
#include <Misc/itos.h>
#include <iostream>

enum Spalten
{  SP_ATT0, SP_ATT1, SP_ATT2, SP_ATT3, SP_ATT4, 
   SP_SUM0, SP_ANZ /* , SP_SUM1, SP_SUM2 */ };

class MyRowData : public RowDataBase
{
// your data 
 int intval,i2,i3;
 std::string stringval,s1;
 
public:

 MyRowData(int i,const std::string &s,int _i2,int _i3,const std::string &_s1)
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
 		 case SP_SUM0 : return cH_EntryValueIntString(Data(_seqnr-SP_SUM0));
 		 default : return cH_EntryValue();
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

#if 0
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
        default : return cH_EntryValue();
      }
   }

 SumNode(guint col, const cH_EntryValue &v, guint child_s_deep, 
 	cH_RowDataBase child_s_data, bool expand, const TreeRow &suminit)
   : TCListNode(col, v, child_s_deep, child_s_data, expand), sum0(0) 
 {  if (suminit.Leaf()) cumulate(child_s_data);
    else sum0=dynamic_cast<const SumNode&>(suminit).sum0;
 }
 static TCListNode *create(guint col, const cH_EntryValue &v, guint child_s_deep, 
 	cH_RowDataBase child_s_data, bool expand,
 	const TreeRow &suminit)
 {  return new SumNode(col,v, child_s_deep, child_s_data, expand, suminit); }
};
#endif

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

window1::window1() : st(SP_ANZ,SP_SUM0)
{  add(st);
   st.show();

  std::vector <std::string> v(st.Cols());
   v[SP_ATT0]="Integer";
   v[SP_ATT1]="String";
   v[SP_ATT2]="something else";
   v[SP_ATT3]="something";
   v[SP_ATT4]="else";
   v[SP_SUM0]="summe 1";
//   v[SP_SUM1]="summe 2";
//   v[SP_SUM2]="summe 3";
   st.setTitles(v);
   std::vector <cH_RowDataBase> datavec;
#if 0
   datavec.push_back(new MyRowData(1,"X",2,3,"A"));
   datavec.push_back(new MyRowData(2,"Y",2,3,"A"));
   datavec.push_back(new MyRowData(10,"Z",2,3,"A"));
#endif
#if 0
   datavec.push_back(new MyRowData(1,"X",2,3,"A"));
   datavec.push_back(new MyRowData(1,"X",2,4,"A"));
   datavec.push_back(new MyRowData(1,"Y",2,3,"A"));
#endif
#if 1
   for (int i=0;i<100;++i)
      datavec.push_back(new MyRowData(i%4+1,/*"same" */
      std::string(1,char('A'+(i%3))),(i%5),(i%7),itos(i)));
#endif
#if 0
   datavec.push_back(new MyRowData(1,"1810",25,755,"25m"));
   datavec.push_back(new MyRowData(1,"1810",40,100,"9999"));
   datavec.push_back(new MyRowData(1,"1955",25,855,"50m"));
   datavec.push_back(new MyRowData(1,"1955",40,210,"Jumbo"));
#endif
//   st.set_NewNode(&SumNode::create);
   st.setDataVec(datavec);
   
//   st.leaf_selected.connect(SigC::slot(this,&with_class::on_leaf_selected));
//   st.selectMatchingLines(2);

#if 0   
   {  OutputFunctor of(std::cout);
      st.ForEachLeaf(of);
      std::cout << "=" << of.Sum() << '\n';
   }
#endif
   st.set_remember("(example)","newtree");
}
