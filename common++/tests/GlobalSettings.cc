#include <Misc/Global_Settings.h>
#include <Misc/dbconnect.h>
#include <iostream>

int main()
{  ManuProC::dbconnect();
   Global_Settings::create(getuid(),"test","test","20:20");
   Global_Settings::create(getuid(),"test","test2","10:20:30:40");
  {Global_Settings gs(getuid(),"test","test");
   std::cout << gs.get_Wert() << ' ' << gs.get_Wert(":",1)
   	<< ' ' << gs.get_Wert(":",2) << '\n';
  }
  {Global_Settings gs(getuid(),"test","test2");
   std::cout << gs.get_Wert() << ' ' << gs.get_Wert(":",1)
   	<< ' ' << gs.get_Wert(":",2) << ' ' << gs.get_Wert(":",3) 
   	<< ' ' << gs.get_Wert(":",4) << '\n';
  }
   ManuProC::dbdisconnect();
   return 0;
}
