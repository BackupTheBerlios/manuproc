// $Id: AufEintragBase2.h,v 1.29 2001/11/27 08:09:38 cvs_christof Exp $
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

#ifndef AUFEINTRAGBASE2
#define AUFEINTRAGBASE2
#include<Artikel/Prozess.h>
#include<Aux/SQLerror.h>
#include<Aux/ppsInstanz.h>
#include <Auftrag/AuftragBase.h>
#include <list>
class cH_Kunde;


// eigentlich AufEintragBaseBase

class AufEintragBase2 : public AuftragBase
{
protected:
 int zeilennr;
  
 // noch benötigt? CP 10'01
private:
 inline int mapKey() const { return auftragid*10000+zeilennr; } 

public:
 AufEintragBase2() : zeilennr(0) {}
 AufEintragBase2(cH_ppsInstanz inst) 
 	: AuftragBase(inst), zeilennr(0) {}
 AufEintragBase2(cH_ppsInstanz inst,int a,int z) 
 	:  AuftragBase(inst,a), zeilennr(z) {}
 
 void setVerarbeitung(const cH_Prozess p) const throw(SQLerror);
 // gibt gelieferte Menge zurück
 int abschreiben(int menge) const throw(SQLerror);
 bool deleteAuftragEntry() const throw(SQLerror);

                                                          
    void setLetztePlanungFuer(int planinstanz) const throw(SQLerror);
    void calculateProzessInstanz();
    void setMaxPlanInstanz(int planinstanz) const throw(SQLerror);
 
    int ZNr() const { return zeilennr; }
    bool valid() const { return AuftragBase::valid(); }

    bool operator<(const AufEintragBase2& b) const 
        {return Id()<b.Id();}
    bool operator==(const AufEintragBase2& b) const 
        {return Id()==b.Id();}
};

class AufEintragBaseList : public AufEintragBase2
{
 AufEintragBase2 AEB;

public:
 AufEintragBaseList(AufEintragBase2 aeb) 
     : AEB(aeb) {}

 struct st_reflist {AufEintragBase2 AEB2;ArtikelBase AB;long Menge;
         st_reflist(AufEintragBase2 aeb2,ArtikelBase ab,long menge) 
              :AEB2(aeb2),AB(ab),Menge(menge){}};
private:
   std::list<cH_Kunde> get_Referenz_Kunden_long() const throw(SQLerror);
public:
    // Eine Benachbarte Liste von Kind- bzw. Elternaufträgen:
    std::list<st_reflist> get_Referenz_list(const AufEintragBase2& aeb,bool kinder=false) const throw(SQLerror);
    // Für einen KOMPLETTEN Auftragsbaum bitte die Klasse AuftragsBaum verwenden  
    // die folgende Funktion liefert nur die Endaufträge OHNE Knoten
    std::list<st_reflist> get_Referenz_listFull(bool kinder) const throw(SQLerror);
                 //kinder=false:   Elternaufträge 
                 //kinder=true:    Kinderaufträge 

    std::list<cH_Kunde> get_Referenz_Kunden() const throw(SQLerror);
    AufEintragBase2 get_AufEintrag_from_Artikel_by_Lfdate   
                   (const ArtikelBase& artikel,const cH_ppsInstanz& instanz);

    static std::list<AufEintragBase2> get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,const cH_ppsInstanz& instanz);
//#warning was will der compiler von mir ??? MAT
//    static AufEintragBase2 get_AufEintrag_from_Artikel_by_Lfdate
//               (const ArtikelBase& artikel,const cH_ppsInstanz& instanz);
   
};


#endif
