// generated 2005/1/27 10:54:39 CET by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// newer (non customized) versions of this file go to bestell_plan.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _BESTELL_PLAN_HH
#  include "bestell_plan_glade.hh"
#  define _BESTELL_PLAN_HH

#include <vector.h>
#include <string.h>
#include <Artikel/ArtikelBase.h>
#include <tclistnode.h>
#include <Misc/EntryValueIntString.h>
#include <Misc/EntryValueDatum.h>
#include <Misc/Datum.h>
#include "datum_kumul.h"

typedef enum {SP_COMP_ZEIT=0, SP_COMP_ZEIT2,SP_LIEF_ZEIT,SP_MENGE,SP_DUMMY} Spalten;

class bestell_plan : public bestell_plan_glade
{  
 std::vector<ArtikelBase> artikel;
 ArtikelBase akt_artikel;
 int akt_index;
 
        void on_bp_quit_clicked();
        void on_prev_artikel_clicked();
        void on_next_artikel_clicked();
        void on_plan_artikel_activate();
        void on_abverkauf_reload_clicked();        
        
 void clear_all();
 void load_artikel_list();
 void load_data(const ArtikelBase a) throw(SQLerror);
 void set_akt_index();
 void load_abverkauf(const ArtikelBase a, KumVal kv);
 
public:
 bestell_plan(const ArtikelBase ab=ArtikelBase());
        
};


class Data_Abverkauf : public RowDataBase
{
 const ManuProC::Datum datum;
 mutable int menge;
 KumVal timecumulate;
 KumVal liefdate_cumulate;
 Kunde::ID kid; 

public:
 Data_Abverkauf(const ManuProC::Datum &d,int m,KumVal kv, KumVal ldk)
   : datum(d), menge(m), timecumulate(kv), 
     liefdate_cumulate(ldk) {}



 virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 {
 switch((Spalten)seqnr) 
   {
    case SP_COMP_ZEIT : 
        {switch(timecumulate) {
         case KUM_DATUM :
           return cH_EntryValueDatum( datum );
         case KUM_WOCHE :
           return cH_EntryValueKalenderwoche(
                datum.valid() ? datum.KW() : Kalenderwoche());
         case KUM_MONAT :
           return cH_EntryValueMonat(datum);
         case KUM_QUARTAL :
           return cH_EntryValueQuartal(datum);
         case KUM_JAHR :
           return cH_EntryValueIntString(
                datum.valid() ? datum.Jahr() : 0);
         default : return cH_EntryValueDatum(datum);
         }
        }
    case SP_COMP_ZEIT2 :
        {switch(liefdate_cumulate) {
         case KUM_DATUM :
           return cH_EntryValueDatum( datum );
         case KUM_WOCHE :
           return cH_EntryValueKalenderwoche(
                datum.valid() ? datum.KW() : Kalenderwoche());
         case KUM_MONAT :
           return cH_EntryValueMonat(datum);
         case KUM_QUARTAL :
           return cH_EntryValueQuartal(datum);
         case KUM_JAHR :
           return cH_EntryValueIntString(
                datum.valid() ? datum.Jahr() : 0);
         default : return cH_EntryValueDatum(datum);
         }
        }    
    case SP_LIEF_ZEIT : return cH_EntryValueDatum( datum ); 
    case SP_MENGE :  return cH_EntryValueIntString(menge);    
  }
 return cH_EntryValue();
 }

 int getMenge() const { return menge; }
};

class Data_AbverkaufNode : public TCListNode
{
 int menge_sum;
public:
 virtual void cumulate(const cH_RowDataBase &rd)
  {
   menge_sum+=dynamic_cast<const Data_Abverkauf &>(*rd).getMenge();
  }
 
 virtual const cH_EntryValue Value(guint index, gpointer gp) const
  {
   switch((Spalten)index)
     {
      case SP_MENGE :
        return cH_EntryValueIntString(menge_sum);
     }
   return cH_EntryValue();
  }
   
 Data_AbverkaufNode(guint col, const cH_EntryValue &v,
              guint child_s_deep, cH_RowDataBase child_s_data,bool expand,
              const TreeRow &suminit)  
        : TreeRow(col,v,child_s_deep,child_s_data,expand),menge_sum(0)
        {
         if (suminit.Leaf()) cumulate(child_s_data);
         else menge_sum=dynamic_cast<const Data_AbverkaufNode&>(suminit).menge_sum;
        }

 static TCListNode *create(guint col, const cH_EntryValue &v,guint child_s_deep,
         cH_RowDataBase child_s_data, bool expand, const TreeRow &suminit)
  {  return new Data_AbverkaufNode(col,v,child_s_deep,child_s_data,expand,suminit);
  }
};


class cH_Data_Abverkauf : public Handle<const Data_Abverkauf>
{
 cH_Data_Abverkauf(const Data_Abverkauf *d) : Handle<const Data_Abverkauf>(d) {}
};





#endif

