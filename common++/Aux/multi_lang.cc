

#include "multi_lang.h"
#include <Misc/FetchIStream.h>
#include <Misc/itos.h>

MultiL_Dict::MultiL_Dict(int sprache, int default_spr) : sprid(sprache)
{
 (Query("select textid,text_lang from text_bausteine "
 	"where sprid=?") << sprid).FetchArray(textmap); 		
 (Query("select textid,text_lang from text_bausteine "
 	"where sprid=?") << default_spr).FetchArray(defaultmap);
}


std::string MultiL_Dict::MLT(const LangTXT textid)
{
 std::string ret(textmap[textid]);
 
 if (ret.empty())
 {  ret=defaultmap[textid];
    if (ret.empty()) ret="MLD-"+itos(textid);
 }
 return ret;
}
