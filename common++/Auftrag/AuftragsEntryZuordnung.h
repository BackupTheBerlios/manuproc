// $Id: AuftragsEntryZuordnung.h,v 1.3 2001/12/23 21:42:15 christof Exp $
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

#ifndef AUFTRAGENTRYZU
#define AUFTRAGENTRYZU
#include<Aux/SQLerror.h>
#include<Aux/ppsInstanz.h>
#include <Auftrag/AufEintragBase2.h>
#include <Auftrag/auftrag_status.h>
#include <list>
#include <Kunde/Kunde.h>

class AufEintragZu : public AufEintragBase2
{
 AufEintragBase2 AEB;

public:
 AufEintragZu(AufEintragBase2 aeb) 
     : AEB(aeb) {}

 struct st_reflist {AufEintragBase2 AEB2;ArtikelBase AB;long Menge;
         st_reflist(AufEintragBase2 aeb2,ArtikelBase ab,long menge) 
              :AEB2(aeb2),AB(ab),Menge(menge){}};
private:
   std::list<cH_Kunde> get_Referenz_Kunden_long() const throw(SQLerror);
public:
    // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen:
    std::list<st_reflist> get_Referenz_list(const AufEintragBase2& aeb,bool kinder=false) const throw(SQLerror);
    // Für einen KOMPLETTEN Auftragsbaum die Klasse AuftragsBaum verwenden  
    // die folgende Funktion liefert nur die Endaufträge OHNE Knoten
    std::list<st_reflist> get_Referenz_listFull(bool kinder) const throw(SQLerror);
                 //kinder=false:   Elternaufträge 
                 //kinder=true:    Kinderaufträge 

    std::list<cH_Kunde> get_Referenz_Kunden() const throw(SQLerror);
    AufEintragBase2 get_AufEintrag_from_Artikel_by_Lfdate   
                   (const ArtikelBase& artikel,const cH_ppsInstanz& instanz);

    static std::list<AufEintragBase2> get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,const cH_ppsInstanz& instanz,AufStatVal status);


    // Neuen Eintrag anlegen:
    void AuftragsEntryZuordnung(const long menge,
                                const AuftragBase& neuAuftrag,const int neuZnr,
                                const int oldZnr=0);
    // Menge eines alten Eintrags ändern:
private:
    void setMenge(const long menge,const AuftragBase& neuAuftrag,const int neuZnr);
public:
    bool setMengeDiff(const long menge,const AuftragBase& neuAuftrag,const int neuZnr);
    // Zuordnung ändern:
    bool setKindZnr(const AuftragBase& neuAuftrag,const int neuZnr);
    //Zeilennummer für Kind erhalten
//    int getKindZnr(const AuftragBase& neuAuftrag) const;
};


#endif
