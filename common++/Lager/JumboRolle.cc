#include"JumboRolle.h"

bool JumboRolle::isRollNrOK(int eancode)
{
 int i=0;
 int code=eancode/10;
 int check=eancode%10;
 int hilf,ziffer,sum=0;

 while(i<(CODELENGTH-1))
   {
    ziffer=code%10;
    code/=10;
    hilf= i%2 ? ziffer : 3*ziffer;
    sum+=hilf;
    i++;
   } 

 sum%=10;
 sum = sum ? 10-sum : 0;
 if(sum==check) return true;
 return false;
}

int JumboRolle::Pruefziffer_anhaengen(int nummer)
{
 int code=nummer;
 int sum=0;

 for (int i=0;i<(CODELENGTH-1);++i)
   {
    int ziffer=code%10;
    code/=10;
    int hilf= i%2 ? ziffer : 3*ziffer;
    sum+=hilf;
   } 
 sum%=10;
 return nummer*10+(10-sum)%10;
}

std::string JumboRolle::CodeStr() const 
{ char ctxt[10];
  snprintf(ctxt,10,"%04d %04d",code/10000,code%10000);
  return ctxt;
}


std::string JumboRolle::MeterStr() const 
{
 char mtxt[20];
 if(barcoist == sollmeter)
   {if(!barcoist) return "--";
    else
      {snprintf(mtxt,20,"%4d",barcoist); return mtxt;}
   }
 
 if(!barcoist){snprintf(mtxt,20,"[%4d]",sollmeter); return mtxt;}
 if(!sollmeter){snprintf(mtxt,20,"%4d",barcoist); return mtxt;}

 snprintf(mtxt,20,"%4d[%4d]",barcoist,sollmeter);
 return mtxt;
}

int JumboRolle::Meter() const 
{
 if(barcoist) return barcoist;
 else return sollmeter;
 abort();
}


