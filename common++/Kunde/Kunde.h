// $Id: Kunde.h,v 1.3 2001/06/06 07:27:39 christof Exp $
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

class cH_Kunde;
class H_Kunde;

class Kunde : protected HandleContent
{

public:
 typedef int Nummer;
 typedef Nummer ID;
	
 struct st_adresse
  {string firma;
   string sortname;
   string postanwvor;
   string strasse;
   string postanwnach;
   string hsnr;
   string plz;
   string postfach;
   string postfachplz;
   string ort;
   string landname;
   string lkz;
  };
 typedef struct st_adresse Adresse;  
 
 struct st_bankverb
   {int bankindex;
    string bank;
    unsigned int blz;
    unsigned long long int konto;
    const string getBankverb() const { return  bank+": BLZ "+itos(blz)+", Kto.Nr. "+ulltos(konto);}
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
    ID preisliste;  // gibt es schon unten ...
    string verein; 
    bool bankeinzug;
    string notiz; 
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
	string IDnr;
	
	ID rngan,preisliste;
        bool rng_an_postfach;
        bool lieferadresse;
        bool rechnungsadresse;
        
	Adresse adresse;
	Bankverbindung bankverb;
        Kundendaten kundendaten;
	
	friend class const_Handle<Kunde>;
	friend class Handle<Kunde>;
	static const ID _wir=1;
	static const ID _illegal=-1;
   // ...
public:
	static const ID default_id=_wir;
	static const ID none_id=_illegal;
	Kunde(ID nr=default_id) throw(SQLerror);
	static const cH_Kunde newKunde(const Kunde::ID kid, const string &firma) throw(SQLerror);
        const string LaTeX_von() const;
        const string LaTeX_an(const string& wo) const;
        
        const string getBank() const { return bankverb.getBankverb(); }
        const unsigned long long int getKtnr() const { return bankverb.konto; }
        const unsigned int getblz() const { return bankverb.blz; }
        const string getbank() const { return bankverb.bank; }
        const int getindex() const { return bankverb.bankindex; }

        const string sortname() const { return adresse.sortname; }
        const string firma() const { return adresse.firma; }
        const string postanwvor() const { return adresse.postanwvor; }
        const string strasse() const { return adresse.strasse; }
        const string hausnr() const { return adresse.hsnr; }
        const string postanwnach() const { return adresse.postanwnach; }
        const string plz() const { return adresse.plz; }
        const string ort() const { return adresse.ort; }
        const string postfach() const { return adresse.postfach; }
        const string postfachplz() const { return adresse.postfachplz; }

        const long int flaeche() const { return kundendaten.flaeche; }
        const long int mitarbeiter() const { return kundendaten.mitarbeiter; }
        const double kundenumsatz() const { return kundendaten.kundenumsatz; }
        const double planumsatz() const { return kundendaten.planumsatz; }
        const double umsatz() const { return kundendaten.umsatz; }
        const double rabatt() const { return kundendaten.rabatt; }
        const double einzugrabatt() const { return kundendaten.einzugrabatt; }
//        const int preisliste() const { return kundendaten.preisliste; }
        const int skontofrist() const { return kundendaten.skontofrist; }
        const string verein() const { return kundendaten.verein; }
        const bool bankeinzug() const { return kundendaten.bankeinzug; }
        const string notiz() const { return kundendaten.notiz; }
        const string stand() const { return kundendaten.stand.c_str(); }

        const string getName() const {  return adresse.firma; }
        const string getSortName() const {  return adresse.sortname; }
        ID Id() const {  return Kundennr; }
        const string idnr() const { return IDnr; } 
        ID getNummer() const {  return Kundennr; }
        cH_ExtBezSchema getSchema(ArtikelTyp t=ArtikelTyp::GewebtesBand) const
        	{  return cH_ExtBezSchema(schema,t); }
        bool isLieferadresse() const { return lieferadresse; }
        bool isRechnungsadresse() const { return rechnungsadresse; }
        ID Rngan() const { return rngan; }


	void update() throw(SQLerror);
        unsigned int nextval();
        
                
	
	// HE und was ist mit der Datenbank? CP
	void isLieferadresse(bool is) { lieferadresse=is; update();}
	void isRechnungsadresse(bool is) { rechnungsadresse=is;update(); }
	void isBankeinzug(bool is) { kundendaten.bankeinzug=is;update(); }
	void RngAn(const Kunde::ID kid) { rngan=kid; update();}

	ID Preisliste() const { return preisliste; }


        // Datenbank schreiben
//        void set_Bank(const string& s); 
        void set_sortname(const string& s) {adresse.sortname = s; update();} 
        void set_firma(const string& s){adresse.firma = s; update();} 
        void set_postanwvor(const string& s){adresse.postanwvor = s; update();} 
        void set_strasse(const string& s){adresse.strasse = s; update();} 
        void set_hausnr(const string& s){adresse.hsnr = s; update();}  
        void set_postanwnach(const string& s){adresse.postanwnach = s; update();} 
        void set_plz(const string& s){adresse.plz = s; update();} 
        void set_ort(const string& s){adresse.ort = s; update();}  
        void set_postfach(const string& s){adresse.postfach = s; update();} 
        void set_postfachplz(const string& s){adresse.postfachplz = s; update();} 
        void set_idnr(const string& s){IDnr = s; update();} 

        void set_planumsatz(const fixedpoint<2> s){kundendaten.planumsatz = s; update();}
        void set_rabatt(const fixedpoint<2> s){kundendaten.rabatt = s; update();}
        void set_flaeche(const int s) {kundendaten.flaeche = s; update();}
        void set_mitarbeiter(const int s){kundendaten.mitarbeiter = s; update();}
        void set_kundenumsatz(const fixedpoint<2> s){kundendaten.kundenumsatz = s; update();}
        void set_umsatz(const fixedpoint<2> s){kundendaten.umsatz = s; update();}
        void set_verein(const string& s){kundendaten.verein = s; update();}
        void set_preisliste(const ID s){kundendaten.preisliste = s; update();}
        void set_skontofrist(const int s){kundendaten.skontofrist = s; update();}
        void set_einzugrabatt(const fixedpoint<2> s){kundendaten.einzugrabatt = s; update();}
        void set_notiz(const string& s){kundendaten.notiz = s; update();} 
        void set_bankindex(const int s){bankverb.bankindex = s; update();} 
        void set_bankkonto(const unsigned long long int s){bankverb.konto = s; update();} 
        

};

class cH_Kunde : public const_Handle<Kunde>
{	// cache
        typedef CacheStatic<Kunde::ID,cH_Kunde> cache_t;
        static cache_t cache;
  cH_Kunde(const Kunde *p) : const_Handle<Kunde>(p) {}	
        friend cache_t::stl_type;
        cH_Kunde() {}
public:
	typedef Kunde::ID ID;
	static const ID default_id=Kunde::default_id;
	cH_Kunde(ID nr);
 
};

class H_Kunde : public Handle<Kunde>
{public:
	H_Kunde(Kunde *p) : Handle<Kunde>(p) {}	
	typedef Kunde::ID ID;
	static const ID default_id=Kunde::default_id;
};

// typedef cH_Kunde const_KundeHandle; deprecated

#endif
