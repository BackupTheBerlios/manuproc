// $Id: AufEintrag.cc,v 1.68 2003/07/04 14:33:59 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski & Christof Petig
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

#include <Auftrag/AufEintrag.h>
#include <Misc/FetchIStream.h>
#include <Misc/string0.h>
#include <Misc/Transaction.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/Auftrag.h>
#include <unistd.h>
#include <Misc/TraceNV.h>
#include <Lager/Lager.h>
#include <Auftrag/VerfuegbareMenge.h>
#include <Misc/Changejournal.h>
//#include <Lieferschein/Lieferschein.h>
#include <Auftrag/AufEintrag_macros.h>
#include <Misc/inbetween.h>
#include <Artikel/ArtikelStamm.h>
#include <Misc/relops.h>

#ifdef MABELLA_EXTENSIONS
#include <Lager/FertigWaren.h>
#include <Artikel/ArtikelBase.h>
#endif

void AufEintrag::setLetztePlanungFuer(cH_ppsInstanz planinstanz) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  AufEintragBase::setLetztePlanungFuer(planinstanz);
  letztePlanInstanz=planinstanz->Id();
}

#if 0
void AufEintrag::setVerarbeitung(const cH_Prozess p)
{
 AufEintragBase::setVerarbeitung(p);

 prozess=p;
 prozdate=ManuProC::Datum().today();
}
#endif


void AufEintrag::setLetzteLieferung(const ManuProC::Datum &datum) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 AufEintragBase::setLetzteLieferung(datum);
 letzte_lieferung=datum;
}


std::string AufEintrag::Planung() const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  int tiefe = ArtikelBaum(Artikel()).Tiefe();
  return itos(maxPlanInstanz)+"/"+itos(tiefe);  
}

// reason ist wichtig, da r_Produziert einen bereits geschlossenen Auftrag erzeugt
void AufEintrag::move_to(int uid,AufEintrag ziel,AuftragBase::mengen_t menge,ManuProC::Auftrag::Action reason) throw(std::exception)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("To",ziel),NV("Menge",menge),NV("Reason",reason));
  Transaction tr;

  assert(reason==ManuProC::Auftrag::r_Planen);
  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*this,
		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
  for(AufEintragZu::list_t::reverse_iterator i=L.rbegin();i!=L.rend();++i)
  { AuftragBase::mengen_t M=AuftragBase::min(i->Menge,menge);
    if (!M) continue;

    MengeAendern(uid,-M,true,i->AEB,reason);
    ziel.MengeAendern(uid,M,true,i->AEB,reason);

    menge-=M;
    if(!menge) break;
  }
  tr.commit();
}      

AufEintragBase AufEintrag::getFirstKundenAuftrag() const
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 std::vector<AufEintragBase> V=AufEintragZu(*this).getKundenAuftragV();
 if(V.empty()) return *this;
 else return *(V.begin());
}

void AufEintrag::Produziert(mengen_t menge,
   ManuProcEntity<>::ID lfrsid) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("Menge",menge));
  ProduziertNG(menge);

#ifdef MABELLA_EXTENSIONS // Lager updaten
#warning Jacek: Das muss raus, sobald es mehrstufig ist
 if(Instanz() == ppsInstanzID::Kundenauftraege)
   {
    FertigWaren fw(Artikel(),(FertigWaren::enum_Aktion)'L',menge.as_int(),lfrsid);
    if(menge < 0) fw.Einlagern(1);
    else if(menge > 0) fw.Auslagern(1);
   }
#endif

}

AufEintragBase AufEintrag::Planen(int uid,mengen_t menge,const AuftragBase &zielauftrag,
   const Petig::Datum &datum,ManuProC::Auftrag::Action reason,
   AufEintragBase *verplanter_aeb,bool rekursiv) throw(std::exception)
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("Menge",menge),NV("Reason",reason),NV("Zielauftrag",zielauftrag),NV("rekursiv",rekursiv));
   assert(Id()==AuftragBase::ungeplante_id);
   assert(entrystatus==OPEN);
   assert(auftragstatus==OPEN);
   assert(menge>0);
   assert(reason==ManuProC::Auftrag::r_Planen);
   assert(!Instanz()->LagerInstanz());
   assert(!rekursiv);

// ist das überhaupt noch erforderlich?
   if(verplanter_aeb) *verplanter_aeb=*this;

   Transaction tr;
//   Auftrag ZA(zielauftrag);
//   ZA.setStatusAuftrag_(OPEN);

// wenn schon eine Zeile mit diesem Datum/Artikel/Status in diesem Auftrag
// existiert, Menge erhöhen. So kann man von Aufträge aus verschiedenen 
// Quellen zusammen fertigen

   // Zeile erzeugen/finden
   AufEintragBase neueZeile=AufEintragBase(zielauftrag,
   	zielauftrag.PassendeZeile(datum,Artikel(),entrystatus,uid));
// nix zu planen -> erledigt   
   if(menge==0) {tr.commit();   return neueZeile;}
   
  AufEintrag AE1er(neueZeile);
   // dispo (2er) Auftrag anlegen bei Überplanung
   if (reason==ManuProC::Auftrag::r_Planen && menge-getRestStk() > 0)
    { assert(!Instanz()->LagerInstanz()); // CP
      mengen_t dispomenge = menge-getRestStk();
      // nur soviel unten verwenden (tatsächlich zu uns ziehen, 
      // 	Rest wird von uns in Ueberplanen bestellt)
      menge-=dispomenge;
      
      AE1er.Ueberplanen(uid,Artikel(),dispomenge,datum);

      // Produktionsplaner (ungetestet)
      if(zielauftrag.Instanz()->GeplantVon()!=ppsInstanzID::None)
       {
	    assert(!"yet implemented");
	    // hier wurde ehemals ein 2er im Planer angelegt
	    // Code siehe CVS
       }
    }
   
    move_to(uid,AE1er,menge,reason);

