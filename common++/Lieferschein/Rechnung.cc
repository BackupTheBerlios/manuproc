// $Id: Rechnung.cc,v 1.42 2006/10/31 16:05:26 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
 *  Copyright (C) 2006 Christof Petig
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


#include "Rechnung.h"
#include <Misc/Query.h>
#include <Misc/itos.h>
#include <Misc/Transaction.h>
#include <Auftrag/auftrag_status.h>
#include <Misc/Trace.h>
#include <Lieferschein/LieferscheinVoll.h>
#include <Artikel/Artikelpreis.h>
#include <Artikel/ArtikelStamm.h>
#include<math.h>




void Rechnung::setRngArt(const RngArt &art) throw(SQLerror)
{
 Transaction tr;
 
  Query("update rechnung set rngart=? where rngid=?")
  	<< char(art) << Id();
   SQLerror::test(__FILELINE__);
 
  if(art==RART_STORNO)
    {
     std::vector<int> lfids;
     ppsInstanz::ID insid(ppsInstanzID::Kundenauftraege);
     
     Query q("select lfrsid from lieferschein where rngid=? "
     		" and instanz=?");
     q << Id() << insid;
     q.FetchArray(lfids);
     
     for(std::vector<int>::iterator i=lfids.begin();
     		i!=lfids.end(); ++i)
     	{ 
         Query("update lieferscheinentry set status="
		"(case when lagerid is not null then ? else ? end) where "
         	" (lfrsid,instanz)=(?,?) and status=?")
         	<< (AufStatVal)OPEN 
		<< (AufStatVal)UNCOMMITED
		<< (*i) 
		<< insid 
		<< (AufStatVal)CLOSED;
         	
         Query("update lieferschein set rngid=null where (lfrsid,instanz)"
         	" = (?,?) and rngid=?")
    	   << (*i) << insid << Id();
    	}
    }
 tr.commit();
}

/*
void Rechnung::convert_to_gutschrift(bool lager_buchung) throw(SQLerror)
{
  Transaction tr;
  (Query("update rechnungentry set preis=abs(preis)*-1 where rngid=?")
  	<< Id()).Check100();

#ifdef MABELLA_EXTENSIONS
  if(lager_buchung)
    {
     Query("insert into fw_lager_buchung "
	"(artikelid,menge,datum,aktion,pid,lfrsid) "
	"(select artikelid,stueck,now(),'L',1,lfrsid from rechnungentry"
	" where rngid=?)")
	<< Id();
    }
#endif
  tr.commit();
}
*/


// zu Zahlender Betrag, inkl Zuschl., etc.
Rechnung::geldbetrag_t Rechnung::Endbetrag(bool with_update_on_db=false) 
const throw(SQLerror)
{


 bool brutto=false;
 fixedpoint<1> stsatz(0.0);

 Preis::geldbetrag_out betrag=Betrag(brutto);
 
 if(entsorgung)
   betrag = betrag.as_float() * (1.0 + ENTSSATZ);

// if(kunde->Id()==kunde->Rngan())
//   {
//    if(kunde->MwSt(Id()))
//      stsatz=Rechnung::MwStProz;
//   }
// else
//  {
//   cH_Kunde rngkd(kunde->Rngan());
//   if(rngkd->MwSt())
//     stsatz=MWSTPROZ;
//   }


 if(with_update_on_db)
  {
   Query("update rechnung set betrag = "
	" (case when rngart='G' then -1 else 1 end)*abs(?)"
//", steuersatz=?"
	" where rngid = ?") << betrag << Id();
// << stsatz 
   SQLerror::test(__FILELINE__);
  }
 return betrag;
}


void Rechnung::setMwSt(const fixedpoint<1> _mwst) const throw(SQLerror)
{
 if(fibu_buchid!=0) return; // darf nicht mehr Ã¤ndern

 fixedpoint<1> stsatz(0.0);

 if(_mwst==-1)
   {if(kunde->MwSt(Id()))
     stsatz=Rechnung::MwStProz;}
 else
  stsatz=_mwst;

 Query("update rechnung set steuersatz = ? where rngid = ?") 
	<< stsatz << Id();
 SQLerror::test(__FILELINE__);
 mwst_satz=stsatz;
}

