#include <Aux/ppsInstanz.h>
#include <Artikel/ArtikelBase.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <map>
#include <list>


struct st_artmeng{int aid;fixedpoint<2> menge;
         st_artmeng(int i,fixedpoint<2> m):aid(i),menge(m){}
         bool operator == (const st_artmeng& b) const
            {return (aid==b.aid && menge==b.menge);}
//         bool operator < (const st_artmeng& b) const
//            {return aid <b.aid ||  (aid==b.aid && menge<b.menge);}
      };

struct st_art{ArtikelBase::ID artikelid; Petig::Datum datum;
   bool operator == (const st_art& b) const 
            {return (artikelid==b.artikelid && datum==b.datum );}
   bool operator <  (const st_art& b) const 
            {return artikelid < b.artikelid || 
                (artikelid==b.artikelid && datum < b.datum );}
   st_art(ArtikelBase::ID a, Petig::Datum d):artikelid(a),datum(d){}
   st_art(){}
      };

static inline  std::ostream &operator << (std::ostream& o,const st_art&b) 
      {return o<<b.artikelid<<"\t"<<b.datum;}


struct st_menge{unsigned long int menge;
   const st_menge& operator += (const st_menge& b) 
            {  menge+=b.menge; return *this; }  
   st_menge():menge(0){}
   st_menge(unsigned long int m):menge(m){} 
      };

static inline  std::ostream &operator << (std::ostream& o,const st_menge&b) 
      {return o<<b.menge;}



void get_all_orders(cH_ppsInstanz instanz);
void cummulate(cH_ppsInstanz instanz);
void create_new_auftrag(cH_ppsInstanz instanz);
list<st_artmeng> get_rohartikel_menge(int artikelid);

