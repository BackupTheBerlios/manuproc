#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/AufEintragBase.h>
#include <list>

AuftragsBaum::AuftragsBaum(const AufEintragBase2 aeb,bool kinder)
{
 list<AufEintragBaseList::st_reflist> tv=AufEintragBaseList(aeb).get_Referenz_list(aeb,kinder);
 list<AufEintragBaseList::st_reflist> tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (list<AufEintragBaseList::st_reflist>::iterator i=tv.begin();i!=tv.end();++i)
   {
cout << "Baum "<<i->AEB2.Id()<<' '<<i->AEB2.Instanz()<<' '<<i->AEB2.ZNr()<<'\n';
     st_AuftragsBlatt AuftragsBlatt(i->AEB2,i->AB,i->Menge);
     auftragsbaum.push_back(AuftragsBlatt);
     tvxx=AufEintragBaseList(i->AEB2).get_Referenz_list(i->AEB2,kinder);
     tv.erase(i);
     goto reloop;
   }
}

