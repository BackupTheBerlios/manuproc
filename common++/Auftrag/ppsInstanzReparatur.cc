/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include <Auftrag/ppsInstanzReparatur.h>
#include <Auftrag/Auftrag.h>
#include <Lager/Lager.h>
#include <Lager/JumboLager.h>
#include <Lager/RohwarenLager.h>
//#include <Misc/SQLerror.h>
#include <Misc/TraceNV.h>
//#include <sqlca.h>
//#include <Auftrag/AufEintragZuMengenAenderung.h>
//#include <Misc/Transaction.h>
#include <Misc/relops.h>
#include <unistd.h>
#include <Misc/inbetween.h>
#include <Misc/compiler_ports.h>
#include <Artikel/ArtikelStamm.h>
#include <math.h>

bool ppsInstanzReparatur::Reparatur_0er_und_2er(SelectedFullAufList &al, const bool analyse_only) const throw(SQLerror)
{  unsigned uid=getuid();
   bool alles_ok=true;
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,analyse_only);
   assert(Instanz() != ppsInstanzID::Kundenauftraege);
   
   // Vorsicht, dass al hiernach nicht verändert wird (remove/push)
   std::vector<SelectedFullAufList::iterator> zweier;
   for(SelectedFullAufList::iterator i=al.begin();i!=al.end();++i)
   {  if (i->Id()==AuftragBase::dispo_auftrag_id)
         zweier.push_back(i);
   }
   
   for(SelectedFullAufList::iterator i=al.begin();i!=al.end();++i)
    {  if (i->Id()!=AuftragBase::ungeplante_id) continue;
       AuftragBase::mengen_t menge0er=i->getRestStk();
       if (!menge0er) continue;
       
      for(std::vector<SelectedFullAufList::iterator>::const_iterator j=zweier.begin();j!=zweier.end();++j)
       {  assert ((*j)->Id()==AuftragBase::dispo_auftrag_id);
          if ((*j)->Artikel()!=i->Artikel()) continue;
         if((*j)->getLieferdatum()>i->getLieferdatum()) continue;
         
         if (!(*j)->getRestStk()) continue;

         AuftragBase::mengen_t M=AuftragBase::min(menge0er,(*j)->getRestStk());
         
         analyse("Es existieren passende 0er und 2er",*i,**j,M);
         alles_ok=false;
         
         if(!analyse_only)
          {  AuftragBase::mengen_t M_rest=M;
             AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*i,
            		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
             for(AufEintragZu::list_t::reverse_iterator k=L.rbegin();k!=L.rend();++k)
             { AuftragBase::mengen_t M2=AuftragBase::min(k->Menge,M_rest);
                if (!M2) continue;

                i->MengeAendern(uid,-M2,true,k->AEB,ManuProC::Auftrag::r_Reparatur);
                AufEintrag::ArtikelInternNachbestellen(Instanz(),M2,i->getLieferdatum(),
                		i->Artikel(),uid,k->AEB);

                M_rest-=M2;
                if(!M_rest) break;
             }
             if (!!M_rest) analyse("Programmfehler: Es ist ein Rest geblieben",*i,M_rest);
             assert(!M_rest);
          }
         menge0er-=M;
         if(!menge0er) break;
       }
    }
  return alles_ok;
}


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool ppsInstanzReparatur::ReparaturLager(const int uid,const bool analyse_only) const throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,Instanz());
  assert(Instanz()->LagerInstanz());
  std::vector<LagerInhalt> LI=getLagerInhalt(); 
  return vormerkungen_subtrahieren(uid,LI,analyse_only);
}