//---- ProzessInstanz setzen ------    
  // nur wenn aktiv durch Benutzer geplant 
  if (ManuProC::Auftrag::r_Planen 
  	&& zielauftrag.Id() != AuftragBase::plan_auftrag_id)
  { assert(zielauftrag.Id()>=handplan_auftrag_id);
    // Kundenauftrag suchen
    AufEintragZu::list_t ReferenzAufEintrag =
			         AufEintragZu(*this).get_Referenz_listFull(false);
    for (AufEintragZu::list_t::iterator i=ReferenzAufEintrag.begin();i!=ReferenzAufEintrag.end();++i)
    {
     if(i->AEB.Instanz()!=ppsInstanzID::Kundenauftraege) continue;
     i->AEB.setLetztePlanungFuer(instanz);
     i->AEB.calculateProzessInstanz();
    }
  }

 tr.commit();
 return neueZeile;
}

#if 0
// was soll diese Funktion? sieht mir fehlerhaft aus CP
void AufEintrag::ProduktionsPlanung(int uid,mengen_t menge,const AuftragBase &zielauftrag, 
      const ManuProC::Datum &datum,cH_ppsInstanz instanz) throw(std::exception)
{
//   Transaction tr;
   assert(!"AufEintrag::ProduktionsPlanung called");
#if 0
   assert(Id()==AuftragBase::ungeplante_id);
   AufEintragBase newAEB;
   Planen(uid,menge,zielauftrag,datum,ManuProC::Auftrag::r_Planen,&newAEB);
   AuftragBase zielauftrag_instanz(instanz,AuftragBase::ungeplante_id);
   //int znr=
   zielauftrag_instanz.BestellmengeAendern(menge,datum,Artikel(),OPEN,uid,newAEB);
//   tr.commit();
#endif
}
#endif

// einen Dispo Auftrag für einen Auftrag anlegen (wegen freier Menge)
void AufEintrag::Ueberplanen(int uid,const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,str(),
   NV("Artikel",artikel),NV("Menge",menge),NV("Datum",datum));
   // automatisch geplant ??
   assert(Id()!=plan_auftrag_id);

   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
   // warum dispoplanung? was tut das letzte true?
   st_BestellmengeAendern st(false,false,true);
   //int znr=
   dispoAB.BestellmengeAendern(menge,datum,artikel,OPEN,uid,*this,st);

   // zusätzliche Menge vermerken und Material bestellen
   MengeAendern(uid,menge,true,AufEintragBase(),ManuProC::Auftrag::r_Anlegen); // oder Planen?
}

AufEintragBase AufEintrag::unbestellteMengeProduzieren(cH_ppsInstanz instanz,
                const ArtikelBase &artikel,mengen_t menge,unsigned uid,bool rekursiv,
                const AufEintragBase &elter)
{  // Code wie in ProduziertNG
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,instanz,
			   NV("artikel",artikel),NV("menge",menge));
   assert(instanz!=ppsInstanzID::Kundenauftraege && instanz!=ppsInstanzID::None);
   Transaction tr;
   AuftragBase zielauftrag(instanz,AuftragBase::plan_auftrag_id);
   AufEintragBase neuerAEB(zielauftrag,   
                       zielauftrag.PassendeZeile(ManuProC::Datum(1,1,1970),artikel,CLOSED,getuid()));
   AufEintrag ae(neuerAEB);
   // elter kann nicht übergeben werden, da sonst bereits mit Menge angelegt
   ae.MengeAendern(uid,menge,false,AufEintragBase(),ManuProC::Auftrag::r_Produziert);
   ae.abschreiben(menge);
   if (elter.valid()) AufEintragZu(elter).Neu(ae,0);
   if (rekursiv)
   {  // NaechsteInstanz geht nicht wegen static (nächste wovon)
      if (instanz->LagerInstanz()) 
      {  cH_ppsInstanz pi=ppsInstanz::getProduktionsInstanz(artikel);
         if (pi!=ppsInstanzID::None && !pi->ProduziertSelbst())
            unbestellteMengeProduzieren(pi,artikel,menge,uid,true,neuerAEB);
      }
      else
      {  ArtikelBaum AB(artikel);
         for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
         {  cH_ppsInstanz bi=ppsInstanz::getBestellInstanz(i->rohartikel);
            if (bi!=ppsInstanzID::None && !bi->ProduziertSelbst())
               unbestellteMengeProduzieren(bi,i->rohartikel,i->menge*menge,
               		uid,true,neuerAEB);
         }
      }
   }
   tr.commit();
   return neuerAEB;
}

namespace { class MichProduzieren
{	AufEintrag &mythis;
	unsigned uid;
public:	
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  if (elter.Id()==AuftragBase::dispo_auftrag_id) return 0;
	   mythis.ProduziertNG(uid,m,elter,elter);
	   return m;
	}
	MichProduzieren(AufEintrag &_mythis) : mythis(_mythis), uid(getuid()) {}
};}

namespace { class AbbestellenUndVormerken
{	AufEintrag &mythis;
	unsigned uid;
public:	
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  mythis.MengeAendern(uid,-m,true,elter,ManuProC::Auftrag::r_Closed);
	   AuftragBase zielauftrag(mythis.Instanz(),AuftragBase::plan_auftrag_id);
	   AufEintragBase neuerAEB(zielauftrag,
                       zielauftrag.PassendeZeile(mythis.getLieferdatum(),
                       			mythis.Artikel(),OPEN,uid));
	   AufEintrag ae(neuerAEB);
	   ae.MengeAendern(uid,m,false,elter,ManuProC::Auftrag::r_Closed);
	   return m;
	}
	AbbestellenUndVormerken(AufEintrag &_mythis) : mythis(_mythis), uid(getuid()) {}
};}

namespace { struct Auslagern_cb
{	unsigned uid;
	Auslagern_cb(unsigned u) : uid(u) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t abschreibmenge) const 
	{  if (ae.Id()==AuftragBase::plan_auftrag_id)
	      ae.abschreiben(abschreibmenge);
	   else
	   {  assert(ae.Id()==AuftragBase::dispo_auftrag_id);
	      abschreibmenge=-ae.updateStkDiffBase__(uid,-abschreibmenge);
	      // als produziert markieren!
	      AufEintrag::unbestellteMengeProduzieren(ae.Instanz(),ae.Artikel(),
	        			abschreibmenge,uid);
	   }
	   return abschreibmenge;
	}
};}

