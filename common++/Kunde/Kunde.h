// $Id: Kunde.h,v 1.67 2004/10/11 16:44:10 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef KUNDE_KUNDE_HH
#define KUNDE_KUNDE_HH
#include <string>
#include <ExtBezSchema/ExtBezSchemaHandle.h>
#include <Misc/fixedpoint.h>
#include <Misc/Datum.h>
#include <Misc/itos.h>
#include <Misc/Waehrung.h>
#include <Kunde/PreisListe.h>
#include <list>
#include <Kunde/TelefonArt.h>
#include <Kunde/LandesListe.h>
#include <Kunde/Anrede.h>
#include <BaseObjects/ManuProcHandle.h>
#include <DynamicEnums/DefaultValues.h>
#include <utility> // for pair
#include <Kunde/Kundengruppe.h>
#include <Kunde/Lieferart.h>

class cH_Kunde;
class H_Kunde;
class cH_Telefon;
#include <Kunde/Zahlungsart.h>

class Kunde : public ManuProcHandle<>
{

public:

 struct st_adresse
  {std::string firma;
   std::string name2;
   cH_Anrede branr;
   std::string sortname;
   std::string postanwvor;
   std::string strasse;
   std::string postanwnach;
   std::string hsnr;
   std::string plz;
   std::string postfach;
   std::string postfachplz;
   std::string ort;
   cH_LandesListe land;
   std::string unsere_kundennr;
  };
 typedef struct st_adresse Adresse;  
 
 struct st_bankverb
   {int bankindex;
    std::string bank;
    unsigned int blz;
    unsigned long long int konto;
    int lieferantenkonto;
    int gegenkonto;
    cH_Zahlungsart zahlungsart;
    cH_Lieferart lieferart;
    const std::string getBankverb() const { return  bank+": BLZ "+itos(blz)+", Kto.Nr. "+ulltos(konto);}
    st_bankverb():bankindex(0),blz(0),konto(0),
    lieferantenkonto(0),gegenkonto(0),zahlungsart(Zahlungsart::none_id),
    lieferart(Lieferart::none_id)
    {}
   };
 typedef struct st_bankverb Bankverbindung;

// struct st_verkaeufer {int verknr; std::string name; std::string kurz;
//        st_verkaeufer() : verknr(none_id),name(""),kurz("") {}
//        st_verkaeufer(int v,std::string n, std::string k)
//           : verknr(v),name(n),kurz(k) {} };
// typedef struct st_verkaeufer Verkaeufer;

 mutable Kunde::ID verkaeufer;   
 
 struct st_kddata
   {int flaeche;
    int mitarbeiter; 
    fixedpoint<2> planumsatz;
    fixedpoint<2> umsatz; 
    fixedpoint<2> kundenumsatz; 
    int kalkulation;
    fixedpoint<2> rabatt;
    bool zeilenrabatt:1;

    std::string verein; 
    std::string notiz; 
    ManuProC::Datum stand;
    ManuProC::Datum gebdatum;
    
    int anzahl_ausdruck_firmenpapier;
    int anzahl_ausdruck_weissespapier;
    st_kddata()
    	:flaeche(0),mitarbeiter(0),planumsatz(0),umsatz(0),kundenumsatz(0),
    	 kalkulation(0),rabatt(0),zeilenrabatt(false),
    	 anzahl_ausdruck_firmenpapier(1),anzahl_ausdruck_weissespapier(1) {}
   };
 typedef struct st_kddata Kundendaten;


private:
	ID KundenGruppennr;
	ExtBezSchema::ID schema;
	std::string IDnr;

  mutable std::vector<Kundengruppe::ID> gruppen;
	
	ID rngan;
	ID lfran;
        bool rng_an_postfach:1;
        bool ab_an_rngadresse:1;        
//        bool lieferadresse:1;
//        bool rechnungsadresse:1;
//        bool auftragsadresse:1;
        bool entsorg:1;
        mutable bool isaktiv:1;
        mutable bool preisautomatik:1;
        bool showean:1;
        bool mwst:1;
        
