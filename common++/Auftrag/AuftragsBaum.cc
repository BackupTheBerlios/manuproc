#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/AuftragsEntryZuordnung.h>
#include <list>

AuftragsBaum::AuftragsBaum(const AufEintragBase2 aeb,bool kinder)
{
 list<AufEintragZu::st_reflist> tv=AufEintragZu(aeb).get_Referenz_list(aeb,kinder);
 list<AufEintragZu::st_reflist> tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (list<AufEintragZu::st_reflist>::iterator i=tv.begin();i!=tv.end();++i)
   {
     st_AuftragsBlatt AuftragsBlatt(i->AEB2,i->AB,i->Menge);
     auftragsbaum.push_back(AuftragsBlatt);
     tvxx=AufEintragZu(i->AEB2).get_Referenz_list(i->AEB2,kinder);
     tv.erase(i);
     goto reloop;
   }
}

