#include "AufArtKonsistenz.h"
#include <Aux/dbconnect.h>
#include <Artikel/ArtikelStamm.h>

int main()
{
  Petig::dbconnect(); 
  AufArtKonsistenz();
  return 1;
}


AufArtKonsistenz::AufArtKonsistenz()
: count(0),count_ok(0)
{
  ofstream ofs("Konflikte.txt");
  // Alle offenen Kundenaufträge
  get_Auftraege(ppsInstanzID::Kundenauftraege,OPEN);  
  for(SelectedFullAufList::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
   {
     iterieren(ofs,*i);
   }     
  cerr << "Aufträge Insgesamt: "<<count<<'\n'
       << "davon korrekt:      "<<count_ok<<'\t'<<double(count_ok)/count*100.<<"%\n";
  cerr << "Die Konflikte sind in der Datei 'Konflikte.txt' beschrieben.\n";
}

bool AufArtKonsistenz::kundenauftrag_testen(ofstream& ofs,const AufEintrag& AE,std::list<AufEintragZu::st_reflist>& AuftragsListe)
{
  if(AuftragsListe.size()!=1)
   {
    fehler(ofs,Kunde_mehr_als_eins,AE);
    ofs << "# "<< AE.Instanz()->Name()<<'\t'<<" hat mehr als ein Kind"<<'\n';
    return false;
   }
/* Unsinn, da es vollständig geplante Aufträge geben kann
  if(!AuftragsListe.empty())
   {
    fehler(ofs,Kunde_Ohne,AE);
    ofs << "# "<< AE.Instanz()->Name()<<'\t'<<" aber AuftragsListe ist leer "<<'\n';
    return false;
   }
*/
  if(ArtikelStamm(AE.Artikel()).BestellenBei() 
        != AuftragsListe.begin()->AEB.Instanz() )
   {
    fehler(ofs,Kunde_Instanz,AuftragsListe.begin()->AEB);
    ofs << "# "<< AE.Instanz()->Name()<<'\t'<<AuftragsListe.begin()->AEB.Instanz()->Name()<<'\n';
    return false;
   }
  if(AE.getRestStk() != AuftragsListe.begin()->Menge )
   {
    fehler(ofs,Kunde_Menge,AE);
    ofs << "# "<<AE.getRestStk() <<"\t\t\t"<<AuftragsListe.begin()->Menge<<'\n';
    return false;
   }
  if(AE.ArtId() != AuftragsListe.begin()->Art.Id() )
   {
    fehler(ofs,Kunde_Artikel,AuftragsListe.begin()->AEB);
    ofs << "# "<<AE.ArtId()  <<'\t'<< AuftragsListe.begin()->Art.Id()<<'\n';
    return false;
   }
  return true;
}


void AufArtKonsistenz::iterieren(ofstream &ofs,const AufEintragBase& AEB)
{
  AufEintrag AE(AEB);
  ++count;  
  ArtikelBaum artbaum=ArtikelBaum(AE.Artikel());
  std::list<AufEintragZu::st_reflist> AuftragsListe(AufEintragZu(AE).get_Referenz_list_ungeplant(AE,true));
  
  bool ok;
  // Zuerst wird getestet, ob für für alle Kundenaufträge die
  // Auftragsliste exakt die Länge 1 hat, bei der richtigen Instanz
  // bestellt wird, der Artikel und die Menge stimmt.
  if(AEB.Instanz()==ppsInstanz::Kundenauftraege)
     ok=kundenauftrag_testen(ofs,AE,AuftragsListe);

  ueberspringe_instanz(ppsInstanz::Rollerei,AuftragsListe,AE);

  bool Lager=false; // Nach dem Lager kommt nichts mehr wenn 'true' (d.h. geung vorrätig)
  ok=teste_lager(ofs,ppsInstanz::Bandlager,AuftragsListe,Lager);

  if(!Lager && ok)
     ok=check_ArtikelAuftrag(ofs,AE,artbaum,AuftragsListe);

  if(!ok) 
  {  cerr <<AE.Instanz()->Name()<<' '<<AE.Id()<<' '
       <<AE.ZNr()<<'\t'<<cH_ArtikelBezeichnung(AE.Artikel())->Bezeichnung()<<'\t';
     cerr << " FEHLER\n";
  }
  else 
   {
     ++count_ok;
//     cerr<<" OK\n";
#warning Die Iteration macht erst Sinn, wenn weniger Fehler im ArtikelBaum sind
     for(std::list<AufEintragZu::st_reflist>::iterator i = AuftragsListe.begin();i!=AuftragsListe.end(); ++i)
      {
//        iterieren(ofs,i->AEB);
      }
   }
}


bool AufArtKonsistenz::teste_lager(ofstream &ofs,const ppsInstanz::ppsInstId instanz,std::list<AufEintragZu::st_reflist>& AuftragsListe,bool& Lager)
{
  if(AuftragsListe.empty()) return true;
  if(AuftragsListe.begin()->AEB.Instanz()==ppsInstanz::Bandlager)  Lager=true;
  else return true;
  if(AuftragsListe.size()>1) 
   { 
    fehler(ofs,LagerLaenge,AuftragsListe.begin()->AEB);
    for(std::list<AufEintragZu::st_reflist>::const_iterator i=AuftragsListe.begin();i!=AuftragsListe.end();++i)
      ofs <<"#Instanz: "<< i->AEB.Instanz()->Name()<<"\tAuftrags ID: "<<i->AEB.Id()<<"\t ZeilenNr.: "<<i->AEB.ZNr()<<'\n';
    ofs << "\n";
    return false;
   }
  std::list<AufEintragZu::st_reflist> L=get_deeper_list(AuftragsListe);
  AuftragBase::mengen_t menge_p=0,menge_u=0,
                        sollmenge = AuftragsListe.begin()->Menge;
  for(std::list<AufEintragZu::st_reflist>::iterator i = L.begin();i!=L.end(); ++i)
   {
     if(i->AEB.Id()==0) // Tieferliegende Instanzen
       menge_u += i->Menge;
     else // Im Lager vorgemerkte Menge
       menge_p += i->Menge;      
   }  
  if(menge_u+menge_p != sollmenge) 
   {
#warning noch kein Mengentest
//    fehler(ofs,LagerMenge,AuftragsListe.begin()->AEB);
//    ofs << "   "<<sollmenge<<" != "<<menge_p<<'+'<<menge_u<<" = "<<menge_u+menge_p<<"\n\n";
//    return false;
   }
  if(menge_u!=fixedpoint<2>(0)) // Tieferliegende Instanzen existieren
   {
    assert(AuftragsListe.size()==1);
    AuftragsListe=get_deeper_list(AuftragsListe);
    assert(AuftragsListe.size()==1);
    assert(AuftragsListe.begin()->AEB.Instanz()==ppsInstanz::Weberei);
    AuftragsListe=get_deeper_list(AuftragsListe);
   }
  else
   {
    assert(AuftragsListe.size()==1);
    AuftragsListe=get_deeper_list(AuftragsListe);
    assert(AuftragsListe.size()==0 || 
          (AuftragsListe.size()==1 && AuftragsListe.begin()->Menge==fixedpoint<2>(0)));
//    if(AuftragsListe.size()==0) // nach dem Bandlager ist Schluß, weil alles vorrätig ist
//      {
//        Lager = true;
//      }    
    if(AuftragsListe.size()==1)
      {
       assert(AuftragsListe.begin()->AEB.Instanz()==ppsInstanz::Weberei);
       AuftragsListe=get_deeper_list(AuftragsListe);
      }
   }
  return true;
}


void AufArtKonsistenz::ueberspringe_instanz(const ppsInstanz::ppsInstId instanz,std::list<AufEintragZu::st_reflist>& AuftragsListe, AufEintrag& AE)
{
  if(AuftragsListe.size()==1 && AuftragsListe.begin()->AEB.Instanz()==instanz)
   {
     AE= AuftragsListe.begin()->AEB;
     AuftragsListe=get_deeper_list(AuftragsListe);
   }
}


/*
ArtikelBase AufArtKonsistenz::get_artikel_for_instanz(const std::list<AufEintragZu::st_reflist>& AuftragsListe,const ppsInstanz::ppsInstId instanz)
{
  for(std::list<AufEintragZu::st_reflist>::const_iterator i = AuftragsListe.begin();i!=AuftragsListe.end(); ++i)
   {
//     if(instanz==)
   }
  return ArtikelBase(); // ArtikelBase mit none_id
}
*/



bool AufArtKonsistenz::check_ArtikelAuftrag(ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AuftragsListe)
{
  std::list<ArtikelBase> LArt,LAuf;
  std::list<AufEintragZu::st_reflist> AufList;
  for(ArtikelBaum::const_iterator i=artbaum.begin();i!=artbaum.end();++i)
     LArt.push_back(i->rohartikel);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=AuftragsListe.begin();i!=AuftragsListe.end();++i)
   {
     // geplante Aufträge NICHT berücksichtigen
     if(i->AEB.Id()==1) continue;
     LAuf.push_back(i->Art);
     AufList.push_back(*i);
   }
  LArt.sort(); 
  LAuf.sort();
  LAuf.unique();

  if(LArt.size()!=LAuf.size()) 
   { 
     falscheAnzahl(ofs,AE,LArt,LAuf);
     delete_auftrag(ofs,AE,artbaum,AufList);
     create_auftrag(ofs,AE,artbaum,AufList);
     ofs <<"\n\n";
     return false;
   }

  bool e=equal(LArt.begin(),LArt.end(),LAuf.begin());
  if(!e) // Artikel-Listen stimmen nicht überein
   {
     pair<ArtCIterator,ArtCIterator> MisIt = mismatch(LArt.begin(),LArt.end(),LAuf.begin()); 
     falscheArtikel(ofs,AE,LArt,LAuf,MisIt);
     delete_auftrag(ofs,AE,artbaum,AufList);
     create_auftrag(ofs,AE,artbaum,AufList);
     ofs <<"\n\n";
     return false;
   }
  return true;
}


