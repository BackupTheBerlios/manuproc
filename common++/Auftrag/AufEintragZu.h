// $Id: AufEintragZu.h,v 1.14 2003/02/10 14:33:59 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef AUFTRAGSENTRYZUORDNUNG_H
#define AUFTRAGSENTRYZUORDNUNG_H
#include<Misc/SQLerror.h>
#include<Instanzen/ppsInstanz.h>
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/auftrag_status.h>
#include <list>
#include <Kunde/Kunde.h>

class AufEintragZu : public AufEintragBase
{
public:
 AufEintragZu(AufEintragBase aeb) 
  : AufEintragBase(aeb) {}

 struct st_reflist 
 {	AufEintragBase AEB;
 	ArtikelBase Art;
 	mengen_t Menge;
        
        st_reflist(AufEintragBase aeb,ArtikelBase ab,mengen_t menge) 
              : AEB(aeb),Art(ab),Menge(menge) {}
        st_reflist() : Menge(0) {}
        bool operator<(const st_reflist &b) const {return Art<b.Art;}
        bool operator==(const st_reflist &b) const 
               {return AEB==b.AEB && Art==b.Art && Menge==b.Menge ;}
  };
 typedef std::list<st_reflist> list_t;

 enum VonNachDel {Von,Nach,Delete} ;

  list_t get_Referenz_list_id(const AuftragBase::ID id,bool kinder) const throw(SQLerror);
  list_t select_Id(const AuftragBase::ID id,const list_t &L) const;
  list_t get_Referenz_list_without_child() const throw(SQLerror);

public:
    AuftragBase::mengen_t getMenge(const AufEintragBase& aeb) const;
    // Eine Benachbarte Liste von Kind- bzw. Elternauftr�gen:
    static list_t get_Referenz_list(const AufEintragBase& aeb,bool kinder=false) throw(SQLerror);
    static const bool list_kinder=true;
    static const bool list_eltern=false;

    // Eine Benachbarte Liste von (Kind-)auftr�gen aber nur ungeplante(0er) Auftr�ge:
    list_t get_Referenz_list_ungeplant(bool kinder=true) const throw(SQLerror)
      {return get_Referenz_list_id(AuftragBase::ungeplante_id,kinder);}

    // Eine Benachbarte Liste von (Kind-)auftr�gen aber nur dispo(2er) Auftr�ge:
    list_t get_Referenz_list_dispo(bool kinder=true) const throw(SQLerror)
      {return get_Referenz_list_id(AuftragBase::dispo_auftrag_id,kinder);}

    // Eine Benachbarte Liste von (Kind-)auftr�gen aber nur geplante(1|20000er) Auftr�ge:
    list_t get_Referenz_list_geplant(bool kinder=true) const throw(SQLerror);

    // Eine Benachbarte Liste von Kind- bzw. Elternauftr�gen des zu mir geh�renden
    // geplanten Auftrags holen
    list_t get_Referenz_list_for_geplant(bool kinder=false) const throw(SQLerror);

    list_t get_Referenz_list_for_geplant_neu(bool kinder=false) const throw(SQLerror);
    // F�r einen KOMPLETTEN Auftragsbaum nur_ende=false setzen
    // die folgende Funktion liefert sonst nur die Endauftr�ge OHNE Knoten
    list_t get_Referenz_listFull(bool kinder,bool nur_ende=true) const throw(SQLerror);
                 //kinder=false:   Elternauftr�ge 
                 //kinder=true:    Kinderauftr�ge 

    // Doppelte Eintr�ge in der Kundenliste werden gel�scht:
    std::list<cH_Kunde> get_Referenz_Kunden() const throw(SQLerror);
    std::vector<AufEintragBase> getKundenAuftragV() const;

    static std::list<AufEintragBase> get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,ppsInstanz::ID instanz,AufStatVal status);


    // Neue Eintr�g anlegen:
    void Neu(const AufEintragBase& neuAEB,const mengen_t menge,
             const int oldZnr=0);

    // Menge eines alten Eintrags �ndern:
    bool setMenge(const AufEintragBase& neuAEB,const mengen_t menge);
    // gibt die Menge zur�ck, die ver�ndert wurde. Falls reduziert werden sollte
    // m�ssen die input/output menge nicht �bereinstimmen, da keine negativen Mengen
    // bestellt werden k�nnen
    mengen_t setMengeDiff__(const AufEintragBase& neuAEB,mengen_t menge);
    AuftragBase::mengen_t verteileMenge(list_t L, AuftragBase::mengen_t menge, bool add);
    

    // Zuordnung �ndern:
    bool setKindZnr(const AufEintragBase& neuAEB);

    static bool remove(const AufEintragBase& alt_AEB,const AufEintragBase& neu_AEB);
    static void moveInstanz(const VonNachDel vdl,const AufEintragBase &oldAEB, const AufEintragBase &newAEB) throw(SQLerror);
};

#endif
