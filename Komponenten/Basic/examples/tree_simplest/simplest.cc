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
{  cout << (string)*(d->Value(0)) << ',' << (string)*(d->Value(1)) << ',' << (string)*(d->Value(2)) <<'\n';
}

simplest::simplest()
{  vector <string> v;
   v.push_back("Integer");
   v.push_back("String");
   v.push_back("something else");
   v.push_back("summe 1");
   v.push_back("summe 2");
   treebase->setTitles(v);
   vector <cH_RowDataBase> datavec;
   datavec.push_back(cH_RowDataStrings("1","C","<none>","8"));
   datavec.push_back(cH_RowDataStrings("2","A","<none>","2"));
   datavec.push_back(cH_RowDataStrings("10","B","<none>","3"));
   treebase->setDataVec(datavec);
   
   treebase->leaf_selected.connect(SigC::slot(this,&simplest::on_leaf_selected));
}

