// $Id: AufEintragBase.cc,v 1.41 2003/03/13 08:19:54 christof Exp $
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

#include <Auftrag/AufEintragBase.h>
#include <Misc/Transaction.h>
#include <Auftrag/AuftragsBaum.h> 
#include <Auftrag/AufEintragZu.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <Auftrag/VerfuegbareMenge.h>
#include <Auftrag/AufEintrag.h>
#ifdef MABELLA_EXTENSIONS
#include <Lager/FertigWaren.h>
#include <Artikel/ArtikelBase.h>
#endif
#include <Misc/TraceNV.h>
#include <Misc/FetchIStream.h>
#include <Auftrag/AufEintragZuMengenAenderung.h>
#include <Misc/FetchIStream_ops.h>
#include <Misc/Changejournal.h>

FetchIStream& operator>>(FetchIStream& is,AufEintragBase &aeb)
{
  int a,b,c;
  is >> a>>b>>c;
  cH_ppsInstanz I((ppsInstanz::ID(a)));
  AuftragBase ab(I,b);
  aeb=AufEintragBase(ab,c);
  return is;
}

Query &operator<<(Query &o, const AufEintragBase &aeb)
{  o.add_argument(itos(aeb.Instanz()->Id()));
   o.add_argument(itos(aeb.Id()));
   o.add_argument(itos(aeb.ZNr()));
   return o;
}

std::ostream &operator<<(std::ostream &o,const AufEintragBase &ae)
{
 o<<ae.Instanz()->Name()<<'('<<ae.Instanz()->Id()<<") "<<ae.Id()<<' '<<ae.ZNr();
 return o;
}

std::string AufEintragBase::str() const
{
 return Instanz()->Name()+"("+itos(Instanz()->Id())+")|"+itos(Id())+"|"+itos(ZNr());
}



void AufEintragBase::calculateProzessInstanz()
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
  assert (Instanz()==ppsInstanzID::Kundenauftraege);
  AuftragsBaum AB(*this,true);
  int anz=0;
  for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
     if(i->AEB2.Id()==0) continue; // 0 = ungeplante Auftr�ge
     if(AufEintrag(i->AEB2).getStueck() == i->menge)
       ++anz;
   }
  setMaxPlanInstanz(anz);
}   

#warning sieht komisch aus ...
int AufEintragBase::split_zuordnungen_to(mengen_t menge,ManuProC::Datum datum,
                        ArtikelBase artikel,AufStatVal status,int uid,
                        bool dispoplanung)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,str(),
   NV("Artikel",artikel),
   NV("Menge",menge),NV("Status",status),NV("DispoPlanung",dispoplanung));

  assert(Id()==AuftragBase::ungeplante_id);
  // ElternListe holen
  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*this,false,AufEintragZu::list_ohneArtikel);
  int znr=none_znr;
  assert(!L.empty());
  for(AufEintragZu::list_t::iterator i=L.begin();i!=L.end();++i)
   {
    mengen_t M=min(i->Menge,menge);
    znr=BestellmengeAendern(M,datum,artikel,status,uid,i->AEB);
// urks ??
    AufEintragZu(i->AEB).setMengeDiff__(*this,-M);
    menge-=M;
    if(M==mengen_t(0)) break;
   }
 return znr;
}

#if 0
void AufEintragBase::ExistMenge(const std::string &s) const
{
 try{AufEintrag AE(*this);
     std::cout << *this<<" existiert mit " <<AE.getStueck()<<" bestellt und "
          <<AE.getGeliefert()<<" geliefert ("<<s<<")\n";
   }catch(...){std::cout << *this<<"\texistiert noch nicht ("<<s<<")\n";}
}
#endif

void AufEintragBase::setVerarbeitung(const cH_Prozess p) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 assert (Instanz()==ppsInstanzID::Kundenauftraege);
 
 Query("update auftrag_prozess set prozessid=?,datum=now() "
 	"where (instanz,auftragid,zeilennr) = (?,?,?)")
	<< p->getProzessID() << *this;
 if (SQLerror::SQLCode()==100)
 {  Query("insert into auftrag_prozess "
        "(instanz,auftragid,zeilennr,prozessid)"
	"values (?,?,?,?)").lvalue()
	<< *this << p->getProzessID();
 }
 SQLerror::test("Prozessaktualisierung (Verarbeitung)");
}


