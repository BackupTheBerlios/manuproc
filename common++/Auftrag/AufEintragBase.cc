// $Id: AufEintragBase.cc,v 1.28 2002/10/24 14:06:49 thoma Exp $
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
#include "AufEintragBase.h"
#include <Aux/Transaction.h>
//#include <Auftrag/auftrag_status.h>
#include <Auftrag/AuftragsBaum.h> 
//#include <Aux/Changejournal.h>
#include <Auftrag/AufEintragZu.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include "AufEintrag.h"
#ifdef MABELLA_EXTENSIONS
#include<Lager/FertigWaren.h>
#include<Artikel/ArtikelBase.h>
#endif
#include <Aux/Trace.h>
#include<Aux/FetchIStream.h>


FetchIStream& operator>>(FetchIStream& is,AufEintragBase &aeb)
{
  int a,b,c;
  is >> a>>b>>c;
  cH_ppsInstanz I((ppsInstanz::ID(a)));
  AuftragBase ab(I,b);
  aeb=AufEintragBase(ab,c);
  return is;
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
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,str());
  assert (Instanz()==ppsInstanzID::Kundenauftraege);
  AuftragsBaum AB(*this,true);
  int anz=0;
  for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
     if(i->AEB2.Id()==0) continue; // 0 = ungeplante Auftr‰ge
     if(AufEintrag(i->AEB2).getStueck() == i->menge)
       ++anz;
   }
  setMaxPlanInstanz(anz);
}   



// Ich weiﬂ nicht mehr wer dies braucht .... MAT :-(
void AufEintragBase::Planen(int uid,std::vector<AufEintrag> LAE,mengen_t menge,
      const AuftragBase &zielauftrag,const ManuProC::Datum &datum)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,
   "Menge="+dtos(menge),"Zielauftrag="+zielauftrag.str(),"Datum="+datum.to_iso());
 int znr=AufEintragBase::none_znr;
 ArtikelBase artikel=ArtikelBase::none_id;
 Transaction tr;
 for(std::vector<AufEintrag>::iterator i=LAE.begin();i!=LAE.end();++i)
  {
    int znrtmp;
    if(i->getRestStk()<=menge)
     {
       menge-=i->getRestStk();
       znrtmp=i->Planen(uid,i->getRestStk(),true,zielauftrag,datum);
     }
    else
     {
      if(menge>0)
        znrtmp=i->Planen(uid,menge,true,zielauftrag,datum);
     }
    if(znr==AufEintragBase::none_znr) znr=znrtmp;
    else assert(znr==znrtmp);
    if(artikel==ArtikelBase::none_id) artikel=i->Artikel();
    else assert(artikel==i->Artikel());
  }
 if(menge>0) // ‹berplanung
  {
    AufEintragBase(zielauftrag,znr).PlanenDispo(uid,artikel,menge,datum);
  }
 tr.commit();
}


void AufEintragBase::PlanenDispo(int uid,const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,str(),
   "Artikel="+cH_ArtikelBezeichnung(artikel)->Bezeichnung(),
   "Menge="+dtos(menge),"Datum="+datum.to_iso());

   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
   st_tryUpdateEntry st(false,false,true);
   int znr=dispoAB.tryUpdateEntry(menge,datum,artikel,OPEN,uid,*this,st);
   AufEintragBase dispoAEB(dispoAB,znr);

   updateStkDiffBase__(uid,menge);
  
   AufEintrag AE(dispoAEB);
   bool automatisch_geplant=false;
   if(Id()==plan_auftrag_id)  automatisch_geplant=true  ;
   dispoAEB.InstanzAuftraegeAnlegen(AE,menge,uid,automatisch_geplant);
   if(automatisch_geplant)  AE.abschreiben(menge) ;
}


int AufEintragBase::split_zuordnungen_to(mengen_t menge,ManuProC::Datum datum,
                        ArtikelBase artikel,AufStatVal status,int uid,
                        bool dispoplanung)
{
 ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,str(),
   "Artikel="+cH_ArtikelBezeichnung(artikel)->Bezeichnung(),
   "Menge="+dtos(menge),"Status="+itos(status),"DispoPlanung="+itos(dispoplanung));

  assert(Id()==AuftragBase::ungeplante_id);
  // ElternListe holen
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list(*this,false);
  int znr=none_znr;
  assert(!L.empty());
  for(std::list<AufEintragZu::st_reflist>::iterator i=L.begin();i!=L.end();++i)
   {
    mengen_t M;
    if(i->Menge > menge) M=menge;
    else M=i->Menge;
    if(!dispoplanung)
       znr=tryUpdateEntry(M,datum,artikel,status,uid,i->AEB);
//       znr=tryUpdateEntry(M,datum,artikel,status,uid,i->AEB,false);
    else
      { int dummy;
        assert(existEntry(artikel,datum,znr,dummy,menge,status));
        AufEintragBase aeb_verplant(*this,znr);
        AufEintragZu(i->AEB).setMengeDiff__(aeb_verplant,M);
      }
    AufEintragZu(i->AEB).setMengeDiff__(*this,-M);
    menge-=M;
    if(M==mengen_t(0)) break;
   }
 return znr;
}

void AufEintragBase::ExistMenge(const std::string &s) const
{
 try{AufEintrag AE(*this);
     std::cout << *this<<" existiert mit " <<AE.getStueck()<<" bestellt und "
          <<AE.getGeliefert()<<" geliefert ("<<s<<")\n";
   }catch(...){std::cout << *this<<"\texistiert noch nicht ("<<s<<")\n";}
}

