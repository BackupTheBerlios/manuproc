#include "AufArtKonsistenz_neu.h"
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
  std::ofstream ofs("Konflikte.txt");
  std::ofstream ofsH("Hinweise.txt");
  // Alle offenen Kundenaufträge
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(ppsInstanzID::Kundenauftraege,OPEN);
  SelectedFullAufList *allaufids = new SelectedFullAufList(psel);
  for(SelectedFullAufList::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
   {
     ++count;
     bool   ok=kundenauftrag_testen(ofs,ofsH,*i);
//     if(ok) ok=iterieren(ofs,ofsH,*i);
     if(ok) ++count_ok;
   }     
  std::cerr << "Aufträge Insgesamt: "<<count<<'\n'
       << "davon korrekt:      "<<count_ok<<'\t'<<double(count_ok)/count*100.<<"%\n";
  std::cerr << "Die Konflikte sind in der Datei 'Konflikte.txt' beschrieben.\n";
  std::cerr << "Die Hinweise sind in der Datei 'Hinweise.txt' beschrieben.\n";
}


bool AufArtKonsistenz::kundenauftrag_testen(std::ofstream& ofs,std::ofstream& ofsH,const AufEintrag& AE)
{
  std::list<AufEintragZu::st_reflist> AZ(AufEintragZu(AE).get_Referenz_list_ungeplant());

  if(AE.Instanz()!=ppsInstanzID::Kundenauftraege)
       return fehler(ofs,ofsH,Kunde_nicht,AE,AZ.begin()->AEB);
  if(AZ.size()!=1)
    {
      if(AZ.empty()) 
         return fehler(ofs,ofsH,Kunde_Ohne,AE,AZ.begin()->AEB);
      else 
         return fehler(ofs,ofsH,Kunde_mehr_als_eins,AE,AZ.begin()->AEB);
    }
  if(ArtikelStamm(AE.Artikel()).BestellenBei()!=AZ.begin()->AEB.Instanz())
       return fehler(ofs,ofsH,Kunde_Instanz,AE,AZ.begin()->AEB);
  if(AE.getStueck() != AZ.begin()->Menge)
   {
//     if(AE.getRestStk()<AZ.begin()->Menge && AZ.begin()->Menge<AE.getStueck())
//        return fehler(ofs,ofsH,H_Kunde_MengeZ,AE,AZ.begin()->AEB,AZ.begin()->Menge);
//     else
        return fehler(ofs,ofsH,Kunde_MengeZ,AE,AZ.begin()->AEB,AZ.begin()->Menge);
   }
  if(AE.ArtId() != AZ.begin()->Art.Id() )
       return fehler(ofs,ofsH,Kunde_Artikel,AE,AZ.begin()->AEB,AZ.begin()->Art.Id());
  {
   // Stimmen die Mengen des Kundenauftrags und der (bestellten)
   // Kinderaufträge überein?
   AuftragBase::mengen_t ist_menge =AuftragBase::mengen_t(0);
   std::list<AufEintragZu::st_reflist> L=AufEintragZu(AZ.begin()->AEB).get_Referenz_list_geplant();
   for(std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
     {
      assert(i->AEB.Id()==1);
      AufEintrag AE1(i->AEB);
      if(AE1.getStueck() != i->Menge)
       {
         if(AE1.getStueck()<i->Menge && AE1.Instanz()->LagerInstanz())
            return fehler(ofs,ofsH,H_Lager_MengeAEB,AE,AE1,i->Menge);
         else
            return fehler(ofs,ofsH,Kunde_MengeAEBZ,AE,AE1,i->Menge);
       }
      ist_menge+=AE1.getStueck();
     }
  // Kundenmenge = Menge des Ungeplanten Auftrags + Menge der geplanten Aufträge
  AufEintrag AE2(AZ.begin()->AEB);
  if(AE.getRestStk() < AE2.getStueck() + ist_menge)
     return fehler(ofs,ofsH,H_Kunde_MengeAEB,AE,AZ.begin()->AEB,AE2.getStueck() + ist_menge);
  else if(AE.getRestStk() > AE2.getStueck() + ist_menge)
     return fehler(ofs,ofsH,Kunde_MengeAEB,AE,AZ.begin()->AEB,AE2.getStueck() + ist_menge);
  }
  return true;
}

std::list<AufEintragZu::st_reflist> AufArtKonsistenz::get_zuordnung_for_artikelbaum(const AufEintrag &AE,const ArtikelBaum &artbaum)
{
  std::list<AufEintragZu::st_reflist> AZ(AufEintragZu(AE).get_Referenz_list_ungeplant());
  if(ArtikelStamm(AE.Artikel()).BestellenBei()==ppsInstanzID::Kundenauftraege)
   { // Sinnvolles ist vorher schon getestet worden, daher gleich die nächste Liste holen
     get_deeper_list(AZ);
   }   
  if(ArtikelStamm(AE.Artikel()).BestellenBei()->LagerInstanz())
   { 
assert(!"TODO");
     get_deeper_list(AZ);
   }   
 return AZ;
}

bool AufArtKonsistenz::iterieren(std::ofstream &ofs,std::ofstream &ofsH,const AufEintragBase& AEB)
{
  AufEintrag AE(AEB);
  ArtikelBaum artbaum=ArtikelBaum(AE.Artikel());
  std::list<AufEintragZu::st_reflist> AZ=get_zuordnung_for_artikelbaum(AE,artbaum);
  bool ok=true;
  
  ok=loesche_falsche_artikel_in_zuordnung(ofs,AE,artbaum,AZ);  

  artbaum.Sort(); // Nach Artikeln sortieren
  AZ.sort();
  assert(artbaum.size()==AZ.size());
  
  int index=0;
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=AZ.begin();i!=AZ.end();++i,++index)
   {
     if(i->AEB.Instanz() != ArtikelStamm(artbaum[index].rohartikel).BestellenBei())
       return fehler(ofs,ofsH,FInstanz,AE,AZ.begin()->AEB);
   }
  
 



/*
  AufEintrag AE(AEB);
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
  {  std::cerr <<AE.Instanz()->Name()<<' '<<AE.Id()<<' '
       <<AE.ZNr()<<'\t'<<cH_ArtikelBezeichnung(AE.Artikel())->Bezeichnung()<<'\t';
     std::cerr << " FEHLER\n";
  }
  else 
   {
     ++count_ok;
//     std::cerr<<" OK\n";
//#warning Die Iteration macht erst Sinn, wenn weniger Fehler im ArtikelBaum sind
//     for(std::list<AufEintragZu::st_reflist>::iterator i = AuftragsListe.begin();i!=AuftragsListe.end(); ++i)
//      {
//        iterieren(ofs,i->AEB);
//      }
   }
*/
 return ok;
}


