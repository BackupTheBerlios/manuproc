#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AuftragsEntryZuordnung.h>
#include <list>

AuftragsBaum::AuftragsBaum(const AufEintragBase aeb,bool kinder)
{
 std::list<AufEintragZu::st_reflist> tv=AufEintragZu(aeb).get_Referenz_list(aeb,kinder);
 std::list<AufEintragZu::st_reflist> tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (std::list<AufEintragZu::st_reflist>::iterator i=tv.begin();i!=tv.end();++i)
   {
     st_AuftragsBlatt AuftragsBlatt(i->AEB2,i->AB,i->Menge);
     auftragsbaum.push_back(AuftragsBlatt);
     tvxx=AufEintragZu(i->AEB2).get_Referenz_list(i->AEB2,kinder);
     tv.erase(i);
     goto reloop;
   }
}