bool ppsInstanzReparatur::vormerkungen_subtrahieren(int uid,const  std::vector<LagerInhalt> &LI,const bool analyse_only) const
{ bool looped=false;
  bool alles_ok=true;
try_again:
//std::cout << "Anzahl der Artikel im Lager = "<<LI.size()<<'\n';
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,Instanz());
  for(std::vector<LagerInhalt>::const_iterator i=LI.begin();i!=LI.end();++i)
   {
     bool set_dispo_to_zero=false;
     AuftragBase::mengen_t menge=i->GesamtMenge();

     ManuProC::Trace(AuftragBase::trace_channel, __FILELINE__,
     		NV("artikel",i->Artikel()),NV("id",i->Artikel().Id()),
     		NV("menge",menge));

     // Vorgemerkte Menge (1er Aufträge) wieder abziehen
     // Schon eingetragene Menge wieder abziehen
     SelectedFullAufList auftraglist1=SelectedFullAufList(SQLFullAuftragSelector::
       sel_Artikel_Planung_id(Instanz()->Id(),ManuProC::DefaultValues::EigeneKundenId,i->Artikel(),AuftragBase::plan_auftrag_id));
     // der Selector holt nur die Aufträge mit dem Status OPEN
     for (SelectedFullAufList::iterator j=auftraglist1.begin();j!=auftraglist1.end();++j)
       {
         assert(j->Id()==AuftragBase::plan_auftrag_id);
         menge-=j->getRestStk() ;
         if(menge<0) // mehr Menge vorgeplant als vorhanden
         {  analyse("mehr Menge vorgeplant als vorhanden",*j,
         	cH_ArtikelBezeichnung(i->Artikel())->Bezeichnung(),
         	menge.String());
            alles_ok=false;
            set_dispo_to_zero=true;
            if(!analyse_only)
            {AuftragBase::mengen_t M_rest=-menge;
             AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*j,
            		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
             for(AufEintragZu::list_t::reverse_iterator k=L.rbegin();k!=L.rend();++k)
             {  AuftragBase::mengen_t M2=AuftragBase::min(k->Menge,M_rest);
                if (!M2) continue;

		// eine Rekursion würde die Menge neu verplanen ...
                j->MengeAendern(uid,-M2,false,k->AEB,ManuProC::Auftrag::r_Reparatur);
                AufEintrag::ArtikelInternNachbestellen(Instanz(),M2,j->getLieferdatum(),
                		j->Artikel(),uid,k->AEB);

                M_rest-=M2;
                if(!M_rest) break;
             }
             if (!!M_rest) 
             {  analyse("Es ist ein Rest geblieben, erneuter Versuch",*j,M_rest);
                if (!looped)
                {  looped=true;
                   goto try_again;
                }
                analyse("Programmfehler? Es ist ein Rest geblieben",*j,M_rest);
             }
            }
            menge=0;
           }       
        }

     SelectedFullAufList auftraglist2=SelectedFullAufList(SQLFullAuftragSelector::
          sel_Artikel_Planung_id(Instanz()->Id(),ManuProC::DefaultValues::EigeneKundenId,i->Artikel(),AuftragBase::dispo_auftrag_id));
     assert(auftraglist2.empty() || auftraglist2.size()==1);
     for (SelectedFullAufList::iterator j=auftraglist2.begin();j!=auftraglist2.end();++j)
      {
         if(j->Id()!=AuftragBase::dispo_auftrag_id) assert(!"never get here");
         assert(j->getStueck()==j->getRestStk());
         menge-=j->getRestStk();
         if(set_dispo_to_zero && !!j->getStueck())
          { analyse("set_dispo_to_zero",*j,j->getStueck());
            alles_ok=false;
            if(!analyse_only)
               j->MengeAendern(uid,-j->getStueck(),false,AufEintragBase(),ManuProC::Auftrag::r_Reparatur);
          }
      }
      
     if(menge>0 && !set_dispo_to_zero) 
      { alles_ok=false;
        analyse("DispoAufträge_anlegen",AufEintragBase(Instanz(),2,-1),cH_ArtikelBezeichnung(i->Artikel())->Bezeichnung(),menge.String());
        if (!analyse_only)
            DispoAuftraege_anlegen(uid,i->Artikel(),menge);
      }
   }
   return alles_ok;
}   


void ppsInstanzReparatur::DispoAuftraege_anlegen(const int uid,const ArtikelBase &artikel,const AuftragBase::mengen_t &menge) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,Instanz());
   assert(Instanz()->EigeneLagerKlasseImplementiert());