bool AufEintragBase::deleteAuftragEntry() const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 // Kundenauftr�ge D�RFEN NICHT gel�scht werden !!!
 assert (InstanzID()!=ppsInstanzID::Kundenauftraege) ;

 // Testen, ob dieser Auftrag Kinder hat, wenn ja, dann darf dieser 
 // Auftrag NICHT gel�scht werden
 AuftragsBaum AB(*this,true);
 if(!AB.empty()) return false;

 Transaction tr;
 // L�schen aus Auftragszuordnung
 Query("delete from auftragsentryzuordnung "
 	"where (neuinstanz,neuauftragid,neuzeilennr)=(?,?,?)").lvalue()
 	<< *this;
 SQLerror::test(__FILELINE__" DELETE AUFTRAG 1");

 // L�schen aus Auftragentry
 Query("delete from auftragentry "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< *this;
 SQLerror::test(__FILELINE__" DELETE AUFTRAG 2");
 tr.commit();
 return true;
}

void AufEintragBase::setLetztePlanungFuer(cH_ppsInstanz planinstanz) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("Instanz",planinstanz));

 assert (Instanz()==ppsInstanzID::Kundenauftraege);

 if(planinstanz->LagerInstanz()) return;

 Query("update auftrag_prozess set letzteplaninstanz=?,"
 	"maxPlanInstanz=NULL,datum=now() "
 	"where (instanz,auftragid,zeilennr) = (?,?,?)")
	<< planinstanz->Id() << *this;
 if (SQLerror::SQLCode()==100)
 {  Query("insert into auftrag_prozess "
        "(instanz,auftragid,zeilennr,prozessid,letztePlanInstanz)"
	"values (?,?,?,?,?)").lvalue()
	<< *this << planinstanz->get_Prozess()->Id() << planinstanz->Id();
 }
 SQLerror::test("Prozessaktualisierung");
}

void AufEintragBase::setMaxPlanInstanz(int anzahl_maxplaninstanzen) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
   NV("Anzahl",anzahl_maxplaninstanzen));
 Query("update auftrag_prozess set maxPlanInstanz=?, datum=now() "
 	"where (instanz,auftragid,zeilennr) = (?,?,?)")
 	<< anzahl_maxplaninstanzen << *this;
 SQLerror::test("Prozessaktualisierung",100);
}

void AufEintragBase::setLetzteLieferung(const ManuProC::Datum &datum) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,datum);
 Query("update auftragentry set letzte_lieferung=? "
 	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
 	<< datum << *this;
 SQLerror::test(__FILELINE__" setLetzeLieferung");
}


AuftragBase::mengen_t AufEintragBase::updateStkDiffBase__(int uid,const mengen_t &menge) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("AEB",*this),NV("menge",menge));

 Transaction tr;
 Query("lock auftragentry in exclusive mode");
 SQLerror::test("lock auftragentry");

 mengen_t OLDMENGE,MENGE=menge;
 ArtikelBase::ID ARTIKELID;
 Query("select bestellt,artikelid from auftragentry "
      "where (instanz,auftragid,zeilennr) = (?,?,?)").lvalue()
      << *this
      >> OLDMENGE >> ARTIKELID;
// Sicherstellen, da� keine negativen Mengen bestellt werden
 if(MENGE < 0 && OLDMENGE < -MENGE)  MENGE = -OLDMENGE;

 mengen_t NEWMENGE=OLDMENGE+MENGE;
 ManuProC::Trace(AuftragBase::trace_channel, __FILELINE__, OLDMENGE, MENGE, NEWMENGE);

 Query("update auftragentry set bestellt=?,lasteditdate=now(),lastedit_uid=? "
	"where (instanz,auftragid,zeilennr)=(?,?,?)").lvalue()
	<< NEWMENGE << uid << *this;
 SQLerror::test("updateStkDiff: update stk in auftragentry");

 pps_ChJournalEntry::newChange(instanz, *this,ARTIKELID,
 	NEWMENGE.as_float(),MENGE.as_float(),pps_ChJournalEntry::CH_MENGE);

 tr.commit(); 
 return MENGE;
}

