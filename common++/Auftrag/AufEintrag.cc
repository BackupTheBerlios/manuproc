// $Id: AufEintrag.cc,v 1.109 2004/05/13 14:44:08 christof Exp $
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
#include <Auftrag/AufEintrag_delayedreclaim.h>
#include <Misc/TemporaryValue.h>

#ifdef MABELLA_EXTENSIONS
//#include <Lager/FertigWarenLager.h>
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

class move_to_Move : public distribute_parents_cb
{	AufEintrag &quelle,&ziel;
public:
	move_to_Move(AufEintrag &q,AufEintrag &z) : quelle(q), ziel(z) {}
	virtual AuftragBase::mengen_t operator()(const AufEintragBase &,
				AuftragBase::mengen_t) const;
	virtual bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return AufEintragZu_sort::auftr_34012(a,b); }
};

AuftragBase::mengen_t move_to_Move::operator()(const AufEintragBase &ae,
				AuftragBase::mengen_t M) const
{  quelle.MengeAendern(-M,true,ae);
   ziel.MengeAendern(M,true,ae);
   return M;
}

void AufEintrag::move_to(AufEintrag ziel,mengen_t menge) throw(std::exception)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,NV("To",ziel),NV("Menge",menge));
  Transaction tr;
  {delayed_reclaim dlr;
   distribute_parents(*this,menge,move_to_Move(*this,ziel));
  }
  tr.commit();
}

AufEintragBase AufEintrag::getFirstKundenAuftrag() const
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__);
 std::vector<AufEintragBase> V=AufEintragZu(*this).getKundenAuftragV();
 if(V.empty()) return *this;
 return *(V.begin());
}

void AufEintrag::Planen(mengen_t menge, AufEintrag &ziel) throw(std::exception)
{
   // dispo (2er) Auftrag anlegen bei Überplanung
   assert(Artikel()==ziel.Artikel());
   assert(getLieferdatum()>=ziel.getLieferdatum());
   assert(Instanz()==ziel.Instanz() 
   	|| (Instanz()->alt_group_nr!=ppsInstanzID::None 
   	    && Instanz()->alt_group_nr==ziel.Instanz()->alt_group_nr);
   mengen_t dispomenge;
   
   if (menge>getRestStk())
    { assert(!Instanz()->LagerInstanz());

      // nur soviel unten verwenden (tatsächlich zu uns ziehen,
      // 	Rest wird von uns in Ueberplanen bestellt)
      dispomenge = menge-getRestStk();
      menge-=dispomenge;
    }

  {TemporaryValue<bool> tmp(lager_bevorzugt_freigeben,ziel.getLieferdatum()<=getLieferdatum());
ManuProC::Trace(trace_channel,__FILELINE__,NV("lager_bevorzugt_freigeben",lager_bevorzugt_freigeben),
	NV("zieldatum",ziel.getLieferdatum()),NV("Lieferdatum",getLieferdatum()));
  
   move_to(ziel,menge);
  }

   if (!!dispomenge)
      ziel.Ueberplanen(Artikel(),dispomenge,ziel.getLieferdatum());
}

void AufEintrag::AlsGeplantMarkieren() const
{  assert(Id()>=handplan_auftrag_id);
//---- ProzessInstanz setzen ------
    // Kundenauftrag suchen
#warning eigentlich sollte das über Verfügbarkeit.cc gehen
    AufEintragZu::list_t ReferenzAufEintrag =
			         AufEintragZu(*this).get_Referenz_listFull(false);
    for (AufEintragZu::list_t::iterator i=ReferenzAufEintrag.begin();i!=ReferenzAufEintrag.end();++i)
    {
     if(i->AEB.Instanz()!=ppsInstanzID::Kundenauftraege) continue;
     i->AEB.setLetztePlanungFuer(Instanz());
     i->AEB.calculateProzessInstanz();
    }
}

AufEintragBase AufEintrag::Planen(mengen_t menge,const AuftragBase &zielauftrag,
   const Petig::Datum &datum) throw(std::exception)
{
   ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Menge",menge)); 
   assert(Id()==ungeplante_id);
   assert(entrystatus==OPEN);
   assert(auftragstatus==OPEN);
   assert(menge>0);
   assert(!Instanz()->LagerInstanz());

   Transaction tr;

// wenn schon eine Zeile mit diesem Datum/Artikel/Status in diesem Auftrag
// existiert, Menge erhöhen. So kann man von Aufträge aus verschiedenen
// Quellen zusammen fertigen

   // Zeile erzeugen/finden
   AufEintragBase neueZeile=AufEintragBase(zielauftrag,
   	zielauftrag.PassendeZeile(datum,Artikel(),entrystatus));
// nix zu planen -> erledigt
   if(menge==0) {tr.commit();   return neueZeile;}

  AufEintrag AE1er(neueZeile);
  Planen(menge,AE1er);
   
  // nur wenn aktiv durch Benutzer geplant
  if (zielauftrag.Id() != plan_auftrag_id)
  { AE1er.AlsGeplantMarkieren();
  }

 tr.commit();
 return neueZeile;
}

// einen Dispo Auftrag für einen Auftrag anlegen (wegen freier Menge)
void AufEintrag::Ueberplanen(const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,str(),
   NV("Artikel",artikel),NV("Menge",menge),NV("Datum",datum));
   // automatisch geplant ??
   assert(Id()!=plan_auftrag_id);

   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
   //int znr=
   dispoAB.BestellmengeAendern(menge,datum,artikel,OPEN,*this);

   // zusätzliche Menge vermerken und Material bestellen
   MengeAendern(menge,true,AufEintragBase());
}

