#include <Aux/Transaction.h>
#include <Aux/dbconnect.h>
#include <Aux/SQLerror.h>

int main()
{  ManuProC::dbconnect();
   { Transaction a;
   }
   { Transaction b;
   }
   
   ManuProC::dbconnect(ManuProC::Connection("","","","second"));
   
   {
   ManuProC::dbdefault();
   Transaction c;
   Transaction d("second");
   
   Transaction e("second");
   c.commit();
   Transaction f;
   }
   ManuProC::dbdisconnect("second");
   ManuProC::dbdisconnect();
}