	Adresse adresse;
	Bankverbindung bankverb;
        Kundendaten kundendaten;
        
//   mutable Verkaeufer verkaeufer;
   ID betreuer;
	
	friend class Handle<const Kunde>;
	friend class Handle<Kunde>;
	static const ID _wir=ManuProC::DefaultValues::EigeneKundenId;
	cP_Waehrung waehrung;
   // ...

    mutable std::list<std::pair<int,PreisListe::ID> >preislisten; 
    mutable bool prlist_valid:1;

   bool lieferung_frei_haus;
public:

	static const ID default_id=ManuProC::DefaultValues::DefaultKundenId;
	static const ID eigene_id=ManuProC::DefaultValues::EigeneKundenId;
	Kunde(ID nr=default_id) throw(SQLerror);
#ifdef MANUPROC_DYNAMICENUMS_CREATED	
	static const cH_Kunde newKunde(const Kunde::ID kid, const std::string &firma,
			const std::string &name2,
			Kundengruppe::ID kgid=KundengruppeID::Kunden) throw(SQLerror);
#endif			
        const std::string LaTeX_von() const;
        const std::string LaTeX_von_gross(const ID kid,
        			const std::string width="7cm") const;
        const std::string LaTeX_an(bool liefer,TelArt telart=TEL_NONE,
        			const std::string width="8cm",
        			const std::string telwidth="5cm") const;
        
        const bool get_lieferung_frei_haus() const {return lieferung_frei_haus;}
        const std::string getBank() const { return bankverb.getBankverb(); }
        const unsigned long long int getKtnr() const { return bankverb.konto; }
        const unsigned int getblz() const { return bankverb.blz; }
        const std::string getbank() const { return bankverb.bank; }
        const int getindex() const { return bankverb.bankindex; }
        const int getLieferantenkonto() const {return bankverb.lieferantenkonto;}
        const int getGegenkonto() const {return bankverb.gegenkonto;}
        const cH_Zahlungsart zahlungsart() const { return bankverb.zahlungsart; }

	const cH_Anrede Anrede() const { return adresse.branr; }
	const std::string name2() const { return adresse.name2; }
        const std::string sortname() const { return adresse.sortname; }
        const std::string firma() const { return adresse.firma; }
        const std::string postanwvor() const { return adresse.postanwvor; }
        const std::string strasse() const { return adresse.strasse; }
        const std::string hausnr() const { return adresse.hsnr; }
        const std::string postanwnach() const { return adresse.postanwnach; }
        const std::string plz() const { return adresse.plz; }
        const std::string ort() const { return adresse.ort; }
        const std::string postfach() const { return adresse.postfach; }
        const std::string postfachplz() const { return adresse.postfachplz; }
        void set_idnr(const std::string& s);
        const cH_LandesListe land() const { return adresse.land; }
        const std::string UnsereKundenNr() const {return adresse.unsere_kundennr;}

        const long int flaeche() const { return kundendaten.flaeche; }
        const long int mitarbeiter() const { return kundendaten.mitarbeiter; }
        const fixedpoint<2> kundenumsatz() const { return kundendaten.kundenumsatz; }
        const fixedpoint<2> planumsatz() const { return kundendaten.planumsatz; }
        const fixedpoint<2> umsatz() const { return kundendaten.umsatz; }
        const fixedpoint<2> rabatt() const { return kundendaten.rabatt; }
        const bool zeilenrabatt() const { return kundendaten.zeilenrabatt; }
        const int anzahl_ausdruck_firmenpapier() const {return kundendaten.anzahl_ausdruck_firmenpapier;}
        const int anzahl_ausdruck_weissespapier() const {return kundendaten.anzahl_ausdruck_weissespapier;}
        const PreisListe::ID preisliste() const;
        // ja das & ist von mir CP
        const std::list<std::pair<int,PreisListe::ID> > &Preislisten() const;
        void push_backPreisListe(const PreisListe::ID p) throw(SQLerror);
        void deletePreisListe(const PreisListe::ID p) throw(SQLerror);
        const std::string verein() const { return kundendaten.verein; }
        const std::string notiz() const { return kundendaten.notiz; }
        const ManuProC::Datum getGebDatum() const { return kundendaten.gebdatum; }
        const std::string stand() const { return kundendaten.stand.c_str(); }

