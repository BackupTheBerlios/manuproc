// $Id: AufEintragZu.cc,v 1.15 2003/07/04 11:08:23 christof Exp $
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

#include <Auftrag/AufEintragZu.h>
#include <Misc/Transaction.h>
#include <Auftrag/auftrag_status.h>
#include <Misc/Changejournal.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Misc/FetchIStream.h>
#include <Misc/TraceNV.h>

// was tut das eigentlich ? CP
AufEintragZu::list_t AufEintragZu::get_Referenz_list_id(const AuftragBase::ID id,bool kinder,bool artikel) const throw(SQLerror)
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Id",id),NV("Kinder",kinder));
   list_t L=get_Referenz_list(*this,kinder,artikel); // kinder/* oder false? */);
   list_t N=select_Id(id,L);
   if(N.empty() && kinder) // Für die Reparatur; ein Pfeil könnte ins nichts zeigen ...
    {
      L=get_Referenz_list_without_child();
      N=select_Id(id,L);
    }   
   return N;
}

AufEintragZu::list_t AufEintragZu::select_Id(const AuftragBase::ID id,const list_t &L) const
{
   list_t N;
   for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
       if(i->AEB.Id()==id) 
         N.push_back(*i);
   return N;
}

AufEintragZu::list_t AufEintragZu::get_Referenz_list_geplant(bool kinder) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Kinder",kinder));

 if(Instanz()->LagerInstanz())
     return get_Referenz_list_id(AuftragBase::plan_auftrag_id,kinder);

 list_t L=get_Referenz_list(*this,kinder,list_ohneArtikel);
 list_t N;
 for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
  {
     if(i->AEB.Id()!=AuftragBase::dispo_auftrag_id &&
        i->AEB.Id()!=AuftragBase::ungeplante_id &&
        i->AEB.Id()!=AuftragBase::invalid_id ) 
       N.push_back(*i);
  }
/*
 else 
   { assert(kinder);
     assert(L.empty() || L.size()==1);
     for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
      {
        list_t N_=AufEintragZu(i->AEB).get_Referenz_list_geplant(kinder);
        N.splice(N.end(),N_);
      }
   }
*/
 return N;
}
      


AufEintragZu::list_t AufEintragZu::get_Referenz_listFull(bool kinder,bool nur_ende) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Kinder",kinder),NV("NurEnde",nur_ende));
 list_t tv=get_Referenz_list(*this,kinder,list_ohneArtikel);
 list_t vaeb;
 list_t tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (list_t::iterator i=tv.begin();i!=tv.end();++i)
   {
     tvxx=get_Referenz_list(i->AEB,kinder,list_ohneArtikel);
     if(nur_ende)
      {
        if (tvxx.empty()) vaeb.splice(vaeb.end(),tv,i) ;
        else tv.erase(i) ; 
      }
     else vaeb.splice(vaeb.end(),tv,i) ;
     goto reloop;
   }
 return vaeb;
}


AufEintragZu::list_t AufEintragZu::get_Referenz_list_for_geplant(bool kinder) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Kinder",kinder));
  // Ungeplante Referenz Aufträge
  list_t URA=get_Referenz_list(*this,false,list_ohneArtikel);
 //NEU
  list_t L;
  for(list_t::const_iterator i=URA.begin();i!=URA.end();++i)
   {
     list_t l=AufEintragZu(i->AEB).get_Referenz_list(i->AEB,kinder,list_ohneArtikel);
     if(kinder) // sich selber aus der Liste entfernen
        for (list_t::iterator j=l.begin();j!=l.end();++j)
           if(j->AEB==*this) {L.erase(j); break;}
     L.merge(l);
   }
  return L; 
}




AuftragBase::mengen_t AufEintragZu::verteileMenge(list_t L, mengen_t menge,bool add)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Menge",menge),NV("Add",add));
  for(list_t::const_iterator i=L.begin();i!=L.end();++i)
   {
     if(menge==AuftragBase::mengen_t(0)) return menge;
     AuftragBase::mengen_t M=AuftragBase::min(menge,i->Menge);
//     if(i->Menge >= menge) M=menge;
//     else M=i->Menge;
     if(add)  setMengeDiff__(i->AEB,M)  ;
     else     setMengeDiff__(i->AEB,-M) ;
     menge-=M;
   }
  if(menge!=AuftragBase::mengen_t(0)) 
      std::cerr << "WARNING: 'AufEintragZu::verteileMenge' Restmenge = "<<menge<<'\n';
  return menge;
}

std::vector<AufEintragBase> AufEintragZu::getKundenAuftragV() const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  std::vector<AufEintragBase> V;
  list_t L=AufEintragZu(*this).get_Referenz_listFull(false);
  for (AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
    V.push_back(i->AEB);
  return V;
}

