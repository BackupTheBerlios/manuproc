/* $Id: LieferscheinEntry.cc,v 1.9 2002/09/18 08:58:34 christof Exp $ */
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

bool LieferscheinEntry::Valid() const
{
  if(Zeile()!=0) return true;
  return false;
}

void LieferscheinEntry::setPalette(int p) throw(SQLerror)
{
  std::string Q="update lieferscheinentry set palette="+itos(p)
                 +" where (instanz,lfrsid,zeile)=("
                 +itos(Instanz())+","+itos(Id())+","+itos(Zeile())+")";
  Query::Execute(Q);
  SQLerror::test(__FILELINE__);
}


bool LieferscheinEntry::changeMenge(int stueck,mengen_t menge) throw(SQLerror)
{
  if(ZusatzInfo()) {cerr <<"Mengenänderung für Zusatzinfos nicht möglich\n" ;return false;}
  Transaction tr;
  mengen_t abmenge=Abschreibmenge(stueck,menge);

cout << "LE: "<<abmenge<<'\n';
  if(abmenge==mengen_t(0)) return true ;//nichts geändert

  if(RefAuftrag().valid()) // Keine Zusatzinfos
   {
cout << "A\t"<<RefAuftrag().valid()<<'\t'<<RefAuftrag().Id()<<'\t'<<Zeile()<<'\n';
     AufEintragBase AEB(RefAuftrag(),AufZeile());
     try{
       AufEintrag AE(AEB);
       mengen_t rest=AE.getRestStk();
//cout << abmenge<<'\t'<<rest<<'\t'<<(abmenge > rest ) <<'\n';
       if(abmenge > rest ) return false;
       updateLieferscheinMenge(stueck,menge);
       AE.abschreiben(abmenge,Id());
     }catch(AufEintrag::NoAEB_Error &e){cerr << AEB<<" existiert nicht\n"; return false;}
   }
  else // Zusatzinfos ODER kein Referenzauftrag
   {
cout <<"B\n";
     LieferscheinEntry LE=*this;
     std::vector<LieferscheinEntry> VLE;
     do
      {
        try{
        LE=LieferscheinEntry(LieferscheinEntryBase(Instanz(),Id(),1+LE.Zeile()));
cout << "a\n";
        if(LE.ZusatzInfo())
         {
           VLE.push_back(LE);
         }
        }catch(SQLerror &e) {cerr<< e<<'\n'; break;}
      } while (LE.ZusatzInfo()) ;
cout << "LS: HIer\n";
     bool ok=menge_bei_zusatzinfos_abschreiben(VLE,abmenge);
     if(!ok) return false;
     updateLieferscheinMenge(stueck,menge);
   }
  tr.commit();
  return true;
}


void LieferscheinEntry::updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror)
{
   std::string Q1="update lieferscheinentry set stueck="+itos(stueck)
         +", menge="+itos(menge)+" where (instanz,lfrsid,zeile)=("
         +itos(Instanz())+","+itos(Id())+","+itos(Zeile())+")";
   Query::Execute(Q1);
   SQLerror::test(__FILELINE__);
}

bool LieferscheinEntry::menge_bei_zusatzinfos_abschreiben(std::vector<LieferscheinEntry>& VLE,mengen_t abmenge)
{
 if(VLE.empty()) return true; // Zeile ohne Referenzauftrag
 // Reduziern
 if(abmenge<mengen_t(0))
  {
  for(std::vector<LieferscheinEntry>::reverse_iterator i=VLE.rbegin();i!=VLE.rend();++i)
   {
    if(i->Stueck()!=1 && i->Menge()!=mengen_t(0)) return false; //das gibt es nicht
    mengen_t aufmenge=i->Stueck();
    if(i->Menge()!=mengen_t(0)) aufmenge*=i->Menge();
    mengen_t M;
    if(aufmenge<=abs(abmenge)) M=-aufmenge;
    else                       M=abmenge;
    if(i->RefAuftrag().valid()) 
     { AufEintragBase AEB(i->RefAuftrag(),i->AufZeile());
       AufEintrag(AEB).abschreiben(M,Id());
     }
    bool del_line=false;
    if(i->Stueck()==1)
     {
       i->updateLieferscheinMenge(1,i->Menge()+M);
       if(i->Menge()+M==mengen_t(0)) del_line=true; // Zeile löschen
     }
    else if(i->Menge()==mengen_t(0))
     { 
       i->updateLieferscheinMenge(i->Stueck()+int(M),mengen_t(0));
       if(i->Stueck()+int(M)==0) del_line=true; // Zeile löschen
     }
    else assert(!"");
    if(del_line)
     {
      std::string Q1 = "delete from lieferscheinentry where "
        "(instanz,lfrsid,zeile) = ("
        +itos(i->Instanz())+","+itos(i->Id())+","+itos(i->Zeile())+")";
      Query::Execute(Q1);
      SQLerror::test(__FILELINE__);
     }
    abmenge -=M;
    if(abs(abmenge)<=mengen_t(0)) break;
   }
  } 
 else
   {
return false;
   }
return true;
}

LieferscheinBase::mengen_t LieferscheinEntry::Abschreibmenge(int stueck,mengen_t menge) const
{
   int stueckdiff =   stueck - Stueck();
   mengen_t mengediff = menge - Menge();
   if(!stueckdiff && !mengediff) return mengen_t(0); // nichts geändert
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
