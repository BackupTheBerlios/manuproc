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
#include <Auftrag/AuftragsEntryZuordnung.h>
#include <fstream>
#include "lr_base.h"

class LR_Entry: public LR_Base
{
   union u_t {const LieferscheinEntry *l ; const RechnungEntry *r; 
              const AufEintrag *a;
            u_t(const LieferscheinEntry *_l) : l(_l) {}
            u_t(const RechnungEntry *_r) : r(_r) {}
            u_t(const AufEintrag *_a) : a(_a) {}
         } u;
public:   
   LR_Entry(const LieferscheinEntry *l)
      : LR_Base(Lieferschein), u(l) {}
   LR_Entry(const RechnungEntry *r)
      : LR_Base(Rechnung), u(r) {}
   LR_Entry(const LR_Base::typ _typ,const AufEintrag *a)
      : LR_Base(_typ),u(a) {}


   const ArtikelBase::ID ArtikelID() const {
      if (Typ()==Rechnung)     return u.r->ArtikelID();
      if (Typ()==Auftrag || Typ()==Intern || Typ()==Extern)      return u.a->ArtId();
      if (Typ()==Lieferschein) return u.l->ArtikelID(); abort();}
   const Preis getPreis(bool brutto=true) const {  
      if (Typ()==Intern||Typ()==Extern) return u.a->EPreis(); 
      if (Typ()==Auftrag) return u.a->EPreis(brutto);  
      if (Typ()==Rechnung)     return u.r->getPreis(brutto); abort();}
   LieferscheinBase::ID Lfrs_Id() const {
      if (Typ()==Rechnung)     return u.r->Lfrs_Id();
      if (Typ()==Lieferschein) return u.l->Id();  abort();}
   fixedpoint<2> Rabatt() const { 
      if (Typ()==Auftrag)    return float(u.a->Rabatt()/100.0);
      if (Typ()==Intern||Typ()==Extern)      return u.a->Rabatt(); 
      if (Typ()==Rechnung)     return u.r->Rabatt(); return 0;}
   fixedpoint<2> Menge() const { 
      if (Typ()==Rechnung)     return u.r->Menge(); 
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return fixedpoint<2>(0);
      if (Typ()==Lieferschein) return (float)(u.l->Menge()); abort();}
   int Stueck() const { 
      if (Typ()==Rechnung)     return u.r->Stueck(); 
      if (Typ()==Auftrag ||  Typ()==Intern||Typ()==Extern)      return u.a->getStueck(); 
      if (Typ()==Lieferschein) return u.l->Stueck(); abort();}
   int Palette() const { 
      if (Typ()==Lieferschein) return u.l->Palette(); return 0;}
   bool ZusatzInfo() const { 
      if (Typ()==Lieferschein) return u.l->ZusatzInfo(); return false;}
   std::string YourAuftrag() const { 
      if (Typ()==Lieferschein) return u.l->YourAuftrag(); abort();}
   int AufId() const { 
      if (Typ()==Lieferschein) return u.l->AufId(); abort();}
   std::list<AufEintragZu::st_reflist> get_Referenz_list_for_geplant(bool b) const {
      AufEintragBase AEB=*(u.a);
      if (Typ()==Intern) return AufEintragZu(AEB).get_Referenz_list_for_geplant(b); 
      abort();}
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
            if (Typ()==Lieferschein) ++u.l; 
            if (Typ()==Rechnung)     ++u.r; 
            if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      ++u.a; 
           return *this;
        }
        bool operator==(const self &b) const
        {  assert(Typ()==b.Typ());
            if (Typ()==Lieferschein) return u.l==b.u.l; 
            if (Typ()==Rechnung)     return u.r==b.u.r; 
            if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return u.a==b.u.a; 
           abort();
        }
        bool operator!=(const self &b) const
        {  return !(*this==b);
        }
        const LR_Entry operator*() const
        {  if (Typ()==Lieferschein) return &*(u.l);
           if (Typ()==Rechnung)     return &*(u.r);
           if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern) return LR_Entry(Typ(),&*u.a);
           abort();
        }
   LR_Iterator(const LieferscheinVoll::const_iterator &l)
   : LR_Base(Lieferschein), u(l) {}
   LR_Iterator(const RechnungVoll::const_iterator &r)
   : LR_Base(Rechnung), u(r) {}
   LR_Iterator(const LR_Base::typ _typ,const AuftragFull::const_iterator &a)
   : LR_Base(_typ), u(a) {}
   
   std::size_t operator-(const LR_Iterator &b) const
        {  assert(Typ()==b.Typ());
           if (Typ()==Lieferschein) return u.l-b.u.l;
           if (Typ()==Rechnung)     return u.r-b.u.r;
           if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return u.a-b.u.a;
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

#define UEBLICHE_INITIALISIERUNG(fp,inst) \
	firmenpapier(fp), kopie(false), stueck_bool(false), menge_bool(false), \
	rabatt_bool(false), preise_addieren(false), \
	instanz(ppsInstanz::default_id), \
	zeilen_passen_noch(0), page_counter(1), \
	spaltenzahl(0), schema_mem(ExtBezSchema::default_ID)
public:
	
  LR_Abstraktion()
  	: LR_Base(NICHTS),UEBLICHE_INITIALISIERUNG(false,ppsInstanz::Kundenauftraege)
  {}
  LR_Abstraktion(const LieferscheinVoll *l, bool fp=false) 
	: LR_Base(Lieferschein),UEBLICHE_INITIALISIERUNG(fp,l->Instanz())
  { u.l=l; }
  LR_Abstraktion(const RechnungVoll *r, bool fp=false) 
	: LR_Base(Rechnung),UEBLICHE_INITIALISIERUNG(fp,ppsInstanz::Kundenauftraege)
  { u.r=r; }
  LR_Abstraktion(LR_Base::typ _typ,const AuftragFull *a, bool fp=false) 
	: LR_Base(_typ), UEBLICHE_INITIALISIERUNG(fp,a->Instanz())
  { u.a=a; }

  bool Firmenpapier() const {return firmenpapier;}
  
  const_iterator begin() const { 
      if (Typ()==Rechnung)     return u.r->begin();
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return LR_Iterator(Typ(),u.a->begin());
      if (Typ()==Lieferschein) return u.l->begin(); abort();}
  const_iterator end() const { 
      if (Typ()==Rechnung)     return u.r->end();
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return LR_Iterator(Typ(),u.a->end());
      if (Typ()==Lieferschein) return u.l->end(); abort();}
  size_t size() const { 
      if (Typ()==Rechnung)     return u.r->size();
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return u.a->size();
      if (Typ()==Lieferschein) return u.l->size(); abort();}

   cP_Waehrung getWaehrung() const { 
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return u.a->getWaehrung(); 
      if (Typ()==Rechnung)     return u.r->getWaehrung(); abort(); }
   fixedpoint<2> Rabatt() const { 
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return 0; 
      if (Typ()==Rechnung)     return u.r->Rabatt(); abort();}
   const Kunde::ID KdNr() const {
      if (Typ()==Rechnung)     return u.r->KdNr(); 
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return u.a->getKundennr(); 
      if (Typ()==Lieferschein) return u.l->KdNr(); abort(); }
   const Petig::Datum getDatum() const {
      if (Typ()==Rechnung)     return u.r->getDatum(); 
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return u.a->getDatum(); 
      if (Typ()==Lieferschein) return u.l->getDatum(); abort(); }
   unsigned int RngNr()   const {
      if (Typ()==Rechnung)     return u.r->Id(); 
      if (Typ()==Auftrag || Typ()==Intern||Typ()==Extern)      return u.a->Id(); 
      if (Typ()==Lieferschein) return u.l->Id(); abort(); }
   cH_Zahlungsart getZahlungsart() const {
      if (Typ()==Rechnung)     return u.r->getZahlungsart(); abort(); }

private:
   void drucken_artikel(std::ostream &os,cH_ArtikelBezeichnung bez,
                        bool zusatzinfo,std::string linecolor,bool& erste_spalte,
                        AuftragBase::mengen_t menge=0);
   void neue_spalte(bool& erste_spalte, std::ostream &os);
   void drucken_header(std::ostream &os);
   void drucken_footer(std::ostream &os);
   void page_header(std::ostream &os);
   void lieferung_an(std::ostream &os, unsigned int lfrs_id, const Petig::Datum& datum,const std::string& sKunde);
   
#ifdef MABELLA_EXTENSIONS
   void auftrag_von(std::ostream &os, const class Auftrag &a,bool only_braces=false);
#endif

   void drucken_table_header(std::ostream &os, const cH_ExtBezSchema& schema,
      		float preismenge, const std::string &preiseinheit);

   void drucken_betrag(std::ostream &os, const std::string &text, fixedpoint<2> betrag);

   void calc_all(cH_Kunde k);
public:
   void drucken(std::ostream &os,bool kopie,const cH_ppsInstanz& instanz);
};

#endif