void AufArtKonsistenz::fehler(std::ofstream &ofs,const Fehler& bug,
   const AufEintrag& AE,const AufEintragBase &AEB,int int_wert)
{

  ofs << "#Fehler in Auftrag "<<AE.Instanz()->Name()<<' '<<AE.Id()<<' '
       <<AE.ZNr()<<'\t'<<cH_ArtikelBezeichnung(AE.Artikel())->Bezeichnung()
       <<" ("<<AE.Artikel().Id()<<")\n";
  switch(bug)
   {
    case Kunde_nicht:
        ofs << "# "<< AE.Instanz()->Name()<<'\t'<<" ist kein Kundenauftrag"<<'\n';
        break;  
    case Kunde_mehr_als_eins:
        ofs <<"#   Für diesen Kundenauftrag existiert mehr als ein Kind\n";
        ofs <<"# "<< AE.Instanz()->Name()<<'\t'<<" hat mehr als ein Kind"<<'\n';
        break;  
    case Kunde_Ohne:
        ofs <<"#   Für diesen Kunden ist nichts weiter bestelt worden:\n";
        ofs <<"./auftrags_shell -x create -R -I"<<AE.Instanz()->Id()<<" -A"<<AE.Id()<<" -Z"<<AE.ZNr()
            << " -r"<<AE.Artikel().Id()<<" -m"<<AE.getStueck()
            <<" -p"<<AE.getStueck()-AE.getRestStk()<<'\n';
        break;  
    case Kunde_Instanz:
        ofs <<"#   Die Kundeninstanz stimmt nicht:\n";
        ofs << "# Kundenauftrag: "<< ArtikelStamm(AE.Artikel()).BestellenBei()->Name()
            <<"\tAuftragsZusammensetzung: "<<AEB.Instanz()->Name()<<'\n';
        ofs <<"./auftrags_shell -x delete -I"<<AE.Instanz()->Id()<<" -A"<<AE.Id()<<" -Z"<<AE.ZNr()
            << " -i"<<AEB.Instanz()->Id()<<" -a"<<AEB.Id()<<" -z"<<AEB.ZNr()<<'\n';
        ofs <<"./auftrags_shell -x create -R -I"<<AE.Instanz()->Id()<<" -A"<<AE.Id()<<" -Z"<<AE.ZNr()
            << " -r"<<AE.Artikel().Id()<<" -m"<<AE.getStueck()
            <<" -p"<<AE.getRestStk()<<'\n';
        break;
    case FInstanz:
        ofs <<"#   Die Instanzen stimmt nicht überein:\n";
        ofs << "# AuftragEntry: "<< AE.Instanz()->Name()
            <<"\tAuftragsZusammensetzung: "<<AEB.Instanz()->Name()<<'\n';
        ofs <<"./auftrags_shell -x delete -I"<<AE.Instanz()->Id()<<" -A"<<AE.Id()<<" -Z"<<AE.ZNr()
            << " -i"<<AEB.Instanz()->Id()<<" -a"<<AEB.Id()<<" -z"<<AEB.ZNr()<<'\n';
        break;
    case H_Kunde_MengeZ:
        ofs <<"# Hinweis: (vermutlich kein Fehler)\n";
        ofs <<"# Kundenauftrag bestellt: "<<AE.getStueck()
            <<"\tRest: "<<AE.getRestStk()<<"\tAuftragsZuordnung: "<<int_wert<<'\n';
        break;
    case Kunde_MengeZ:
        ofs <<"#   Die Menge des Kundenauftrags und der Zuordnungstabelle stimmt nicht überein:\n";
        ofs <<"# Kundenauftrag:"<<AE.getStueck()<<"("<<AE.getRestStk()<<")" 
            <<"\tAuftragsZuordnung ("<<AE.Instanz()->Name()<<"): "<<int_wert<<'\n';
        ofs <<"./auftrags_shell -x change -A"<<AE.Id()
            <<" -I"<<AE.Instanz()->Id() <<" -Z"<<AE.ZNr()
            <<" -a"<<AEB.Id()<<" -i"<<AEB.Instanz()->Id()
            <<" -z"<<AEB.ZNr()<<" -m"<<AE.getRestStk()<<'\n';
        break;
    case Kunde_MengeAEB:
        ofs <<"#   Die Kunden Menge stimmt nicht mit der Summe der (bestellten) Kinderaufträge überein:\n";
        ofs <<"# Kundenauftrag:"<<AE.getRestStk() 
            <<"\tSumme der Kinderaufträge ("<<AEB.Instanz()->Name()
            <<"): "<<int_wert<<'\n';
        break;
    case H_Kunde_MengeAEB:
        ofs <<"#  Hinweis: In Instanz "<< AEB.Instanz()->Name()<< " sind "
            << int_wert<<" bestellt,\n#   der Kunde hat aber nur noch "
            << AE.getRestStk() <<" offen (Rest)\n";
        break;
    case Kunde_MengeAEBZ:
      {
        ofs <<"#   Die Menge in der Zuordnung und des bestellten Auftrags stimmt nicht überein:\n";
         AufEintrag A(AEB);
        ofs <<"# "<< A.Instanz()->Name()<<' '<<A.Id()<<' '<<A.ZNr()<<' '<<A.getStueck()
                  <<" != "<< int_wert<<'\n'; 
        break;
      }
    case H_Lager_MengeAEB:
      {
        ofs <<"#   Im Lager ist mehr bestellt als der Auftrag benötigt :\n";
        AufEintrag A(AEB);
        ofs <<"# "<< A.Instanz()->Name()<<' '<<A.Id()<<' '<<A.ZNr()<<' '<<A.getStueck()
                  <<" != "<< int_wert<<'\n'; 
        break;
      }
    case Kunde_Artikel:
        ofs <<"#   Der Kunden Artikel stimmt nicht überein:\n";
        ofs <<"# Kundendauftrag: "<<AE.ArtId()
            <<"\tAuftragsZusammensetzung: "<<int_wert<<'\n';
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
 ofs<<'\n';
}

bool AufArtKonsistenz::fehler(std::ofstream &ofsK,std::ofstream &ofsH,const Fehler& bug,
   const AufEintrag& AE,const AufEintragBase &AEB,int int_wert)
{
  if(bug==H_Kunde_MengeZ || bug==H_Kunde_MengeAEB || bug==H_Lager_MengeAEB) 
   {
      fehler(ofsH,bug,AE,AEB,int_wert);
      return true;
   }
  else 
   {
      fehler(ofsK,bug,AE,AEB,int_wert);
      return false;
   }
}




#ifdef NIX
bool AufArtKonsistenz::teste_lager(std::ofstream &ofs,const ppsInstanz::ppsInstId instanz,std::list<AufEintragZu::st_reflist>& AuftragsListe,bool& Lager)
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



bool AufArtKonsistenz::check_ArtikelAuftrag(std::ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AuftragsListe)
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
     std::pair<ArtCIterator,ArtCIterator> MisIt = mismatch(LArt.begin(),LArt.end(),LAuf.begin()); 
     falscheArtikel(ofs,AE,LArt,LAuf,MisIt);
     delete_auftrag(ofs,AE,artbaum,AufList);
     create_auftrag(ofs,AE,artbaum,AufList);
     ofs <<"\n\n";
     return false;
   }
  return true;
}