std::cout << "Mengenänderung im Lager "<<Instanz()<<'\t'<<menge<<" von " << artikel.Id() << '\n';
   if(menge>=0)
      LagerBase(make_value(Instanz())).rein_ins_lager(artikel,menge,uid);
}

std::vector<LagerInhalt> ppsInstanzReparatur::getLagerInhalt() const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,Instanz());
  std::vector<LagerInhalt> LI;
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
  if(Instanz() == ppsInstanzID::Rohwarenlager)  LI=RohwarenLager().LagerInhalt();
  else if(Instanz() == ppsInstanzID::Bandlager) LI=JumboLager().LagerInhalt();
  else 
#endif 
   { std::cout << Instanz()<<' '<<"\tKeine LagerKlasse implementiert\n";
     assert(!"never get here\n");
     return LI;
   }
  LagerBase::LagerInhaltSum(LI);
  return LI;
}

////////////////////////////////////////////////////////////////////////////////

void ppsInstanzReparatur::analyse(const std::string &s,const AufEintragBase &AE,const std::string &x,const std::string &y) const
{
  std::cout<<"Fehler Analyse: " << AE <<"  => "<<s<<"\t("<<x<<"), ("<<y<<")\n";
}

void ppsInstanzReparatur::analyse(const std::string &s,const AufEintragBase &AE,const ABmt &x,const ABmt &y) const
{analyse(s,AE,x.String(),y.String());}

void ppsInstanzReparatur::analyse(const std::string &s,const AufEintragBase &AE,const AufEintragBase &x,const ABmt &y) const
{analyse(s,AE,x.Instanz()->Name()+"/"+itos(x.Id())+"/"+itos(x.ZNr()),y.String());}

void ppsInstanzReparatur::analyse(const std::string &s,const AufEintragBase &AE,const ArtikelBase &x,const ArtikelBase &y) const
{analyse(s,AE,itos(x.Id()),itos(y.Id()));}

void ppsInstanzReparatur::analyse(const std::string &s,const AufEintragBase &AE,const cH_ppsInstanz &x,const cH_ppsInstanz &y) const
{analyse(s,AE,x->Name(),y->Name());}

void ppsInstanzReparatur::Zuordnung_erniedrigen(AufEintrag &ae,
	AufEintragZu::list_t &eltern,AuftragBase::mengen_t &m,
	AuftragBase::ID typ)
{  for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();++i)
   {  if ((typ==AuftragBase::handplan_auftrag_id && i->AEB.Id()<typ)
   		|| i->AEB.Id()!=typ) continue;
      AuftragBase::mengen_t M=AuftragBase::min(m,i->Menge);
      if (!M) continue;
      AufEintragZu(i->AEB).setMengeDiff__(ae,-M);
      i->Menge-=M;
      m-=M;
      if (!m) break;
   }
}

void ppsInstanzReparatur::KinderErniedrigen(AufEintrag &ae,
	AufEintragZu::list_t &kinder,AuftragBase::mengen_t m,unsigned uid)
{  AufEintrag::ArtikelInternAbbestellen_cb aia(ae,uid,ManuProC::Auftrag::r_Reparatur);
   for (AufEintragZu::list_t::iterator i=kinder.begin();i!=kinder.end();++i)
   {  AuftragBase::mengen_t M=AuftragBase::min(m,i->Menge);
      if (!M) continue;
      M=aia(ArtikelBase(),i->AEB,M);
      i->Menge-=M;
      m-=M;
      if (!m) break;
   }
}

