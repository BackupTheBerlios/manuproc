// $Id: Kunde.h,v 1.10 2002/01/11 07:59:28 christof Exp $
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

class cH_Kunde;
class H_Kunde;

class Kunde : protected HandleContent
{

public:
 typedef int Nummer;
 typedef Nummer ID;
	
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
   std::string landname;
   std::string lkz;
  };
 typedef struct st_adresse Adresse;  
 
 struct st_bankverb
   {int bankindex;
    std::string bank;
    unsigned int blz;
    unsigned long long int konto;
    const std::string getBankverb() const { return  bank+": BLZ "+itos(blz)+", Kto.Nr. "+ulltos(konto);}
   };
 typedef struct st_bankverb Bankverbindung;
 
 struct st_kddata
   {int flaeche;
    int mitarbeiter; 
    fixedpoint<2> planumsatz;
    fixedpoint<2> umsatz; 
    fixedpoint<2> kundenumsatz; 
    int kalkulation;
    fixedpoint<2> rabatt;
    int skontofrist; 
    ID preisliste; 
    std::string verein; 
    bool bankeinzug;
    std::string notiz; 
    Petig::Datum stand;
    fixedpoint<2> einzugrabatt;
    st_kddata()
    	:flaeche(0),mitarbeiter(0),planumsatz(0),umsatz(0),kundenumsatz(0),
    	 kalkulation(0),rabatt(0),skontofrist(0),preisliste(0),bankeinzug(false),
    	 einzugrabatt(0) {}
   };
 typedef struct st_kddata Kundendaten;


private:
	ID Kundennr;
	ExtBezSchema::ID schema;
	std::string IDnr;
	
	ID rngan;
        bool rng_an_postfach;
        bool lieferadresse;
        bool rechnungsadresse;
        
	Adresse adresse;
	Bankverbindung bankverb;
        Kundendaten kundendaten;
	
	friend class Handle<const Kunde>;
	friend class Handle<Kunde>;
	static const ID _wir=1;
	static const ID _illegal=-1;
	cP_Waehrung waehrung;
   // ...

public:
	static const ID default_id=_wir;
	static const ID none_id=_illegal;
	Kunde(ID nr=default_id) throw(SQLerror);
	static const cH_Kunde newKunde(const Kunde::ID kid, const std::string &firma) throw(SQLerror);
        const std::string LaTeX_von() const;
        const std::string LaTeX_an(bool liefer) const;
        
        const std::string getBank() const { return bankverb.getBankverb(); }
        const unsigned long long int getKtnr() const { return bankverb.konto; }
        const unsigned int getblz() const { return bankverb.blz; }
        const std::string getbank() const { return bankverb.bank; }
        const int getindex() const { return bankverb.bankindex; }

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

        const long int flaeche() const { return kundendaten.flaeche; }
        const long int mitarbeiter() const { return kundendaten.mitarbeiter; }
        const fixedpoint<2> kundenumsatz() const { return kundendaten.kundenumsatz; }
        const fixedpoint<2> planumsatz() const { return kundendaten.planumsatz; }
        const fixedpoint<2> umsatz() const { return kundendaten.umsatz; }
        const fixedpoint<2> rabatt() const { return kundendaten.rabatt; }
        const fixedpoint<2> einzugrabatt() const { return kundendaten.einzugrabatt; }
        const ID Preisliste() const { return kundendaten.preisliste; }
        const int skontofrist() const { return kundendaten.skontofrist; }
        const std::string verein() const { return kundendaten.verein; }
        const bool bankeinzug() const { return kundendaten.bankeinzug; }
        const std::string notiz() const { return kundendaten.notiz; }
        const std::string stand() const { return kundendaten.stand.c_str(); }

        const std::string getName() const {  return adresse.firma; }
        const std::string getSortName() const {  return adresse.sortname; }
        ID Id() const {  return Kundennr; }
        const std::string idnr() const { return IDnr; } 
        ID getNummer() const {  return Kundennr; }
        cH_ExtBezSchema getSchema(ArtikelTyp t=ArtikelTyp::GewebtesBand) const
        	{  return cH_ExtBezSchema(schema,t); }
        bool isLieferadresse() const { return lieferadresse; }
        bool isRechnungsadresse() const { return rechnungsadresse; }
        ID Rngan() const { return rngan; }


	void update() throw(SQLerror);
	void delete_Kunde(Kunde::ID kid) throw(SQLerror);
        unsigned int nextval();
        
	void reread_Stand() throw(SQLerror);                
	
	// HE und was ist mit der Datenbank? CP
	void isLieferadresse(bool is) { lieferadresse=is; update();}
	void isRechnungsadresse(bool is) { rechnungsadresse=is;update(); }
	void RngAn(const Kunde::ID kid) { rngan=kid; update();}

	ID Schema() const { return schema; }


        // Datenbank schreiben
        void update_Bankeinzug(bool be) throw(SQLerror);
        void set_sortname(const std::string& s) {adresse.sortname = s; update();} 
        void set_firma(const std::string& s){adresse.firma = s; update();} 
        void set_postanwvor(const std::string& s){adresse.postanwvor = s; update();} 
        void set_strasse(const std::string& s){adresse.strasse = s; update();} 
        void set_hausnr(const std::string& s){adresse.hsnr = s; update();}  
        void set_postanwnach(const std::string& s){adresse.postanwnach = s; update();} 
        void set_plz(const std::string& s){adresse.plz = s; update();} 
        void set_ort(const std::string& s){adresse.ort = s; update();}  
        void set_postfach(const std::string& s){adresse.postfach = s; update();} 
        void set_postfachplz(const std::string& s){adresse.postfachplz = s; update();} 
        void set_idnr(const std::string& s){IDnr = s; update();} 
        void set_schema(ID s){schema = s; update();} 

        void set_planumsatz(const fixedpoint<2> s){kundendaten.planumsatz = s; update();}
        void set_rabatt(const fixedpoint<2> s){kundendaten.rabatt = s; update();}
        void set_flaeche(const int s) {kundendaten.flaeche = s; update();}
        void set_mitarbeiter(const int s){kundendaten.mitarbeiter = s; update();}
        void set_kundenumsatz(const fixedpoint<2> s){kundendaten.kundenumsatz = s; update();}
        void set_umsatz(const fixedpoint<2> s){kundendaten.umsatz = s; update();}
        void set_verein(const std::string& s){kundendaten.verein = s; update();}
        void set_preisliste(const ID s){kundendaten.preisliste = s; update();}
        void set_skontofrist(const int s){kundendaten.skontofrist = s; update();}
        void set_einzugrabatt(const fixedpoint<2> s){kundendaten.einzugrabatt = s; update();}
        void set_notiz(const std::string& s){kundendaten.notiz = s; update();} 
        void set_bankindex(const int s){bankverb.bankindex = s; update();} 
        void set_bankkonto(const unsigned long long int s){bankverb.konto = s; update();} 
        
        unsigned long int neue_bank_anlegen(const std::string& name, unsigned long int blz);        
        void get_blz_from_bankindex(unsigned int bankindex);
        cP_Waehrung getWaehrung() const { return waehrung; }

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
//        friend cache_t::stl_type;
	friend class std::map<int, cH_Kunde>;
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
	friend class std::map<int, H_Kunde>;
	H_Kunde(Kunde *p) : Handle<Kunde>(p) {}	
	H_Kunde() {}
public:
	typedef Kunde::ID ID;
	static const ID default_id=Kunde::default_id;
	H_Kunde(ID nr);
};

// typedef cH_Kunde const_KundeHandle; deprecated

#endif
