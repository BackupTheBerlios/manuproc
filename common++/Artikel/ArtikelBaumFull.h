#ifndef ARTIKELBAUMFULL_H
#define ARTIKELBAUMFULL_H

#include <Artikel/ArtikelBaum.h>
#include <Aux/fixedpoint.h>
#include <map>

class ArtikelBaumFull
{
 private: 
   map<ArtikelBase,fixedpoint<5,double,long long int> > map_artikel;
   
   void ArtikelBaum_Pfad(ArtikelBase AB,fixedpoint<5,double,long long> menge);
 public:
   ArtikelBaumFull(ArtikelBase artbase);
   map<ArtikelBase,fixedpoint<5,double,long long int> > get_Artikel_Map() const {return map_artikel;}
};

#endif