struct AufEintrag::Planen_cb : public auf_positionen_verteilen_cb
{  AufEintrag &ziel;

   Planen_cb(AufEintrag &ae) : ziel(ae) {}
   mengen_t operator()(AufEintrag &ae, mengen_t planmenge) const
   {  if (ae.getLieferdatum()<ziel.getLieferdatum()) return 0;
      ae.Planen(planmenge,ziel);
      return planmenge;
   }
};

// quelle und seine Kinder wurden bereits verändert
struct AufEintrag::Planen_undo_cb : public distribute_parents_cb
{  AufEintrag &quelle;

   Planen_undo_cb(AufEintrag &ae) : quelle(ae) {}
   mengen_t operator()(const AufEintragBase &ae, mengen_t m) const
   {  if (ae.Id()==dispo_id) 
         return -AufEintrag(ae).MengeAendern(-m,false,quelle,false);
      else
      {  assert(delayed_reclaim::Active());
         mengen_t m2=-AufEintragZu(ae).setMengeDiff__(quelle,-m);
         assert(m2==m);
//         AufEintrag(quelle).Verzeigern(-m2,false);
//         if (m!=m2) std::cerr << __FILELINE__ << ": " << m << "!=" << m2 << '\n';
         AufEintrag(ae).Verzeigern(m2,false);
         return m2;
      }
   }
   // _umgekehrte_ Priorität
   virtual bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
   {  return AufEintragZu_sort::priority(b,a);
   }
};

void AufEintrag::Verzeigern(mengen_t M, bool nach_oben)
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),NV("M",M));
  if (!M) return;
  assert(!nach_oben || Id()>=handplan_id);
  if (!nach_oben || Instanz()->KundenInstanz())
  {  if (M>=0) ArtikelInternNachbestellen(M);
     else ArtikelInternAbbestellen(-M);
  }
    else if (M>0) // planen (Pfeile von oben ebenfalls anlegen)
    {  assert(getCombinedStatus()==OPEN);
       SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::sel_Artikel_Planung_id
    		(Instanz()->Id(),Kunde::eigene_id,Artikel(),ungeplante_id,
    			OPEN)));
       Transaction tr;
       mengen_t m=M;
       // Menge wird gleich häppchenweise wieder hinzugebucht
       MengeAendern(-m,false,AufEintragBase());
       m=auf_positionen_verteilen(sel,m,Planen_cb(*this));
       // 2er anlegen
       if (!!m) Ueberplanen(Artikel(),m,getLieferdatum());
       tr.commit();
    }
    else // planen rückgängig (Pfeile von oben ebenfalls anlegen)
    {  Transaction tr;
       mengen_t m=-M;
       {  delayed_reclaim dlr;
          AufEintrag(*this).Verzeigern(-m,false);
          m=distribute_parents(*this,m,Planen_undo_cb(*this));
       }
//       if (!!m) std::cerr << __FILELINE__ << ": " << m << '\n';
       assert(!m);
       tr.commit();
    }
}