AuftragBase::mengen_t AufEintrag::Auslagern
	(const AuftragBase &ab,const ArtikelBase &artikel,mengen_t menge, unsigned uid, bool fuer_auftraege)
{  assert(ab.Instanz()->LagerInstanz());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,ab,
				   NV("artikel",artikel),NV("menge",menge));
   return
    auf_positionen_verteilen(SQLFullAuftragSelector(
	  		SQLFullAuftragSelector::sel_Artikel_Planung_id
  			(ab.Instanz()->Id(),Kunde::eigene_id,artikel,ab.Id(),
  			 menge<0?CLOSED:OPEN)),
  		menge,Auslagern_cb(uid));
     //if(abmenge<0) 
     //   abschreibmenge=-AuftragBase::min(-abmenge,i->getGeliefert());
}

namespace { class Einlagern_cb
{	bool abbestellen;
public:
	Einlagern_cb(bool abbest=false) : abbestellen(abbest) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t m) const 
	{  AuftragBase::mengen_t rest;
	   if (!abbestellen)
	      rest=distribute_parents(ae,m,MichProduzieren(ae));
	   else
	      rest=distribute_parents(ae,m,AbbestellenUndVormerken(ae));
           return m-rest;
	}
};}

void AufEintrag::MengeVormerken(cH_ppsInstanz instanz,const ArtikelBase &artikel,
		mengen_t menge, bool abbestellen)
{  
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("instanz",instanz),
      NV("artikel",artikel),NV("menge",menge),(abbestellen?"abbestellen":"produzieren"));
  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==0) return;
   Transaction tr;
   mengen_t m=
    auf_positionen_verteilen(SQLFullAuftragSelector(SQLFullAuftragSelector::
      			sel_Artikel_Planung_id(instanz->Id(),
      				Kunde::eigene_id,artikel,
      				AuftragBase::ungeplante_id)),
      			menge,Einlagern_cb(abbestellen));
   if (m!=0)
      AuftragBase(instanz,AuftragBase::dispo_auftrag_id).
   		BestellmengeAendern(m,LagerBase::Lagerdatum(),artikel,
	   		OPEN,getuid(),AufEintragBase());
   tr.commit();
}

// Ins Lager gekommene Menge neu vormerken
   // wird aufgerufen wenn Menge ins Lager kommt (LagerBase::rein_ins_lager)
   // und gibt reservierte Menge zurück
   // sollte Aufträge als produziert markieren
// ehemals AuftragBase::menge_neu_verplanen
void AufEintrag::Einlagern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,
         const mengen_t &menge,bool produziert,const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("Instanz",instanz),
   NV("Artikel",artikel),NV("Menge",menge),NV("Reason",reason));

  assert(reason==ManuProC::Auftrag::r_Produziert);
  MengeVormerken(instanz,artikel,menge,false);  
}

void AufEintrag::WiederEinlagern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,
         mengen_t menge,const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("Instanz",instanz),
   NV("Artikel",artikel),NV("Menge",menge),NV("Reason",reason));

  assert(reason==ManuProC::Auftrag::r_Produziert);
  // eigentlich sind nur die 1er interessant, dann aber OPEN wie CLOSED ...
  SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::
      			sel_Artikel(instanz->Id(),artikel)));
  SelectedFullAufList auftraglist(sel);
  for (SelectedFullAufList::reverse_iterator i=auftraglist.rbegin();i!=auftraglist.rend();++i)
  {  AuftragBase::mengen_t M=AuftragBase::min(i->getGeliefert(),menge);
     if (!M) continue;
                 
     i->ProduziertNG(-M);
                      
     menge-=M;
     if(!menge) break;
  }
  assert(!menge); // eigentlich darf das nicht passieren, da es ja vorher produziert wurde ...
}

// Menge wurde geliefert. 
// Um die Pfeile über uns muss sich jemand anderes kümmern

// menge kann negativ sein ...
// wo wird der 2er reduziert?
// wo werden die Instanzen darunter behandelt?
// ==> WurdeProduziert macht das ganze rekusiv
void AufEintrag::abschreiben(mengen_t menge) throw(SQLerror)
{int uid=getuid();
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("this",*this),NV("Menge",menge));

  mengen_t GELIEFERT=getGeliefert()+menge;
  mengen_t BESTELLT=getStueck();
  AufStatVal STATUS=getEntryStatus();

 if(menge>=0 && getCombinedStatus()!=(AufStatVal)OPEN) 
    throw(SQLerror(__FILELINE__,-1,"Auftragszeile ist nicht offen sondern "+itos(STATUS)));
 if(menge<0 && getCombinedStatus()==(AufStatVal)UNCOMMITED) 
    throw(SQLerror(__FILELINE__,-1,"Auftragszeile ist nicht bestätigt"));
 if (menge<0 && -menge>getGeliefert()) 
 {  menge=-getGeliefert(); GELIEFERT=0; }

 if (!menge) return;

 AufStatVal oldstatus=getCombinedStatus();

 if(mengen_t(GELIEFERT)>=getStueck()) STATUS=(AufStatVal)CLOSED;
 else if(menge<0 && mengen_t(GELIEFERT)<getStueck()) STATUS=(AufStatVal)OPEN;

 Transaction tr;
 Query("lock table auftragentry in exclusive mode");

