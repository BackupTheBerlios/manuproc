/* $Id: AufEintragBase.h,v 1.4 2001/06/06 07:27:39 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef CXX_AUFTRAG_AUFEINTRAGBASE_H
#define CXX_AUFTRAG_AUFEINTRAGBASE_H

// na ob das alles erforderlich ist ... CP
#include <Artikel/ArtikelBaum.h> //?
#include <Artikel/ArtikelBezeichnung.h> //?
#include <Artikel/Preis.h>
#include <Auftrag/AufEintragBase2.h>
#include <Auftrag/auftrag_status.h>
#include <Auftrag/selAufEntry.h> //?
#include <Aux/CacheStatic.h>
#include <Aux/Datum.h>
#include <Aux/Handles.h>
#include <Aux/Kalenderwoche.h>
#include <Aux/SQLerror.h>
#include <Aux/Waehrung.h>
#include <Kunde/Kunde.h>
#include <map>
#include <string>
#include <vector>

class cH_AufArtikel;

class AufArtikel : public ArtikelBaum, protected HandleContent
{friend class cH_AufArtikel;
 friend class const_Handle<AufArtikel>;

 ArtikelBase rohartikel; // = webartikel = rohrohroh...artikel
 cH_ArtikelBezeichnung bez;
 
public:

 void setArtikelBezeichnung(const cH_ExtBezSchema &cs) 
 	{ bez= cH_ArtikelBezeichnung(Id(),cs); }
 static const ID default_id=0;

 AufArtikel(const ID &stamp,const cH_ExtBezSchema &schema)
 	: ArtikelBaum(stamp),ArtikelBase(stamp),bez(stamp,schema)
 	{}
 AufArtikel(const cH_ExtBezSchema &schema) 
 	: bez(ID(),schema) {}
 AufArtikel() : bez(cH_ArtikelBezeichnung::Default())
 	{}

 AufArtikel(const vector<EntryValue> &_values, const cH_ExtBezSchema &schema)
 	: bez(1,_values,schema)
 	{
 	 ArtikelBaum::setID(bez->Id());
 	}
 AufArtikel(const ArtikelBase &artb, const cH_ExtBezSchema &schema)
 	: ArtikelBaum(artb.Id()), ArtikelBase(artb),  bez(artb,schema)
 	{ }

 double Stueckgroesse() const;
 const ID getRohArtid() const 
 { return rohartikel.Id(); }
 void setRohArtid(const ID i) // besser const ArtikelBase & !
 { rohartikel=ArtikelBase(i); }
 
 // ArtikelBezeichnung
 typedef ArtikelBezeichnung::const_iterator const_iterator;
  const_iterator begin() const { return bez->begin(); }
  const_iterator end() const { return bez->end(); }
  int size() const { return bez->size(); }
  const EntryValue operator[](int feld) const
  {  return (*bez)[feld]; }
  string Bezeichnung(char seperator=0,int felder=-1) const throw()
  {  return bez->Bezeichnung(seperator,felder); }
  const cH_ExtBezSchema getExtBezSchema() const throw()
  {  return bez->getExtBezSchema(); }
};



class cH_AufArtikel : public const_Handle<AufArtikel>
{
private:
 typedef CacheStatic<AufArtikel::ID,cH_AufArtikel> cache_t;
 static cache_t cache;
 cH_AufArtikel(const AufArtikel *p) : const_Handle<AufArtikel>(p) {}
 friend cache_t::stl_type;
 cH_AufArtikel() {}

public:
 static const AufArtikel::ID default_pid=AufArtikel::default_id;
 cH_AufArtikel(AufArtikel::ID pid, const cH_ExtBezSchema &schema);
};



class AufEintragBase : public AufEintragBase2
{
protected: 
 cH_AufArtikel artikel;

 int dispoentrynr;
 int disponr;
 
 AufStatVal status; /* Status des zugehörigen Auftrages */
 AufStatVal entrystatus; /* Status des zugehörigen Eintrags */
 
 int kdnr;
 string youraufnr;
 long bestellt;    /* Stück */
 long geliefert;   /* Stück */
 double rest;        /* Meter */
 double menge;        /* Meter */
 Petig::Datum lieferdatum;
 Petig::Datum lasteditdate;
 int jahrgang;
 cH_Prozess prozess;
 Petig::Datum prozdate;

