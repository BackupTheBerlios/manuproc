#include <Auftrag/selFullAufEntry.h>
#include <Auftrag/AuftragFull.h>
#include <Auftrag/AuftragsEntryZuordnung.h>
#include <Aux/dbconnect.h>
#include <iomanip>
#include <algorithm>
#include <Aux/exception.h>
#include <Aux/FetchIStream.h>
#include <map>
#include <Lager/Lager_Vormerkungen.hh>
//#include <Auftrag/AufEintrag.h>

static const int auftargid_for_geplant=1;

/*
std::map<int,vector<AufEintragZu::st_reflist> > sortPMap(const std::list<AufEintragZu::st_reflist>& L)
{
  std::map<int,vector<AufEintragZu::st_reflist> > PMap;
  for (std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
   {
     PMap[i->AEB2.Id()].push_back(*i);
   }  
  return PMap;
}
*/

void KinderPlanen(std::list<AufEintragZu::st_reflist> KL,const AufEintrag& KundenAE)
{
  int KundeGeplantBisSort=cH_ppsInstanz(KundenAE.LetztePlanInstanz())->Sortierung();
  for (std::list<AufEintragZu::st_reflist>::const_iterator i=KL.begin();i!=KL.end();++i)
   {
     AufEintrag AE(i->AEB2);
//     ArtikelBaum::faktor_t faktor = ArtikelBaum(KundenAE->ArtId()).Faktor(i->AB);             
//     AuftragBase::mengen_t menge = KundenAE->getRestStk() * faktor;

//L ist jetzt unten     std::map<int,vector<AufEintragZu::st_reflist> > PMap = sortPMap(L);
    cout << "Dieser Auftrag: "<<AE.Id()<<' '<<AE.Instanz()->Id()<<' '<<AE.ZNr()
         <<'\t'<<AE.getStueck()<<' '<<AE.getRestStk()<<' '<<AE.getEntryStatus()
         <<AE.getAufStatus()<<'\n';

     if (AE.Id()==0) // ungeplanter Auftrag
       {
         if(AE.Instanz()->Sortierung() >= KundeGeplantBisSort ) 
            {
              cout << "Kundenplanung bis "<<KundeGeplantBisSort
                   <<" Aktuelle Instanz "<<AE.Instanz()->Name()<<'\n';
/*
              AuftragBase::mengen_t menge=AE.getRestStk();
              if( !(AE.Instanz()->LagerInstanz()) || 
                   (AE.Instanz()->LagerInstanz() && PMap[1].empty() ))
               {
//                 menge=i->Menge;
                 menge=AE.getRestStk();
               }
              else // LagerInstanz mit Vormerkung: Planen der eigenen Menge
               {
                 menge=AE.getRestStk();
               }  
*/
              AuftragBase::mengen_t menge=i->Menge;
              cout << "Ungeplanter Auftrag Planen bis "<<AE.Instanz()->Sortierung()<<' '
                   << "Menge " <<menge<<'\n';
              AuftragBase zielauftrag(AE.Instanz(),auftargid_for_geplant);
              int znr=AE.Planen(menge,zielauftrag);

              AufEintrag ZIEL(AufEintragBase(zielauftrag,znr)); //debug
cout << ZIEL.Id()<<' '<<ZIEL.Instanz()->Name()<<' '<<ZIEL.ZNr()<<' ';
              cout << "Zielauftragsmenge = "<<ZIEL.getStueck()<<' '
                   <<ZIEL.getRestStk()<<'\n';
              try{ 
              AufEintragBase(zielauftrag,znr).abschreiben(menge);
                  }catch(SQLerror &e) {cerr << e<<'\n';}
            }
       }
     else if (AE.Id()==Lager_Vormerkungen::LagerAuftragsId) // geplanter LagerAuftrag
       {
         cout <<"Lager, nicht Planen sondern "<<i->Menge<<" "
             <<" "<<AE.getRestStk()<<" abschreiben\n";
//         try{
//             AE.abschreiben(i->Menge);
//             AE.abschreiben(AE.getRestStk());
//             }catch(SQLerror &e) {cerr << e<<'\n';}
       }
     else 
      {
        assert(!"Was soll das denn sein?\n");
      }
     std::list<AufEintragZu::st_reflist> L=AufEintragZu(AE).get_Referenz_list(AE,true);
     KinderPlanen(L,KundenAE);
   }
}


int main()
{
  ManuProC::PrintUncaughtExceptions();
 try {
  ManuProC::dbconnect();
  
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(ppsInstanz::Kundenauftraege,(AufStatVal)OPEN);
  SelectedFullAufList allaufids(psel);

  for (AuftragFull::iterator i=allaufids.begin();i!=allaufids.end();++i)
  {  
     cH_ppsInstanz KundenPlanInstanz=i->LetztePlanInstanz();
     if (KundenPlanInstanz != ppsInstanz::None)
     {  AufEintragZu aez(*i);
        std::list<AufEintragZu::st_reflist> liste=aez.get_Referenz_list(*i,true);
        KinderPlanen(liste,*i);
     }
  }
 
  ManuProC::dbdisconnect();
   } catch (SQLerror &e)
  {  std::cerr << e << '\n';
  }
}