#if 0
 bool delete_entry=false;

  // Lieferung rückgängig machen, dann 1er löschen wenn es alle Menge war
  // funktioniert nicht gut, da Zeiger verloren gehen,
  // außerdem werden diese Einträge bald wieder erzeugt
  if(menge<0 &&
     Instanz()!=ppsInstanzID::Kundenauftraege && 
     !Instanz()->ProduziertSelbst() && 
     Id()==AuftragBase::plan_auftrag_id)
   {
     BESTELLT+=menge.as_int();
     if(BESTELLT<0) BESTELLT=0;
     if(BESTELLT==GELIEFERT) STATUS=CLOSED;
     if(BESTELLT==0 && GELIEFERT==0 &&  STATUS==CLOSED) delete_entry=true;
   }

 if(delete_entry)
   Query("delete from auftragentry where (instanz,auftragid,zeilennr) = (?,?,?)").lvalue()
     << static_cast<const AufEintragBase&>(*this);
 else
#endif   
   Query("update auftragentry set geliefert=?, status=?, "
     "bestellt=?, letzte_lieferung=now() "
     "where (instanz,auftragid,zeilennr) = (?,?,?)").lvalue()
     << GELIEFERT << STATUS << BESTELLT 
     << static_cast<const AufEintragBase&>(*this);
 SQLerror::test(__FILELINE__);

 geliefert=GELIEFERT;
 if(geliefert>=bestellt) entrystatus=(AufStatVal)CLOSED;

 // zumindest 2er werden nie geschlossen, nur auf 0 gesetzt
 if(STATUS!=oldstatus && Id()!=dispo_auftrag_id)
     setStatus(AufStatVal(STATUS),uid,true);
 
 // eigentlich könnte das if hier weg ... geht aber schneller so
 // dispomenge auf offene Menge beschränken
 if (Instanz() != ppsInstanzID::Kundenauftraege && menge>0) 
    DispoBeschraenken(uid);

 tr.commit();
}


void AufEintrag::setStatus(AufStatVal newstatus,int uid,bool force) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("status",newstatus),force?"force":"");
 if(entrystatus == newstatus)
 {  ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,*this,NV("status bereits",newstatus));
    return;
 }
 if(newstatus == STORNO) force=true;
 if(newstatus == STORNO && getGeliefert()!=mengen_t(0))  
 {  std::cerr << "Kann nicht storniert werden da bereits geliefert\n";
    return;
 }
 if(entrystatus == CLOSED && !force )
 {  std::cerr << "Auftragszeile bereits geschlossen\n";
    return;
 }
 if(newstatus == UNCOMMITED)
 {  std::cerr << "bestätigte Zeilen können nicht wieder unbestätigt werden\n";
    return;
 }
 if(newstatus == OPEN && entrystatus != UNCOMMITED && !force)
 {  std::cerr << "nur unbestätigte Zeilen können geöffnet werden\n";
    return;
 }

 Transaction tr; 
 Query("lock auftragentry in exclusive mode");
 SQLerror::test("setStatus: lock table auftragentry");

 // InternAbbestellen
 if ((newstatus == CLOSED || newstatus == STORNO) && getRestStk()!=0)
     ArtikelInternAbbestellen(uid,getRestStk(),ManuProC::Auftrag::r_Closed);

 std::string sqlcommand = "update auftragentry set status=?";
 if(uid) sqlcommand+=", lasteditdate = now(), lastedit_uid="+itos(uid);
 sqlcommand +=" where (instanz,auftragid,zeilennr)=(?,?,?)";

 Query(sqlcommand).lvalue() << newstatus << static_cast<AufEintragBase&>(*this);
 SQLerror::test("setStatus: update auftragentry");
 AufStatVal oldentrystatus=entrystatus;
 entrystatus=newstatus;


 if(newstatus==OPEN && auftragstatus==UNCOMMITED)
 {  setStatusAuftragBase(newstatus);
    auftragstatus=newstatus;
 }

 if(newstatus == OPEN  &&  oldentrystatus==UNCOMMITED && getRestStk()!=0)
    ArtikelInternNachbestellen(uid,getRestStk(),ManuProC::Auftrag::r_Anlegen);

 if(newstatus==OPEN && bestellt!=0)
   {
    pps_ChJournalEntry::newChange(
			instanz, *this, artikel,
			bestellt.as_float(),
			bestellt.as_float(),
			pps_ChJournalEntry::CH_MENGE);
  }
 else if(newstatus==CLOSED && oldentrystatus!=UNCOMMITED
 	&& geliefert-bestellt!=0) 
 // UNCOMMITED->CLOSED => kein Eintrag
   {
    pps_ChJournalEntry::newChange(
			instanz, *this, artikel,
			geliefert.as_float(),
			(geliefert-bestellt).as_float(),
			pps_ChJournalEntry::CH_MENGE);
   }

 tr.commit(); 
}

// statische Variante (behandelt z.B. Vormerkungen, freie Menge etc.)
AufEintragBase AufEintrag::ArtikelInternNachbestellen(const cH_ppsInstanz &wo,
  mengen_t menge,const ManuProC::Datum &lieferdatum,const ArtikelBase& artikel,
  int uid,const AufEintragBase& ElternAEB)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
   	wo,menge,lieferdatum,artikel,NV("ElternAEB",ElternAEB));
   if (!menge) return AufEintragBase(); // hmmm
   if (wo==ppsInstanzID::None || wo==ppsInstanzID::Kundenauftraege)
   {  std::cerr << "ArtikelInternNachbestellen: ungültige Instanz für " << menge 
   		<< " von " << cH_ArtikelBezeichnung(artikel)->Bezeichnung() 
   		<< " Elter " << ElternAEB << '\n';
      return AufEintragBase();
   }
   assert(menge>0);
   assert(ElternAEB.valid());

   // 2er Menge (freie Menge im Lager, freie Produktionsplanung)
   // vormerken
   VerfuegbareMenge AIL(wo,artikel,lieferdatum);
   mengen_t M_dispo=AuftragBase::min(AIL.getMengeDispo(),menge);
   if(M_dispo>0) menge-=AIL.reduce_in_dispo(uid,M_dispo,ElternAEB);

   // Im Lager von späteren Aufträgen vorgemerkte Menge wegschnappen
   if (menge>0 && wo->LagerInstanz())
   {  M_dispo=AuftragBase::min(AIL.getMengePlan(),menge);
      menge-=AIL.reduce_in_plan(uid,M_dispo,ElternAEB);
   }
   // Rest nachbestellen
   AuftragBase ab(wo,ungeplante_id);
   int znr=ManuProcEntity<>::none_id;
   if (menge>0)
      znr=ab.BestellmengeAendern(menge,lieferdatum,artikel,OPEN,uid,ElternAEB);
   return AufEintragBase(ab,znr);
}

