

#include "multi_lang.h"
#include <Misc/FetchIStream.h>
#include <Misc/itos.h>

MultiL_Dict::MultiL_Dict(int sprache, int default_spr) : sprid(sprache)
{
 std::string qu="select textid,text_lang from text_bausteine";
 qu+=" where sprid="+itos(sprid);
 		
 Query(qu).FetchArray(textmap); 		
 
 qu="select textid,text_lang from text_bausteine";
 qu+=" where sprid="+itos(default_spr);
 		
 Query(qu).FetchArray(defaultmap); 		 

}


std::string MultiL_Dict::MLT(const LangTXT textid)
{
 std::string ret(textmap[textid]);
 
 if(ret.empty()) return defaultmap[textid];
 
 return ret;
 
}







