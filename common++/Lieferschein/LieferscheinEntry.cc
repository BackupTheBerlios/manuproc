/* $Id: LieferscheinEntry.cc,v 1.15 2002/11/07 07:48:59 christof Exp $ */
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

#include"LieferscheinEntry.h"
#include<Aux/FetchIStream.h>
#include<Aux/Transaction.h>
#include<Auftrag/AufEintrag.h>
#include <unistd.h>
#include <Lieferschein/Lieferschein.h>
#include <Instanzen/ppsInstanzProduziert.h>

bool LieferscheinEntry::Valid() const
{
  if(Zeile()!=0) return true;
  return false;
}

void LieferscheinEntry::setPalette(int p) throw(SQLerror)
{
  std::string Q="update lieferscheinentry set palette="+itos(p)
                 +" where (instanz,lfrsid,zeile)=("
                 +itos(Instanz()->Id())+","+itos(Id())+","+itos(Zeile())+")";
  Query::Execute(Q);
  SQLerror::test(__FILELINE__);
}


bool LieferscheinEntry::changeMenge(int stueck,mengen_t menge) throw(SQLerror)
{
  if(stueck==Stueck() && menge==Menge()) return true ; //nichts ge�ndert

  Transaction tr;
  Query::Execute("lock table lieferscheinentry in exclusive mode");

  AuftragBase::mengen_t abmenge=Abschreibmenge(stueck,menge);

  if(RefAuftrag().valid()) // => Keine Zusatzinfos
   {
     AufEintragBase AEB(RefAuftrag(),AufZeile());
     try{
       AufEintrag AE(AEB);
       if     (abmenge>AuftragBase::mengen_t(0) &&  abmenge > AE.getRestStk()) return false;
       else if(abmenge<AuftragBase::mengen_t(0) && -abmenge > AE.getGeliefert())  return false;
       AE.Produziert(abmenge,Id());
     }catch(AufEintrag::NoAEB_Error &e){std::cerr << AEB<<" existiert nicht\n"; return false;}
   }
  else if(!ZusatzInfo()) ;// s.u. updateLieferscheinMenge(stueck,menge) ;
  else // Zusatzinfos existieren
   {
     std::vector<LieferscheinEntry::st_zusatz> VZ=getZusatzInfos();
     if(abmenge>0) 
     for(std::vector<LieferscheinEntry::st_zusatz>::iterator i=VZ.begin();i!=VZ.end();++i)
      {
        AuftragBase::mengen_t actualmenge=abmenge;
        if(i->aeb.valid())
         {
           AufEintrag AE(i->aeb);
           if(AE.getEntryStatus()!=OPEN) continue;
           if(AE.getRestStk()<abmenge) actualmenge=AE.getRestStk();
           AE.Produziert(actualmenge,Id());
           updateZusatzEntry(*i,i->menge + actualmenge);
         }
        else 
           updateZusatzEntry(*i,i->menge + actualmenge);
        abmenge-=actualmenge;
        if(!abmenge) break;
      }
     else
     for(std::vector<LieferscheinEntry::st_zusatz>::reverse_iterator i=VZ.rbegin();i!=VZ.rend();++i)
      {
        AuftragBase::mengen_t actualmenge=abmenge;
        if(i->menge.abs() < actualmenge.abs()) actualmenge = -i->menge;
        if(i->menge + actualmenge==AuftragBase::mengen_t(0)) 
             deleteZusatzEntry(*i);
        else updateZusatzEntry(*i,i->menge + actualmenge);
        if(i->aeb.valid()) 
           AufEintrag(i->aeb).Produziert(actualmenge,Id());
        abmenge-=actualmenge;
        if(!abmenge) break;
      }
   }
  if(stueck==0) deleteMe(*this);
  else updateLieferscheinMenge(stueck,menge);

  tr.commit();
  return true;
}

void LieferscheinEntry::updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror)
{
   std::string Q1="update lieferscheinentry set stueck="+itos(stueck)
         +", menge=nullif("+menge.String()+",0) where (instanz,lfrsid,zeile)=("
         +itos(Instanz()->Id())+","+itos(Id())+","+itos(Zeile())+")";
   Query::Execute(Q1);
   SQLerror::test(__FILELINE__);
}

void LieferscheinEntry::deleteMe(const LieferscheinEntry &LE)  throw(SQLerror)
{
   std::string Q1="delete from lieferscheinentry "
         " where (instanz,lfrsid,zeile)=("
         +itos(LE.Instanz()->Id())+","+itos(LE.Id())+","+itos(LE.Zeile())+")";
   Query::Execute(Q1);
   SQLerror::test(__FILELINE__);
}

LieferscheinBase::mengen_t LieferscheinEntry::Abschreibmenge(int stueck,mengen_t menge) const
{
   int stueckdiff =   stueck - Stueck();
   mengen_t mengediff = menge - Menge();
//cout << stueck<<'-'<<Stueck()<<'='<<stueckdiff<<'\n';
   if(!stueckdiff && !mengediff) return mengen_t(0); // nichts ge�ndert
   mengen_t abmenge;
   if(!menge && stueckdiff)  abmenge=stueckdiff;
   else 
     {  
       if      (menge==Menge())   abmenge = stueckdiff*menge;
       else if (stueck==Stueck()) abmenge = stueck*mengediff;
       else  abmenge = menge*mengen_t(stueck) - Menge()*mengen_t(Stueck());
     }
  return abmenge;
}

void LieferscheinEntry::updateZusatzEntry(const st_zusatz &Z,const AuftragBase::mengen_t &menge) throw(SQLerror)
{
  std::string Q="update lieferscheinentryzusatz set menge="
      +menge.String()+" where (instanz,lfrsid,lfsznr) = ("
            +itos(Instanz()->Id())+","
            +itos(Id())+","+itos(Zeile())+") and ";

  if(Z.aeb.Id()==ManuProcEntity<>::none_id)
    Q += "auftragid is null and auftragznr is null"; 
  else 
    Q += "(auftragid,auftragznr)=("
         +itos(Z.aeb.Id())+","+itos(Z.aeb.ZNr())+")";

  Query::Execute(Q);
  SQLerror::test(__FILELINE__);
}

void LieferscheinEntry::deleteZusatzEntry(const st_zusatz &Z) throw(SQLerror)
{
  std::string Q="delete from lieferscheinentryzusatz "
      " where (instanz,lfrsid,lfsznr) = ("
            +itos(Instanz()->Id())+","
            +itos(Id())+","+itos(Zeile())+") and ";

  if(Z.aeb.Id()==ManuProcEntity<>::none_id)
    Q += "auftragid is null and auftragznr is null"; 
  else 
    Q += "(auftragid,auftragznr)=("
         +itos(Z.aeb.Id())+","+itos(Z.aeb.ZNr())+")";

  Query::Execute(Q);
  SQLerror::test(__FILELINE__);
}

