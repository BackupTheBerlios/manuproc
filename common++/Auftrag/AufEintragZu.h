// $Id: AufEintragZu.h,v 1.3 2002/05/09 12:45:59 christof Exp $
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
#include<Aux/SQLerror.h>
#include<Aux/ppsInstanz.h>
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/auftrag_status.h>
#include <list>
#include <Kunde/Kunde.h>

class AufEintragZu : public AufEintragBase
{
 AufEintragBase AEB;

public:
 AufEintragZu(AufEintragBase aeb) 
     : AEB(aeb) {}

 struct st_reflist {AufEintragBase AEB;ArtikelBase Art;mengen_t Menge;
         st_reflist(AufEintragBase aeb,ArtikelBase ab,mengen_t menge) 
              :AEB(aeb),Art(ab),Menge(menge){}
              bool operator<(const st_reflist &b) const {return Art<b.Art;}
              bool operator==(const st_reflist &b) const 
               {return AEB==b.AEB && Art==b.Art && Menge==b.Menge ;}
              };

 enum VonNachDel {Von,Nach,Delete} ;

public:
    AuftragBase::mengen_t getMenge(const AufEintragBase& aeb) const;
    // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen:
    std::list<st_reflist> get_Referenz_list(const AufEintragBase& aeb,bool kinder=false) const throw(SQLerror);
    // Eine Benachbarte Liste von Kindaufträgen aber nur ungeplante Aufträge:
    std::list<st_reflist> get_Referenz_list_ungeplant() const throw(SQLerror);
    // Eine Benachbarte Liste von Kindaufträgen aber nur geplante Aufträge:
    std::list<st_reflist> get_Referenz_list_geplant() const throw(SQLerror);
    // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen des zu mir gehörenden
    // geplanten Auftrags holen
    std::list<st_reflist> get_Referenz_list_for_geplant(bool kinder=false) const throw(SQLerror);
    // Für einen KOMPLETTEN Auftragsbaum nur_ende=false setzen
    // die folgende Funktion liefert sonst nur die Endaufträge OHNE Knoten
    std::list<st_reflist> get_Referenz_listFull(bool kinder,bool nur_ende=true) const throw(SQLerror);
                 //kinder=false:   Elternaufträge 
                 //kinder=true:    Kinderaufträge 


    // Doppelte Einträge in der Kundenliste werden gelöscht:
    std::list<cH_Kunde> get_Referenz_Kunden() const throw(SQLerror);

    static std::list<AufEintragBase> get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,ppsInstanz::ID instanz,AufStatVal status);


    // Neue Einträg anlegen:
    void Neu(const AufEintragBase& neuAEB,const mengen_t menge,
             const int oldZnr=0);

    // Menge eines alten Eintrags ändern:
    bool setMenge(const AufEintragBase& neuAEB,const mengen_t menge);
    // gibt die Menge zurück, die verändert wurde. Falls reduziert werden sollte
    // müssen die input/output menge nicht übereinstimmen, da keine negativen Mengen
    // bestellt werden können
    mengen_t setMengeDiff__(const AufEintragBase& neuAEB,const mengen_t menge);

    // Zuordnung ändern:
    bool setKindZnr(const AufEintragBase& neuAEB);

    static bool remove(const AufEintragBase& alt_AEB,const AufEintragBase& neu_AEB);
    static void moveInstanz(const VonNachDel vdl,const AufEintragBase &oldAEB, const AufEintragBase &newAEB) throw(SQLerror);
};


#endif