// ehemals updateStkDiffInstanz__ mit menge>0
// + ehemals BaumAnlegen
void AufEintrag::ArtikelInternNachbestellen(int uid,mengen_t menge,
	ManuProC::Auftrag::Action reason) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
   NV("menge",menge),NV("Reason",reason));
  assert(menge>0);
   // Rekursion von 2ern verbieten
   assert(Id()!=dispo_auftrag_id);

   // 1er im Lager wird erhöht ... nichts tun
   if (Instanz()->LagerInstanz() && Id()==plan_auftrag_id)
      return;
  ppsInstanz::ID next=Instanz()->NaechsteInstanz(ArtikelStamm(Artikel()));
  if (next!=ppsInstanzID::None)
  {  AufEintrag::ArtikelInternNachbestellen(next,menge,getLieferdatum(),
  			Artikel(),uid,*this);
  }
  else
  {  ManuProC::Datum newdate=getLieferdatum()-Instanz()->ProduktionsDauer();

     ArtikelBaum AB(Artikel());
     for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
     {  ArtikelInternNachbestellen(ppsInstanz::getBestellInstanz(i->rohartikel),
  		i->menge*menge,newdate,i->rohartikel,uid,*this);
     }
  }
}

// bei 2ern geht der Pfeil in die andere Richtung
AuftragBase::mengen_t AufEintrag::MengeAendern(int uid,mengen_t menge,bool instanzen,
     const AufEintragBase &ElternAEB,ManuProC::Auftrag::Action reason) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
   NV("Eltern",ElternAEB),
   NV("menge",menge),NV("instanzen",instanzen),NV("reason",reason));
 assert(reason!=ManuProC::Auftrag::r_None);
 if (!menge)  return menge;

 Transaction tr; // Beschleunigung
 Query("lock auftragentry in exclusive mode");
 SQLerror::test("updateStkDiff: lock table auftragentry");

 mengen_t menge2=menge;
 if (menge2<0) // nicht mehr abbestellen als noch offen ist
 		// aber dennoch den Pfeil reduzieren
 {  menge2=-AuftragBase::min(-menge2,getRestStk());
 }
 if (!!menge2) 
 { menge2=updateStkDiffBase__(uid,menge2);
   bestellt+=menge2;
   if (menge2>0 && entrystatus==CLOSED) 
      setStatus(OPEN,uid,true);
 }

 if (ElternAEB.valid()) 
 {  if (Id()!=dispo_auftrag_id) AufEintragZu(ElternAEB).Neu(*this,menge);
    // bei dispoaufträgen geht der Pfeil in die andere Richtung
    else AufEintragZu(*this).Neu(ElternAEB,menge);
 }

 if(auftragstatus==OPEN && instanzen && !!menge2)
  {
   // Rekursion von 0ern im Lager (es gibt keine 3er im Lager)
   //  Verplanen von freigewordener Menge bei 1er im Lager
   // Rekursion bei 0er, 1er oder 3er in Produktion
      updateStkDiffInstanz__(uid,menge2,*this,reason);
  }
  tr.commit();
  // wir haben zwar weniger abbestellt, aber nur weil wir geliefert haben
 return menge;
}

AuftragBase::mengen_t AufEintrag::ArtikelInternAbbestellen_cb::operator()
	(const ArtikelBase &i,
 		const AufEintragBase &j,AuftragBase::mengen_t M) const
{  AufEintrag AE(j);
   if (j.Id()==ungeplante_id)
      M=-AE.MengeAendern(uid,-M,true,mythis,reason);
   else if (j.Instanz()->LagerInstanz())
   {  // vorgemerkte Menge freigeben
      assert(j.Id()==plan_auftrag_id);
      M=-AE.MengeAendern(uid,-M,true,mythis,reason);
   }
   else 
     { // 1er oder 3er - dispo anlegen, Bestellpfeil erniedrigen
       assert(j.Id()!=dispo_auftrag_id);
       AuftragBase(j.Instanz(),dispo_auftrag_id)
       	.BestellmengeAendern(M,AE.getLieferdatum(),AE.Artikel(),OPEN,uid,j);
       AufEintragZu(mythis).setMengeDiff__(j,-M);
     } 
   return M;
}

void AufEintrag::ArtikelInternAbbestellen(int uid,mengen_t menge,ManuProC::Auftrag::Action reason) const
{ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
   NV("menge",menge),NV("Reason",reason));

 assert(menge>0);
 // Rekursion von 2ern verbieten
 assert(Id()!=dispo_auftrag_id);

 // Menge im Lager freigeben == Einlagern ohne Produktion? 
 if (Instanz()->LagerInstanz() && Id()==AuftragBase::plan_auftrag_id)
 {  AufEintrag::MengeVormerken(Instanz(),Artikel(),menge,true);
 }
 else try{
      distribute_children(*this,menge,Artikel(),
   			ArtikelInternAbbestellen_cb(*this,uid,reason));
  }catch(NoAEB_Error &e) 
  {std::cerr <<"Falsche Anzahl von AufEinträgen: "<<e.what()<<'\n'; 
   exit(1);}
}

void AufEintrag::updateStkDiffInstanz__(int uid,mengen_t menge,const AufEintragBase &ElternAEB,ManuProC::Auftrag::Action reason) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
   NV("ElternAEB",ElternAEB),NV("Menge",menge),NV("Reason",reason));
 if (menge<0) 
 {  assert(ElternAEB==AufEintragBase(*this)); // ?
    ArtikelInternAbbestellen(uid,-menge,reason);
 }
 else 
 {  assert(ElternAEB==AufEintragBase(*this));
    ArtikelInternNachbestellen(uid,menge,reason);
 }
}