void AufArtKonsistenz::create_auftrag(ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AufList)
{
  std::list<ArtikelBase> CreateList;
  for(ArtikelBaum::iterator j=artbaum.begin();j!=artbaum.end();++j)
   {
     bool ok=false;
     for(std::list<AufEintragZu::st_reflist>::const_iterator i=AufList.begin();i!=AufList.end();++i)
      {
        if(j->rohartikel==i->Art) {ok=true; break;}
      }
     if(!ok) CreateList.push_back(j->rohartikel);
   }
  for(std::list<ArtikelBase>::const_iterator i=CreateList.begin();i!=CreateList.end();++i)
   {
     AuftragBase::mengen_t menge=AE.getRestStk()*ArtikelBaum(AE.ArtId()).Faktor(*i);
     ofs << "./auftrags_shell -x create -I"<<AE.Instanz()->Id()<<" -A"<<AE.Id()<<" -Z"<<AE.ZNr()
         << " -r"<< i->Id() <<" -m"<<menge<<"\n";
   }
}

void AufArtKonsistenz::delete_auftrag(ofstream &ofs,const AufEintrag& AE,const ArtikelBaum& artbaum,const std::list<AufEintragZu::st_reflist>& AufList)
{
  std::list<AufEintragBase> DelList;
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=AufList.begin();i!=AufList.end();++i)
   {
     bool ok=false;
     for(ArtikelBaum::const_iterator j=artbaum.begin();j!=artbaum.end();++j)
      {
        if(j->rohartikel==i->Art) {ok=true; break;}
      }
     if(!ok) DelList.push_back(i->AEB);
   } 
 for(std::list<AufEintragBase>::const_iterator i=DelList.begin();i!=DelList.end();++i)
  {
   ofs << "./auftrags_shell -x delete -I"<<AE.Instanz()->Id()<<" -A"<<AE.Id()<<" -Z"<<AE.ZNr()
       << " -i"<<i->Instanz()->Id()<<" -a"<<i->Id()<<" -z"<<i->ZNr()<<'\n';
  }
}




