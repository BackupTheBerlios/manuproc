#ifndef ARTIKELBAUMFULL_H
#define ARTIKELBAUMFULL_H

#include <Artikel/ArtikelBaum.h>
#include <Aux/fixedpoint.h>
#include <map>

class ArtikelBaumFull
{
 private: 
//   std::map<ArtikelBase,fixedpoint<5,double,long long int> > map_artikel;
   std::map<ArtikelBase,fixedpoint<5> > map_artikel;
   int tiefe;
   
//   void ArtikelBaum_Pfad(ArtikelBase AB,fixedpoint<5,double,long long> menge);
   void ArtikelBaum_Pfad(ArtikelBase AB,fixedpoint<5> menge);
// public:
private:
   ArtikelBaumFull(ArtikelBase artbase);
//   std::map<ArtikelBase,fixedpoint<5,double,long long int> > get_Artikel_Map() const {return map_artikel;}
   std::map<ArtikelBase,fixedpoint<5> > get_Artikel_Map() const {return map_artikel;}
   int Tiefe() const {return tiefe;}
};

#endif
