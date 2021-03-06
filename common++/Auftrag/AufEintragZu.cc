// $Id: AufEintragZu.cc,v 1.42 2006/08/11 09:33:10 christof Exp $
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
#include <Misc/Query.h>
#include <Misc/TraceNV.h>

// was tut das eigentlich ? CP
AufEintragZu::list_t AufEintragZu::get_Referenz_list_id(const ID id,bool kinder,bool artikel) const throw(SQLerror)
{
   ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,NV("Id",id),NV("Kinder",kinder));
   list_t L=get_Referenz_list(*this,kinder,artikel); // kinder/* oder false? */);
   list_t N=select_Id(id,L);
   if(N.empty() && kinder) // Fr die Reparatur; ein Pfeil k�nte ins nichts zeigen ...
    {
      L=get_Referenz_list_without_child();
      N=select_Id(id,L);
    }   
   return N;
}

AufEintragZu::list_t AufEintragZu::select_Id(const ID id,const list_t &L) const
{
   list_t N;
   for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
       if(i->AEB.Id()==id) 
         N.push_back(*i);
   return N;
}

AufEintragZu::list_t AufEintragZu::get_Referenz_list_geplant(bool kinder) const throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,NV("Kinder",kinder));

 if(Instanz()->LagerInstanz())
     return get_Referenz_list_id(plan_auftrag_id,kinder);

 list_t L=get_Referenz_list(*this,kinder,list_ohneArtikel);
 list_t N;
 for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
  {
     if(i->AEB.Id()!=dispo_auftrag_id &&
        i->AEB.Id()!=ungeplante_id &&
        i->AEB.Id()!=invalid_id ) 
       N.push_back(*i);
  }
 return N;
}
      


AufEintragZu::list_t AufEintragZu::get_Referenz_listFull(bool kinder,bool nur_ende) const throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,NV("Kinder",kinder),NV("NurEnde",nur_ende));
 list_t tv=get_Referenz_list(*this,kinder,list_ohneArtikel);
 list_t result;
 list_t tvxx;
reloop:
 tv.splice(tv.end(),tvxx); // tvxx anhängen
 for (list_t::iterator i=tv.begin();i!=tv.end();++i)
   {
     tvxx=get_Referenz_list(i->AEB,kinder,list_ohneArtikel);
     if(nur_ende)
      {
        if (tvxx.empty()) result.splice(result.end(),tv,i) ;
        else tv.erase(i) ; 
      }
     else result.splice(result.end(),tv,i) ;
     goto reloop;
   }
 return result;
}

#if 0
std::vector<AufEintragBase> AufEintragZu::getKundenAuftragV() const
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__);
  std::vector<AufEintragBase> V;
  list_t L=AufEintragZu(*this).get_Referenz_listFull(false);
  for (AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
    V.push_back(i->AEB);
  return V;
}
#endif

AufEintragZu::map_t AufEintragZu::get_Kinder_nach_Artikel(const AufEintragBase &aeb,bool kinder,bool sorted)
{  list_t KindListeU(AufEintragZu::get_Referenz_list(aeb,kinder,AufEintragZu::list_Artikel,sorted));
   map_t MapArt;

   for(list_t::const_iterator i=KindListeU.begin();i!=KindListeU.end();++i)
    {
      // Nach dem Planen kann es zu einem Auftrag mehrere Kindauftr�e 
      // mit demselben Artikel geben, nur bei EINEM darf die Menge ge�dert
      // werden
      MapArt[i->Art].push_back(*i);
     }
   return MapArt;
}

Query::Row &operator>>(Query::Row &i, AufEintragZu::st_reflist &rl)
{  return i >> rl.AEB >> rl.Menge >> rl.Art >> rl.Pri;
}

AufEintragZu::list_t AufEintragZu::get_Referenz_list(const AufEintragBase& aeb,bool kinder,bool artikel,bool sorted) throw(SQLerror) 
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,aeb,kinder?"Kinder":"Eltern",artikel?"mit Artikel":"ohne Artikel");

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
 		+selected+"zeilennr,menge,"+artsel+",prioritaet "
      "from auftragsentryzuordnung "
      +join+
      "where ("+specified+"instanz,"+specified+"auftragid,"
      			+specified+"zeilennr) = (?,?,?) ";
 if (sorted)
 { squery+="order by ";
   if (!kinder) squery+="prioritaet,";
   squery+=selected+"instanz,"+selected+"auftragid,"+selected+"zeilennr";
   if (kinder) squery+=",prioritaet";
 }
 
 std::list<st_reflist> result;
 if (kinder && sorted && artikel)
 { static PreparedQuery pq;
   if (pq.Command().empty()) pq=PreparedQuery(squery);
   (Query(pq) << aeb).FetchArray(result);
 }
 else if (kinder && !sorted && artikel)
 { static PreparedQuery pq;
   if (pq.Command().empty()) pq=PreparedQuery(squery);
   (Query(pq) << aeb).FetchArray(result);
 }
 else if (kinder && !sorted && !artikel)
 { static PreparedQuery pq;
   if (pq.Command().empty()) pq=PreparedQuery(squery);
   (Query(pq) << aeb).FetchArray(result);
 }
 else if (!kinder && sorted && !artikel)
 { static PreparedQuery pq;
   if (pq.Command().empty()) pq=PreparedQuery(squery);
   (Query(pq) << aeb).FetchArray(result);
 }
 else
 { (Query(squery).lvalue() << aeb).FetchArray(result);
 }
 return result;
}