AufEintragZu::map_t AufEintragZu::get_Kinder_nach_Artikel(const AufEintragBase &aeb,bool kinder)
{  list_t KindListeU(AufEintragZu::get_Referenz_list(aeb,kinder,AufEintragZu::list_Artikel));
   map_t MapArt;

   for(list_t::const_iterator i=KindListeU.begin();i!=KindListeU.end();++i)
    {
      // Nach dem Planen kann es zu einem Auftrag mehrere Kindaufträge 
      // mit demselben Artikel geben, nur bei EINEM darf die Menge geändert
      // werden
      MapArt[i->Art].push_back(*i);
     }
   return MapArt;
}

FetchIStream &operator>>(FetchIStream &i, AufEintragZu::st_reflist &rl)
{  return i >> rl.AEB >> rl.Menge >> rl.Art;
}

AufEintragZu::list_t AufEintragZu::get_Referenz_list(const AufEintragBase& aeb,bool kinder,bool artikel) throw(SQLerror) 
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,aeb,kinder?"Kinder":"Eltern",artikel?"mit Artikel":"ohne Artikel");

 assert(kinder || !artikel); // Eltern mit Artikel macht keinen Sinn

 std::string squery, selected,specified, artsel,join;
 
 if (kinder) { selected="neu"; specified="alt"; }
 else { selected="alt"; specified="neu"; }
 
 if (artikel) 
 {  artsel="artikelid"; 
    join="join auftragentry on "
      "("+selected+"instanz,"+selected+"auftragid,"+selected+"zeilennr)"
      			   "= (instanz,auftragid,zeilennr)";
 }
 else { artsel="NULL"; }
 
 squery = "select "+selected+"instanz,"+selected+"auftragid,"
 		+selected+"zeilennr,menge,"+artsel+" "
      "from auftragsentryzuordnung "
      +join+
      "where ("+specified+"instanz,"+specified+"auftragid,"
      			+specified+"zeilennr) = (?,?,?) "
      "order by "+selected+"instanz,"+selected+"auftragid,"
      			+selected+"zeilennr ";
 std::list<st_reflist> vaeb;
 (Query(squery).lvalue() << aeb).FetchArray(vaeb);
 return vaeb;
}

AufEintragZu::list_t AufEintragZu::get_Referenz_list_without_child() const throw(SQLerror)
{return get_Referenz_list(*this,list_kinder,list_ohneArtikel);
}

std::list<cH_Kunde> AufEintragZu::get_Referenz_Kunden() const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
 std::list<cH_Kunde> LK;
 std::list<st_reflist> vaeb = get_Referenz_listFull(false);

 for (std::list<st_reflist>::const_iterator i=vaeb.begin();i!=vaeb.end();++i)
  {
    int kdnr=(Query("select kundennr from auftrag "
    		"where (instanz,auftragid)=(?,?) order by kundennr")
    		<< i->AEB.InstanzID() << i->AEB.Id())
    	.FetchOne<int>(); 
    LK.push_back(cH_Kunde(kdnr));
  }
 LK.sort();
 LK.unique();
 return LK;
}

// wieso ist das in AufEintragZu ?
std::list<AufEintragBase> AufEintragZu::get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,ppsInstanz::ID instanz,AufStatVal status)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("artikel",artikel),NV("status",status),NV("instanz",instanz));
  std::list<AufEintragBase> L;
  
  (Query("select instanz,auftragid,zeilennr from auftragentry "
  	"where artikelid=? "
  	"and instanz<>? " //Nur interne Aufträge sind interessant
  	"and auftragid=0 " // Nur die UNGEPLANTEN Aufträge bekommen neue Kinder
  	+(status!=NOSTAT ? " and status="+itos(status) : std::string())+
  	"order by auftragid,zeilennr").lvalue()
    << artikel.Id() << int(ppsInstanzID::Kundenauftraege))
  .FetchArray(L);
  return L;
}

void AufEintragZu::Neu(const AufEintragBase& neuAEB,const mengen_t menge)
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
      "--",menge,"-->",neuAEB);

 // erst erhöhen versuchen 
 try{ mengen_t mt=setMengeDiff__(neuAEB,menge); 
      if (!AufEintragBase::tolerate_inconsistency)
         assert(menge==mt);
    }
 catch (SQLerror &e) // anlegen
 {  Query("insert into auftragsentryzuordnung "
         "(altinstanz,altauftragid,altzeilennr, menge, "
           "neuinstanz,neuauftragid,neuzeilennr) "
         "values (?,?,?, ?, ?,?,?)").lvalue() 
      << static_cast<const AufEintragBase &>(*this) << menge << neuAEB;
    SQLerror::test(__FILELINE__);
 }
}

