/* $Id: Lieferschein.cc,v 1.6 2002/05/06 13:41:23 christof Exp $ */
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

#include"Lieferschein.h"
#include <Aux/Transaction.h>
#include <Auftrag/selFullAufEntry.h>
#include <Artikel/Einheiten.h>

Lieferschein::Lieferschein(const LieferscheinBase &lsbase, const Petig::Datum &_lsdatum,
int _kdnr,int _rngid, int _paeckchen, int _pakete, const Petig::Datum &_geliefertam,
int _dpdlnr)
: LieferscheinBase(lsbase), lsdatum(_lsdatum), kunde(_kdnr), rngid(_rngid),
paeckchen(_paeckchen),pakete(_pakete), geliefertam(_geliefertam),
dpdliefnr(_dpdlnr)
{}

void Lieferschein::push_back(const ArtikelBase &artikel, int anzahl, 
		mengen_t mengeneinheit, int palette)
{  Transaction tr;
   SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Kunde_Artikel
   			(instanz->Id(),getKunde()->Id(),artikel.Id()));
   SelectedFullAufList auftraglist(psel);

   AuftragBase::mengen_t menge(anzahl);
   Einheit e(artikel);
   if (!e.hatMenge()) mengeneinheit=0;
   else menge *= mengeneinheit;

   if (auftraglist.aufidliste.begin()==auftraglist.aufidliste.end())
     // kann nicht abschreiben
   {  LieferscheinEntry(*this, artikel, anzahl,
                           mengeneinheit,palette,false);
   }
   else if (menge<=auftraglist.aufidliste.begin()->getRestStk())
     // kann in einem Stueck abschreiben
   {  SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
      LieferscheinEntry(*this, *i,artikel, anzahl,mengeneinheit,palette,false);
      i->abschreiben(menge);
   }
   else
   // stueckeln (1*Lieferung, dann Zuordnung)
   {  LieferscheinEntry(*this,artikel,anzahl,mengeneinheit,palette);

        for (SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
     	        menge && i!=auftraglist.aufidliste.end(); ++i)
        {  AuftragBase::mengen_t abmenge(menge);
           if (i->getRestStk()<abmenge) abmenge=i->getRestStk();
           
           int lstueck=1;
           LieferscheinBase::mengen_t lmenge=0;
           if (!e.hatMenge()) lstueck=int(float(abmenge)+.5);
           else lmenge=abmenge;
           LieferscheinEntry(*this,*i,artikel,lstueck,lmenge,0,true);
           
           i->abschreiben(abmenge);

           menge-=abmenge;
    	   }
        if (menge)
        {  
           int lstueck=1;
           LieferscheinBase::mengen_t lmenge=0;
           if (!e.hatMenge()) lstueck=int(float(menge)+.5);
           else lmenge=menge;
           LieferscheinEntry(*this,artikel,lstueck,lmenge,0,true);
        }
   }
   tr.commit();
}

void Lieferschein::push_back(const AufEintragBase &aufeintrag,
		const ArtikelBase &artikel, int anzahl, 
		mengen_t menge, int palette)
{  LieferscheinEntry(*this, aufeintrag ,artikel, anzahl,menge,palette);
   if (!menge)
      aufeintrag.abschreiben(anzahl);
   else
      aufeintrag.abschreiben(anzahl*menge);
}