void AufArtKonsistenz::falscheArtikel(ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf,const pair<ArtCIterator,ArtCIterator>& MisIt)
{
  fehler(ofs,Artikel,AE);
  ofs << "# "<<MisIt.first->Id()<<" ("<<cH_ArtikelBezeichnung(*(MisIt.first))->Bezeichnung()<<")"
      <<'\t'<<MisIt.second->Id()<<" ("<<cH_ArtikelBezeichnung(*(MisIt.second))->Bezeichnung()<<")\n";
//  korrektur(ofs,AE,MisIt);
}

void AufArtKonsistenz::korrektur(ofstream &ofs,const AufEintrag& AE,const pair<ArtCIterator,ArtCIterator>& MisIt)
{
//  ofs << "auftrags_shell -a delete -l"<<AE.Artikel().Id()<<" -r"<<MisIt.first->Id()<<'\n';  
//  ofs << "artikel_shell -a delete -l"<<AE.Artikel().Id()<<" -r"<<MisIt.first->Id()<<'\n';  
//  ofs << "artikel_shell -a create -l"<<AE.Artikel().Id()<<" -r"<<MisIt.second->Id()<<" -m? -p??"<<"\n\n";  
}



void AufArtKonsistenz::falscheAnzahl(ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf)
{
  fehler(ofs,Anzahl,AE);
  ofs << "# Nr.   AuftragsZusammensetzung("<<LAuf.size()<<")  ArtikelBaum("<<LArt.size()<<")        \n";
  std::map<int,ArtikelBase> MArt,MAuf;
  int m=0;
  for(std::list<ArtikelBase>::const_iterator i=LArt.begin();i!=LArt.end();++i)
     MArt[m++]=*i;
  m=0;
  for(std::list<ArtikelBase>::const_iterator i=LAuf.begin();i!=LAuf.end();++i)
     MAuf[m++]=*i;
  unsigned int max= (MArt.size()>MAuf.size()) ? max=MArt.size() :max=MAuf.size();
  for(unsigned int i=0;i<max;++i)
   {
    ofs <<"# "<<i+1<<'\t'<<MAuf[i].Id()<<" ("<<cH_ArtikelBezeichnung(MAuf[i])->Bezeichnung()<<")\t"
              <<MArt[i].Id()<<" ("<<cH_ArtikelBezeichnung(MArt[i])->Bezeichnung()<<")\n";
   }
}



