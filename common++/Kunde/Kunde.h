// $Id: Kunde.h,v 1.20 2002/05/09 12:46:00 christof Exp $
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
#include<Aux/fixedpoint.h>
#include<Aux/Datum.h>
#include<Aux/itos.h>
#include<Aux/Waehrung.h>
#include <Kunde/PreisListe.h>
#include <list>
#include <Kunde/TelefonArt.h>
#include <Kunde/LandesListe.h>
#include <Kunde/Person.h>
#include <BaseObjects/ManuProcEntity.h>

class cH_Kunde;
class H_Kunde;
class cH_Telefon;

class Kunde : public ManuProcEntity
{

public:
	
 struct st_adresse
  {std::string firma;
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
    const std::string getBankverb() const { return  bank+": BLZ "+itos(blz)+", Kto.Nr. "+ulltos(konto);}
    st_bankverb():bankindex(0),blz(0),konto(0),
    lieferantenkonto(0),gegenkonto(0) {}
   };
 typedef struct st_bankverb Bankverbindung;

 struct st_verkaeufer {int verknr; std::string name; std::string kurz;
        st_verkaeufer() : verknr(0),name(""),kurz("") {}
        st_verkaeufer(int v,std::string n, std::string k)
           : verknr(v),name(n),kurz(k) {} };
 typedef struct st_verkaeufer Verkaeufer;
    
 
 struct st_kddata
   {int flaeche;
    int mitarbeiter; 
    fixedpoint<2> planumsatz;
    fixedpoint<2> umsatz; 
    fixedpoint<2> kundenumsatz; 
    int kalkulation;
    fixedpoint<2> rabatt;
    bool zeilenrabatt:1;
    int skontofrist; 

    std::string verein; 
    bool bankeinzug:1;
    std::string notiz; 
    Petig::Datum stand;
    fixedpoint<2> einzugrabatt;
    fixedpoint<2> skontosatz;    
    st_kddata()
    	:flaeche(0),mitarbeiter(0),planumsatz(0),umsatz(0),kundenumsatz(0),
    	 kalkulation(0),rabatt(0),zeilenrabatt(false),skontofrist(0),
    	 bankeinzug(false),
    	 einzugrabatt(0),skontosatz(0) {}
   };
 typedef struct st_kddata Kundendaten;


private:
	ID KundenGruppennr;
	ExtBezSchema::ID schema;
	std::string IDnr;
	
	ID rngan;
        bool rng_an_postfach:1;
        bool lieferadresse:1;
        bool rechnungsadresse:1;
        bool entsorg:1;
        
	Adresse adresse;
	Bankverbindung bankverb;
        Kundendaten kundendaten;
        
   Verkaeufer verkaeufer;
   Person::ID betreuer;
	
	friend class Handle<const Kunde>;
	friend class Handle<Kunde>;
	static const ID _wir=1;
	cP_Waehrung waehrung;
   // ...

    mutable list<PreisListe::ID> preislisten; 
    mutable bool prlist_valid:1;

public:
	static const ID default_id=_wir;
	Kunde(ID nr=default_id) throw(SQLerror);
	static const cH_Kunde newKunde(const Kunde::ID kid, const std::string &firma) throw(SQLerror);
        const std::string LaTeX_von() const;
        const std::string LaTeX_von_gross(const ID kid,
        			const string width="7cm") const;
        const std::string LaTeX_an(bool liefer,TelArt telart=TEL_NONE,
        			const string width="8cm",
        			const string telwidth="5cm") const;
        
        const std::string getBank() const { return bankverb.getBankverb(); }
        const unsigned long long int getKtnr() const { return bankverb.konto; }
        const unsigned int getblz() const { return bankverb.blz; }
        const std::string getbank() const { return bankverb.bank; }
        const int getindex() const { return bankverb.bankindex; }
        const int getLieferantenkonto() const {return bankverb.lieferantenkonto;}
        const int getGegenkonto() const {return bankverb.gegenkonto;}

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
//        const std::string land() const { return adresse.land; }
//        const cH_LandesListe lkz() const { return adresse.lkz; }
        const cH_LandesListe land() const { return adresse.land; }
        const std::string UnsereKundenNr() const {return adresse.unsere_kundennr;}

        const long int flaeche() const { return kundendaten.flaeche; }
        const long int mitarbeiter() const { return kundendaten.mitarbeiter; }
        const fixedpoint<2> kundenumsatz() const { return kundendaten.kundenumsatz; }
        const fixedpoint<2> planumsatz() const { return kundendaten.planumsatz; }
        const fixedpoint<2> umsatz() const { return kundendaten.umsatz; }
        const fixedpoint<2> rabatt() const { return kundendaten.rabatt; }
        const bool zeilenrabatt() const { return kundendaten.zeilenrabatt; }
        const fixedpoint<2> einzugrabatt() const { return kundendaten.einzugrabatt; }
        const fixedpoint<2> skontosatz() const { return kundendaten.skontosatz; }
        const PreisListe::ID preisliste() const;
        // ja das & ist von mir CP
        const list<PreisListe::ID> &Preislisten() const;
        void push_backPreisListe(const PreisListe::ID p) throw(SQLerror);
        void deletePreisListe(const PreisListe::ID p) throw(SQLerror);
        const int skontofrist() const { return kundendaten.skontofrist; }
        const std::string verein() const { return kundendaten.verein; }
        const bool bankeinzug() const { return kundendaten.bankeinzug; }
        const std::string notiz() const { return kundendaten.notiz; }
        const std::string stand() const { return kundendaten.stand.c_str(); }