// cP_Waehrung waehrung;
 Preis preis;
// int preis;      // Pf
 int rabatt;     // % * 100
// Preisflag pflag;
 float preismenge;
 	
public:
 AufEintragBase() 
   : artikel(0,cH_ExtBezSchema(ExtBezSchema::default_ID)), dispoentrynr(0),
   	disponr(0), status((AufStatVal)OPEN), entrystatus((AufStatVal)OPEN),
   	kdnr(0), 
   	bestellt(0),
   	geliefert(0), rest(0), menge(0), jahrgang(0), prozess(Prozess::default_id),
	rabatt(0)
 {}

 AufEintragBase(const SelectedAufentry &aufentry, 
 		const cH_ExtBezSchema schema) throw(SQLerror);

 AufEintragBase(int _auftragid, int _zeilennr, int _bestellt,
	int _artikel, const Petig::Datum _lieferdatum,
	int _geliefert,
	int _dispoentrynr, int _disponr, int _jahrgang,
	AufStatVal _aufstatus,
	int _kdnr, const string _youraufnr,
	const string _prozdate,
	int _prozess,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystat, const Petig::Datum _lasteditdate,
	const cH_ExtBezSchema schema) throw();
	
 AufEintragBase(int aufid, int znr, const cH_ExtBezSchema schema) throw(SQLerror);

 void updateDispoENr(int dinr) throw(SQLerror);
 void updateStk(long stk) throw(SQLerror);
 void updateLieferdatum(const Petig::Datum &ld) throw(SQLerror);	
 void updatePreis(const Preis &pr) throw(SQLerror);
 void updateRabatt(int rb) throw(SQLerror);
 void setStatus(AufStatVal st) throw(SQLerror);		
 void split(int newmenge, const Petig::Datum &newld) throw(SQLerror);
 
 long getMeter() const { return (long)menge; }
 long getRest() const { return (long)rest; }	
 long getStueck() const { return bestellt;}
 long getRestStk() const { return bestellt-geliefert;}
 long getGeliefert() const { return geliefert;}
 long getGeliefertM() const { return long(
 		(geliefert*(artikel->Stueckgroesse()*10.0))/10);}
// const char *getLastDate() const { return lasteditdate.c_str();}
// const char *getLastEdit() const { return lastedit.c_str(); }
 AufStatVal getAufStatus() const { return status; }
 int getZnr() const { return zeilennr;}
 int getAuftragid() const {return auftragid;}
 AufStatVal getEntryStatus() const { return entrystatus;}
 const string getEntryStatusStr() const;
 const Petig::Datum LastEditDate() const { return lasteditdate; }
 string getYourAufNr() const { return youraufnr;}
 int getDispoENr() const { return dispoentrynr;}
 const Petig::Datum getLieferdatum() const { return lieferdatum;}
 int getKdNr() const { return kdnr;}
 int getJahrgang() const { return jahrgang;}
 const Petig::Datum getProzDat() const { return prozdate;} 
 cH_Prozess getProzess() const { return prozess;}
 const cP_Waehrung getWaehrung() const { return preis.getWaehrung(); }
 const Preis GPreis() const;
 void setVerarbeitung(const cH_Prozess p);  
 const Preis EPreis() const { return preis;}
 int Rabatt() const { return rabatt;}
 float PreisMenge() const { return preis.PreisMenge(); }
 const ArtikelBase::ID &ArtikelID() const { return artikel->Id(); }

 void setArtikelBezeichnung(const cH_ExtBezSchema &cs)
 	{const_cast<AufArtikel&>(*artikel).setArtikelBezeichnung(cs); } 
  
 friend ostream &operator<<(ostream &o,const AufEintragBase &aeb);
};

ostream &operator<<(ostream &o,const AufEintragBase &aeb);

#endif