void AufArtKonsistenz::fehler(ofstream &ofs,const Fehler& bug,const AufEintrag& AE,ppsInstanz::ppsInstId instanz)
{
  ofs << "#Fehler in Auftrag "<<AE.Instanz()->Name()<<' '<<AE.Id()<<' '
       <<AE.ZNr()<<'\t'<<cH_ArtikelBezeichnung(AE.Artikel())->Bezeichnung()
       <<" ("<<AE.Artikel().Id()<<")\n";
  switch(bug)
   {
    case Kunde_mehr_als_eins:
        ofs <<"#   Für diesen Kundenauftrag existiert mehr als ein Kinde:\n";
        ofs <<"#     ArtikelBaum         AuftragsZusammensetzung \n";
        break;  
    case Kunde_Ohne:
        ofs <<"#   Für diesen Kunden ist nichts weiter besetlt worden:\n";
        ofs <<"#     ArtikelBaum         AuftragsZusammensetzung \n";
        break;  
    case Kunde_Instanz:
        ofs <<"#   Die Kunden Instanz stimmt nicht überein:\n";
        ofs <<"#     ArtikelBaum         AuftragsZusammensetzung \n";
        break;  
    case Kunde_Menge:
        ofs <<"#   Die Kunden Menge stimmt nicht überein:\n";
        ofs <<"#     ArtikelBaum         AuftragsZusammensetzung \n";
        break;
    case Kunde_Artikel:
        ofs <<"#   Der Kunden Artikel stimmt nicht überein:\n";
        ofs <<"#     ArtikelBaum         AuftragsZusammensetzung \n";
        break;
    case Anzahl:
        ofs <<"#   Die Anzahl der Kinder-Artikel stimmt nicht überein:\n";
        ofs <<"#    ArtikelBaum         AuftragsZusammensetzung \n";
        break;
    case Artikel:
        ofs <<"#   Die Art der Kinder-Artikel stimmt nicht überein:\n";
        ofs <<"#    AuftragsZusammensetzung          ArtikelBaum \n";
        break;
    case LagerMenge:
        ofs <<"#   Die Mengen der bestellten und vorrätigen Artikel stimmt nicht:\n";
        ofs <<"#    Sollmenge   != vorgemerkte Menge + bestellte Menge\n";
        break;
    case LagerLaenge:
        ofs <<"#   Es sollte nur ein(!) Kindauftrag existieren, aber:\n";
        break;
   }
}


