#include "config.h"
#include "window1.hh"
#include <Misc/EntryValueIntString.h>
#include <Misc/EntryValueEmptyInt.h>
#include <Misc/itos.h>
#include <Misc/Trace.h>
#include <iostream>
#include <TreeModelDebug.h>
//#include <Misc/TraceNV.h>
//#include <gtkmm/treemodel.h>
//#include <sigc++/compatibility.h>
//#include <sigc++/bind.h>

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
	
 virtual cH_EntryValue Value(guint _seqnr,gpointer gp) const
	{	
	 switch((Spalten)_seqnr)
		{case SP_ATT0 : return cH_EntryValueIntString(intval);
		 case SP_ATT1 : return cH_EntryValueIntString(stringval);
		 case SP_ATT2 : return cH_EntryValueIntString(i2);
		 case SP_ATT3 : return cH_EntryValueIntString(i3);
		 case SP_ATT4 : return cH_EntryValueIntString(s1);
 		 case SP_SUM0 : return cH_EntryValueIntString(Data(_seqnr-SP_SUM0));
 		 default : return cH_EntryValue();
		}
	}
 int Data(int i) const
 {  if (!i) return intval; 
    if (i==1) return intval*2;
    return intval*intval;
 }
 void setIntVal(int i) { intval=i; }
 void setIntVal2(int i) { i2=i; }
 int getIntVal() const { return intval; }
 int getIntVal2() const { return i2; }

#if 0
 std::vector<cH_EntryValueIntString> get_orderd(guint _seqnr) const
   {
     
   }
#endif   
};

class SumNode : public TreeRow
{  int sum0;//,sum1,sum2;
public:
	// const for historical reasons
 virtual void cumulate(const cH_RowDataBase &rd)
   {
    sum0 += (dynamic_cast<const MyRowData &>(*rd)).Data(0);
//    sum1 += (dynamic_cast<const MyRowData &>(*rd)).Data(1);
//    sum2 += (dynamic_cast<const MyRowData &>(*rd)).Data(2);
   }
 virtual void deduct(const cH_RowDataBase &rd)
   {
    sum0 -= (dynamic_cast<const MyRowData &>(*rd)).Data(0);
//    sum1 -= (dynamic_cast<const MyRowData &>(*rd)).Data(1);
//    sum2 -= (dynamic_cast<const MyRowData &>(*rd)).Data(2);
   }

   virtual cH_EntryValue Value(guint col,gpointer gp) const
   {
    switch(col) 
      { case SP_SUM0 : return cH_EntryValueEmptyInt(sum0);
//        case SP_SUM1 : return cH_EntryValueEmptyInt(sum1);
//        case SP_SUM2 : return cH_EntryValueEmptyInt(sum2);
        default : return cH_EntryValue();
      }
   }

 SumNode(const Handle<const TreeRow> &suminit)
   : sum0(0) 
 {  if (suminit) sum0=dynamic_cast<const SumNode&>(*suminit).sum0;
 }
 static Handle<TreeRow> create(const Handle<const TreeRow> &suminit)
 {  return new SumNode(suminit); }
};

// this creates a nice Handle class for convenience
// not strictly needed
class H_MyRowData : public Handle<RowDataBase>
{
public:
 typedef MyRowData ContentType;
 H_MyRowData(ContentType *r) : Handle<RowDataBase>(r) {}
//  H_MyRowData() {}  // not recommended
 H_MyRowData(int i,const std::string &s,int _i2,int _i3,const std::string _s1)
	: Handle<RowDataBase>(new MyRowData(i,s,_i2,_i3,_s1)) {}
 H_MyRowData(const cH_RowDataBase &d)
// better check here ...
 	: Handle<RowDataBase>(const_cast<RowDataBase*>(&*d)) {}
 ContentType *operator*() const
 	{  return &((Handle<ContentType>*)this)->operator*(); }
 ContentType *operator->()
 	{  return &((Handle<ContentType>*)this)->operator*(); }
 operator cH_RowDataBase() const
        {  return operator*(); }
};

bool operator==(const cH_RowDataBase &b, int i)
{  try
   {  H_MyRowData m(b);
//      std::cout << m->Data(0) << ',' << i << '\n';
      return m->Data(0)==i;
   }
   catch (...)
   {  return false; }
}

class OutputFunctor
{	std::ostream &os;
	int sum;
public:
	OutputFunctor(std::ostream &o) : os(o), sum(0) {}
	void operator()(const cH_RowDataBase &b)
	{  H_MyRowData m(b);
	   os << m->Data(0) << ',';
	   sum+=m->Data(0);
	}
	int Sum() const { return sum; }
};

window1::window1() : st(SP_ANZ)
{  scrolledwindow1->add(st);
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
   std::vector <gfloat> v2(st.Cols());
   v2[SP_ATT0]=1;
   v2[SP_ATT1]=0;
   v2[SP_ATT2]=0.5;
   v2[SP_ATT3]=0.5;
   v2[SP_ATT4]=0;
   v2[SP_SUM0]=1;
   st.setAlignment(v2);
   
   std::vector <cH_RowDataBase> datavec;
#if 1
   datavec.push_back(new MyRowData(1,"X",2,3,"A"));
   datavec.push_back(new MyRowData(2,"Y",2,3,"A"));
   datavec.push_back(new MyRowData(10,"Z",2,3,"A"));
#endif
#if 0
   datavec.push_back(new MyRowData(1,"X",2,3,"A"));
   datavec.push_back(new MyRowData(1,"X",2,4,"A"));
   datavec.push_back(new MyRowData(1,"Y",2,3,"A"));
#endif
#if 0
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
   st.set_NewNode(&SumNode::create);
   st.setDataVec(datavec);
   
//   st.signal_leaf_selected().connect(SigC::slot(this,&window1::on_leaf_selected));
   st.selectMatchingLines(2);

#if 1
   {  OutputFunctor of(std::cout);
      st.ForEachLeaf(of);
      std::cout << "=" << of.Sum() << '\n';
   }
#endif
   st.set_remember("(example)","newtree");
//   TreeModelDebug(st.getTreeModel());
   st.debug();
   
//   ManuProC::Tracer::Enable(SimpleTreeStore::trace_channel);
}

void window1::add_one()
{  cH_RowDataBase newrow=new MyRowData(1,"1810",25,755,"25m");
   st.getModel().append_line(newrow);
}

void window1::eine_weg()
{  try 
   {  cH_RowDataBase row=st.getSelectedRowDataBase();
      st.getModel().remove_line(row);
   }
   catch (...) {}
}

void window1::refresh()
{  st.getModel().signal_please_detach()();
   st.getModel().signal_please_attach()();
}

void window1::change1()
{  try 
   {  H_MyRowData row=st.getSelectedRowDataBase();
      st.getModel().about_to_change(row);
      row->setIntVal(row->getIntVal()+1);
      st.getModel().has_changed(row);
   }
   catch (...) {}
}

void window1::change2()
{  try 
   {  H_MyRowData row=st.getSelectedRowDataBase();
      st.getModel().about_to_change(row);
      row->setIntVal2(row->getIntVal2()+1);
      st.getModel().has_changed(row);
   }
   catch (...) {}
}
