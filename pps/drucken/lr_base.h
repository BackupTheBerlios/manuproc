

#ifndef AUFTRRAG_DRUCKEN_LR_BASE
#define AUFTRRAG_DRUCKEN_LR_BASE

#include<Aux/Datum.h>
#include<string>

class LR_Base
{
 bool rueckstand:1;
 
public:
   void setRueckstand(bool r) { rueckstand=r;}
   bool Rueckstand() const { return rueckstand;}
   typedef enum typ {Lieferschein, Rechnung, Auftrag, Intern, Extern, NICHTS}; 
  
    std::string typString(bool gut=false) const { switch(t)
			{case Lieferschein : return std::string("Lieferschein"); break;
			case Rechnung : 
				if(gut) return std::string("Gutschrift");
				else return std::string("Rechnung"); break;
			case Auftrag : 
			     if(rueckstand)
			       {ManuProC::Datum heute=ManuProC::Datum().today();
			        return std::string("~\\\\Rückstand am ")+heute.c_str()+" zum Auftrag ";	
				}
			     else
			     	return std::string("Auftrag");
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