void AufArtKonsistenz::get_Auftraege(ppsInstanz::ppsInstId instanz,AufStatVal status)
{
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz,status);
  allaufids = new SelectedFullAufList(psel);
}


std::list<AufEintragZu::st_reflist> AufArtKonsistenz::get_deeper_list(const std::list<AufEintragZu::st_reflist>& AuftragsListe)
{
  assert(AuftragsListe.size()==1);
  return AufEintragZu(AuftragsListe.begin()->AEB).get_Referenz_list_ungeplant(AuftragsListe.begin()->AEB,true);  
}


/*
bool AufArtKonsistenz::check_if_Rollerei_Lager(const ppsInstanz::ppsInstId instanz,ofstream &ofs,const ArtikelBaum& artbaum, std::list<AufEintragZu::st_reflist>& AuftragsListe)
{
  if(AuftragsListe.empty()) 
      return true; // Probleme treten nicht hier auf
  ArtikelBase art=get_artikel_for_instanz(AuftragsListe,instanz);
  if(art.Id()==ArtikelBase::none_id) return true; // Probleme woanders suchen
 
  if(AuftragsListe.begin()->AEB.Instanz() == instanz)
   {
     if(ArtikelStamm(AE.Artikel()).BestellenBei()!=instanz)
       {
         fehler(ofs,RollereiLager,AE,instanz);
         ofs <<"#   "<<ArtikelStamm(AE.Artikel()).BestellenBei()->Name() <<'\t'
             <<AuftragsListe.begin()->AEB.Instanz()->Name()<<"\n\n";
         return false;
       }
     AuftragsListe=AufEintragZu(AuftragsListe.begin()->AEB).get_Referenz_list_ungeplant(AuftragsListe.begin()->AEB,true);  
   }

  if(instanz==ppsInstanz::Bandlager && AuftragsListe.begin()->AEB.Instanz() == ppsInstanz::Bandlager)
   { // Bandlager bestellt in der Weberei, daher noch eine Ebene überspringen
     if(AuftragsListe.size()!=1) fehler(ofs,Kritisch,AE) ;
cout <<"# "  <<AuftragsListe.begin()->AEB.Instanz()->Name()<<'\t'<<flush;
cout <<"# " AuftragsListe.begin()->AEB.Instanz()->Name()<<'\n';
   }
  return true;
}
*/
