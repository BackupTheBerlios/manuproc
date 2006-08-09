// $Id: bestell_plan.hh,v 1.9 2006/08/09 15:36:51 christof Exp $

#ifndef _BESTELL_PLAN_HH
#  include "bestell_plan_glade.hh"
#  define _BESTELL_PLAN_HH

#include <vector.h>
#include <string.h>
#include <Artikel/ArtikelBase.h>
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



 virtual cH_EntryValue Value(guint seqnr,gpointer gp) const
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

class Data_AbverkaufNode : public TreeRow
{
 int menge_sum;
public:
 virtual void cumulate(const cH_RowDataBase &rd)
  {
   menge_sum+=dynamic_cast<const Data_Abverkauf &>(*rd).getMenge();
  }
 virtual void deduct(const cH_RowDataBase &rd)
  {
   menge_sum-=dynamic_cast<const Data_Abverkauf &>(*rd).getMenge();
  }
 
 virtual cH_EntryValue Value(guint index, gpointer gp) const
  {
   switch((Spalten)index)
     {
      case SP_MENGE :
        return cH_EntryValueIntString(menge_sum);
     }
   return cH_EntryValue();
  }
   
 Data_AbverkaufNode(const Handle<const TreeRow> &suminit)  
        : menge_sum()
        {
           if (suminit) menge_sum=suminit.cast_dynamic<const Data_AbverkaufNode>()->menge_sum;
        }

 static Handle<TreeRow> create(const Handle<const TreeRow> &suminit)
  {  return new Data_AbverkaufNode(suminit);
  }
};


class cH_Data_Abverkauf : public Handle<const Data_Abverkauf>
{
 cH_Data_Abverkauf(const Data_Abverkauf *d) : Handle<const Data_Abverkauf>(d) {}
};





#endif

