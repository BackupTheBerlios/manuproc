#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/AufEintragBase.h>
#include <list>

AuftragsBaum::AuftragsBaum(const AufEintragBase2 aeb,bool kinder)
{
 list<pair<AufEintragBase2,long> > tv=AufEintragBase(aeb).get_Referenz_list(aeb,kinder);
 list<pair<AufEintragBase2,long> > tvxx;
reloop:
 tv.merge(tvxx);
 for (list<pair<AufEintragBase2,long> >::iterator i=tv.begin();i!=tv.end();++i)
   {
     st_AuftragsBlatt AuftragsBlatt(i->first,i->second);
     auftragsbaum.push_back(AuftragsBlatt);
     tvxx=AufEintragBase(i->first).get_Referenz_list(i->first,kinder);
     tv.erase(i);
     goto reloop;
   }
}