void Rechnung::setBezahlt(bool _bezahlt) throw(SQLerror)
{
 RechnungBase::setBezahlt(_bezahlt);
 bezahlt=_bezahlt;
}

void RechnungBase::setBezahlt(bool _bezahlt) throw(SQLerror)
{Query("update rechnung set bezahlt=? where rngid=?")
 	<< _bezahlt << rngid;
 SQLerror::test(__FILELINE__);
}

void Rechnung::setEntsorgung(bool ent) throw(SQLerror)
{
 Query("update rechnung set entsorgung=? where rngid=?")
	<< ent << Id(); 
 SQLerror::test(__FILELINE__);
 
 entsorgung=ent;
}

unsigned Rechnung::push_back(ArtikelBase art,int stk,mengen_t menge)
{ Transaction tr;
  unsigned lineno=0;
  Query("lock table rechnungentry in exclusive mode"); 
  Query("select max(zeilennr) from rechnungentry where rngid=?")
          << Id()
          >> Query::Row::MapNull(lineno);
  lineno++;

//  Preis ek_preis;
//  ArtikelTyp at(art);
  cH_Kunde self(Kunde::eigene_id);
  Artikelpreis  ek_preis(self,art,
    		AuftragBase::Gesamtmenge(stk,menge).as_int());

  push_back(lineno,art,0,0,stk,menge,Preis(),0,0,ek_preis);
  tr.commit();
}

// geht nur mit Zeilen ohne Lieferschein !
void Rechnung::erase(unsigned lineno)
{(Query("delete from rechnungentry where (rngid,zeilennr)=(?,?) and lfrsid is null")
 	<< Id() << lineno).Check100(); 
}

void Rechnung::setze_Zahlziel(ManuProC::Datum zziel) throw(SQLerror)
{
 (Query("update rechnung set zahlziel=? where rngid=?")
 	<< zziel << Id()).Check100();
 zahlziel=zziel;
}


void Rechnung::setze_Datum(ManuProC::Datum rgdatum) throw(SQLerror)
{
 (Query("update rechnung set datum=? where rngid=?")
 	<< rgdatum << Id()).Check100();
 // in Struktur nicht ändern ???
}


void Rechnung::setze_Zahlungsart(cH_Zahlungsart z) throw(SQLerror)
{
 (Query("update rechnung set zahlart=? where rngid=?")
 	<< z->Id() << Id()).Check100();
 // in Struktur nicht ändern ???
}


void Rechnung::deleteLieferschein(const LieferscheinBase &lfrs) throw(SQLerror)
{Transaction tr;
 
 (Query("delete from rechnungentry where lfrsid=? and rngid=?")
 	<< lfrs.Id() << Id()).Check100(); 
 (Query("update lieferschein set rngid=null where "
	" (lfrsid,instanz)=(?,?)")
 	<< lfrs.Id() << lfrs.Instanz()->Id()).Check100();

 // gebuchte wieder auf OPEN
 Query("update lieferscheinentry set status=? where "
	" (lfrsid,instanz)=(?,?) and status=? and lagerid is not null")
 	<< (AufStatVal)OPEN << lfrs.Id() << lfrs.Instanz()->Id()
	<< (AufStatVal)CLOSED;

 // ungebuchte wieder auf UNCOMMITED
 Query("update lieferscheinentry set status=? where "
	" (lfrsid,instanz)=(?,?) and status=? and lagerid is null")
 	<< (AufStatVal)UNCOMMITED << lfrs.Id() << lfrs.Instanz()->Id()
	<< (AufStatVal)CLOSED;
 tr.commit();
 
}

#include <sys/types.h>
#include <unistd.h>

//template <class T>
// T min(const T &a, const T &b)
//{ return a<b?a:b; }

