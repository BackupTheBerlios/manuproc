#ifndef _CLASS_PREISL_HH
#  define _CLASS_PREISL_HH

#include <rowdata.h>
#include <Aux/EntryValueIntString.h>
#include <Kunde/PreisListe.h>

class Data_SPreis : public RowDataBase
{
   cH_PreisListe PL;
 public:
   Data_SPreis(cH_PreisListe pl) : PL(pl) {}

   enum Spalten  {ID,NOTIZ};
   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
     {
       switch((Spalten)seqnr) 
        {
          case ID    : return cH_EntryValueIntString(PL->Id());
          case NOTIZ : return cH_EntryValueIntString(PL->Name());
        }
      return cH_EntryValueIntString();
     }
   cH_PreisListe getPreisListe() const {return PL;}
};


class cH_Data_SPreis : public Handle<Data_SPreis>
{
public:
 cH_Data_SPreis(Data_SPreis *r) : Handle<Data_SPreis>(r) {}
};



#endif
