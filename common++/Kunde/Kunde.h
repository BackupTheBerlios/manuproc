// $Id: Kunde.h,v 1.2 2001/04/23 12:05:50 christof Exp $
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
	
	friend class const_Handle<Kunde>;
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
	
	// HE und was ist mit der Datenbank? CP
	void isLieferadresse(bool is) { lieferadresse=is; }
	void isRechnungsadresse(bool is) { rechnungsadresse=is; }
	void RngAn(const Kunde::ID kid) { rngan=kid; }

	ID Preisliste() const { return preisliste; }
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

typedef cH_Kunde const_KundeHandle;

#endif