// 2 bool Parameter für instanzen (oben wie unten)
void AufEintrag::setStatus(AufStatVal newstatus,bool force,bool instanzen,bool planen) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("status",newstatus),force?"force":"",
 	NV("open",bestellt-geliefert),NV("oldst",entrystatus));
 assert(!planen || instanzen);
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
 if ((newstatus == CLOSED || newstatus == STORNO) && getRestStk()!=0 && instanzen)
 {   Verzeigern(-getRestStk(),planen);
 }

 Query("update auftragentry "
 	"set status=?, lasteditdate=now(), lastedit_uid=? "
	"where (instanz,auftragid,zeilennr)=(?,?,?)") 
 	<< newstatus << getuid() 
 	<< static_cast<AufEintragBase&>(*this);
 SQLerror::test("setStatus: update auftragentry");
 AufStatVal oldentrystatus=entrystatus;
 entrystatus=newstatus;


 if(newstatus==OPEN && auftragstatus==UNCOMMITED)
 {  setStatusAuftragBase(newstatus);
    auftragstatus=newstatus;
 }

 if(newstatus == OPEN  &&  oldentrystatus==UNCOMMITED && getRestStk()!=0 && instanzen)
 {  Verzeigern(getRestStk(),planen);
 }

 if(newstatus==OPEN && bestellt!=0 && Id()>=handplan_auftrag_id)
   {
    pps_ChJournalEntry::newChange(
			instanz, *this, artikel,
			bestellt.as_float(),
			bestellt.as_float(),
			pps_ChJournalEntry::CH_MENGE);
  }
 else if(newstatus==CLOSED && oldentrystatus!=UNCOMMITED
 	&& geliefert-bestellt!=0 && Id()>=handplan_auftrag_id)
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


void AufEintrag::updateLieferdatum(const Petig::Datum &ld,bool planen) throw(SQLerror)
{ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Datum",ld));
 Transaction tr;
 Query("lock auftragentry in exclusive mode"); // unnötig? CP
 SQLerror::test("updateLieferdatum: lock table auftragentry");

 {delayed_reclaim dlr;
 if(getCombinedStatus()==OPEN) Verzeigern(-getStueck(),planen);

 Query("update auftragentry "
 	"set lieferdate=?, lasteditdate=now() "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< ld
 	<< static_cast<const AufEintragBase&>(*this);
 SQLerror::test("updateLiefDatum: update lieferdate in auftragentry");
 lieferdatum=ld;
 lasteditdate=ManuProC::Datum::today();

 if(getCombinedStatus()==OPEN) Verzeigern(getStueck(),planen);
 }

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

int AufEintrag::split(mengen_t newmenge, const Petig::Datum &newld,bool dispoplanung) throw(SQLerror)
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

 int ZEILENNR;
 {delayed_reclaim dlr;
 mengen_t mt=MengeAendern(-newmenge,true,AufEintragBase());
 assert(mt==-newmenge);

 if(Instanz()==ppsInstanzID::Kundenauftraege)
   {Auftrag A(*this);
    AufEintragBase newaeb=A.push_back(newmenge,newld,artikel,entrystatus,true,preis,rabatt);
    ZEILENNR=newaeb.ZNr();
   }
 else
   ZEILENNR=split_zuordnungen_to(newmenge,newld,artikel,entrystatus,dispoplanung);
 }

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
   return bestellt-geliefert-tmp_geliefert;
}

fixedpoint<2> AufEintrag::ProvSatz() const throw(SQLerror)
{
 if(provsatz!=-1) return provsatz;

 Query q("select provsatz from auftragentry where"
	" (instanz,auftragid,zeilennr) = (?,?,?)");

 q << InstanzID() << Id() << getZnr() >> FetchIStream::MapNull(provsatz,0);
 SQLerror::test(__FILELINE__);

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

/*
void AufEintrag::setDefaultProvSatz() throw(SQLerror)
{
 std::string tabelle="artbez_"+
        itos(ExtBezSchema::default_Typ)+"_"+
        itos(ExtBezSchema::default_id);


 Query q("update auftragentry set provsatz="
	"(select case when c.provsatznr=1 then provsatz1 else provsatz2 end"
	" from auftragentry e join auftrag a on (e.instanz=a.instanz"
	" and e.auftragid=a.auftragid and "
	" (a.instanz,a.auftragid,e.zeilennr)=(?,?,?))"
	" join "+tabelle+" b on (e.artikelid=b.id) "
	" join prov_config c using (artikel) join"
	" prov_verkaeufer v on (v.kundennr=a.kundennr and v.verknr=a.verknr))"
	" where (instanz,auftragid,zeilennr)=(?,?,?)");

 q << InstanzID() << Id() << getZnr() << InstanzID() << Id() << getZnr();

 SQLerror::test(__FILELINE__);

 ProvSatz();
}
*/

