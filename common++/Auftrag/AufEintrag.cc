// $Id: AufEintrag.cc,v 1.87 2003/07/22 08:32:59 christof Exp $
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
#include <Auftrag/AufEintrag_loops.h>
#include <Misc/inbetween.h>
#include <Artikel/ArtikelStamm.h>
#include <Misc/relops.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>

#ifdef MABELLA_EXTENSIONS
#include <Lager/FertigWarenLager.h>
#include <Artikel/ArtikelBase.h>
#endif

void AufEintrag::setLetztePlanungFuer(cH_ppsInstanz planinstanz) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__);
  AufEintragBase::setLetztePlanungFuer(planinstanz);
  letztePlanInstanz=planinstanz->Id();
}

void AufEintrag::setLetzteLieferung(const ManuProC::Datum &datum) throw(SQLerror)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__);
 AufEintragBase::setLetzteLieferung(datum);
 letzte_lieferung=datum;
}


std::string AufEintrag::Planung() const
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__);
  int tiefe = ArtikelBaum(Artikel()).Tiefe();
  return itos(maxPlanInstanz)+"/"+itos(tiefe);
}

// reason ist wichtig, da r_Produziert einen bereits geschlossenen Auftrag erzeugt
void AufEintrag::move_to(int uid,AufEintrag ziel,mengen_t menge,ManuProC::Auftrag::Action reason) throw(std::exception)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,NV("To",ziel),NV("Menge",menge),NV("Reason",reason));
  Transaction tr;

  assert(reason==ManuProC::Auftrag::r_Planen);
  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*this,
		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
  for(AufEintragZu::list_t::reverse_iterator i=L.rbegin();i!=L.rend();++i)
  { mengen_t M=min(i->Menge,menge);
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
 ManuProC::Trace _t(trace_channel, __FUNCTION__);
 std::vector<AufEintragBase> V=AufEintragZu(*this).getKundenAuftragV();
 if(V.empty()) return *this;
 else return *(V.begin());
}

AufEintragBase AufEintrag::Planen(int uid,mengen_t menge,const AuftragBase &zielauftrag,
   const Petig::Datum &datum,ManuProC::Auftrag::Action reason,
   AufEintragBase *verplanter_aeb,bool rekursiv) throw(std::exception)
{
   ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Menge",menge),NV("Reason",reason),NV("Zielauftrag",zielauftrag),NV("rekursiv",rekursiv));
   assert(Id()==ungeplante_id);
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
  	&& zielauftrag.Id() != plan_auftrag_id)
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

// einen Dispo Auftrag für einen Auftrag anlegen (wegen freier Menge)
void AufEintrag::Ueberplanen(int uid,const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,str(),
   NV("Artikel",artikel),NV("Menge",menge),NV("Datum",datum));
   // automatisch geplant ??
   assert(Id()!=plan_auftrag_id);

   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
   //int znr=
   dispoAB.BestellmengeAendern(menge,datum,artikel,OPEN,uid,*this);

   // zusätzliche Menge vermerken und Material bestellen
   MengeAendern(uid,menge,true,AufEintragBase(),ManuProC::Auftrag::r_Anlegen); // oder Planen?
}

void AufEintrag::setStatus(AufStatVal newstatus,int uid,bool force) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("status",newstatus),force?"force":"");
 if(entrystatus == newstatus)
 {  ManuProC::Trace(trace_channel,__FILELINE__,*this,NV("status bereits",newstatus));
    return;
 }
 if(newstatus == STORNO) force=true;
 if(newstatus == STORNO && getGeliefert()!=0)
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

void AufEintrag::updatePreis(const Preis &pr) throw(SQLerror)
{ManuProC::Trace _t(trace_channel, __FUNCTION__);
 Query("update auftragentry "
 	"set preis=?, preismenge=? "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< pr.Wert() << pr.PreisMenge()
 	<< static_cast<const AufEintragBase&>(*this);
 SQLerror::test("updatePreis: update preis in auftragentry");
 preis=pr;
}



void AufEintrag::updateRabatt(rabatt_t rb) throw(SQLerror)
{ManuProC::Trace _t(trace_channel, __FUNCTION__);
 Query("update auftragentry "
 	"set rabatt=? "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< rb
 	<< static_cast<const AufEintragBase&>(*this);
 SQLerror::test("updateRabatt: update rabatt in auftragentry");
 rabatt=rb;
}


void AufEintrag::updateLieferdatum(const Petig::Datum &ld,int uid) throw(SQLerror)
{ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Datum",ld));
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
 ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("NewMenge",newmenge),NV("NewDatum",newld),NV("dispoplanung(bool)",dispoplanung));
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
 	>> FetchIStream::MapNull(provsatz,0);

 return provsatz;
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
 	>> FetchIStream::MapNull(provsatz,0);

 SQLerror::test(__FILELINE__);
}

