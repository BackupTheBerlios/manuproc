/* $Id: LieferscheinEntry.cc,v 1.11 2002/10/04 08:23:21 thoma Exp $ */
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
#include <Instanzen/Produziert.h>

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
  if(stueck==Stueck() && menge==Menge()) return true ; //nichts geändert

  Transaction tr;
  mengen_t abmenge=Abschreibmenge(stueck,menge);

  if(RefAuftrag().valid()) // Keine Zusatzinfos
   {
     AufEintragBase AEB(RefAuftrag(),AufZeile());
     try{
       AufEintrag AE(AEB);
       mengen_t rest=AE.getRestStk();
       if(abmenge > rest ) return false;
       updateLieferscheinMenge(stueck,menge);
//       Produziert(instanz->Id(),artikel,abmenge,getuid(),Id()).NichtSelbst();    
       Produziert(AE,abmenge,getuid(),Id()).NichtSelbst();    
//       AE.abschreiben(abmenge,Id());
     }catch(AufEintrag::NoAEB_Error &e){cerr << AEB<<" existiert nicht\n"; return false;}
   }
  else // Zusatzinfos ODER kein Referenzauftrag
   {
     LieferscheinEntry LE=*this;
     std::vector<LieferscheinEntry> VLE;
     do
      {
        try{
        LE=LieferscheinEntry(LieferscheinEntryBase(Instanz(),Id(),1+LE.Zeile()));
        if(LE.ZusatzInfo())  VLE.push_back(LE);
        }catch(SQLerror &e) {cerr<< e<<'\n'; break;}
      } while (LE.ZusatzInfo()) ;
//     bool ok=menge_bei_zusatzinfos_abschreiben(VLE,abmenge);
      menge_bei_zusatzinfos_abschreiben(VLE,stueck,menge);
//     if(!ok) return false;
     updateLieferscheinMenge(stueck,menge);
   }
  tr.commit();
  return true;
}


void LieferscheinEntry::updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror)
{
   std::string Q1="update lieferscheinentry set stueck="+itos(stueck)
         +", menge=nullif("+itos(menge)+",0) where (instanz,lfrsid,zeile)=("
         +itos(Instanz())+","+itos(Id())+","+itos(Zeile())+")";
   Query::Execute(Q1);
   SQLerror::test(__FILELINE__);
}

//bool LieferscheinEntry::menge_bei_zusatzinfos_abschreiben(std::vector<LieferscheinEntry>& VLE,mengen_t abmenge)
void LieferscheinEntry::menge_bei_zusatzinfos_abschreiben(std::vector<LieferscheinEntry>& VLE,int stueck,mengen_t menge)
{
  if(VLE.empty()) return;
//  AuftragBase::mengen_t kundenmenge(0);
  // Erhöhen
//cout << stueck<<' '<<menge<<'\n';
   AuftragBase::mengen_t km(0);
   int ks(0),AuftragS=0;
   for(std::vector<LieferscheinEntry>::iterator i=VLE.begin();i!=VLE.end();++i)
    {
      if(i->RefAuftrag().valid()) // Kundenauftrag vorhanden
       {
         AufEintrag AE(i->getAufEintragBase());
         if(AE.getEntryStatus()==CLOSED)
          {  ks+=i->Stueck(); km+=i->Menge(); }
       } // der Auftrag OHNE Kundenreferenzauftrag steht am Ende
      else 
       {
         int S=stueck-ks;
         AuftragBase::mengen_t M=menge-km;
         if(S<0) { AuftragS=S;S=0;} 
         i->updateLieferscheinMenge(S,M);
       }
    } 

  // Jetzt die Kundenaufträge behandeln
//  bool del_line=false;
  if(AuftragS)
   {
    assert(AuftragS<0);
//cout <<"AAAAAA\n";
    mengen_t abmenge=Abschreibmenge(stueck,menge);
    for(std::vector<LieferscheinEntry>::reverse_iterator i=VLE.rbegin();i!=VLE.rend();++i)
     {
      if(i->RefAuftrag().valid()) // Kundenauftrag vorhanden
       {
         AufEintrag AE(i->getAufEintragBase());
         mengen_t M=AuftragS;
         if(AE.getStueck()<M) M=-AE.getStueck();
//cout << "Be HERE\t"<<i->Stueck()<<' '<<i->Menge()<<'\t'<<M<<'\n';
//         Produziert(instanz->Id(),artikel,M,getuid(),Id()).NichtSelbst();    
         Produziert(AE,M,getuid(),Id()).NichtSelbst();    
//         AE.abschreiben(M,Id());
         // Lieferscheinentry:
         if(i->Stueck()==1)
             i->updateLieferscheinMenge(1,i->Menge()+M);
         else if(i->Menge()==mengen_t(0))
             i->updateLieferscheinMenge(i->Stueck()+int(M),mengen_t(0));
         else assert(!"");
         abmenge+=M;
         if(!abmenge) break;
       }
     }
   }  

   for(std::vector<LieferscheinEntry>::iterator i=VLE.begin();i!=VLE.end();++i)
    {
     // Neuerzeugung mit aktueller Menge
     LieferscheinEntry LE((LieferscheinEntryBase(*i)));
     if( (LE.Stueck()==1          && LE.Menge()==mengen_t(0)) ||
         (LE.Menge()==mengen_t(0) && LE.Stueck()==0) )
     {
      std::string Q1 = "delete from lieferscheinentry where "
        "(instanz,lfrsid,zeile) = ("
        +itos(i->Instanz())+","+itos(i->Id())+","+itos(i->Zeile())+")";
      Query::Execute(Q1);
      SQLerror::test(__FILELINE__);
     }
    }
//      

/*
//cout << "Reduzieren? "<<abmenge<<'\n';
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

//cout << i->Zeile()<<'\t'<<M<<'\t'<<i->AufZeile()<<' '
//<<i->RefAuftrag().valid()<<'\n';
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
*/
//return true;
}

LieferscheinBase::mengen_t LieferscheinEntry::Abschreibmenge(int stueck,mengen_t menge) const
{
   int stueckdiff =   stueck - Stueck();
   mengen_t mengediff = menge - Menge();
//cout << stueck<<'-'<<Stueck()<<'='<<stueckdiff<<'\n';
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
