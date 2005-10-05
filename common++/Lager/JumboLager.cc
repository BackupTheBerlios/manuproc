// $Id: JumboLager.cc,v 1.11 2005/10/05 08:49:27 christof Exp $
/*  pps: ManuProC's production planning system
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

#include "JumboLager.h"
#include <Artikel/ArtikelBase.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Misc/Datum.h>
//#include <Misc/string0.h>
#include <Misc/SQLerror.h>
#include <Misc/Transaction.h>
#include <Misc/itos.h>
#include <unistd.h>
#include <Misc/TraceNV.h>
#include <Misc/FetchIStream.h>

void JumboLager::Jumbo_Einlagern(const LagerPlatz position,JumboRolle& jumbo,Jumbo_LogTyp typ,
   const std::string& user,const Zeitpunkt_new *zeit,bool produziert)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("code",jumbo.Code()),
  		NV("zeit",zeit?*zeit:Zeitpunkt_new()),
  		NV("position",position.IPosition()));
  Transaction tr;
  Zeitpunkt_new zeit_local_storage;

  int CODE=jumbo.Code()/10;
  int STATUS=JumboRolle::ImLager;
  int LAGERPLATZ=position.IPosition();
  bool REST=(typ==Rest || typ==Rest_L);
  bool DUMMY=bool();

  if (!zeit)
  {  Query("select now()") >> zeit_local_storage;
     zeit=&zeit_local_storage;
  }

  if (jumbo.VerarbDatum().valid() &&  *zeit<jumbo.VerarbDatum()) 
   { STATUS=JumboRolle::Verarbeitet;
     LAGERPLATZ=0;
   }
   
  if (jumbo.Archiviert()) jumbo.ausArchivHolen();

  Query("update rohjumbo "
       "set wiederinslager=?,rest=?,status=?,lagerplatz=? where code=?")
	<< *zeit << REST << STATUS 
	<< Query::NullIf(LAGERPLATZ) << CODE;
  SQLerror::test("LagerSQL einlagern");

  jumbo.setLagerPosition_nodb(position);
  jumbo.setInsLager_nodb(*zeit);
  if(jumbo.getStatus()==JumboRolle::ImLager) typ=Umraeumen;
  jumbo.setStatus_nodb(JumboRolle::Jumbo_Status(STATUS));
  if(typ!=Umraeumen && STATUS==JumboRolle::ImLager)
  {  if (jumbo.VerarbDatum().valid())
        wiedereinlagern(jumbo.Artikel(),jumbo.Meter());
     else
        rein_ins_lager(jumbo.Artikel(),jumbo.Meter(),produziert);
  }
  Jumbo_Log(jumbo,typ,user,zeit);
  tr.commit();
}

void JumboLager::Jumbo_Entnahme(JumboRolle& jumbo,Jumbo_LogTyp typ,
   const std::string& user,const Zeitpunkt_new *zeit,bool fuer_auftraege)
{  
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
  		NV("code",jumbo.Code()),
  		NV("zeit",zeit?*zeit:Zeitpunkt_new()));
  Transaction tr;
  Zeitpunkt_new zeit_local_storage;
  int CODE=jumbo.Code()/10;
  int STATUS=JumboRolle::Verarbeitet;
  int LAGERPLATZ=jumbo.LagerPosition().IPosition();
  bool throw100=false;
  STATUS=JumboRolle::Verarbeitet;

  if (!zeit)
  {  Query("select now()") >> zeit_local_storage;
     zeit=&zeit_local_storage;
  }

  if (jumbo.Archiviert()) jumbo.ausArchivHolen();

  if (jumbo.InsLagerDatum().valid() && *zeit<jumbo.InsLagerDatum()) 
     STATUS=JumboRolle::ImLager;
  else 
  {  LAGERPLATZ=0;
     if (jumbo.VerarbDatum().valid() && jumbo.VerarbDatum()<*zeit)
     {  Zeitpunkt_new old=jumbo.VerarbDatum(),
     		      newzt=*zeit;
        old.Precision(Zeitpunkt_new::seconds);
        newzt.Precision(Zeitpunkt_new::seconds);
        // 5 minuten
        if (newzt-old<300) throw100=true;
     }
  }
  
  Query("update rohjumbo set verarb_datum=?,status=?,lagerplatz=? "
  	"where code=?")
	<< *zeit << STATUS << Query::NullIf(LAGERPLATZ) << CODE;
  SQLerror::test("JumboRolle::buchen entnahme");

#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
  jumbo.setLagerPosition_nodb(LagerPlatz(ppsInstanzID::Bandlager,LAGERPLATZ));
#endif
  jumbo.setVerarbDatum_nodb(*zeit);
  if (jumbo.getStatus()==JumboRolle::Verarbeitet) typ=SchonDraussen;
  jumbo.setStatus_nodb(JumboRolle::Jumbo_Status(STATUS));
  if(typ!=SchonDraussen && STATUS==JumboRolle::Verarbeitet)
  {  if (!jumbo.InsLagerDatum().valid())  // oder vielleicht false?
        rein_ins_lager(jumbo.Artikel(),jumbo.Meter(),fuer_auftraege);
     raus_aus_lager(jumbo.Artikel(),jumbo.Meter(),fuer_auftraege);
  }
  Jumbo_Log(jumbo,typ,user,zeit);
  // hupen
  if (throw100) throw SQLerror("Entnahme",100,"Rolle schon erfasst");
  tr.commit();
}

void JumboLager::Jumbo_Log(const JumboRolle& jumbo,Jumbo_LogTyp typ,const std::string& user,const Zeitpunkt_new *zeit)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  Query q("insert into lager_bewegung "
      "(code,action,name,lagerplatz,zeit) "
      "values (?,?,?,?,?)");
  q << jumbo.Code()/10 << char(typ) << user.substr(0,4) << jumbo.LagerPosition().IPosition();
  if (!zeit) q.add_argument("now",25);
  else q << *zeit;
} 

std::vector<class LagerInhalt> JumboLager::LagerInhalt_(const ArtikelBase& artikel) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);

  std::string squery = "select coalesce(artikelid,0),"
         " sum(case rest when true then 0 else 1 end),"
         " sum(case rest when true then 1 else 0 end),"
         " sum(coalesce(barcoist,soll_meter,0) * (case rest when true then 0 else 1 end)),"
         " sum(coalesce(barcoist,soll_meter,0) * (case rest when true then ? else 0 end))"
         " from rohjumbo where status=?"
         " and lagerplatz is not null";
  if(artikel.valid()) squery += " and artikelid ="+itos(artikel.Id());
  squery += " group by artikelid";
  Query q(squery);
  q << reste_faktor << JumboRolle::ImLager;
  std::vector<class LagerInhalt> V;
  q.FetchArray(V);
  return V;
}

#if !defined(__GNUC__) || __GNUC__>=3
const float JumboLager::reste_faktor;
#endif
