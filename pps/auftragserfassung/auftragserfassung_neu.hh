#ifndef _A_HH
#  define _A_HH

#include <Aux/ppsInstanz.h>
#include <Artikel/ArtikelBase.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <map>
#include <list>
//#include <Kunde/Kunde.h>
#include <Auftrag/AufEintrag.h>

struct st_artmeng{int aid;fixedpoint<2> menge;AufEintragBase aeb;
         st_artmeng(int i,fixedpoint<2> m,AufEintragBase v)
            : aid(i),menge(m),aeb(v) {}
//         bool operator == (const st_artmeng& b) const
//            {return (aid==b.aid && menge==b.menge);}
      };

struct st_art{ArtikelBase::ID artikelid; Petig::Datum datum;
   bool operator == (const st_art& b) const 
            {return (artikelid==b.artikelid && datum==b.datum );}
   bool operator <  (const st_art& b) const 
            {return  artikelid < b.artikelid ||
               (artikelid == b.artikelid && datum < b.datum)
               ;}
   st_art(ArtikelBase::ID a, Petig::Datum d)
      :artikelid(a),datum(d){}
   st_art(){}
      };

static inline  std::ostream &operator << (std::ostream& o,const st_art&b) 
      {return o<<b.artikelid<<"\t"<<b.datum;}


struct st_menge{unsigned long int menge;vector<AufEintragBase> vAEB;
//   const st_menge& operator += (const st_menge& b) 
//            {  menge+=b.menge; return *this; }  
   st_menge():menge(0){}
   st_menge(unsigned long int m):menge(m){} 
   void addiere(unsigned long int m,AufEintragBase aeb)
      {menge+=m; vAEB.push_back(aeb);}
      };

static inline  std::ostream &operator << (std::ostream& o,const st_menge&b) 
      {return o<<b.menge;}



void get_all_orders(cH_ppsInstanz instanz);
void create_new_auftrag();
list<st_artmeng> get_rohartikel_menge(int artikelid,const AufEintrag& aeb);
void AuftragsEntryZuordnung(const AufEintrag& AEB,long menge,const AuftragBase& AB,int znr);
#endif