void Rechnung::push_back(unsigned &lineno,ArtikelBase art,
		unsigned lfrsid,unsigned lfrsznr, int stk,
		mengen_t menge,const Preis &p, AufEintragBase::rabatt_t rabatt,
		fixedpoint<2> provsatz, const Preis &ek_preis)
{ Query("insert into rechnungentry "
 	"(rngid,zeilennr, artikelid, lfrsid,lfrszeile, stueck,menge,"
 		"preis,rabatt,preismenge, provsatz,ek_preis) "
 	"VALUES (?,?, ?, ?,?, ?,?,"
 		"?,?,?, ?,?)")
 		<< Id() << lineno << art 
 		<< Query::NullIf(lfrsid) << Query::NullIf(lfrsznr)
	 	<< abs(stk) << Query::NullIf(menge)
 		<< p.Wert(getWaehrung(),p.BezugsMenge()) 
	 	<< rabatt << p.BezugsMenge() << provsatz 
	 	<< ek_preis.Wert(Waehrung::default_id,p.BezugsMenge());
  lineno++;
#if defined(PETIG_EXTENSIONS) && defined(MANUPROC_DYNAMICENUMS_CREATED)
  ArtikelStamm as(art);
  cH_ArtikelBezeichnung ab(art);
  if (as.Warengruppe()==ArtikelTypID::Ketten)
  { ArtikelBase lohn,material;
    Query("select lohn_id,material_id from "
	+cH_ExtBezSchema(ExtBezSchema::default_id,as.Warengruppe())->TableName()+
	" where id=?") << art
	>> Query::Row::MapNull(lohn) >> Query::Row::MapNull(material);
    int faeden=atoi(ab->Komponente(2).c_str());
    int dtex=atoi(ab->Komponente(1).c_str());
    if (!!lohn) 
      push_back(lineno,lohn,lfrsid,lfrsznr,std::max(50,faeden),
	  int(stk)*std::max(menge,mengen_t(10000))/1000.0,Preis(),rabatt,
	  provsatz,Preis());
    if (!!material) 
      push_back(lineno,material,lfrsid,lfrsznr,stk,
	  1.3*float(menge/1000.0)*faeden*(dtex/10000.0),
	  Preis(),rabatt,provsatz,Preis());
  }
#endif
}

void Rechnung::addLieferschein(const LieferscheinBase &lfrs) throw(SQLerror)
{
 int MAXZNR,MINZNR;

 Transaction tr;

 Query("select min(zeile) from lieferscheinentry where "
	" (instanz,lfrsid)=(?,?) and status in (0,1) ")
 	<< lfrs.Instanz()->Id() << lfrs.Id()
 	>> Query::Row::MapNull(MINZNR);
 Query("lock table rechnungentry in exclusive mode"); 
 Query("select max(zeilennr) from rechnungentry where rngid=?")
 	<< Id()
 	>> Query::Row::MapNull(MAXZNR);
 MAXZNR++;
 
 LieferscheinVoll lv(lfrs.Instanz(),lfrs.Id());
 
 unsigned lineno=MAXZNR;
 cH_Kunde self(Kunde::eigene_id);

 for (LieferscheinVoll::const_iterator i=lv.begin();i!=lv.end();++i)
 {  Preis p;
    AufEintragBase::rabatt_t rabatt;
    fixedpoint<2> provsatz=0;

    if (!i->getZusatzInfos().empty() && i->getZusatzInfos()[0].aeb.valid())
    { AufEintrag ae(i->getZusatzInfos()[0].aeb);
      rabatt=ae.Rabatt();
      p=ae.EPreis(ae.Rabatt()!=0);
      provsatz=ae.ProvSatz();
    }

    // ohne Auftrag, nur Lieferschein: z.B. Gutschrift
    if(i->getZusatzInfos().empty() || !(i->getZusatzInfos()[0].aeb.valid()))
       provsatz=lv.getKunde()->getProvSatz_Artikel(
				ArtikelBase(i->Artikel()),rabatt);

//    ArtikelTyp at(i->Artikel());
    Artikelpreis ek_art_preis(self,i->Artikel(),
    		AuftragBase::Gesamtmenge(i->Stueck(),i->Menge()).as_int());

//    Preis EK_Preis(ek_art_preis.In(
    push_back(lineno,i->Artikel(),lfrs.Id(),i->ZNr(),i->Stueck(),i->Menge(),
		p,rabatt,provsatz,ek_art_preis);
 }


 Query("update lieferschein set rngid=? "
 	"where (instanz,lfrsid)=(?,?)")
 	<< Id()
 	<< lfrs.Instanz()->Id() << lfrs.Id();
 Query("update lieferscheinentry set status=? where "
	" (lfrsid,instanz)=(?,?) and "
	"((status=?  and lagerid is not null) or"
	"(status=? and lagerid=0))")
 	<< (AufStatVal)CLOSED << lfrs.Id() << lfrs.Instanz()->Id()
	<< (AufStatVal)OPEN
	<< (AufStatVal)UNCOMMITED;
 SQLerror::test(__FILELINE__,100);

 tr.commit();
}