        const std::string getName() const {  return adresse.firma; }
        const std::string getName2() const {  return adresse.name2; }        
        const std::string getFullName() const;
        const std::string getSortName() const {  return adresse.sortname; }
        ID Id() const {  return entityid; }
//        ID GruppenId() const {  return KundenGruppennr; }
        const std::string idnr() const { return IDnr; } 
private:        
        bool idnr_valid() const;
public:
        ID getNummer() const {  return entityid; }
        cH_ExtBezSchema getSchema(class ArtikelTyp t) const
        	{  return cH_ExtBezSchema(schema,t); }
        ExtBezSchema::ID getSchemaId() const {  return schema; }
//        bool isLieferadresse() const;
//        bool isRechnungsadresse() const;
//        bool isAuftragsadresse() const;
        bool isInGrp(const Kundengruppe::ID gid) const;
        void putInGrp(const Kundengruppe::ID gid); 
        void pullFromGrp(const Kundengruppe::ID gid);
        ID Rngan() const { return rngan; }
        ID Lfran() const { return lfran; }        
        bool entsorgung() const { return entsorg; }
        bool Rng_an_postfach() const {return rng_an_postfach;}
        bool AB_an_rngadresse() const {return ab_an_rngadresse;}        
        bool Auslaender() const { return adresse.land->Auslaender(); }
        bool MwSt() const;
        bool EU() const { return adresse.land->EU(); }
	bool Preisautomatik() const { return preisautomatik;}
	void Preisautomatik(bool pa) throw(SQLerror);
	bool showEAN() const { return showean;}
	void showEAN(bool pa) throw(SQLerror);	
	

        // Personen
        struct st_ansprech{ID Person;
			   std::string position;
			   std::string notiz;
               st_ansprech():Person(Kunde::none_id) {}
               st_ansprech(ID P,std::string p,std::string n)             
                     :Person(P),position(p),notiz(n) {}
                          };
        std::vector<st_ansprech> getPersonen() const;
        void newKontaktperson(const cH_Kunde Person) const;
        void updateKontaktperson(const st_ansprech &A) const;
        static void deleteKontaktperson(const H_Kunde K,const cH_Kunde P);
        // Telefon
        std::list<cH_Telefon> getTelefon(Kunde::ID pid=Kunde::none_id) const;
        std::string get_first_telefon(const TelArt& art) const;
        ID getBetreuer() const { return betreuer;}
        void setBetreuer(const ID) throw(SQLerror);
  
  // gets the provsatz for article artid      
  fixedpoint<2> getProvSatz_Artikel(const ArtikelBase artid) const throw(SQLerror);

  private:
  	void load_Gruppen() const throw(SQLerror);
  
