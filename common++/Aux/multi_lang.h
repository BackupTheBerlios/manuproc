
#include <map>
#include <string>

#ifndef MULTI_LANG
#define MULTI_LANG


class MultiL_Dict
{
 int sprid;
 map<int,std::string> textmap;
 map<int,std::string> defaultmap; 

public:
 typedef enum {	TXT_RECHNUNG=1,
		TXT_AUFTRAG,
		TXT_VOM,
		TXT_DANKE_AUFTR,
		TXT_LIEF_FREI,
		TXT_WARE_ZOLL,
		TXT_BANKVERB,
		TXT_KONTO,		
		TXT_BANK,				
		TXT_SEITE,
		TXT_ZAHLUNG,
		PRINTF_ZAHLUNG1,
		PRINTF_ZAHLUNG2,		
	     } LangTXT;

 MultiL_Dict(int sprache, int default_spr=1);

 std::string MLT(LangTXT textid);


};


#endif