void Rechnung::setzeWaehrung(const cP_Waehrung &w) throw(SQLerror)
{  (Query("update rechnung set waehrung=? where rngid=?")
   	<< w->Id() << Id()).Check100();
   waehrung=w;
}

void Rechnung::setze_Rabatt(rabatt_t r) throw(SQLerror)
{  (Query("update rechnung set rabatt=? where rngid=?")
	<< r << Id()).Check100();
   rabatt=r;
}

// nur Warenwert, ohne MwSt., ggf. rabattiert
Rechnung::geldbetrag_t Rechnung::Betrag(bool brutto) const throw(SQLerror)
{
 int ROUND=ManuProC::Precision::GeldBetrag; //Rundungstellen
 int HELPFKT=(int)pow(10.0,ROUND);
 Preis::geldbetrag_out betrag; // Rückgabe immer mit zwei Stellen

 if(brutto)
   {
    Query q("select sum(round(preis* "
	       "((coalesce(menge,1)*stueck) / "
		" (coalesce(preismenge,1))),2)) "
	    " from rechnungentry where rngid = ?");
    q << Id();
    q >> Query::Row::MapNull(betrag,0.0);   
   }
 else
   {Query q("select  sum(round((round(preis*((100-coalesce(rabatt,0))/100), case "
	" when (preis*?)%10!=0 then ? else 2 end))*"
	"((coalesce(menge,1)*stueck)) / "
	" (coalesce(preismenge,1)),2))" 
	" from rechnungentry where rngid = ?");
    q << HELPFKT << ROUND << Id(); 
    q >> Query::Row::MapNull(betrag,0.0);   

    float rab = 1 - .01*rabatt.as_float();
    betrag = betrag.as_float() * rab; 
   }

 return betrag;
}




void RechnungBase::setEingezogen(int refnr, bool bezahlt) throw(SQLerror)
{(Query("update rechnung set bezahlt=?, einzug_refnr=? where rngid=?")
 	<< bezahlt << refnr << rngid).Check100();
   // member nicht ändern?
}



const std::string Rechnung::Notiz() const throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,Id());
 if(notiz_valid) return notiz;

 Query("select notiz from rechnung where rngid=?")
 	<< Id()
 	>> Query::Row::MapNull(notiz);
 notiz_valid=true;
 return notiz;
}

void Rechnung::Notiz(const std::string n) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,Id(),n);

 (Query("update rechnung set notiz=? where rngid=?")
 	<< Query::NullIf(n) << Id()).Check100();
 notiz=n;
}


int Rechnung::size() const throw(SQLerror)
{
 int ret;
 Query("select count(*) from rechnungentry where rngid=?")
	<< Id() >> ret;
 return ret;
}

