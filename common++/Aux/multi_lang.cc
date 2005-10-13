

#include "multi_lang.h"
#include <Misc/FetchIStream.h>
#include <Misc/itos.h>

MultiL_Dict::MultiL_Dict(int sprache, int default_spr) : 
	sprid(sprache), def_sprid(default_spr),show_id(false)
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
	  
	   try{
	   Query q1("select textid,text_lang from text_bausteine "
 	   "where sprid=? and textid=?");
 	   q1 << sprid << textid;
 	   FetchIStream fi=q1.FetchOne();

 	   if(fi.good()) {fi >> text;}
 	   textmap[textid]=text; 	   
 	   
 	   ret=text;
 	   
	   Query q("select textid,text_lang from text_bausteine "
 	   "where sprid=? and textid=?");
 	   q << def_sprid << textid;
 	   fi=q.FetchOne();
 	   if(fi.good()) {fi >> text;}
 	   
 	   defaultmap[textid]=text;
	   }

	   catch(SQLerror &e)
	   { if(e.Code()!=100) throw(e); }

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
 if(show_id)
   return std::string("<")+ret+"<"+itos(textid)+"> >";
 else
   return ret;
}

