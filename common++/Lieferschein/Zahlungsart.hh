#ifndef ZAHLUNGSART__
#define ZAHLUNGSART__

#include <string>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>  


class Zahlungsart  : public HandleContent
{
        std::string kurzbezeichnung;
        std::string bezeichnung;
 protected:
        int id;
        static const int _illegal=-1;
 public:
        typedef int ID;
        static const ID none_id=_illegal;
        
        Zahlungsart() : id(none_id) {}
        Zahlungsart(int _id);

        ID Id() const {return id;}
        std::string Kurzbezeichnung() const {return kurzbezeichnung;}
        std::string Bezeichnung() const {return bezeichnung;}
};
  
class cH_Zahlungsart : public Handle<const Zahlungsart>
{
     typedef CacheStatic<Zahlungsart::ID,cH_Zahlungsart> cache_t;
     static cache_t cache;
     cH_Zahlungsart(const Zahlungsart *s) : Handle<const Zahlungsart>(s) {};
     friend class std::map<Zahlungsart::ID,cH_Zahlungsart>;
  public:
//     cH_Zahlungsart(){} //{*this=new Zahlungsart();}
     cH_Zahlungsart(){*this=new Zahlungsart();}
     cH_Zahlungsart(Zahlungsart::ID _id);

//     bool operator==(const cH_Zahlungsart &b) const {return (*this)->Id()==b->Id();}
};
  
  
#endif