void Rechnung::setFiBuBuchid(int fid) throw(SQLerror)
{
 #ifdef MABELLA_EXTENSIONS
 (Query("update rechnung set fibu_buchid=? where rngid=?")
   << Query::NullIf(fid,0) << Id()).Check100();
 #endif
 fibu_buchid=fid;  
}


const ManuProC::Datum Rechnung::sent_at() const throw(SQLerror)
{
 ManuProC::Datum sent;
 Query("select sent_at from rechnung where rngid=?")
	<< Id() >> sent;
 return sent;
}

const std::string Rechnung::sent_to() const throw(SQLerror)
{
 std::string sent;
 Query("select sent_to from rechnung where rngid=?") << Id() >> sent;
 return sent;
}

void Rechnung::Set_sent_at() const throw(SQLerror)
{
 Query("update rechnung set sent_at=now() where rngid=?") << Id();
}


void Rechnung::Set_sent_to(const std::string t) const throw(SQLerror)
{
 Query("update rechnung set sent_to=? where rngid=?") << t << Id();
}

Rechnung::Rechnung(const cH_Kunde k,int jahr) throw(SQLerror)
	: RechnungBase(none_id), kunde(k),rabatt(0),
   zahlungsart(Zahlungsart::none_id),notiz_valid(false)
{
 int RNGID;
 int KUNDENNR;
 int JAHR=jahr;
 int ZAHLUNGSART;
 int WAEHRUNG;
 
 assert(Rechnung::MwStProz>0.0);

 if (!JAHR) JAHR=AuftragBase::aktuellesJahr();
 JAHR%=100;

 KUNDENNR=k->Id();

 cH_Kunde krng(k->Rngan());

 if(KUNDENNR != krng->Id())
   {
    ZAHLUNGSART=krng->zahlungsart()->Id();
    WAEHRUNG=krng->getWaehrung()->Id();
   }
 else
   {
    ZAHLUNGSART=k->zahlungsart()->Id();
    WAEHRUNG=k->getWaehrung()->Id();
   }

 Transaction tr;
  
 Query("lock table lieferschein in exclusive mode");
 
 Query("select max(rngid)+1 from rechnung where rngid between ? and ?")
      << (JAHR*AuftragBase::jahresmultiplikator) 
      << ((JAHR+1)*AuftragBase::jahresmultiplikator-1)
      >> Query::Row::MapNull(RNGID,JAHR*AuftragBase::jahresmultiplikator);
 
 fixedpoint<1> STSATZ(0.0);
 if(k->MwSt())
   STSATZ=Rechnung::MwStProz;

 Query("insert into rechnung (rngid, kundennr,zahlart,waehrung,steuersatz) "
 	"values (?,?,?,?,?)") 
      << RNGID << KUNDENNR << ZAHLUNGSART << WAEHRUNG << STSATZ;

#ifdef MABELLA_EXTENSIONS
 bool ENTSORGUNG=krng->entsorgung();
// int VERKNR=k->VerkNr();
// int IVERKNR=0;

 Query("select 1/(coalesce(fkt,1)) from waehrung where wid=?")
   << WAEHRUNG >> kurs;

// if(VERKNR==Kunde::none_id) IVERKNR=-1;
 (Query("update rechnung set entsorgung=?, "
/*	verknr=:VERKNR:IVERKNR,*/
	"kurs=? where rngid=?") 
	<< ENTSORGUNG << kurs << RNGID).Check100();

 (Query("update rechnung set notiz=(select rng_notiz from kunden "
        "where kundennr=?) where rngid=?") 
        << KUNDENNR << RNGID).Check100();
 
 std::string tmp_notiz;
 if(k->Id()!=k->Rngan()) // Notiz der Rechnungsadresse besorgen
   {Query("select coalesce(rng_notiz,'') from kunden where"
          " kundennr=?") << k->Rngan() >> tmp_notiz;
    Query("update rechnung set notiz=notiz||? "
          "where rngid=?") << tmp_notiz << RNGID;
    }
#endif

 rngid=RNGID;
 rgdatum=ManuProC::Datum::today();

 tr.commit();
}

