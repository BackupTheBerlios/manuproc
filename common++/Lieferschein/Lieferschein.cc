/* $Id: Lieferschein.cc,v 1.17 2002/11/07 07:48:59 christof Exp $ */
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
#include <Aux/FetchIStream.h>
//#include <Instanzen/Produziert.h>
#include <Instanzen/ppsInstanzProduziert.h>
#include <unistd.h> 
#include <Aux/Trace.h>

Lieferschein::Lieferschein(const LieferscheinBase &lsbase, const ManuProC::Datum &_lsdatum,
int _kdnr,int _rngid, int _paeckchen, int _pakete, const ManuProC::Datum &_geliefertam,
int _dpdlnr)
: LieferscheinBase(lsbase), lsdatum(_lsdatum), kunde(_kdnr), rngid(_rngid)
, geliefertam(_geliefertam)
#ifdef DPD_LIEFERSCHEINE
,dpdliefnr(_dpdlnr),paeckchen(_paeckchen),pakete(_pakete)
#endif
{}

void Lieferschein::push_back(const ArtikelBase &artikel, int anzahl, 
		mengen_t mengeneinheit, int palette)
{  
   // Eine Instanz, die Lieferscheine schreibt, produziert auch selber (assert in ppsInstanz).
   // Wenn man einn Lieferschein schriebe, ohne daß diese Instanz 
   // selber produzierte, würde doppelt produziert werden.
   assert(Instanz()->Lieferschein());
   ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"Artikel="+cH_ArtikelBezeichnung(artikel)->Bezeichnung(),
           "Anzahl="+itos(anzahl),"Menge=",mengeneinheit,"Palette="+itos(palette));                     
   Transaction tr;
   SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Artikel_Planung_id
   			(instanz->Id(),getKunde()->Id(),artikel.Id(),AuftragBase::handplan_auftrag_id));
   SelectedFullAufList auftraglist(psel);

   AuftragBase::mengen_t menge(anzahl);
   Einheit e(artikel);
   if (!e.hatMenge()) mengeneinheit=0;
   else menge *= mengeneinheit.as_float();

   if (auftraglist.aufidliste.begin()==auftraglist.aufidliste.end())
     // kann nicht abschreiben
   {  LieferscheinEntry::create(*this, artikel, anzahl,mengeneinheit,palette,false);
      if(Instanz()!=ppsInstanzID::Kundenauftraege)
       {
         ManuProC::st_produziert p(artikel,menge,getuid(),getKunde()->Id(),Id());
         Instanz()->Produziert(p);
       }
   }
   else if (menge<=auftraglist.aufidliste.begin()->getRestStk())
     // kann in einem Stueck abschreiben
   {  SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
      LieferscheinEntry::create(*this, *i,artikel, anzahl,mengeneinheit,palette,false);
      i->Produziert(menge,Id());
   }
   else
   // stueckeln (1*Lieferung, dann Zuordnung)
   {  LieferscheinEntry LE=LieferscheinEntry::create(*this,artikel,anzahl,mengeneinheit,palette,true);

        for (SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
     	        !!menge && i!=auftraglist.aufidliste.end(); ++i)
        {  
           AuftragBase::mengen_t abmenge(menge);
           if (i->getRestStk()<abmenge) abmenge=i->getRestStk();
           
           LE.setZusatzInfo(*i,abmenge);
           i->Produziert(abmenge,Id());
           menge-=abmenge;
    	   }
        if (!!menge)
        {  
           LE.setZusatzInfo(AufEintragBase(),menge);
        }
   }
   tr.commit();
}

void Lieferschein::push_back(AufEintrag &aufeintrag,
		const ArtikelBase &artikel, int anzahl, 
		mengen_t menge, int palette)
{
   // Eine Instanz, die Lieferscheine schreibt, produziert auch selber (assert in ppsInstanz).
   // Wenn man einn Lieferschein schriebe, ohne daß diese Instanz 
   // selber produzierte, würde doppelt produziert werden.
   assert(Instanz()->Lieferschein());
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"AufEintrag="+aufeintrag.str(),
           "Artikel="+cH_ArtikelBezeichnung(artikel)->Bezeichnung(),
           "Anzahl="+itos(anzahl),"Menge=",menge,"Palette="+itos(palette));                     
 LieferscheinEntry::create(*this, aufeintrag ,artikel, anzahl,menge,palette);

 mengen_t mng;
 if(!menge) mng = anzahl;
 else mng= anzahl*menge;
 
 aufeintrag.Produziert(mng,Id());
}

void Lieferschein::aufraumen() throw(SQLerror)
{
 std::string query="update lieferschein set rngid=0 where rngid is null "
      " and lfrsid not in (select e.lfrsid from lieferscheinentry e "
      " where e.lfrsid=lieferschein.lfrsid)";
 Query::Execute(query);
 SQLerror::test(__FILELINE__,100);
}


void Lieferschein::closeLfrs()
{
 std::string query="update lieferschein set rngid=0 where rngid is null "
 	" and lfrsid = "+itos(Id());
 Query::Execute(query);
 SQLerror::test(__FILELINE__);
}


#ifdef DPD_LIEFERSCHEINE

void Lieferschein::setDPDlnr(int d) const throw(SQLerror)
{
 std::string query="update lieferschein set dpdliefnr=nullif("+itos(d)
      +","+itos(Offen)+") where (instanz,lfrsid) = ("+itos(Instanz()->Id())+","+itos(Id())+")";
 Query::Execute(query);
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setPakete(const int p) throw(SQLerror)
{  
 std::string query="update lieferschein set pakete="+itos(p)
   +" where (instanz,lfrsid) = ("+itos(Instanz()->Id())+","+itos(Id())+")";
 Query::Execute(query);
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setPaeckchen(const int p) throw(SQLerror)
{  
 std::string query="update lieferschein set paeckchen="+itos(p)
   +" where (instanz,lfrsid) = ("+itos(Instanz()->Id())+","+itos(Id())+")";
 Query::Execute(query);
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setGewichtNetto(const fixedpoint<1> i) throw(SQLerror)
{  
 std::string query="update lieferschein set netto_kg="+i.String()
   +" where (instanz,lfrsid) = ("+itos(Instanz()->Id())+","+itos(Id())+")";
 Query::Execute(query);
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setGewichtBrutto(const fixedpoint<1> i) throw(SQLerror)
{  
 std::string query="update lieferschein set brutto_kg="+i.String()
   +" where (instanz,lfrsid) = ("+itos(Instanz()->Id())+","+itos(Id())+")";
 Query::Execute(query);
 SQLerror::test(__FILELINE__);
}

#endif