AufEintragZu::list_t AufEintragZu::get_Referenz_list_without_child() const throw(SQLerror)
{return get_Referenz_list(*this,list_kinder,list_ohneArtikel);
}

// wer braucht das? class_instanz_trees.hh - Warum sabotiertst Du mich?
#if 1
std::list<cH_Kunde> AufEintragZu::get_Referenz_Kunden() const throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,*this);
 std::list<cH_Kunde> LK;
 std::list<st_reflist> vaeb = get_Referenz_listFull(false);

 for (std::list<st_reflist>::const_iterator i=vaeb.begin();i!=vaeb.end();++i)
  {
    int kdnr=(Query("select kundennr from auftrag "
    		"where (instanz,auftragid)=(?,?)")
    		<< i->AEB.InstanzID() << i->AEB.Id())
    	.FetchOne<int>(); 
    LK.push_back(cH_Kunde(kdnr));
  }
 LK.sort();
 LK.unique();
 return LK;
}
#endif

#if 0
// Komische Funktion!!!
// [wieso ist das in AufEintragZu?: Es ist zu speziell]
// nur 0er Aufträge
std::list<AufEintragBase> AufEintragZu::get_AufEintragList_from_Artikel
               (const ArtikelBase& artikel,ppsInstanz::ID nichtinstanz,AufStatVal status)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("artikel",artikel),NV("status",status),NV("instanz",instanz));
  std::list<AufEintragBase> L;
  
  (Query("select instanz,auftragid,zeilennr from auftragentry "
  	"where artikelid=? "
  	"and instanz<>? " //Nur interne Auftr�e sind interessant
  	"and auftragid=0 " // Nur die UNGEPLANTEN Auftr�e bekommen neue Kinder
  	+(status!=NOSTAT ? " and status="+itos(status) : std::string())+
  	"order by auftragid,zeilennr").lvalue()
    << artikel.Id() << int(ppsInstanzID::Kundenauftraege))
  .FetchArray(L);
  return L;
}
#endif

void AufEintragZu::Neu(const AufEintragBase& neuAEB,const mengen_t menge)
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,
      "--",menge,"-->",neuAEB);

 // erst erh�en versuchen 
 if (!menge)
 {  bool da;
    Query("select exists (select true from auftragsentryzuordnung where"
         "(altinstanz,altauftragid,altzeilennr, "
           "neuinstanz,neuauftragid,neuzeilennr)="
         "(?,?,?, ?,?,?))")
      << *this << neuAEB
      >> da;
    if (da) return;
 }
 else if (menge<0) 
 {  mengen_t mt=setMengeDiff__(neuAEB,menge);   
    assert(AufEintragBase::tolerate_inconsistency || menge==mt);
    return;
 }
 // menge >= 0
 {  Query("insert into auftragsentryzuordnung "
         "(altinstanz,altauftragid,altzeilennr, menge, "
           "neuinstanz,neuauftragid,neuzeilennr, prioritaet) "
         "values (?,?,?, ?, ?,?,?, "
            "cast(timeofday() as timestamp with time zone))")
      << static_cast<const AufEintragBase &>(*this) << menge << neuAEB;
    SQLerror::test(__FILELINE__);
 }
}

namespace { struct pri_menge
{  ManuProC::TimeStamp pri;
   AuftragBase::mengen_t menge;
};}

Query::Row &operator>>(Query::Row &is,pri_menge &a)
{  return is >> a.pri >> a.menge;
}

AuftragBase::mengen_t AufEintragZu::setMengeDiff__(const AufEintragBase &neuAEB,mengen_t menge)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,neuAEB,NV("Menge",menge));
 if (menge>=0) { Neu(neuAEB,menge); return menge; }
