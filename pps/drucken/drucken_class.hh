/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef DRUCKEN_CLASS
#define DRUCKEN_CLASS
#include <Lieferschein/LieferscheinVoll.h>
#include <Lieferschein/RechnungVoll.h>
#include <Auftrag/AuftragFull.h>
#include <fstream>
#include "lr_base.h"

class LR_Entry: public LR_Base
{
   union u_t {const LieferscheinEntry *l ; const RechnungEntry *r; 
              const AufEintragBase *a;
            u_t(const LieferscheinEntry *_l) : l(_l) {}
            u_t(const RechnungEntry *_r) : r(_r) {}
            u_t(const AufEintragBase *_a) : a(_a) {}
         } u;
public:   
   LR_Entry(const LieferscheinEntry *l)
      : LR_Base(Lieferschein), u(l) {}
   LR_Entry(const RechnungEntry *r)
      : LR_Base(Rechnung), u(r) {}
   LR_Entry(const AufEintragBase *a)
      : LR_Base(Auftrag), u(a) {}


   const ArtikelBase::ID ArtikelID() const {
      if (t==Rechnung)     return u.r->ArtikelID();
      if (t==Auftrag)      return u.a->ArtId();
      if (t==Lieferschein) return u.r->ArtikelID(); abort();}
   const Preis getPreis() const {  
      if (t==Auftrag)      return u.a->EPreis(); 
      if (t==Rechnung)     return u.r->getPreis(); abort();}
   LieferscheinBase::ID Lfrs_Id() const {
      if (t==Rechnung)     return u.r->Lfrs_Id();
      if (t==Lieferschein) return u.l->Id();  abort();}
   fixedpoint<2> Rabatt() const { 
      if (t==Auftrag)      return u.a->Rabatt(); 
      if (t==Rechnung)     return u.r->Rabatt(); return 0;}
   fixedpoint<2> Menge() const { 
      if (t==Rechnung)     return u.r->Menge(); 
      if (t==Auftrag)      return fixedpoint<2>(0);
      if (t==Lieferschein) return (float)(u.l->Menge()); abort();}
   int Stueck() const { 
      if (t==Rechnung)     return u.r->Stueck(); 
      if (t==Auftrag)      return u.a->getStueck(); 
      if (t==Lieferschein) return u.l->Stueck(); abort();}
   int Palette() const { 
      if (t==Lieferschein) return u.l->Palette(); return 0;}
   bool ZusatzInfo() const { 
      if (t==Lieferschein) return u.l->ZusatzInfo(); return false;}
   std::string YourAuftrag() const { 
      if (t==Lieferschein) return u.l->YourAuftrag(); abort();}
  int AufId() const { 
      if (t==Lieferschein) return u.l->AufId(); abort();}

};

class LR_Iterator: public LR_Base
{
   typedef LR_Iterator self;  
   union u_t { LieferscheinVoll::const_iterator l; 
   	       RechnungVoll::const_iterator r;
               AuftragFull::const_iterator a;
            u_t(const LieferscheinVoll::const_iterator &_l) : l(_l) {}
            u_t(const RechnungVoll::const_iterator         &_r) : r(_r) {}
            u_t(const AuftragFull::const_iterator      &_a) : a(_a) {}
         } u;
      
public:
        self operator++(int x)
        {  self tmp=*this;  
           ++(*this);
           return tmp;
        }
        self &operator++()
        {  
            if (t==Lieferschein) ++u.l; 
            if (t==Rechnung)     ++u.r; 
            if (t==Auftrag)      ++u.a; 
           return *this;
        }
        bool operator==(const self &b) const
        {  assert(t==b.t);
            if (t==Lieferschein) return u.l==b.u.l; 
            if (t==Rechnung)     return u.r==b.u.r; 
            if (t==Auftrag)      return u.a==b.u.a; 
           abort();
        }
        bool operator!=(const self &b) const
        {  return !(*this==b);
        }
        const LR_Entry operator*() const
        {  if (t==Lieferschein) return &*(u.l);
           if (t==Rechnung)     return &*(u.r);
           if (t==Auftrag)      return &*(u.a);
           abort();
        }
   LR_Iterator(const LieferscheinVoll::const_iterator &l)
   : LR_Base(Lieferschein), u(l) {}
   LR_Iterator(const RechnungVoll::const_iterator &r)
   : LR_Base(Rechnung), u(r) {}
   LR_Iterator(const AuftragFull::const_iterator &a)
   : LR_Base(Auftrag), u(a) {}
   
   std::size_t operator-(const LR_Iterator &b) const
        {  assert(t==b.t);
           if (t==Lieferschein) return u.l-b.u.l;
           if (t==Rechnung)     return u.r-b.u.r;
           if (t==Auftrag)      return u.a-b.u.a;
           abort();
        }
};

class LR_Abstraktion: public LR_Base
{
 bool firmenpapier:1;
 bool kopie:1;
 bool stueck_bool:1;
 bool menge_bool:1;
 bool rabatt_bool:1;
 bool preise_addieren:1;
 cH_ppsInstanz instanz;
 
