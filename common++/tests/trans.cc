#include <Aux/Transaction.h>
#include <Aux/dbconnect.h>
#include <Aux/SQLerror.h>

int main()
{  Petig::dbconnect();
   { Transaction a;
   }
   { Transaction b;
   }
   
   Petig::dbconnect(Petig::Connection("","","","second"));
   
   {
   Petig::dbdefault();
   Transaction c;
   Transaction d("second");
   
   Transaction e("second");
   c.commit();
   Transaction f;
   }
   Petig::dbdisconnect("second");
   Petig::dbdisconnect();
}
