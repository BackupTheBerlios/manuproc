

#include "multi_lang.h"
#include <Misc/FetchIStream.h>
#include <Misc/itos.h>

MultiL_Dict::MultiL_Dict(int sprache, int default_spr) : 
	sprid(sprache), def_sprid(default_spr)
{
 (Query("select textid,text_lang from text_bausteine "
 	"where sprid=?") << sprid).FetchArray(textmap); 		
 (Query("select textid,text_lang from text_bausteine "
 	"where sprid=?") << default_spr).FetchArray(defaultmap);
}


std::string MultiL_Dict::MLT(const LangTXT textid, std::string def_str)
{
 std::string ret(textmap[textid]);
 
 if (ret.empty())
 {  ret=defaultmap[textid];
    if (ret.empty()) 
      {
	if(textid>=CUSTOMER_IDS)
	  { std::string text;
	  
	   Query q1("select textid,text_lang from text_bausteine "
 	   "where sprid=? and textid=?");
 	   q1 << sprid << textid;
 	   
 	   if(q1.good()) {FetchIStream f=q1.FetchOne(); f >> text;}
 	   textmap[textid]=text; 	   
 	   
 	   ret=text;
 	   
	   Query q("select textid,text_lang from text_bausteine "
 	   "where sprid=? and textid=?");
 	   q << def_sprid << textid;
 	   
 	   if(q.good()) {FetchIStream f=q.FetchOne(); f >> text;}
 	   
 	   defaultmap[textid]=text;

 	   if(ret.empty()) 
 	     {ret=text;
 	      if(ret.empty()) 
 	        {ret=def_str;
        	 defaultmap[textid]=ret;    
 	        }
 	     }
	  }
	else  
          ret=def_str;//ret="MLD-"+itos(textid);
      }
 }
 return ret;
}