 unsigned int zeilen_passen_noch;
 unsigned int page_counter;
 unsigned int spaltenzahl;
 std::string zur_preisspalte;

 cH_ExtBezSchema schema_mem;

 static const unsigned int signifikanz=1;
 static const unsigned int ZEILEN_SEITE_1=33;
 static const unsigned int ZEILEN_SEITE_N=43;

 fixedpoint<2> betrag;		//
 fixedpoint<2> tabellenbetrag;	// 

 fixedpoint<2> nettobetrag;	// netto
 fixedpoint<2> entsbetrag;	// netto + entsorgungskosten
 fixedpoint<2> entskosten;	// entsorgungskosten
 fixedpoint<2> bruttobetrag;	// brutto (mit MWST)
 fixedpoint<2> mwstbetrag;	// mwst betrag
 fixedpoint<2> skontobetrag;	// brutto - skonto
 fixedpoint<2> einzugbetrag;	// brutto - skonto - einzugrabatt
 
public:
  typedef LR_Iterator const_iterator;

private:
  union { const LieferscheinVoll *l; 
          const class RechnungVoll *r; 
          const class AuftragFull *a; } u;

#define UEBLICHE_INITIALISIERUNG(fp) \
	firmenpapier(fp), kopie(false), stueck_bool(false), menge_bool(false), \
	rabatt_bool(false), preise_addieren(false), \
	instanz(ppsInstanz::default_id), \
	zeilen_passen_noch(0), page_counter(1), \
	spaltenzahl(0), schema_mem(ExtBezSchema::default_ID)
public:
	
  LR_Abstraktion()
  	: LR_Base(NICHTS),UEBLICHE_INITIALISIERUNG(false)
  {}
  LR_Abstraktion(const LieferscheinVoll *l, bool fp=false) 
	: LR_Base(Lieferschein), UEBLICHE_INITIALISIERUNG(fp)
  { u.l=l; }
  LR_Abstraktion(const RechnungVoll *r, bool fp=false) 
	: LR_Base(Rechnung), UEBLICHE_INITIALISIERUNG(fp)
  { u.r=r; }
  LR_Abstraktion(const AuftragFull *a, bool fp=false) 
	: LR_Base(Auftrag), UEBLICHE_INITIALISIERUNG(fp)
  { u.a=a; }

  bool Firmenpapier() const {return firmenpapier;}
  
  const_iterator begin() const { 
      if (t==Rechnung)     return u.r->begin();
      if (t==Auftrag)      return u.a->begin();
      if (t==Lieferschein) return u.l->begin(); abort();}
  const_iterator end() const { 
      if (t==Rechnung)     return u.r->end();
      if (t==Auftrag)      return u.a->end();
      if (t==Lieferschein) return u.l->end(); abort();}
  size_t size() const { 
      if (t==Rechnung)     return u.r->size();
      if (t==Auftrag)      return u.a->size();
      if (t==Lieferschein) return u.l->size(); abort();}

   cP_Waehrung getWaehrung() const { 
      if (t==Auftrag)      return u.a->getWaehrung(); 
      if (t==Rechnung)     return u.r->getWaehrung(); abort(); }
   fixedpoint<2> Rabatt() const { 
      if (t==Auftrag)      return 0; 
      if (t==Rechnung)     return u.r->Rabatt(); abort();}
   const Kunde::ID KdNr() const {
      if (t==Rechnung)     return u.r->KdNr(); 
      if (t==Auftrag)      return u.a->getKundennr(); 
      if (t==Lieferschein) return u.l->KdNr(); abort(); }
   const Petig::Datum getDatum() const {
      if (t==Rechnung)     return u.r->getDatum(); 
      if (t==Auftrag)      return u.a->getDatum(); 
      if (t==Lieferschein) return u.l->getDatum(); abort(); }
   unsigned int RngNr()   const {
      if (t==Rechnung)     return u.r->Id(); 
      if (t==Auftrag)      return u.a->Id(); 
      if (t==Lieferschein) return u.l->Id(); abort(); }
   cH_Zahlungsart getZahlungsart() const {
      if (t==Rechnung)     return u.r->getZahlungsart(); abort(); }

   typ Typ()         const { return t; }

private:
   void drucken_header(std::ostream &os);
   void drucken_footer(std::ostream &os);
   void page_header(std::ostream &os);
   void lieferung_an(std::ostream &os, unsigned int lfrs_id, const Petig::Datum& datum,const std::string& sKunde);
   
#ifdef MABELLA_EXTENSIONS
   void auftrag_von(std::ostream &os, const class Auftrag &a);
#endif

   void drucken_table_header(std::ostream &os, const cH_ExtBezSchema& schema,
      		float preismenge, const std::string &preiseinheit);

   void drucken_betrag(std::ostream &os, const std::string &text, fixedpoint<2> betrag);

   void calc_all(cH_Kunde k);
public:
   void drucken(std::ostream &os,bool kopie,const cH_ppsInstanz& instanz);
};

#endif