        enum B_UPDATE_BITS_ADRESSE{B_Name2,B_Sortname,B_Idnr,B_Firma,
           B_Postanwvor,B_Strasse,B_Postanwnach,B_Hsnr,B_Plz,B_Postfach,
           B_Postfachplz,B_Ort,B_Lkz,B_Lieferadresse,B_Rechnungadresse,
           B_Auftragadresse,
           B_Rng_an_postfach,B_BrAnrede,B_MaxAnzA};
        enum B_UPDATE_BITS_FIRMA{B_Planumsatz,B_Umsatz,B_Mitarbeiter,
           B_Kundenumsatz,B_Flaeche,B_UnsereKundenNr,B_Verein,B_MaxAnzF};
        enum B_UPDATE_BITS_BANK{B_Ktonr,B_Blz,B_Bankindex,
           B_Rabatt,B_Zeilenrabatt,B_Waehrungid,
           B_Zahlungsart,B_Lieferantenkonto,
           B_Gegenkonto,B_MaxAnzB};
        enum B_UPDATE_BITS_SONST{B_Rechnungan,B_Lieferscheinan,B_Extartbezid,
           B_Preisliste,B_Notiz,B_Entsorgung,B_Verknr,B_Kalkulation,
           B_Stand,B_KP_Position,B_KP_Notiz,
           B_AnzAusFirmenPapier,B_AnzAusWeissesPapier,
           B_lieferung_frei_haus,B_Betreuer,
           B_GebDatum,B_MaxAnzS};
 public: 
         enum UpdateBitsAdresse {FName2=1<<B_Name2,
           FSortname=1<<B_Sortname,FIdnr=1<<B_Idnr,FFirma=1<<B_Firma,
           FPostanwvor=1<<B_Postanwvor,FStrasse=1<<B_Strasse,
           FPostanwnach=1<<B_Postanwnach,FHsnr=1<<B_Hsnr,FPlz=1<<B_Plz,
           FPostfach=1<<B_Postfach,FPostfachplz=1<<B_Postfachplz,
           FOrt=1<<B_Ort,FLkz=1<<B_Lkz,FLieferadresse=1<<B_Lieferadresse,
           FRechnungadresse=1<<B_Rechnungadresse,
           FAuftragadresse=1<<B_Auftragadresse,
           FRng_an_postfach=1<<B_Rng_an_postfach,
           FBranr=1<<B_BrAnrede};
         enum UpdateBitsFirma{FPlanumsatz=1<<B_Planumsatz,FUmsatz=1<<B_Umsatz,
           FMitarbeiter=1<<B_Mitarbeiter,FKundenumsatz=1<<B_Kundenumsatz,
           FFlaeche=1<<B_Flaeche,FUnsereKundenNr=1<<B_UnsereKundenNr,
           FVerein=1<<B_Verein};
         enum UpdateBitsBank{FKtonr=1<<B_Ktonr,FBlz=1<<B_Blz,
           FBankindex=1<<B_Bankindex,
           FRabatt=1<<B_Rabatt,FZeilenrabatt=1<<B_Zeilenrabatt,
           FWaehrungid=1<<B_Waehrungid,
           FZahlungsart=1<<B_Zahlungsart,FLieferantenkonto=1<<B_Lieferantenkonto,
           FGegenkonto=1<<B_Gegenkonto};
         enum UpdateBitsSonst{FRechnungan=1<<B_Rechnungan,
         	FLieferscheinan=1<<B_Lieferscheinan,
           FExtartbezid=1<<B_Extartbezid,FPreisliste=1<<B_Preisliste,
           FNotiz=1<<B_Notiz,FEntsorgung=1<<B_Entsorgung,
            FVerknr=1<<B_Verknr,Kalkulation=1<<B_Kalkulation,
            FStand=1<<B_Stand,FKP_Position=1<<B_KP_Position,
            FKP_Notiz=1<<B_KP_Notiz,
            FAnzAusFirmenPapier=1<<B_AnzAusFirmenPapier,
            FAnzAusWeissesPapier=1<<B_AnzAusWeissesPapier,
            Flieferung_frei_haus=1<<B_lieferung_frei_haus,
            FBetreuer=1<<B_Betreuer,
            FGebDatum=1<<B_GebDatum};
            

        void update_e(UpdateBitsAdresse e);
        void update_e(UpdateBitsFirma e);
        void update_e(UpdateBitsBank e);
        void update_e(UpdateBitsSonst e);

//	void delete_Kunde(Kunde::ID kid) throw(SQLerror);
        static const Kunde::ID nextval() throw(SQLerror);
        
	void reread_Stand() throw(SQLerror);                
	
	void isLieferadresse(bool is);
	void isRechnungsadresse(bool is);
	void isAuftragsadresse(bool is);
	void RngAn(const Kunde::ID kid) { rngan=kid; }
	void LfrAn(const Kunde::ID kid) { lfran=kid; }	
        void Rng_an_postfach(bool b) {rng_an_postfach=b;}
        void AB_an_rngadresse(bool b) throw(SQLerror);        
        void entsorgung(bool b) { entsorg=b; }