void AufEintragZu::Neu(const AufEintragBase& neuAEB,
                       const mengen_t menge,const int oldZnr)
{ if (oldZnr) Neu(AufEintragBase(AuftragBase(*this),oldZnr),menge);
  else Neu(neuAEB,menge);
}

bool AufEintragZu::setMenge(const AufEintragBase& neuAEB,const mengen_t menge)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,neuAEB,NV("Menge",menge));

 Query("update auftragsentryzuordnung set menge=? "
 	"where (altinstanz,altauftragid,altzeilennr, "
 		"neuinstanz,neuauftragid,neuzeilennr)= (?,?,?, ?,?,?)").lvalue()
	<< menge 		
 	<< static_cast<const AufEintragBase&>(*this) << neuAEB;
 SQLerror::test(__FILELINE__,100);
 return !SQLerror::SQLCode();
}


AuftragBase::mengen_t AufEintragZu::setMengeDiff__(const AufEintragBase &neuAEB,mengen_t menge)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,neuAEB,NV("Menge",menge));
 if(menge<0)
  {  AuftragBase::mengen_t M;
     Query("select menge from auftragsentryzuordnung "
     	"where (altinstanz,altauftragid,altzeilennr, "
     	       "neuinstanz,neuauftragid,neuzeilennr)= (?,?,?, ?,?,?)").lvalue()
	<< static_cast<const AufEintragBase&>(*this) << neuAEB
	>> M;
     if(mengen_t(M)<-menge) menge=-M;
  }
//  if (menge!=0) auch 0er sind interessant (bei Produziert)
  {  Query("update auftragsentryzuordnung set menge=menge+? "
 	"where (altinstanz,altauftragid,altzeilennr, "
 		"neuinstanz,neuauftragid,neuzeilennr)= (?,?,?, ?,?,?)").lvalue()
	<< menge 		
 	<< static_cast<const AufEintragBase&>(*this) << neuAEB;
     SQLerror::test(__FILELINE__);
  }
 return menge;
}


// wer braucht denn so etwas krankes? CP
bool AufEintragZu::setKindZnr(const AufEintragBase& neuAEB)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,neuAEB);
 std::cerr << "mit "<< __PRETTY_FUNCTION__ << " bin ich nicht einverstanden CP\n";

 Query("update auftragsentryzuordnung set neuzeilennr=? "
 	"where (altinstanz,altauftragid,altzeilennr, "
 		"neuinstanz,neuauftragid)= (?,?,?, ?,?)").lvalue()
	<< neuAEB.ZNr()
 	<< static_cast<const AufEintragBase&>(*this) 
 	<< neuAEB.InstanzID() << neuAEB.Id();
 SQLerror::test(__FILELINE__,100);
 return !SQLerror::SQLCode();
}

void AufEintragZu::moveInstanz(const VonNachDel vdl,const AufEintragBase &oldAEB, const AufEintragBase &newAEB) throw(SQLerror)
{

 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,oldAEB,NV("VonNachDel",vdl));

 switch(vdl)
  {
   case Von:
    Query("update auftragsentryzuordnung set "
      "altinstanz=?,altauftragid=?,altzeilennr=? where "
      "(altinstanz,altauftragid,altzeilennr)=(?,?,?)").lvalue()
      	<< newAEB << oldAEB;
    break;
   case Nach:
    Query("update auftragsentryzuordnung set "
      "neuinstanz=?,neuauftragid=?,neuzeilennr=? where "
      "(neuinstanz,neuauftragid,neuzeilennr)=(?,?,?)").lvalue()
      	<< newAEB << oldAEB;
    break;
   case Delete:
    std::cerr << "please use remove instead of " << __PRETTY_FUNCTION__ << '\n';
    remove(oldAEB,newAEB);
    return;
  }
 SQLerror::test(__FILELINE__,100);
}


AuftragBase::mengen_t AufEintragZu::getMenge(const AufEintragBase& aeb) const
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,aeb);
 return (Query("select menge from auftragsentryzuordnung "
    "where (altinstanz,altauftragid,altzeilennr, "
    	"neuinstanz,neuauftragid,neuzeilennr) = (?,?,?, ?,?,?)").lvalue()
    << static_cast<const AufEintragBase&>(*this) << aeb).FetchOne<int>();
}

bool AufEintragZu::remove(const AufEintragBase& alt,const AufEintragBase& neu)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,alt,neu);

  Query("delete from auftragsentryzuordnung where "
                 "(altinstanz,altauftragid,altzeilennr,"
                 "neuinstanz,neuauftragid,neuzeilennr)"
                 "=(?,?,?, ?,?,?)").lvalue()
      << alt << neu;
 SQLerror::test(__FILELINE__,100);
 return !SQLerror::SQLCode();
}

