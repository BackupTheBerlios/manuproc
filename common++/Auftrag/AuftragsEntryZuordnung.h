// $Id: AuftragsEntryZuordnung.h,v 1.8 2002/02/08 21:47:14 christof Exp $
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
// mutable int tiefe; // Geasamtmenge der zu planenden Instanzen

public:
 AufEintragZu(AufEintragBase aeb) 
     : AEB(aeb) /*, tiefe(-1)*/ {}

 struct st_reflist {AufEintragBase AEB2;ArtikelBase AB;mengen_t Menge;
         st_reflist(AufEintragBase aeb2,ArtikelBase ab,mengen_t menge) 
              :AEB2(aeb2),AB(ab),Menge(menge){}};

 enum VonNachDel {Von,Nach,Delete} ;
private:
   std::list<cH_Kunde> get_Referenz_Kunden_long() const throw(SQLerror);
public:
    // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen:
    std::list<st_reflist> get_Referenz_list(const AufEintragBase& aeb,bool kinder=false) const throw(SQLerror);
    // Für einen KOMPLETTEN Auftragsbaum die Klasse AuftragsBaum verwenden oder nur_ende=false setzen
    // die folgende Funktion liefert sonst nur die Endaufträge OHNE Knoten
    std::list<st_reflist> get_Referenz_listFull(bool kinder,bool nur_ende=true) const throw(SQLerror);
                 //kinder=false:   Elternaufträge 
                 //kinder=true:    Kinderaufträge 
    // Um diesen Werte zu erhalten muß zuvor(!) get_Referenz_listFull(kinder,false);
    // aufgerufen werden.
//    int Tiefe() const {return tiefe;}


    std::list<cH_Kunde> get_Referenz_Kunden() const throw(SQLerror);

    static std::list<AufEintragBase> get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,ppsInstanz::ppsInstId instanz,AufStatVal status);


    // Neuen Eintrag anlegen:
    void AuftragsEntryZuordnung(const mengen_t menge,
                                const AuftragBase& neuAuftrag,const int neuZnr,
                                const int oldZnr=0);
    // Menge eines alten Eintrags ändern:
private:
    void setMenge(const mengen_t menge,const AuftragBase& neuAuftrag,const int neuZnr);
public:
    bool setMengeDiff(const mengen_t menge,const AuftragBase& neuAuftrag,const int neuZnr);
    // Zuordnung ändern:
    bool setKindZnr(const AuftragBase& neuAuftrag,const int neuZnr);

    static void moveInstanz(const VonNachDel vdl,const AufEintragBase &oldAEB, const AufEintragBase &newAEB) throw(SQLerror);
};


#endif