	ID Schema() const { return schema; }

//        const Verkaeufer &getVerkaeufer() const throw(SQLerror);
        void setVerkNr(Kunde::ID v) throw(SQLerror);
        ID VerkNr() const { return verkaeufer; }
        const std::string VerkName() const throw(SQLerror);
        

        // update_Bankeinzug machte Datenbnakzugriff;
//        void update_Bank_einzug(bool b) {kundendaten.bankeinzug=b;}
        void set_lieferung_frei_haus(bool b) {lieferung_frei_haus=b;}
//        void set_gruppen_nr(const ID i) {KundenGruppennr=i;}
        void set_sortname(const std::string& s) {adresse.sortname = s; } 
        void set_firma(const std::string& s){adresse.firma = s; } 
        void set_name2(const std::string& s){adresse.name2 = s; }
        void set_postanwvor(const std::string& s){adresse.postanwvor = s; } 
        void set_strasse(const std::string& s){adresse.strasse = s; } 
        void set_hausnr(const std::string& s){adresse.hsnr = s; }  
        void set_postanwnach(const std::string& s){adresse.postanwnach = s; } 
        void set_plz(const std::string& s){adresse.plz = s; } 
        void set_ort(const std::string& s){adresse.ort = s; }  
        void set_postfach(const std::string& s){adresse.postfach = s; } 
        void set_postfachplz(const std::string& s){adresse.postfachplz = s; } 
        void set_land(cH_LandesListe i){adresse.land = i; } 
        void set_UnsereKundenNr(const std::string& s){adresse.unsere_kundennr = s; } 
        void set_schema(ID s) throw(SQLerror);

        void set_planumsatz(const fixedpoint<2> s){kundendaten.planumsatz = s; }
        void set_rabatt(const fixedpoint<2> s){kundendaten.rabatt = s; }
        void set_zeilenrabatt(const bool s){kundendaten.zeilenrabatt = s; }
        void set_flaeche(const int s) {kundendaten.flaeche = s; }
        void set_mitarbeiter(const int s){kundendaten.mitarbeiter = s; }
        void set_kundenumsatz(const fixedpoint<2> s){kundendaten.kundenumsatz = s; }
        void set_umsatz(const fixedpoint<2> s){kundendaten.umsatz = s; }
        void set_verein(const std::string& s){kundendaten.verein = s; }
        void set_anzahl_ausdruck_firmenpapier(const int i){kundendaten.anzahl_ausdruck_firmenpapier=i; }        
        void set_anzahl_ausdruck_weissespapier(const int i){kundendaten.anzahl_ausdruck_weissespapier=i; }        
        void set_notiz(const std::string& s){kundendaten.notiz = s; } 
        void set_bankindex(const int s){bankverb.bankindex = s; } 
        void set_bank_konto(const unsigned long long int s) {bankverb.konto = s;} 
        void set_Lieferantenkonto(const int i) {bankverb.lieferantenkonto=i;}
        void set_Gegenkonto(const int i) {bankverb.gegenkonto=i;}
        void set_Zahlungsart(cH_Zahlungsart i) {bankverb.zahlungsart=i; }
        void set_Anrede(const cH_Anrede &s) { adresse.branr=s; }
        void setGebDatum(const ManuProC::Datum &d) { kundendaten.gebdatum=d; }        

        // Notfall API sollte ersetzt werden MAT
        fixedpoint<2> skontosatz() const {return zahlungsart()->getSkonto(1).skontosatz;} 
        int skontofrist() const {return zahlungsart()->getSkonto(1).skontofrist;} 
        fixedpoint<2> einzugrabatt() const {return zahlungsart()->getEinzugrabatt();} 
        bool bankeinzug() const {return zahlungsart()->getBankeinzug();} 

        // set_bankkonto machte einen Datenbankzugriff

        static unsigned long int neue_bank_anlegen(const std::string& name, unsigned long int blz);
        static unsigned long int bankname_aendern(unsigned long int blz, const std::string& name);
        