void AufEintrag::updatePreis(const Preis &pr) throw(SQLerror)
{ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 Query("update auftragentry "
 	"set preis=?, preismenge=? "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< pr.Wert() << pr.PreisMenge() 
 	<< static_cast<const AufEintragBase&>(*this);
 SQLerror::test("updatePreis: update preis in auftragentry");
 preis=pr;
}



void AufEintrag::updateRabatt(rabatt_t rb) throw(SQLerror)
{ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 Query("update auftragentry "
 	"set rabatt=? "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< rb 
 	<< static_cast<const AufEintragBase&>(*this);
 SQLerror::test("updateRabatt: update rabatt in auftragentry");
 rabatt=rb;
}


void AufEintrag::updateLieferdatum(const Petig::Datum &ld,int uid) throw(SQLerror)
{ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("Datum",ld));
 Transaction tr;
 Query("lock auftragentry in exclusive mode"); // unnötig? CP
 SQLerror::test("updateLieferdatum: lock table auftragentry");

 ArtikelInternAbbestellen(uid,getStueck(),ManuProC::Auftrag::r_Anlegen);

 Query("update auftragentry "
 	"set lieferdate=? "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< ld
 	<< static_cast<const AufEintragBase&>(*this);
 SQLerror::test("updateLiefDatum: update lieferdate in auftragentry");
 lieferdatum=ld;

 ArtikelInternNachbestellen(uid,getStueck(),ManuProC::Auftrag::r_Anlegen);

 if(getCombinedStatus()==OPEN)// status->entrystatus
  {
   try 
    {
     pps_ChJournalEntry::newChange(
  			instanz, *this,
  			artikel, ld,
  			(double)(ld-getLieferdatum()),
  			pps_ChJournalEntry::CH_LIEFDAT);
    }
   catch(SQLerror &e)
     {tr.rollback(); throw; }
  }
 
 tr.commit(); 
}

int AufEintrag::split(int uid,mengen_t newmenge, const Petig::Datum &newld,bool dispoplanung) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("NewMenge",newmenge),NV("NewDatum",newld),NV("dispoplanung(bool)",dispoplanung));
 if(getCombinedStatus()==CLOSED) return none_znr;

 mengen_t BESTELLT_OLD=bestellt-newmenge;
 int STATUS=entrystatus;

 double GELIEFERT;
 double GELIEFERT_OLD;

 if(lieferdatum>newld)
   {GELIEFERT=(geliefert>newmenge ? mengen_t(newmenge) : geliefert).as_float();
    GELIEFERT_OLD=(geliefert>newmenge ? geliefert-mengen_t(newmenge) : mengen_t(0)).as_float();}
 else
   {GELIEFERT_OLD=(geliefert>BESTELLT_OLD ? mengen_t(BESTELLT_OLD) : geliefert).as_float();
    GELIEFERT=(geliefert>BESTELLT_OLD ? geliefert-mengen_t(BESTELLT_OLD) : mengen_t(0)).as_float();}
    
 Transaction tr;
 Query("lock auftragentry in exclusive mode");
 SQLerror::test("split: lock table auftragentry");

 mengen_t mt=MengeAendern(uid,-newmenge,true,AufEintragBase(),ManuProC::Auftrag::r_Anlegen);
 assert(mt==-newmenge);

 int ZEILENNR;
 if(Instanz()==ppsInstanzID::Kundenauftraege)
   {Auftrag A(*this);
    AufEintragBase newaeb=A.push_back(newmenge,newld,artikel,entrystatus,uid,true,preis,rabatt);
    ZEILENNR=newaeb.ZNr();
   }
 else
   ZEILENNR=split_zuordnungen_to(newmenge,newld,artikel,entrystatus,uid,dispoplanung);

 if(STATUS==OPEN)
   {   pps_ChJournalEntry::newChange(
    			instanz, *this,
    			artikel, BESTELLT_OLD.as_float(), (BESTELLT_OLD-bestellt).as_float(),
    			pps_ChJournalEntry::CH_MENGE);
       pps_ChJournalEntry::newChange(
    			instanz, *this,
    			artikel, newmenge.as_float(), newmenge.as_float(),
    			pps_ChJournalEntry::CH_MENGE);
   }

 tr.commit();
 return ZEILENNR;
}

#if 0
// hässlich ! warum denn ein cH_Lieferschein statt eines LieferscheinEntryBase?
cH_Lieferschein AufEintrag::getLieferschein() const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  int LFRSID,LFRZNR;
 // bislang cH_Lieferschein(ppsInstanzID::None,0); wenn keine Zeile
 // jetzt SQLerror
 Query("select lfrsid,zeile from lieferscheinentry "
 	"where (instanz,refauftragid,refzeilennr)=(?,?,?) "
 	"limit 1").lvalue()
 	<< static_cast<const AufEintragBase&>(*this)
 	>> LFRSID >> LFRZNR;
 // kann es mehr als eine Zeile geben? kann es, außerdem fehlt hier ZusatzInfo
 return cH_Lieferschein(Instanz(),LFRSID);
}
#endif

void AufEintrag::ProduziertNG(AuftragBase::mengen_t M)
{  if (Id()>=handplan_auftrag_id || Id()==plan_auftrag_id)
   {  ProduziertNG(getuid(),M,AufEintragBase(),AufEintragBase());
   }
   else
   {  assert(M>=0);
      if (distribute_parents(*this,M,MichProduzieren(*this))!=0)
   	 assert(!"Rest geblieben");
   }
}

namespace {
class ProduziertNG_cb2
{  unsigned uid;
   AufEintragBase alterAEB,neuerAEB;
public:
	ProduziertNG_cb2(unsigned _uid, const AufEintragBase &aAEB, const AufEintragBase &nAEB)
		: uid(_uid), alterAEB(aAEB), neuerAEB(nAEB) {}
	AuftragBase::mengen_t operator()(const ArtikelBase &art,
		const AufEintragBase &aeb,AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("aeb",aeb),NV("M",M));
	   if (!aeb.Instanz()->ProduziertSelbst())
              AufEintrag(aeb).ProduziertNG(uid,M,alterAEB,neuerAEB);
           else 
           {  AufEintragZu(alterAEB).setMengeDiff__(aeb,-M);
              AufEintrag(aeb).AnElternMengeAnpassen();
           }
           return M;
	}
	