void AufArtKonsistenz::create_auftrag(std::ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AufList)
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

void AufArtKonsistenz::delete_auftrag(std::ofstream &ofs,const AufEintrag& AE,const ArtikelBaum& artbaum,const std::list<AufEintragZu::st_reflist>& AufList)
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




void AufArtKonsistenz::falscheArtikel(std::ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf,const std::pair<ArtCIterator,ArtCIterator>& MisIt)
{
  fehler(ofs,Artikel,AE);
  ofs << "# "<<MisIt.first->Id()<<" ("<<cH_ArtikelBezeichnung(*(MisIt.first))->Bezeichnung()<<")"
      <<'\t'<<MisIt.second->Id()<<" ("<<cH_ArtikelBezeichnung(*(MisIt.second))->Bezeichnung()<<")\n";
//  korrektur(ofs,AE,MisIt);
}

void AufArtKonsistenz::korrektur(std::ofstream &ofs,const AufEintrag& AE,const std::pair<ArtCIterator,ArtCIterator>& MisIt)
{
//  ofs << "auftrags_shell -a delete -l"<<AE.Artikel().Id()<<" -r"<<MisIt.first->Id()<<'\n';  
//  ofs << "artikel_shell -a delete -l"<<AE.Artikel().Id()<<" -r"<<MisIt.first->Id()<<'\n';  
//  ofs << "artikel_shell -a create -l"<<AE.Artikel().Id()<<" -r"<<MisIt.second->Id()<<" -m? -p??"<<"\n\n";  
}