        void get_blz_from_bankindex(unsigned int bankindex);
        cP_Waehrung getWaehrung() const { return waehrung; }
        void setWaehrung(cP_Waehrung w) {waehrung=w; }
        
        bool Aktiv() const { return isaktiv; }
        void setAktiv(bool a) throw(SQLerror);

        bool operator==(const Kunde& b) const
                {return Id()==b.Id();} 
        bool operator<(const Kunde& b) const
                {return Id()<b.Id();} 

friend enum Kunde::B_UPDATE_BITS_ADRESSE &operator++(enum Kunde::B_UPDATE_BITS_ADRESSE &s);
friend enum Kunde::B_UPDATE_BITS_FIRMA &operator++(enum Kunde::B_UPDATE_BITS_FIRMA &s);
friend enum Kunde::B_UPDATE_BITS_BANK &operator++(enum Kunde::B_UPDATE_BITS_BANK &s);
friend enum Kunde::B_UPDATE_BITS_SONST &operator++(enum Kunde::B_UPDATE_BITS_SONST &s);


#ifdef MABELLA_EXTENSIONS
private:
	mutable std::string cab, abi, acc; // RiBa Kontobeschreibung; Bankeinzug in Italy
	mutable std::string iban;  // nr für LCR Verfahren (z.B. Frankreich)
	mutable std::string bankname; // Name der Bank in Italien 
	void init_AuslandZahlung() const throw(SQLerror);	
public:
	std::string getABI_Code(bool force=false) const;
	std::string getCAB_Code(bool force=false) const;	
	std::string getRiBaACC(bool force=false) const;
	std::string getIBAN_Code(bool force=false) const;
	std::string getABIBankName(bool force=false) const;
	void setABI_CAB(const std::string abi, const std::string cab,
			const std::string acc) const throw(SQLerror);
	void setIBAN(const std::string iban) const throw(SQLerror);	
	int getAbrZyklus() const throw(SQLerror);
	void setAbrZyklus(int mon) const throw(SQLerror);
	fixedpoint<2> getProvAbschlag() const throw(SQLerror);
#endif

 int Sprache() const throw(SQLerror);
 typedef enum {RNG_NOTIZ=1,LIEF_NOTIZ=2,AUF_NOTIZ=3} NotizZiel;
 std::string fixeNotiz(const NotizZiel nz) throw(SQLerror);
 void setFixeNotiz(const NotizZiel nz, const std::string n) throw(SQLerror); 

};

class cH_Kunde : public Handle<const Kunde>
{	// cache
        typedef CacheStatic<Kunde::ID,cH_Kunde> cache_t;
        static cache_t cache;
        cH_Kunde(const Kunde *p) : Handle<const Kunde>(p) {}	
#if __GNUC__ > 2
        friend class cache_t::stl_type;
#else
        friend cache_t::stl_type;
#endif        
//	friend class std::map<int, cH_Kunde>;
        cH_Kunde() {}
public:
	typedef Kunde::ID ID;
	static const ID default_id=Kunde::default_id;
	cH_Kunde(ID nr);
        bool operator==(const cH_Kunde& b) const
                {return (*this)->Id()==b->Id();} 
        bool operator<(const cH_Kunde& b) const
                {return (*this)->Id()<b->Id();} 
};


class H_Kunde : public Handle<Kunde>
{
        typedef CacheStatic<Kunde::ID,H_Kunde> cache_t;
        static cache_t cache;
//	friend class CacheStatic<Kunde::ID, H_Kunde>;
#if __GNUC__ > 2
        friend class cache_t::stl_type;
#else
        friend cache_t::stl_type;
#endif
	H_Kunde(Kunde *p) : Handle<Kunde>(p) {}	
	H_Kunde() {}
public:
	typedef Kunde::ID ID;
	static const ID default_id=Kunde::default_id;
	H_Kunde(ID nr);
};

std::ostream &operator<<(std::ostream &o,const cH_Kunde &k);

#endif