bool ppsInstanzReparatur::Eltern(AufEintrag &ae, AufEintragZu::list_t &eltern, bool analyse_only, bool raise_prodselbst) const
{  // 2er und Kundenaufträge dürfen keine Kinder haben!
   unsigned uid=getuid();
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,ae,/*eltern.size(),*/analyse_only);
   if (ae.Id()==AuftragBase::dispo_auftrag_id || ae.Instanz()==ppsInstanzID::Kundenauftraege)
   {  if (!eltern.empty())
      {  analyse("2er und Kundenaufträge dürfen keine Kinder haben!",ae);
         for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();)
         {  if (!analyse_only) AufEintragZu::remove(i->AEB,ae);
            i=eltern.erase(i);
         }
         return false;
      }
      return true;
   }
   // 
   bool alles_ok=true;
   for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();)
   {  if (ae.Instanz()==i->AEB.Instanz())
      {  if (i->AEB.Id()!=AuftragBase::dispo_auftrag_id)
         {  analyse("Eltern auf gleicher Instanz müssen 2er sein",ae,i->AEB,i->Menge);
           weg:
            if (!analyse_only) AufEintragZu::remove(i->AEB,ae);
            i=eltern.erase(i);
            alles_ok=false;
            continue;
         }
         AufEintrag ae2(i->AEB);
         if (ae2.Artikel()!=ae.Artikel()
         	|| ae2.getLieferdatum()!=ae.getLieferdatum()) 
         {  analyse("Artikel oder Datum passt nicht",ae,i->AEB,i->Menge);
            goto weg;
         }
      }
      else
      {  if (i->AEB.Id()==AuftragBase::dispo_auftrag_id) 
         {  analyse("2er müssen auf der gleichen instanz liegen",ae,i->AEB,i->Menge);
            goto weg;
         }
         // schadet nicht ... ist aber wohl eher die Angelegenheit des 1ers
         if (i->AEB.Id()==AuftragBase::plan_auftrag_id && !!i->Menge)
         {  analyse("1er können keine Menge bestellen",ae,i->AEB,i->Menge);
            if (!analyse_only) AufEintragZu(i->AEB).setMenge(ae,0);
            i->Menge=0;
            alles_ok=false;
         }
      }
      ++i;
   }
   
   // schauen ob Sum(eltern)=offeneMenge,
   // wenn produziert selbst kann Sum(eltern) niedriger sein (sollte aber nicht)
   AufEintragBase::mengen_t menge2,menge;
   for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();++i)
   {  menge+=i->Menge;
      if (i->AEB.Id()==AuftragBase::dispo_auftrag_id)
      {  menge2+=i->Menge;
         if (menge2>ae.getRestStk())
         {  analyse("Mehr Dispomenge als noch frei",ae,i->AEB,menge2-ae.getRestStk());
            if (!analyse_only) 
               AufEintragZu(i->AEB).setMengeDiff__(ae,ae.getRestStk()-menge2);
            i->Menge+=ae.getRestStk()-menge2;
            // vielleicht noch den Ziel 2er reduzieren?
            alles_ok=false;
         }
      }
   }
   if (menge>ae.getRestStk()
   	&& (!ae.Instanz()->ProduziertSelbst() || raise_prodselbst))
   {  analyse("mehr v.o. benötigt als noch offen",ae,menge,ae.getStueck());
      alles_ok=false;
      if (!analyse_only)
      {if (ae.Id()==AuftragBase::ungeplante_id) 
         ae.MengeAendern(uid,menge-ae.getStueck(),true,AufEintragBase(),
         	ManuProC::Auftrag::r_Reparatur);
       else
       { // Zuordnung erniedrigen, Reihenfolge: 2,0,1,3)
         // danach müssen die Eltern neu bestellen (Reparatur)!
         AuftragBase::mengen_t m=menge-ae.getRestStk(); // positiv
         Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::dispo_auftrag_id);
         if (!!m) Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::ungeplante_id);
         if (!!m) Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::plan_auftrag_id);
         if (!!m) Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::handplan_auftrag_id);
       }
      }
   }
   // Sum zu groß: nachbestellen (kann nur bei ProduziertSelbst passieren)
   else if (menge>ae.getStueck())
   {  analyse("mehr v.o. benötigt als jemals bestellt",ae,menge,ae.getStueck());
      alles_ok=false;
      if (!analyse_only)
      {if (ae.Id()==AuftragBase::ungeplante_id) 
         ae.MengeAendern(uid,menge-ae.getStueck(),true,AufEintragBase(),
         	ManuProC::Auftrag::r_Reparatur);
       else
       { // Zuordnung erniedrigen, Reihenfolge: 2,0,1,3)
         // danach müssen die Eltern neu bestellen (Reparatur)!
         AuftragBase::mengen_t m=menge-ae.getStueck(); // positiv
         Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::dispo_auftrag_id);
         if (!!m) Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::ungeplante_id);
         if (!!m) Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::plan_auftrag_id);
         if (!!m) Zuordnung_erniedrigen(ae,eltern,m,AuftragBase::handplan_auftrag_id);
       }
      }
   }
   // Sum zu klein: abbestellen (falls 0er, bei 1er 2er erzeugen)
   else if (menge<ae.getRestStk())
   {  analyse("mehr offen als nun v.o. benötigt",ae,menge,ae.getStueck());
      alles_ok=false;
      if (!analyse_only)
      {if (in(ae.Id(),AuftragBase::ungeplante_id,AuftragBase::plan_auftrag_id))
       {  assert(ae.Id()!=AuftragBase::plan_auftrag_id || ae.Instanz()->LagerInstanz());
          ae.MengeAendern(uid,menge-ae.getRestStk(),true,AufEintragBase(),
         	ManuProC::Auftrag::r_Reparatur);
       }
       else 
       { assert(ae.Id()>=AuftragBase::handplan_auftrag_id);
         // 2er erhöhen
         AuftragBase(ae.Instanz(),AuftragBase::dispo_auftrag_id).
              BestellmengeAendern(ae.getRestStk()-menge,ae.getLieferdatum(),
              		ae.Artikel(),OPEN,uid,ae);
       }
      }
   }
   return alles_ok;
}

