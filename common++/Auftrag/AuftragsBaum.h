#ifndef CXX_AUFTRAG_AUFTRAGSBAUM_H
#define CXX_AUFTRAG_AUFTRAGSBAUM_H

#include <Auftrag/AufEintragBase2.h>
#include <vector>

class AuftragsBaum 
{
 public:
        struct st_AuftragsBlatt
                { AufEintragBase2 AEB2; 
                  long menge;
                  st_AuftragsBlatt(AufEintragBase2 a,long m)
                    : AEB2(a),menge(m) {} 
                };

 private:
        typedef vector<st_AuftragsBlatt> auftragsbaum_t;
 public:
        typedef auftragsbaum_t::const_iterator const_iterator;
        
 private:
        auftragsbaum_t auftragsbaum;
        
 public:
        AuftragsBaum(const AufEintragBase2 aeb,bool kinder=false); 
                //false: Alle Referenzaufträge
                //true:  Alle Kinderaufträge
                
         size_t size() const
          {  return auftragsbaum.size(); }
         const_iterator begin() const
          {  return auftragsbaum.begin(); }
         const_iterator end() const
          {  return auftragsbaum.end(); }
              
};                    
#endif
