// generated 2004/5/11 13:36:34 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0.2
//
// newer (non customized) versions of this file go to kunden_selector.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "kunden_selector.hh"
#include "treebase_data.h"

kunden_selector::kunden_selector(Query &q,
	std::string *_nr,std::string *_name,std::string *_ort)
: nr(_nr), name(_name), ort(_ort)
{
 std::vector<std::string> v;

 v.push_back("Customer No.");
 v.push_back("Name");
 v.push_back("Lcation");

 kunden_select_liste->setTitles(v);

 std::vector <cH_RowDataBase> dv;
 FetchIStream fi=q.Fetch();

 while(fi.good())
   {
    std::string nr,name,ort;
    fi >> nr >> name >> ort;
    dv.push_back(cH_RowDataStrings(nr,name,ort));
    fi=q.Fetch();
   }
 kunden_select_liste->setDataVec(dv);

}



void kunden_selector::on_cancel_kunde_select_clicked()
{  
}

void kunden_selector::on_ok_kunde_select_clicked()
{  
}

void kunden_selector::on_kundenlist_leaf_selected(cH_RowDataBase d)
{
 *nr=d->Value(0,0)->getStrVal();
 *name=d->Value(1,0)->getStrVal();
 *ort=d->Value(2,0)->getStrVal();
}