bool ppsInstanzReparatur::Kinder(AufEintrag &ae, AufEintragZu::map_t &kinder, bool analyse_only) const
{  bool alles_ok=true;
   unsigned uid=getuid();
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,ae,/*kinder.size(),*/analyse_only);
   
   if (ae.Id()==AuftragBase::dispo_auftrag_id)
   {  // 2er: Kinder gleiche instanz
      AuftragBase::mengen_t menge2;
      
      for (AufEintragZu::map_t::iterator i=kinder.begin();i!=kinder.end();++i)
      {  for (AufEintragZu::list_t::iterator j=i->second.begin();j!=i->second.end();)
         {  if (ae.Instanz()->LagerInstanz())
            {  analyse("Ein Lager 2er darf keine Kinder haben",ae,j->AEB,j->Menge);
             weg:
               if (!analyse_only) AufEintragZu::remove(ae,j->AEB);
               j=i->second.erase(j);
               alles_ok=false;
               continue;
            }
            if (j->AEB.Instanz()!=ae.Instanz())
            {  analyse("Instanz passt nicht",ae,j->AEB,j->Menge);
               goto weg;
            }
            if (j->AEB.Id()<AuftragBase::handplan_auftrag_id)
            {  analyse("Kind kein 3er",ae,j->AEB,j->Menge);
               goto weg;
            }
            AufEintrag ae2(j->AEB);
            if (ae2.Artikel()!=ae.Artikel()
         	|| ae2.getLieferdatum()!=ae.getLieferdatum()) 
            {  analyse("Artikel oder Datum passt nicht",ae,j->AEB,j->Menge);
               goto weg;
            }
            menge2+=j->Menge;
            ++j;
         }
      }
      if (!ae.Instanz()->LagerInstanz() && menge2!=ae.getStueck())
      {  analyse("2er: Zuordnungen!=eigene Menge",ae,menge2,ae.getStueck());
         if (!analyse_only) 
            ae.MengeAendern(uid,menge2-ae.getStueck(),false,
            		AufEintragBase(),ManuProC::Auftrag::r_Reparatur);
      }
   }
   else // 0er, 1er, 3er
   {  ArtikelBaum ab(ae.Artikel());
      ArtikelStamm as(ae.Artikel());
      ppsInstanz::ID next=ae.Instanz()->NaechsteInstanz(as);
      ManuProC::Datum newdate=ae.getLieferdatum()-ae.Instanz()->ProduktionsDauer();
      
      for (AufEintragZu::map_t::iterator i=kinder.begin();i!=kinder.end();++i)
      {  AuftragBase::mengen_t menge;
         bool artikel_passt_nicht=false;
         ManuProC::Trace(AuftragBase::trace_channel,"-Artikel-",i->first/*,i->second.size()*/);
         cH_ppsInstanz bestellinstanz=next!=ppsInstanzID::None?
				cH_ppsInstanz(next)
                          :ppsInstanz::getBestellInstanz(i->first);
         for (AufEintragZu::list_t::iterator j=i->second.begin();j!=i->second.end();)
         {  if (next!=ppsInstanzID::None && j->AEB.Instanz()!=next)
            {  analyse("Instanz passt nicht",ae,j->AEB,j->Menge);
             weg1:
               if (!analyse_only) AufEintragZu::remove(ae,j->AEB);
               j=i->second.erase(j);
               alles_ok=false;
               continue;
            }
            else if (next==ppsInstanzID::None)
            {  if (!ab.istKind(i->first))
               {  analyse("Kindartikel falsch "+itos(ab.Id())+" "+itos(i->first.Id()),ae,j->AEB,j->Menge);
                  artikel_passt_nicht=true;
                  goto weg1;
               }
               // Artikel ist schon mal richtig ...
               if (j->AEB.Instanz()!=bestellinstanz && !bestellinstanz->PlanungsInstanz())
               {  analyse("Kindartikel bei falscher Instanz",ae,j->AEB,j->Menge);
                  goto weg1;
               }
            }
            if (ae.Id()==AuftragBase::plan_auftrag_id && ae.Instanz()->LagerInstanz()
            	&& (!!j->Menge || j->AEB.Id()==AuftragBase::ungeplante_id))
            {  analyse("Ein Lager 1er darf keine Kinder >0||->0er haben",ae,j->AEB,j->Menge);
               goto weg1;
            }
            if (j->AEB.Id()==AuftragBase::dispo_auftrag_id)
            {  analyse("Kind darf kein 2er sein",ae,j->AEB,j->Menge);
               goto weg1;
            }
            if (ae.getEntryStatus()!=OPEN)
            {  if (!!j->Menge)
               {  analyse("Nichtoffene Aufträge dürfen nichts (mehr) bestellen",ae,j->AEB,j->Menge);
                  goto weg1;
               }
            }

            AufEintrag ae2(j->AEB);
            if (ae2.getLieferdatum()>newdate 
                // Ausnahme für unbestellte aber doch gelieferte Mengen
            	&& !(ae.Id()==AuftragBase::plan_auftrag_id 
            		&& !ae.Instanz()->LagerInstanz()
            		&& ae.getLieferdatum()==LagerBase::Lagerdatum()))
            {  analyse("Datum passt nicht",ae,j->AEB,j->Menge);
               goto weg1;
            }
            menge+=j->Menge;
            ++j;
         }
         // schauen ob offeneMenge=Sum(kinder)
         if (ae.Instanz()==ppsInstanzID::Kundenauftraege)
         {  if (ae.Artikel()!=i->first) // Artikel passt nicht!
               continue;
         }
         else // Intern
            if (artikel_passt_nicht 
            	|| (ae.Id()==AuftragBase::plan_auftrag_id && ae.Instanz()->LagerInstanz()))
               continue;
         
         AuftragBase::mengen_t richtigeMenge=
         	ae.getRestStk()*ab.Faktor(i->first);
         if (menge!=richtigeMenge && 
         	between<AuftragBase::mengen_t>(menge,
         		floor(ae.getRestStk()*ab.Faktor(i->first)),
         		richtigeMenge))
         {  analyse("Zuordnungen!=eigene RestMenge (aber innerhalb Rundungsfehler)",ae,menge,richtigeMenge);
         }
         else if (menge!=richtigeMenge)
         {  analyse("Zuordnungen!=eigene RestMenge",ae,menge,richtigeMenge);
            alles_ok=false;
            if (!analyse_only)
            {  if (menge<richtigeMenge) // Sum zu klein: nachbestellen
               {  AufEintrag::ArtikelInternNachbestellen(bestellinstanz,
                                richtigeMenge-menge,newdate,i->first,uid,ae);
               }
               else // mehr bestellt als noch offen: Bestellungen weg!
               {  KinderErniedrigen(ae,i->second,menge-richtigeMenge,uid);
               }
            }
         }
      }
      if (ae.Id()==AuftragBase::plan_auftrag_id && ae.Instanz()->LagerInstanz())
         goto exit;
      if (ae.getEntryStatus()!=OPEN) goto exit;
      // kam der Artikel überhaupt vor ?
      if (next!=ppsInstanzID::None)
      {  AufEintragZu::map_t::const_iterator f=kinder.find(ae.Artikel());
         AuftragBase::mengen_t M=ae.getRestStk();
         if (f==kinder.end() && !!M)
         {  alles_ok=false;
            analyse("Artikel fehlt intern völlig",ae,itos(ae.Artikel().Id()),M.String(true));
            if (!analyse_only)
            {  AufEintrag::ArtikelInternNachbestellen(next,M,newdate,
            				ae.Artikel(),uid,ae);
            }
         }
      }
      else for (ArtikelBaum::const_iterator i=ab.begin();i!=ab.end();++i)
      {  AufEintragZu::map_t::const_iterator f=kinder.find(i->rohartikel);
         AuftragBase::mengen_t M=ae.getRestStk()*i->menge;
         if (f==kinder.end() && !!M) // Artikel nie bestellt
         {  alles_ok=false;
            analyse("Rohartikel fehlt völlig",ae,itos(i->rohartikel.Id()),M.String(true));
            if (!analyse_only)
            {  AufEintrag::ArtikelInternNachbestellen(
            		ppsInstanz::getBestellInstanz(i->rohartikel),
            		M,newdate,i->rohartikel,uid,ae);
            }
         }
      }
   }