	// Überproduktion
	void operator()(const ArtikelBase &art,AuftragBase::mengen_t M) const
	{  assert(M>0); // NaechsteInstanz?
	   cH_ppsInstanz wo=ppsInstanz::getBestellInstanz(art);
	   if (wo==neuerAEB.Instanz()) wo=ppsInstanz::getProduktionsInstanz(art);
	   assert(wo!=neuerAEB.Instanz());
	   AufEintrag::unbestellteMengeProduzieren(wo,art,M,uid,true,neuerAEB);
	}
};
}

namespace {
class ProduziertRueckgaengig2
{  unsigned uid;
   AufEintrag alterAEB;
public:
	ProduziertRueckgaengig2(unsigned _uid, const AufEintrag &aAEB)
		: uid(_uid), alterAEB(aAEB) {}
	AuftragBase::mengen_t operator()(const ArtikelBase &art,
		const AufEintragBase &aeb,AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("aeb",aeb),NV("M",M));
	   if (aeb.Instanz()->ProduziertSelbst())
           {  AufEintragZu(alterAEB).setMengeDiff__(aeb,-M);
              AufEintrag(aeb).AnElternMengeAnpassen();
           }
           return M;
	}
	
	// Überproduktion
	void operator()(const ArtikelBase &art,AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("M",M));
	   //assert(!"needed");
	   ArtikelStamm as(art);
	   cH_ppsInstanz next=alterAEB.Instanz()->NaechsteInstanz(as);
	   if (next==ppsInstanzID::None)
	      next=ppsInstanz::getBestellInstanz(as);
	   if (alterAEB.Id()==AuftragBase::ungeplante_id && next->ProduziertSelbst())
	   {  AufEintrag::ArtikelInternNachbestellen(next,-M,
	   		alterAEB.getLieferdatum()
	   		  -alterAEB.Instanz()->ProduktionsDauer(),
	   		art,uid,alterAEB);
	   }
	   else std::cout << "ProduziertRueckgaengig2: Überproduktion " << M 
	   	<< " von "<< art.Id() <<  '\n';
	}
};
}

// similar to move_to
// Produktion im Lager bedeutet:
// Einlagern==MengeVormerken   Auslagern==Produktion
void AufEintrag::ProduziertNG(unsigned uid, AuftragBase::mengen_t M,
		const AufEintragBase &elter_alt,
		const AufEintragBase &elter_neu)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,M,elter_alt,elter_neu);
//   assert(M>=0); // notwendig?
   assert(Id()!=dispo_auftrag_id);
   
   AufEintragBase neuerAEB=*this;
   if ((Id()==plan_auftrag_id && M>0)
	|| Id()>=handplan_auftrag_id)
   {  assert(!Instanz()->LagerInstanz());
      // Überproduktion wird einfach vermerkt (geht nur bei 3ern)
      if (M<0) assert(-M<=getGeliefert());
     try
     {abschreiben(M); // M<0 ? -M : M);
      if (elter_alt.valid()) 
      {  mengen_t zmenge=AufEintragZu(elter_alt).getMenge(*this);
         if (M<0)
         {  assert(elter_neu.valid());
            AufEintragZu(elter_neu).setMengeDiff__(*this,M);
         }
         else if (!!zmenge)
            AufEintragZu(elter_alt).setMengeDiff__(*this,-AuftragBase::min(zmenge,M));
         else
            std::cout << "ProduziertNG: kann von " << elter_alt << " nach " << *this 
            	<< " keine " << M << " abziehen\n";
      }
      if (M>0 && elter_neu!=elter_alt) AufEintragZu(elter_neu).Neu(*this,0);
     } catch (SQLerror &e)
     {  if (tolerate_inconsistency) return;
        throw;
     }
   }
   else
   {  if (M<0) assert(Id()==plan_auftrag_id);
      if (M<0) /* && !Instanz()->LagerInstanz())*/ abschreiben(M);
      // Rekursion bedeutet hier: freigewordene Menge neu verplanen
      bool rek=Instanz()->LagerInstanz() && M<0;
      MengeAendern(uid,M<0 ? M : -M,rek,M>0 ? elter_alt : AufEintragBase(),
      				ManuProC::Auftrag::r_Produziert);
      if (M<0 && !getRestStk()) setStatus(AufStatVal(CLOSED),uid,true);
      if (M>0 || !Instanz()->LagerInstanz())
      {  AuftragBase zielauftrag(Instanz(),M>=0?plan_auftrag_id:ungeplante_id);
         AufStatVal st=(Instanz()->LagerInstanz() || M<0) ? OPEN : CLOSED;
         neuerAEB=AufEintragBase(zielauftrag,
         		zielauftrag.PassendeZeile(getLieferdatum(),Artikel(),st,uid));
         AufEintrag ae(neuerAEB);
         ae.MengeAendern(uid,M<0 ? -M : M,false,
         		M<0 || Instanz()->LagerInstanz()?elter_neu:AufEintragBase(),
         		ManuProC::Auftrag::r_Produziert);
         if (M>0 && !Instanz()->LagerInstanz())
         {  ae.abschreiben(M);
            AufEintragZu(elter_neu).Neu(neuerAEB,0);
         }
      }
   }
   // Kinder bearbeiten
   distribute_children(*this,M,Artikel(),ProduziertNG_cb2(uid,*this,neuerAEB));
   // bei ProduziertSelbst hilft obiges nicht allein (keine Pfeile nach unten)
   if (M<0)
   {  AufEintrag neuerAE=neuerAEB;
      distribute_children(neuerAE,M,Artikel(),ProduziertRueckgaengig2(uid,neuerAE));
   }

   cH_ppsInstanz EI=Instanz()->EinlagernIn();
   if(EI->AutomatischEinlagern())
   {  assert(Instanz()->ProduziertSelbst()); // sonst Endlosrekursion
      LagerBase(EI).rein_ins_lager(Artikel(),M,uid,true);
   }
}

