#include <Auftrag/selFullAufEntry.h>
#include <Auftrag/AuftragFull.h>
#include <Auftrag/AuftragsEntryZuordnung.h>
#include <Aux/dbconnect.h>
#include <iomanip>
#include <algorithm>
#include <Aux/exception.h>
#include <Aux/FetchIStream.h>

int main()
{
  ManuProC::PrintUncaughtExceptions();
 try {
  ManuProC::dbconnect();
  
  Query::Execute("update auftrag_prozess set letzteplaninstanz = "
      "(select tmp_conv_instanz from prozesse"
      " where auftrag_prozess.prozessid=prozesse.prozessid) "
      "where letzteplaninstanz is null");
                          
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(ppsInstanz::Kundenauftraege,(AufStatVal)OPEN);
  SelectedFullAufList allaufids(psel);

//  AuftragFull::const_reverseiterator e=allaufids.rend();
  for (AuftragFull::const_reverse_iterator i=allaufids.rbegin();i!=allaufids.rend();++i)
  {  cH_ppsInstanz inst=i->LetztePlanInstanz();
     int sort=inst->Sortierung();
     if (inst!=ppsInstanz::None)
     {  AufEintragZu aez(*i);
        std::list<AufEintragZu::st_reflist> liste=aez.get_Referenz_listFull(true,false);
        // ich verlasse mich mal darauf, dass ich ab einer bestimmten Stelle
        // alles verplant machen darf (d.h. parent first in der Liste)
        for (std::list<AufEintragZu::st_reflist>::const_iterator j=liste.begin();j!=liste.end();++j)
        {  
         try{
          AufEintrag AE(j->AEB2);
cout << AE.Instanz()->Name()<<' '<<AE.Id()<<' '<<AE.ZNr()<<' ';
cout << cH_ArtikelBezeichnung(AE.ArtId())->Bezeichnung()<<' ';
          if(AE.Id()!=0) 
            {
cout <<"Lager, nicht Planen\n";            
              continue;
            }
          int aksort = AE.Instanz()->Sortierung();
          if(aksort >= sort) 
            {
cout << "Planen bis "<<AE.Instanz()->Sortierung()<<' ';
             AuftragBase zielauftrag(j->AEB2.Instanz(),1);
             int znr=AE.Planen(AE.getStueck(),zielauftrag);
             AufEintragBase(zielauftrag,znr).abschreiben(AE.getStueck());
            }
cout <<'\n';
         }catch(SQLerror &e) {cerr << e<<'\n';}
        }
        cout << '\n';
     }
  }
 
  ManuProC::dbdisconnect();
   } catch (SQLerror &e)
  {  std::cerr << e << '\n';
  }
}

