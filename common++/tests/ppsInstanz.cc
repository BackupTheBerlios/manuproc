#include <Instanzen/ppsInstanz.h>
#include <vector>
#include <Aux/dbconnect.h>
#include <map>

int main()
{
  Petig::dbconnect();
  cH_ppsInstanz I(ppsInstanz::INST_WEB);
  std::map<cH_ppsInstanz,int> V;
  V[cH_ppsInstanz(ppsInstanz::INST_SCHAER)]=1;
  V[cH_ppsInstanz(ppsInstanz::INST_SPRITZ)]=2;
  V[cH_ppsInstanz(ppsInstanz::INST_DRUCK)]=5;
  V[cH_ppsInstanz(ppsInstanz::INST_WEB)]=6;
  V[cH_ppsInstanz(ppsInstanz::INST_KNDAUF)]=3;
  V[cH_ppsInstanz(ppsInstanz::INST_FAERB)]=4;

  for(std::map<cH_ppsInstanz,int>::const_iterator i=V.begin();i!=V.end();++i)
   {
    cout << i->first->Sortierung() <<'\n';
   }
}