exit:
   return alles_ok;
}

bool ppsInstanzReparatur::Lokal(AufEintrag &ae, bool analyse_only) const
{  bool alles_ok=true;
   unsigned uid=getuid();
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,ae,analyse_only);
   if (in(ae.Id(),AuftragBase::dispo_auftrag_id,AuftragBase::ungeplante_id))
   {  if (!!ae.getGeliefert())
      {  alles_ok=false;
         analyse("Bei 0/2ern darf nichts geliefert worden sein.",ae);
         if (!analyse_only) 
            Query("update auftragentry set geliefert=0 "
         	"where (instanz,auftragid,zeilennr) = (?,?,?)")
         	<< ae;
//         ae.geliefert=0;
      }
      if (ae.getAufStatus()!=OPEN)
      {  alles_ok=false;
         analyse("0/2er Aufträge müssen offen sein",ae,AuftragBase::getStatusStr(ae.getAufStatus()));
         if (!analyse_only) Auftrag(ae).setStatusAuftrag_(OPEN);
      }
      if (ae.getEntryStatus()!=OPEN)
      {  alles_ok=false;
         analyse("0/2er Einträge müssen offen sein",ae);
         if (!analyse_only) ae.setStatus(OPEN,uid,true);
      }
   }
   else if (ae.Instanz()!=ppsInstanzID::Kundenauftraege 
   	&& ae.getGeliefert()>ae.getStueck())
   {  alles_ok=false;
      analyse("Geliefert muss kleiner gleich bestellt sein.",ae,ae.getGeliefert(),ae.getStueck());
      if (!analyse_only) 
         Query("update auftragentry set geliefert=? "
		"where (instanz,auftragid,zeilennr) = (?,?,?)")
		<< ae.getStueck() << ae;
      ae.geliefert=ae.getStueck();
   }
   
   if (ae.Instanz()!=ppsInstanzID::Kundenauftraege 
   	&& !ae.Instanz()->ExterneBestellung()
   	&& ae.getKdNr()!=Kunde::eigene_id)
   {  alles_ok=false;
      analyse("Interne Kundennr falsch",ae,ae.getKdNr());
      if (!analyse_only) 
         Query("update auftrag set kundennr=? "
		"where (instanz,auftragid) = (?,?)")
		<< Kunde::eigene_id << static_cast<AuftragBase&>(ae);
      ae.kdnr=Kunde::eigene_id;
   }
   
   ArtikelStamm as(ae.Artikel());
   if (ae.Instanz()!=ppsInstanzID::Kundenauftraege 
   	&& !in(ae.Instanz(),ppsInstanz::getBestellInstanz(as),
   			ppsInstanz::getProduktionsInstanz(as))
   	&& !ppsInstanz::getBestellInstanz(as)->PlanungsInstanz())
   {  alles_ok=false;
      analyse("Artikel auf falscher Instanz",ae,cH_ArtikelBezeichnung(ae.Artikel())->Bezeichnung(),itos(ae.Artikel().Id()));
      // den lösche ich aber nicht automatisch!
      std::cout << "$ delete from auftragentry where (instanz,auftragid,zeilennr)=("
      	<< ae.Instanz()->Id() << ',' << ae.Id() << ',' << ae.ZNr() << ");\n";
   }
   
   if (ae.Id()==AuftragBase::plan_auftrag_id)
   {  if (ae.Instanz()==ppsInstanzID::Kundenauftraege)
      {  alles_ok=false;   
         analyse("Es darf keine 1er bei den Kundenaufträgen geben",ae);
         // Bitte von Hand reparieren!
         std::cout << "$ delete from aufeintragbase where (instanz,auftragid,zeile)=("
 	     	<< ae.Instanz()->Id() << ',' << ae.Id() << ',' << ae.ZNr() << ");\n";
      }
      else if (!!ae.getRestStk() && !ae.Instanz()->LagerInstanz())
      {  alles_ok=false;
         analyse("ausgelieferte 1er Aufträge sollten CLOSED sein",ae,AuftragBase::getStatusStr(ae.getEntryStatus()));
         if (!analyse_only) ae.abschreiben(ae.getRestStk());
      }
      if (ae.getEntryStatus()!=CLOSED && !ae.Instanz()->LagerInstanz())
      {  alles_ok=false;
         analyse("1er Aufträge sollten CLOSED sein",ae,AuftragBase::getStatusStr(ae.getEntryStatus()));
         if (!analyse_only) ae.setStatus(CLOSED,uid,true);
      }
   }
   if (ae.Instanz()!=ppsInstanzID::Kundenauftraege
   	&& !in(ae.getEntryStatus(),OPEN,CLOSED))
   {  alles_ok=false;
      analyse("Interne Aufträge müssen OPEN/CLOSED sein",ae,AuftragBase::getStatusStr(ae.getEntryStatus()));
      if (!analyse_only) ae.setStatus(CLOSED,uid,true);
   }
   if (ae.getGeliefert()>=ae.getStueck()
   	&& ae.getEntryStatus()!=CLOSED
   	&& !(ae.Instanz()!=ppsInstanzID::Kundenauftraege 
   	    && in(ae.Id(),AuftragBase::dispo_auftrag_id,AuftragBase::ungeplante_id))
   	&& !(ae.Id()==AuftragBase::plan_auftrag_id && !ae.getStueck()))
   {  alles_ok=false;
      analyse("Erfüllte Aufträge müssen CLOSED sein",ae,ae.getGeliefert(),ae.getStueck());
      if (!analyse_only) ae.setStatus(CLOSED,uid,true);
   }
   else if (ae.Instanz()!=ppsInstanzID::Kundenauftraege 
   	&& ae.getGeliefert()!=ae.getStueck()
   	&& ae.getEntryStatus()!=OPEN)
   {  alles_ok=false;
      analyse("Offene interne Aufträge müssen OPEN sein",ae,ae.getGeliefert(),ae.getStueck());
      if (!analyse_only) ae.setStatus(OPEN,uid,true);
   }
   
   if (ae.Instanz()->LagerInstanz() 
   	&& ae.Id()==AuftragBase::dispo_auftrag_id
   	&& ae.getLieferdatum()!=LagerBase::Lagerdatum())
   {  alles_ok=false;
      analyse("Das Datum von 2ern im Lager sollte 'epoch' sein",ae);
      // Bitte von Hand reparieren!
   }
   return alles_ok;
}
