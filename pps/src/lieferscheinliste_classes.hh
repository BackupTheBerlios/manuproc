

#ifndef LLISTE_CLASSES
#define LLISTE_CLASSES

#include <Misc/EntryValueBase.h>

#include <Misc/Datum.h>
#include <Lieferschein/LieferscheinList.h>
#include <Lieferschein/LieferscheinEntry.h>
#include <Lieferschein/Lieferschein.h>
#include <Lieferschein/Rechnung.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Artikel/Einheiten.h>
#include <typeinfo>
#include <Misc/Ausgabe_neu.h>
#include <Artikel/ArtikelMengeSumme.h>
#include "datum_kumul.h"
#include <SimpleTree.hh>

class Data_LListe : public RowDataBase
{
private:

    cH_Lieferschein liefer;
    LieferscheinEntry entry;
    Rechnung rechnung;
    ArtikelMenge menge;
    KumVal option_timecumulate;

 public:



   Data_LListe(const cH_Lieferschein& l,const LieferscheinEntry& e,
         const Rechnung& r, KumVal kv=KUM_DATUM)
      :  liefer(l),entry(e),rechnung(r),
         menge(ArtikelBase(entry.ArtikelID()),entry.Stueck(),entry.Menge()),
	 option_timecumulate(kv) {}

   typedef enum {KUNDE,AUFTRAG,ARTIKEL,BREITE,FARBE,AUFMACHUNG,LIEFERNR,
         LIEFERDATUM,RECHNUNG,RECHNUNGSDATUM,ARTIKELTYP,SUM_MENGE,SUM_AMENGE} Spalten;

   virtual cH_EntryValue Value(guint seqnr,gpointer gp) const;
   ArtikelMenge const& getArtikelMenge() const { return menge; }
   void setTimeCumulate(KumVal opt) { option_timecumulate=opt;}
   
   Rechnung const& getRechnung() const { return rechnung; }
   LieferscheinEntry const& getLEntry() const { return entry; }
   cH_Lieferschein const& getLieferschein() const { return liefer; }
};


class Data_ListeNode : public TreeRow
{
   ArtikelMengeSumme sum;
 public:
   virtual void cumulate(const cH_RowDataBase &rd)
     { 
       const ArtikelMenge &am=dynamic_cast<const Data_LListe &>(*rd).getArtikelMenge();
       sum.cumulate(am);
     }      
   virtual void deduct(const cH_RowDataBase &rd)
     { 
       const ArtikelMenge &am=dynamic_cast<const Data_LListe &>(*rd).getArtikelMenge();
       sum.deduct(am);
     }      
   cH_EntryValue Value(guint index,gpointer gp) const
    {
      switch(index)
        {
         case Data_LListe::SUM_MENGE :
              return cH_EntryValueIntString(sum.Menge());
         case Data_LListe::SUM_AMENGE :
              return cH_EntryValueIntString(sum.abgeleiteteMenge());
        }
      return cH_EntryValue();
    }
 Data_ListeNode(const Handle<const TreeRow> &suminit)
   { if (suminit) sum=suminit.cast_dynamic<const Data_ListeNode>()->sum;
   }

  static Handle<TreeRow> create(const Handle<const TreeRow> &suminit)
  {  return new Data_ListeNode(suminit);
  }
};

class cH_Data_LListe : public Handle<const Data_LListe>
{
protected:
 cH_Data_LListe() {}
public:   
 cH_Data_LListe(Data_LListe *r) : Handle<const Data_LListe>(r) {}
};

#endif

