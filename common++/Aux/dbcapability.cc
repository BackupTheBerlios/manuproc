

#include "dbcapability.h"
#include <vector.h>


DBCapability::DBCapability() throw(SQLerror)
{
 vector<pair<std::string,Action> > tab_act;
 
 std::string qu="select tabelle,aktion from capability where "
	" benutzer=user and spalte is null";

 Query(qu).FetchArray(tab_act);
 SQLerror::test(__FILELINE__,100);

 for(vector<pair<std::string,Action> >::const_iterator i=tab_act.begin();
 		i!=tab_act.end(); ++i)
    wta.insert(pair<std::string,Action>((*i).first,(*i).second));

 qu="select tabelle,spalte,aktion from capability where "
	" benutzer=user and spalte is not null";


 vector<pair<WhiteColumn,Action> > tabcol_act;
 
 Query(qu).FetchArray(tabcol_act);
 SQLerror::test(__FILELINE__,100);

 for(vector<pair<WhiteColumn,Action> >::const_iterator i=tabcol_act.begin();
 		i!=tabcol_act.end(); ++i)
    wca.insert(pair<WhiteColumn,Action>((*i).first,(*i).second));



}

bool DBCapability::isWhite(const StrAct sa)
{

}

bool DBCapability::isWhite(const ColAct ca)
{
 for(WhiteColumnAction::const_iterator i=wca.begin(); i!=wca.end();++i)
   if((*i).first == ca.first)
     if((*i).second == ca.second) return true;
 return false;       
}






FetchIStream &operator>>(FetchIStream &is, 
		pair<std::string,DBCapability::Action> &p)
{
 char a;
 is >> p.first >> a;
 p.second=(DBCapability::Action)a;
 
 return is;
}

FetchIStream &operator>>(FetchIStream &is, 
		pair<DBCapability::WhiteColumn,DBCapability::Action> &p)
{
 std::string tab;
 std::string col;
 char a;
 is >> tab >> col >> a;
 
 p.first=DBCapability::WhiteColumn(tab,col);
 p.second=(DBCapability::Action)a;
 
 return is;
}