// wird bislang von menge_neu_verplanen für 0er aufgerufen
// vermutlich ehemals abschreiben_oder_reduzieren 
// & ehemals ppsInstanz::Produziert
void AufEintrag::WurdeProduziert(AuftragBase::mengen_t M,const AufEintragBase &ElternAEB)
{  unsigned uid=getuid();
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,M,NV("Eltern",ElternAEB));
//   assert(M>=0);

   ProduziertNG(uid,M,ElternAEB,ElternAEB);
}

#if 0
void AufEintrag::ProduktionRueckgaengig(AuftragBase::mengen_t M)
{
}
#endif

// auf noch offene Menge beschränken
// auf f.o. umstellen geht nicht sinnvoll, da die Funktion ein Unikat ist
void AufEintrag::DispoBeschraenken(int uid)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*this,AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);

  AuftragBase::mengen_t S=0;
  for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
   {
     if(i->AEB.Id()==AuftragBase::dispo_auftrag_id) S+=i->Menge;
   }
  if (getRestStk()<S)
  {   ManuProC::Trace(AuftragBase::trace_channel, __FILELINE__,
  		"Dispo muss um ",S-getRestStk()," erniedrigt werden");
      for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
      {
        if(i->AEB.Id()==AuftragBase::dispo_auftrag_id)
         {
           AuftragBase::mengen_t M=AuftragBase::min(i->Menge,S-getRestStk());
           // warum nicht MengeAendern?
           M=AufEintrag(i->AEB).MengeAendern(uid,-M,false,*this,ManuProC::Auftrag::r_Produziert);
           S-=M;
         } 
      }  
  }
}

#if 0 // wahrscheinlich nicht mehr erforderlich
// gibt negative Zahl zurück und erwartet solche
AuftragBase::mengen_t AufEintrag::ProdRueckgaengigMenge(mengen_t max_neg) const
{  AufEintragZu::list_t Eltern =
	AufEintragZu::get_Referenz_list(*this,AufEintragZu::list_eltern,
	                                AufEintragZu::list_ohneArtikel);
   mengen_t res=getStueck();
   for (AufEintragZu::list_t::iterator i=Eltern.begin();i!=Eltern.end();++i)
      res-=i->Menge;
   return -AuftragBase::min(res,-max_neg);
}
#endif

AuftragBase::mengen_t AufEintrag::getRestStk() const
{  if (in(entrystatus,CLOSED,STORNO)) return 0;
   if (in(auftragstatus,CLOSED,STORNO)) return 0;
   return bestellt-geliefert;
}

fixedpoint<2> AufEintrag::ProvSatz() const throw(SQLerror)
{
 if(provsatz!=-1) return provsatz;

 Query("select provsatz from auftragentry where"
	" (instanz,auftragid,zeilennr) = (?,?,?)")
 	<< InstanzID() << Id() << getZnr() 
 	>> FetchIStream::MapNull(provsatz);

 return provsatz;
}

// angepasste Variante von ppsInstanzReparatur::Eltern
void AufEintrag::AnElternMengeAnpassen()
{  AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(*this,
		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
   unsigned uid=getuid();
   assert(Id()!=dispo_auftrag_id);
   assert(Instanz()!=ppsInstanzID::Kundenauftraege);
   mengen_t menge,menge2;
   for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();++i)
   {  menge+=i->Menge;
      if (i->AEB.Id()==dispo_auftrag_id) menge2+=i->Menge;
   }
   if (menge>getStueck())
   // mehr v.o. benötigt als jemals bestellt (z.B. durch ProdRückgängig)
   {  assert(in(Id(),ungeplante_id,plan_auftrag_id));
      assert(Id()!=plan_auftrag_id || Instanz()->LagerInstanz());
      //assert(Id()==ungeplante_id); // sonst Problem !
      MengeAendern(uid,menge-getStueck(),true,AufEintragBase(),
                      ManuProC::Auftrag::r_Reparatur);
   }
   else if (menge<getRestStk())
   // mehr offen als nun v.o. benötigt
   {  assert(in(Id(),ungeplante_id,plan_auftrag_id));
      assert(Id()!=plan_auftrag_id || Instanz()->LagerInstanz());
      MengeAendern(uid,menge-getRestStk(),true,AufEintragBase(),
         	ManuProC::Auftrag::r_Reparatur);
   }
}

void AufEintrag::setProvSatz(const fixedpoint<2> ps) throw(SQLerror)
{
 Query q("update auftragentry set provsatz=? where"
	" (instanz,auftragid,zeilennr) = (?,?,?)");

 q << ps << InstanzID() << Id() << getZnr();
 SQLerror::test(__FILELINE__);
 provsatz=ps;
}


void AufEintrag::setDefaultProvSatz() throw(SQLerror)
{
 Query q("update auftragentry set provsatz="
	"(select case when provsatznr=1 then provsatz1 else provsatz2 end"
	" from auftragentry e join auftrag a on (e.instanz=a.instanz"
	" and e.auftragid=a.auftragid and "
	" (a.instanz,a.auftragid,e.zeilennr)=(?,?,?))"
	" join prov_config c using (artikelid) join"
	" prov_verkaufer v on (v.kundennr=a.kundennr and v.verknr=a.verknr))"
	" where (e.instanz,e.auftragid,e.zeilennr)=(?,?,?)");

 q << InstanzID() << Id() << getZnr() << InstanzID() << Id() << getZnr();

 SQLerror::test(__FILELINE__);

// warum kein Aufruf von AufEintrag::ProvSatz() ? CP
 Query("select provsatz from auftragentry where "
	"(instanz,auftragid,zeilennr)=(?,?,?)")
	<< InstanzID() << Id() << getZnr()
 	>> FetchIStream::MapNull(provsatz);

 SQLerror::test(__FILELINE__);
}

