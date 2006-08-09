
#include "lieferscheinliste_classes.hh"
#include <Misc/EntryValueIntString.h>
#include <Misc/EntryValueDatum.h>


cH_EntryValue Data_LListe::Value(guint seqnr,gpointer gp) const
{

 switch(seqnr) {
   case ARTIKEL ... AUFMACHUNG: {
      cH_ArtikelBezeichnung AB(ArtikelBase(entry.ArtikelID()));
      return AB->Komponente_als_EntryValue(seqnr-ARTIKEL);
    }
   case ARTIKELTYP: { ArtikelTyp atyp(make_value(ArtikelBase(entry.ArtikelID())));
      return cH_EntryValueIntString(atyp.Id(),ArtikelTyp::get_string(atyp.Id()));
    }
   case KUNDE : return cH_EntryValueIntString(cH_Kunde(liefer->KdNr())->firma());
   case AUFTRAG : 
     {  std::string res;
        std::vector<LieferscheinEntry::st_AuftragMenge> V=entry.getAuftragsMenge();
        
        for (std::vector<LieferscheinEntry::st_AuftragMenge>::const_iterator i=V.begin();i!=V.end();++i)
        {  if (!i->ab) res+="- ";
           else res+=itos(i->ab.Id())+" ";
        }
        return cH_EntryValueIntString(res);
     }
   case LIEFERNR :return cH_EntryValueIntString( entry.Id());
   case LIEFERDATUM : 
   	{switch(option_timecumulate) {
   	 case KUM_DATUM :
	   return cH_EntryValueDatum( liefer->getDatum());
   	 case KUM_WOCHE :
	   return cH_EntryValueKalenderwoche(
	   	liefer->getDatum().valid() ? liefer->getDatum().KW() : Kalenderwoche());
   	 case KUM_MONAT :
	   return cH_EntryValueMonat(liefer->getDatum());
   	 case KUM_QUARTAL :
	   return cH_EntryValueQuartal( liefer->getDatum());	   	   
   	 case KUM_JAHR :
	   return cH_EntryValueIntString( 
	   	liefer->getDatum().valid() ? liefer->getDatum().Jahr() : 0);	   
	 default : return cH_EntryValueDatum( liefer->getDatum());
   	 }
   	}
   case RECHNUNG : return cH_EntryValueIntString( liefer->RngNr());
   case RECHNUNGSDATUM : return cH_EntryValueDatum(rechnung.getDatum());
   case SUM_MENGE: 
        return cH_EntryValueIntString(menge.Menge());
   case SUM_AMENGE: 
        return cH_EntryValueIntString(menge.abgeleiteteMenge());
  }
 return cH_EntryValue();
}
