#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AufEintragZu.h>
#include <list>

AuftragsBaum::AuftragsBaum(const AufEintragBase aeb,bool kinder)
{
 std::list<AufEintragZu::st_reflist> tv=AufEintragZu(aeb).get_Referenz_list(aeb,kinder);
 std::list<AufEintragZu::st_reflist> tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (std::list<AufEintragZu::st_reflist>::iterator i=tv.begin();i!=tv.end();++i)
   {
     st_AuftragsBlatt AuftragsBlatt(i->AEB,i->Art,i->Menge);
     auftragsbaum.push_back(AuftragsBlatt);
     tvxx=AufEintragZu(i->AEB).get_Referenz_list(i->AEB,kinder);
     tv.erase(i);
     goto reloop;
   }
}

