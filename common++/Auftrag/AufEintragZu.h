// $Id: AufEintragZu.h,v 1.29 2004/09/01 12:25:48 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
#include <map>
#include <Kunde/Kunde.h>
#include <Misc/compiler_ports.h>
#include <Misc/TimeStamp.h>

class AufEintragZu : public AufEintragBase
{
public:
 AufEintragZu(AufEintragBase aeb) 
  : AufEintragBase(aeb) {}

 struct st_reflist 
 {	AufEintragBase AEB;
 	ArtikelBase Art;
 	mengen_t Menge;
 	ManuProC::TimeStamp Pri;
        
//        st_reflist(AufEintragBase aeb,ArtikelBase ab,mengen_t menge) 
//              : AEB(aeb),Art(ab),Menge(menge) {}
        st_reflist() : Menge() {}
//        bool operator<(const st_reflist &b) const {return Art<b.Art;}
        bool operator==(const st_reflist &b) const 
               {return AEB==b.AEB && Art==b.Art && Menge==b.Menge ;}
  };
 typedef std::list<st_reflist> list_t;
 typedef std::map<ArtikelBase,list_t> map_t;

 enum VonNachDel {Von,Nach,Delete} ;

  list_t get_Referenz_list_id(const AuftragBase::ID id,bool kinder,bool artikel=list_ohneArtikel) const throw(SQLerror);
  list_t select_Id(const AuftragBase::ID id,const list_t &L) const;
  list_t get_Referenz_list_without_child() const throw(SQLerror);

public:
    AuftragBase::mengen_t getMenge(const AufEintragBase& aeb) const;
    // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen:
    static list_t get_Referenz_list(const AufEintragBase& aeb,bool kinder,bool artikel,bool sorted=true) throw(SQLerror);
    static const bool list_kinder=true;
    static const bool list_eltern=false;
    static const bool list_Artikel=true;
    static const bool list_ohneArtikel=false;
    static const bool list_unsorted=false;

    static map_t get_Kinder_nach_Artikel(const AufEintragBase &aeb,bool kinder=list_kinder,bool sorted=true);

    // Eine Benachbarte Liste von (Kind-)aufträgen aber nur ungeplante(0er) Aufträge:
    __deprecated list_t get_Referenz_list_ungeplant(bool kinder=true,bool artikel=list_ohneArtikel) const throw(SQLerror)
      {return get_Referenz_list_id(AuftragBase::ungeplante_id,kinder,artikel);}

    // Eine Benachbarte Liste von (Kind-)aufträgen aber nur dispo(2er) Aufträge:
    __deprecated list_t get_Referenz_list_dispo(bool kinder=true) const throw(SQLerror)
      {return get_Referenz_list_id(AuftragBase::dispo_auftrag_id,kinder);}

    // Eine Benachbarte Liste von (Kind-)aufträgen aber nur geplante(1|20000er) Aufträge:
    __deprecated list_t get_Referenz_list_geplant(bool kinder=true) const throw(SQLerror);

    // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen des zu mir gehörenden
    // geplanten Auftrags holen
    __deprecated list_t get_Referenz_list_for_geplant(bool kinder=false) const throw(SQLerror);

    __deprecated list_t get_Referenz_list_for_geplant_neu(bool kinder=false) const throw(SQLerror);
    // Für einen KOMPLETTEN Auftragsbaum nur_ende=false setzen
    // die folgende Funktion liefert sonst nur die Endaufträge OHNE Knoten
// noch mal überdenken, was davon notwendig ist
    list_t get_Referenz_listFull(bool kinder,bool nur_ende=true) const throw(SQLerror);
                 //kinder=false:   Elternaufträge 
                 //kinder=true:    Kinderaufträge 

    // Doppelte Einträge in der Kundenliste werden gelöscht:
    std::list<cH_Kunde> get_Referenz_Kunden() const throw(SQLerror);
    std::vector<AufEintragBase> getKundenAuftragV() const;

    static std::list<AufEintragBase> get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,ppsInstanz::ID instanz,AufStatVal status);

    static mengen_t Summe(const list_t &l,ID wovon=none_id);

    // Neue Einträg anlegen:
    void Neu(const AufEintragBase& neuAEB,const mengen_t menge);

    // gibt die Menge zurück, die verändert wurde. Falls reduziert werden sollte
    // müssen die input/output menge nicht übereinstimmen, da keine negativen Mengen
    // bestellt werden können
    // äquivalent zu Neu?
    mengen_t setMengeDiff__(const AufEintragBase& neuAEB,mengen_t menge);

    static bool remove(const AufEintragBase& alt_AEB,const AufEintragBase& neu_AEB,
    	const ManuProC::TimeStamp &Pri=ManuProC::TimeStamp());
    static void moveInstanz(const VonNachDel vdl,const AufEintragBase &oldAEB, const AufEintragBase &newAEB) throw(SQLerror);
};

namespace AufEintragZu_sort
{	bool auftr_1230(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b);
	bool priority(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b);
	bool auftr_0123(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b);
	bool auftr_43210(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b);
	bool auftr_34012(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b);
};

AufEintragZu::mengen_t Summe(const AufEintragZu::list_t &l);

#endif