        const std::string getName() const {  return adresse.firma; }
        const std::string getSortName() const {  return adresse.sortname; }
        ID Id() const {  return entityid; }
        ID GruppenId() const {  return KundenGruppennr; }
        const std::string idnr() const { return IDnr; } 
        ID getNummer() const {  return entityid; }
        cH_ExtBezSchema getSchema(class ArtikelTyp t) const
        	{  return cH_ExtBezSchema(schema,t); }
        ExtBezSchema::ID getSchemaId() const {  return schema; }
        bool isLieferadresse() const { return lieferadresse; }
        bool isRechnungsadresse() const { return rechnungsadresse; }
        ID Rngan() const { return rngan; }
        bool entsorgung() const { return entsorg; }
        bool Rng_an_postfach() const {return rng_an_postfach;}


        // Personen
        struct st_ansprech{cH_Person Person;std::string position;std::string notiz;
               st_ansprech():Person(Person::none_id) {}
               st_ansprech(cH_Person P,std::string p,std::string n)             
                     :Person(P),position(p),notiz(n) {}};
        std::vector<st_ansprech> getPersonen() const;
        void newKontaktperson(const cH_Person &Person) const;
        void updateKontaktperson(const st_ansprech &A) const;
        static void deleteKontaktperson(const H_Kunde& K,const cH_Person &P);
        // Telefon
        std::list<cH_Telefon> getTelefon() const;
        std::string get_first_telefon(const TelArt& art) const;
        Person::ID getBetreuer() const { return betreuer;}

  private:
        enum B_UPDATE_BITS_ADRESSE{B_Gruppennr,B_Sortname,B_Idnr,B_Firma,
           B_Postanwvor,B_Strasse,B_Postanwnach,B_Hsnr,B_Plz,B_Postfach,
           B_Postfachplz,B_Ort,B_Lkz,B_Lieferadresse,B_Rechnungadresse,
           B_Rng_an_postfach,B_MaxAnzA};
        enum B_UPDATE_BITS_FIRMA{B_Planumsatz,B_Umsatz,B_Mitarbeiter,
           B_Kundenumsatz,B_Flaeche,B_UnsereKundenNr,B_Verein,B_MaxAnzF};
        enum B_UPDATE_BITS_BANK{B_Ktonr,B_Blz,B_Bankindex,B_Bankeinzug,
           B_Rabatt,B_Zeilenrabatt,B_Waehrungid,B_Einzugrabatt,
           B_Skontosatz,B_Skontofrist,B_Lieferantenkonto,
           B_Gegenkonto,B_MaxAnzB};
        enum B_UPDATE_BITS_SONST{B_Rechnungan,B_Extartbezid,
           B_Preisliste,B_Notiz,B_Entsorgung,B_Verknr,B_Kalkulation,
           B_Stand,B_KP_Position,B_KP_Notiz,B_MaxAnzS};
 public: 
         enum UpdateBitsAdresse {FGruppennr=1<<B_Gruppennr,
           FSortname=1<<B_Sortname,FIdnr=1<<B_Idnr,FFirma=1<<B_Firma,
           FPostanwvor=1<<B_Postanwvor,FStrasse=1<<B_Strasse,
           FPostanwnach=1<<B_Postanwnach,FHsnr=1<<B_Hsnr,FPlz=1<<B_Plz,
           FPostfach=1<<B_Postfach,FPostfachplz=1<<B_Postfachplz,
           FOrt=1<<B_Ort,FLkz=1<<B_Lkz,FLieferadresse=1<<B_Lieferadresse,
           FRechnungadresse=1<<B_Rechnungadresse,
           FRng_an_postfach=1<<B_Rng_an_postfach};
         enum UpdateBitsFirma{FPlanumsatz=1<<B_Planumsatz,FUmsatz=1<<B_Umsatz,
           FMitarbeiter=1<<B_Mitarbeiter,FKundenumsatz=1<<B_Kundenumsatz,
           FFlaeche=1<<B_Flaeche,FUnsereKundenNr=1<<B_UnsereKundenNr,
           FVerein=1<<B_Verein};
         enum UpdateBitsBank{FKtonr=1<<B_Ktonr,FBlz=1<<B_Blz,
           FBankindex=1<<B_Bankindex,FBankeinzug=1<<B_Bankeinzug,
           FRabatt=1<<B_Rabatt,FZeilenrabatt=1<<B_Zeilenrabatt,
           FWaehrungid=1<<B_Waehrungid,
           FEinzugrabatt=1<<B_Einzugrabatt,FSkontosatz=1<<B_Skontosatz,
           FSkontofrist=1<<B_Skontofrist,FLieferantenkonto=1<<B_Lieferantenkonto,
           FGegenkonto=1<<B_Gegenkonto};
         enum UpdateBitsSonst{FRechnungan=1<<B_Rechnungan,
           FExtartbezid=1<<B_Extartbezid,FPreisliste=1<<B_Preisliste,
           FNotiz=1<<B_Notiz,FEntsorgung=1<<B_Entsorgung,
            FVerknr=1<<B_Verknr,Kalkulation=1<<B_Kalkulation,
            FStand=1<<B_Stand,FKP_Position=1<<B_KP_Position,
            FKP_Notiz=1<<B_KP_Notiz};
            

