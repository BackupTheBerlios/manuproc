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
//#include <cstdio>
//#include <Gtk2TeX.h>
//#include <Artikel/Einheiten.h>

#define ZEILEN_SEITE_1 33
#define ZEILEN_SEITE_N 37



class LR_Base
{
public:
   enum typ {Lieferschein, Rechnung, Auftrag, NICHTS}; 
protected:
   typ t; 
   LR_Base(typ _t) : t(_t) {}
};


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
      if (t==Auftrag)      return u.a->ArtikelID();
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
      if (t==Auftrag)      return u.a->getMeter(); 
      if (t==Lieferschein) return (float)(u.l->Menge()); abort();}
   int Stueck() const { 
      if (t==Rechnung)     return u.r->Stueck(); 
      if (t==Auftrag)      return u.a->getStueck(); 
      if (t==Lieferschein) return u.l->Stueck(); abort();}
   int Palette() const { 
      if (t==Lieferschein) return u.l->Palette(); return 0;}
   bool ZusatzInfo() const { 
      if (t==Lieferschein) return u.l->ZusatzInfo(); return false;}
   string YourAuftrag() const { 
      if (t==Lieferschein) return u.l->YourAuftrag(); abort();}


};

class LR_Iterator: public LR_Base
{
   typedef LR_Iterator self;  
   union u_t { LieferscheinVoll::const_iterator l; RechnungVoll::const_iterator r;
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
};

class LR_Abstraktion: public LR_Base
{
public:
  typedef LR_Iterator const_iterator;
  union {const LieferscheinVoll *l; const class RechnungVoll *r; 
   const class AuftragFull *a;} u;
public:
  LR_Abstraktion():LR_Base(NICHTS) {}
  LR_Abstraktion(const LieferscheinVoll *l) : LR_Base(Lieferschein) 
  { u.l=l; }
  LR_Abstraktion(const class RechnungVoll *r) : LR_Base(Rechnung) 
  { u.r=r; }
  LR_Abstraktion(const class AuftragFull *a) : LR_Base(Auftrag) 
  { u.a=a; }

  const_iterator begin() const { 
      if (t==Rechnung)     return u.r->begin();
      if (t==Auftrag)      return u.a->begin();
      if (t==Lieferschein) return u.l->begin(); abort();}
  const_iterator end() const { 
      if (t==Rechnung)     return u.r->end();
      if (t==Auftrag)      return u.a->end();
      if (t==Lieferschein) return u.l->end(); abort();}


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
   string Was()         const {
      if (t==Rechnung)     return "Rechnung"; 
      if (t==Auftrag)      return "Auftrag"; 
      if (t==Lieferschein) return "Lieferschein"; abort(); }



   void drucken_header(ofstream &os);
   void drucken_footer(ofstream &os);
   void page_header(int page,ofstream &os,const string& kopie);
   void lieferung_an(ofstream &os, unsigned int lfrs_id, const Petig::Datum& datum,const string& sKunde);

   void drucken_table(ofstream &os,const string& kopie);
   unsigned int drucken_table_header(ofstream &os,  cH_ExtBezSchema& schema,
      unsigned int signifikanz,bool stueck_bool,bool menge_bool,bool rabatt_bool,
      float preismenge, string waehrung, string einheit);
   fixedpoint<2> drucken_table_preissum_warengruppe(ofstream &os,
      vector<fixedpoint<2> >& preissum_zeile, vector<fixedpoint<2> >& preissum_warengruppe,
      unsigned int spaltenzahl,string text="");

};

#endif

