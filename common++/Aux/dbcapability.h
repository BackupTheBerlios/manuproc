

#include <Misc/SQLerror.h>
#include <multimap.h>
#include <pair.h>
#include <Misc/FetchIStream.h>

class DBCapability
{

public:
 typedef enum {UPDATE='U', INSERT='I', DELETE='D', SELECT='S'} Action;
 typedef std::multimap<std::string,Action> WhiteTableAction;
 typedef std::pair<std::string,std::string> WhiteColumn;
 typedef std::multimap<WhiteColumn,Action> WhiteColumnAction;
 typedef std::pair<std::string, Action> StrAct;
 typedef std::pair<WhiteColumn,Action> ColAct;
 
private:
 
 WhiteTableAction wta;
 WhiteColumnAction wca; 
 
public:
 DBCapability() throw(SQLerror);
 bool isWhite(const StrAct sa);
 bool isWhite(const ColAct ca);

 friend FetchIStream &operator>>(FetchIStream &is, std::pair<std::string,Action> &p);
 friend FetchIStream &operator>>(FetchIStream &is, std::pair<WhiteColumn,Action> &p); 

};