        void update_e(UpdateBitsAdresse e);
        void update_e(UpdateBitsFirma e);
        void update_e(UpdateBitsBank e);
        void update_e(UpdateBitsSonst e);

	void delete_Kunde(Kunde::ID kid) throw(SQLerror);
        unsigned int nextval();
        
	void reread_Stand() throw(SQLerror);                
	
	void isLieferadresse(bool is) { lieferadresse=is; }
	void isRechnungsadresse(bool is) { rechnungsadresse=is; }
	void RngAn(const Kunde::ID kid) { rngan=kid; }
        void Rng_an_postfach(bool b) {rng_an_postfach=b;}
        void entsorgung(bool b) { entsorg=b; }

	ID Schema() const { return schema; }

        const Verkaeufer getVerkaeufer() ;
        void setVerkNr(int v) {verkaeufer.verknr = v;}

        // update_Bankeinzug machte Datenbnakzugriff;
        void update_Bank_einzug(bool b) {kundendaten.bankeinzug=b;}
        void set_gruppen_nr(const ID i) {KundenGruppennr=i;}
        void set_sortname(const std::string& s) {adresse.sortname = s; } 
        void set_firma(const std::string& s){adresse.firma = s; } 
        void set_postanwvor(const std::string& s){adresse.postanwvor = s; } 
        void set_strasse(const std::string& s){adresse.strasse = s; } 
        void set_hausnr(const std::string& s){adresse.hsnr = s; }  
        void set_postanwnach(const std::string& s){adresse.postanwnach = s; } 
        void set_plz(const std::string& s){adresse.plz = s; } 
        void set_ort(const std::string& s){adresse.ort = s; }  
        void set_postfach(const std::string& s){adresse.postfach = s; } 
        void set_postfachplz(const std::string& s){adresse.postfachplz = s; } 
        void set_idnr(const std::string& s){IDnr = s; } 
        void set_land(cH_LandesListe i){adresse.land = i; } 
        void set_UnsereKundenNr(const std::string& s){adresse.unsere_kundennr = s; } 
        void set_schema(ID s){schema = s; } 

        void set_planumsatz(const fixedpoint<2> s){kundendaten.planumsatz = s; }
        void set_rabatt(const fixedpoint<2> s){kundendaten.rabatt = s; }
        void set_zeilenrabatt(const bool s){kundendaten.zeilenrabatt = s; }
        void set_flaeche(const int s) {kundendaten.flaeche = s; }
        void set_mitarbeiter(const int s){kundendaten.mitarbeiter = s; }
        void set_kundenumsatz(const fixedpoint<2> s){kundendaten.kundenumsatz = s; }
        void set_umsatz(const fixedpoint<2> s){kundendaten.umsatz = s; }
        void set_verein(const std::string& s){kundendaten.verein = s; }
        void set_skontofrist(const int s){kundendaten.skontofrist = s; }
        void set_einzugrabatt(const fixedpoint<2> s){kundendaten.einzugrabatt = s; }
        void set_skontosatz(const fixedpoint<2> s){kundendaten.skontosatz = s; }        
        void set_notiz(const std::string& s){kundendaten.notiz = s; } 
        void set_bankindex(const int s){bankverb.bankindex = s; } 
        void set_bank_konto(const unsigned long long int s) {bankverb.konto = s;} 
        void set_Lieferantenkonto(const int i) {bankverb.lieferantenkonto=i;}
        void set_Gegenkonto(const int i) {bankverb.gegenkonto=i;}
        // set_bankkonto machte einen Datenbankzugriff

        unsigned long int neue_bank_anlegen(const std::string& name, unsigned long int blz);        
        void get_blz_from_bankindex(unsigned int bankindex);
        cP_Waehrung getWaehrung() const { return waehrung; }
        void setWaehrung(cP_Waehrung w) {waehrung=w; }

        bool operator==(const Kunde& b) const
                {return Id()==b.Id();} 
        bool operator<(const Kunde& b) const
                {return Id()<b.Id();} 
};

class cH_Kunde : public Handle<const Kunde>
{	// cache
        typedef CacheStatic<Kunde::ID,cH_Kunde> cache_t;
        static cache_t cache;
        cH_Kunde(const Kunde *p) : Handle<const Kunde>(p) {}	
        friend cache_t::stl_type;
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
        friend cache_t::stl_type;	
	H_Kunde(Kunde *p) : Handle<Kunde>(p) {}	
	H_Kunde() {}
public:
	typedef Kunde::ID ID;
	static const ID default_id=Kunde::default_id;
	H_Kunde(ID nr);
};


#endif
