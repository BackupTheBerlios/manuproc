#ifndef _CLASS_INSTANZZ_HH
#  define _CLASS_INSTANZZ_HH

#include <Aux/EntryValueEmptyInt.h>
#include <Aux/EntryValueIntString.h>
#include <Aux/itos.h>
#include <Aux/Datum.h>

class Data_instanz_herkunft : public RowDataBase
{
   cH_ppsInstanz instanz;
   ArtikelBase AB;
   guint menge;
   Petig::Datum datum;
   std::string einheit;

public:
   Data_instanz_herkunft(cH_ppsInstanz _instanz,ArtikelBase _AB,
         guint _menge,Petig::Datum _datum,const std::string& _einheit="") 
     : instanz(_instanz),AB(_AB),menge(_menge),datum(_datum),
         einheit(_einheit) {}

   enum SPALTEN{ARTIKEL,MENGE,INSTANZ};

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case INSTANZ : return cH_EntryValueIntString(instanz->get_Name());
      case ARTIKEL : {
         ArtikelBezeichnung AZ(AB);
         return cH_EntryValueIntString(AZ.Bezeichnung());
        }
      case MENGE   : return cH_EntryValueIntString(itos(menge)+einheit);
     }
   return cH_EntryValueIntString("?");
 }
 ArtikelBase ArtBase() const {return AB;}
 guint Menge() const {return menge;}
 Petig::Datum get_Datum() const {return datum;}
};

class cH_Data_instanz_herkunft : public Handle<const Data_instanz_herkunft>
{
protected:
 cH_Data_instanz_herkunft() {}
public:
 cH_Data_instanz_herkunft(Data_instanz_herkunft *r) : Handle<const Data_instanz_herkunft>(r){}
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class Data_neuer_auftrag : public RowDataBase
{
   const auftrag_main *AM;
   ArtikelBase AB;
   guint menge;
   Petig::Datum datum;
   AufStatVal status;
   int znr;
   std::string einheit;

public:
   Data_neuer_auftrag(auftrag_main* _AM, ArtikelBase _AB,guint _menge,
         Petig::Datum _datum, AufStatVal _status,int _znr,const std::string& _einheit="")
      :  AM(_AM),AB(_AB),menge(_menge),datum(_datum),
            status(_status),znr(_znr),einheit(_einheit) {} 


   enum SPALTEN{ARTIKEL,MENGE,DATUM,STATUS};

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case ARTIKEL : {
         ArtikelBezeichnung AZ(AB);
//         return cH_EntryValueIntString(itos(znr)+AZ.Bezeichnung());
         return cH_EntryValueIntString(AZ.Bezeichnung());
        }
      case MENGE   : return cH_EntryValueIntString(itos(menge)+einheit);
      case DATUM   : {
         std::string lw;
         if (AM->Zeit_kw_bool())
           { int lieferwoche = datum.KW().Woche();
             int lieferjahr =  datum.KW().Jahr();
             string lj=itos (lieferjahr).substr(2,2);
             lw = itos(lieferwoche)+"/"+lj;
           }
         else lw =  datum.c_str();
         return cH_EntryValueIntString(lw); 
        }
      case STATUS : {
         std::string stat;
         if (status==OPEN) stat="offen";
         else if (status==CLOSED) stat="fertig";
         else if (status==STORNO) stat="storniert";
         else if (status==UNCOMMITED) stat="unbestätigt";
         else stat="undefiniert";
         return cH_EntryValueIntString(stat);
        }
     }
   return cH_EntryValueIntString("?");
 }
 ArtikelBase ArtBase() const {return AB;}
 guint Menge() const {return menge;}
 Petig::Datum Datum() const {return datum;}
 AufStatVal Status() const {return status;}
 int getZnr() const {return znr;}
};

class cH_Data_neuer_auftrag : public Handle<const Data_neuer_auftrag>
{
protected:
 cH_Data_neuer_auftrag() {}
public:
 cH_Data_neuer_auftrag(Data_neuer_auftrag *r) : Handle<const Data_neuer_auftrag>(r){}
};

#endif