

#ifndef AUFTRRAG_DRUCKEN_LR_BASE
#define AUFTRRAG_DRUCKEN_LR_BASE

#include<Aux/Datum.h>
#include<string>
#include <Aux/multi_lang.h>
#include <stdio.h>

extern MultiL_Dict *mld;

class LR_Base
{
 bool rueckstand:1;
 
public:
   void setRueckstand(bool r) { rueckstand=r;}
   bool Rueckstand() const { return rueckstand;}
   typedef enum typ {Lieferschein, Rechnung, Auftrag, Intern, Extern, NICHTS}; 
  
    std::string typString(bool gut=false) const { switch(t)
			{case Lieferschein : 
				return mld->MLT(MultiL_Dict::TXT_LIEFERSCHEIN) ;break;
			case Rechnung : 
				if(gut) return std::string("Gutschrift");
				else return mld->MLT(MultiL_Dict::TXT_RECHNUNG); break;
			case Auftrag : 
			     if(rueckstand)
			       {ManuProC::Datum heute=ManuProC::Datum().today();
			        char buf[200];
			        snprintf(buf,sizeof buf,mld->MLT(MultiL_Dict::PRINTF_RUECKSTAND).c_str(),heute.c_str());
			        return std::string("~\\\\")+buf;	
				}
			     else
			     	return mld->MLT(MultiL_Dict::TXT_AUFTRAG);
			     break;     
			case Intern : return std::string("Intern"); break;
			case Extern : return std::string("Bestellung \\small Nr."); break;
			case NICHTS : default : return std::string("-");
			}
		}
  private:
       typ t;


protected:
       bool gut;
public:
   typ Typ() const {return t;}
protected:

   LR_Base(typ _t) : rueckstand(false), t(_t)  {}
   bool gutschrift() const { return gut; }
};

#endif