void AufArtKonsistenz::falscheAnzahl(std::ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf)
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

#endif

bool AufArtKonsistenz::loesche_falsche_artikel_in_zuordnung(std::ofstream &ofs,const AufEintragBase &AEB,const ArtikelBaum& artbaum,const std::list<AufEintragZu::st_reflist>& AZ)
{
  std::list<AufEintragBase> DelList;
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=AZ.begin();i!=AZ.end();++i)
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
   ofs << "./auftrags_shell -x delete -I"<<AEB.Instanz()->Id()<<" -A"<<AEB.Id()<<" -Z"<<AEB.ZNr()
       << " -i"<<i->Instanz()->Id()<<" -a"<<i->Id()<<" -z"<<i->ZNr()<<'\n';
  }
 if(DelList.empty())  return true;
 else return false;
}




void AufArtKonsistenz::get_deeper_list(std::list<AufEintragZu::st_reflist>& AuftragsListe)
{
  assert(AuftragsListe.size()==1);
  AuftragsListe=AufEintragZu(AuftragsListe.begin()->AEB).get_Referenz_list_ungeplant();  
}


/*
bool AufArtKonsistenz::check_if_Rollerei_Lager(const ppsInstanz::ppsInstId instanz,std::ofstream &ofs,const ArtikelBaum& artbaum, std::list<AufEintragZu::st_reflist>& AuftragsListe)
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
std::cout <<"# "  <<AuftragsListe.begin()->AEB.Instanz()->Name()<<'\t'<<flush;
std::cout <<"# " AuftragsListe.begin()->AEB.Instanz()->Name()<<'\n';
   }
  return true;
}
*/