// if(menge<0)
  {  Transaction tr;
     std::vector<pri_menge> V;
     (Query("select prioritaet,menge from auftragsentryzuordnung "
     	"where (altinstanz,altauftragid,altzeilennr, "
     	       "neuinstanz,neuauftragid,neuzeilennr)= (?,?,?, ?,?,?) "
     	       "order by prioritaet desc "
     	       "for update")
	<< static_cast<const AufEintragBase&>(*this) << neuAEB)
	.FetchArray(V);
     mengen_t noch_verteilen=menge;
     for (std::vector<pri_menge>::iterator i=V.begin();i!=V.end();)
     {  mengen_t M=-AuftragBase::min(i->menge,-noch_verteilen);
        ManuProC::Trace _t(trace_channel, __FILELINE__,
        	NV("noch_verteilen",noch_verteilen),NV("pri",i->pri),
        	NV("menge",i->menge), NV("M",M));
        if (!M) goto delete_if_possible;

        Query("update auftragsentryzuordnung set menge=menge+? "
     	"where (altinstanz,altauftragid,altzeilennr, "
     	       "neuinstanz,neuauftragid,neuzeilennr, "
     	       "prioritaet)= (?,?,?, ?,?,?, ?) ")
     	   << M
     	   << static_cast<const AufEintragBase&>(*this) << neuAEB
     	   << i->pri;
     	SQLerror::test("AufEintragZu::setMengeDiff__");
     	noch_verteilen-=M;
     	i->menge+=M;
      delete_if_possible: 
        if (!i->menge)
        {  if (Id()==dispo_auftrag_id || neuAEB.Id()==ungeplante_id
        	|| Id()==ungeplante_id 
           	|| V.size()>1)
           {  remove(*this,neuAEB,i->pri);
              i=V.erase(i);
              goto loop_test;
           }
        }
     	// if (!noch_verteilen) break; 
     	// Optimierung lohnt nicht, da eventuell noch etwas gel�cht werden k�nte
        ++i;
      loop_test:
        ;
     }
     tr.commit();
     return menge-noch_verteilen;
  }
}

void AufEintragZu::moveInstanz(const VonNachDel vdl,const AufEintragBase &oldAEB, const AufEintragBase &newAEB) throw(SQLerror)
{

 ManuProC::Trace _t(trace_channel, __FUNCTION__,oldAEB,NV("VonNachDel",vdl));

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
 ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,aeb);
 return (Query("select sum(menge) from auftragsentryzuordnung "
    "where (altinstanz,altauftragid,altzeilennr, "
    	"neuinstanz,neuauftragid,neuzeilennr) = (?,?,?, ?,?,?)")
    << static_cast<const AufEintragBase&>(*this) << aeb).FetchOne<int>();
}

bool AufEintragZu::remove(const AufEintragBase& alt,const AufEintragBase& neu,
	const ManuProC::TimeStamp &ts)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,alt,neu,ts);

  if (!ts.valid())
     return (Query("delete from auftragsentryzuordnung where "
                 "(altinstanz,altauftragid,altzeilennr,"
                 "neuinstanz,neuauftragid,neuzeilennr)"
                 "=(?,?,?, ?,?,?)").lvalue()
      << alt << neu).LinesAffected();
  else
     return (Query("delete from auftragsentryzuordnung where "
                 "(altinstanz,altauftragid,altzeilennr,"
                 "neuinstanz,neuauftragid,neuzeilennr, "
                 "prioritaet)"
                 "=(?,?,?, ?,?,?, ?)")
      << alt << neu << ts).LinesAffected();
}

AufEintragZu::mengen_t AufEintragZu::Summe(const list_t &l,ID wovon)
{  mengen_t result;
   for (list_t::const_iterator i=l.begin(); i!=l.end(); ++i)
      if (wovon==none_id || i->AEB.Id()==wovon)
         result+=i->Menge;
   return result;
}

bool AufEintragZu_sort::auftr_1230(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b)
{  int order=0;
   if (a.AEB.Id()==AuftragBase::ungeplante_id) order+=1;
   if (b.AEB.Id()==AuftragBase::ungeplante_id) order-=1;
   if (order) return order<0;
   return AufEintragZu_sort::auftr_0123(a,b);
}

bool AufEintragZu_sort::priority(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b)
{  int order=0;
   if (a.AEB.Id()==AuftragBase::dispo_id) order+=1;
   if (b.AEB.Id()==AuftragBase::dispo_id) order-=1;
   if (order) return order<0;
   return a.Pri<b.Pri || (a.Pri==b.Pri && a.AEB<b.AEB);
}

bool AufEintragZu_sort::auftr_0123(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b)
{  return a.AEB<b.AEB || (a.AEB==b.AEB && a.Pri<b.Pri);
}

bool AufEintragZu_sort::auftr_43210(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b)
{  return AufEintragZu_sort::auftr_0123(b,a);
}

bool AufEintragZu_sort::auftr_34012(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b)
{  int order=0;
   if (a.AEB.Id()<AuftragBase::handplan_auftrag_id) order+=1;
   if (b.AEB.Id()<AuftragBase::handplan_auftrag_id) order-=1;
   if (order) return order<0;
   return AufEintragZu_sort::auftr_0123(a,b);
}

AufEintragZu::mengen_t Summe(const AufEintragZu::list_t &l)
{  AufEintragZu::mengen_t res;
   for (AufEintragZu::list_t::const_iterator i=l.begin();i!=l.end();++i)
      res+=i->Menge;
   return res;
}
