#include "auftragserfassung.hh"
#include <Auftrag/Auftrag.h>
#include <Aux/dbconnect.h>
#include <Aux/Global_Settings.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/ArtikelStamm.h>
#include <iomanip>
#include <algorithm>
#include <Aux/exception.h>

std::map<st_art,st_menge> map_artikel;
SelectedFullAufList *allaufids;


int main(int argc,char *argv[])
{
  Petig::PrintUncaughtExceptions();
 try {
  Petig::dbconnect(Petig::Connection("","petigdb"));
  cH_ppsInstanz instanz_to(ppsInstanz::INST_BANDLAGER);
  if (argc==2)
   {
     // INST_BANDLAGER = 8
     // INST_WEB = 4
     // INST_FAERB = 2
     // INST_DRUCK = 3
     // INST_SCHAER = 5 
     ppsInstanz::ID ppsInst = (ppsInstanz::ID)atoi(argv[1]);
     if (ppsInst==ppsInstanz::INST_BANDLAGER) instanz_to = cH_ppsInstanz(ppsInstanz::INST_BANDLAGER);
     else if (ppsInst==ppsInstanz::INST_WEB) instanz_to = cH_ppsInstanz(ppsInstanz::INST_WEB);
     else if (ppsInst==ppsInstanz::INST_FAERB) instanz_to = cH_ppsInstanz(ppsInstanz::INST_FAERB);
     else if (ppsInst==ppsInstanz::INST_DRUCK) instanz_to = cH_ppsInstanz(ppsInstanz::INST_DRUCK);
     else if (ppsInst==ppsInstanz::INST_SCHAER) instanz_to = cH_ppsInstanz(ppsInstanz::INST_SCHAER);
     else {cout << "FEHLER\n";exit(1);}
   }

                          
  cH_ppsInstanz instanz_from(ppsInstanz::INST_KNDAUF) ;
  get_all_orders(instanz_from);

  cummulate(instanz_to);
  create_new_auftrag(instanz_to);
  Global_Settings gs(0,"pps",instanz_to->get_Name(),"now()");
  gs.update_Date();

  Petig::dbdisconnect("petigdb");
   } catch (SQLerror &e)
  {  std::cerr << e << '\n';
  }
}

void create_new_auftrag(cH_ppsInstanz instanz)
{
  // Neuen Auftrag anlegen
  double size = map_artikel.size();
  unsigned long int count=0;
  cout << "Neue Aufträge anlegen\n";

  Auftrag A=Auftrag(Auftrag::Anlegen2(AuftragBase(instanz->Id(),0)), 1);//kundennummer=1
  A.setStatusAuftrag(OPEN);
  for (map<st_art,st_menge>::const_iterator i=map_artikel.begin();i!=map_artikel.end();++i)
   {
      int znr=A.insertNewEntry(i->second.menge,i->first.datum,i->first.artikelid);      
      for (vector<AufEintragBase2>::const_iterator j=i->second.vAEB.begin();j!=i->second.vAEB.end();++j)
       {
         AuftragsEntryZuordnung(*j,AufEintragBase(*j).getRestStk(),A,znr);
       }
      cout << setprecision(2)<<++count/size*100. <<"%\r"<<flush;
   }  
   cout << "\n";
}

void cummulate(cH_ppsInstanz instanz)
{
  for(vector<AufEintragBase>::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
   {
     int artikelid       = (*i).ArtId();
     Petig::Datum datum  = (*i).getLieferdatum();
     unsigned long int offene_stueck  = (*i).getRestStk();
     list<st_artmeng> bandlagerliste, bestellliste;
     bestellliste.push_back(st_artmeng(artikelid,(fixedpoint<2>)offene_stueck,*i));

reloop:
     for(list<st_artmeng>::iterator i=bestellliste.begin();i!=bestellliste.end();++i)
      {
        ArtikelStamm AS = ArtikelStamm(ArtikelBase((*i).aid));
        if ( AS.BestellenBei() == instanz->Id() ) 
          { 
            bandlagerliste.splice(bandlagerliste.end(),bestellliste,i) ;
          }
        else // Artikel aus anderen Artikeln zusammengestzt
         {
            // Woraus wird dieser Artikel erzeugt?
            list<st_artmeng> list_art = get_rohartikel_menge((*i).aid,(*i).aeb);
            // Wieviel brauche ich dafür?
            for(list<st_artmeng>::iterator j=list_art.begin();j!=list_art.end();++j)
               (*j).menge *= (*i).menge;
            // Die neuen Artikel am Ende anhängen
            bestellliste.splice(bestellliste.end(),list_art);
            // und dann diesen Artikel aus der Liste entfernen
            bestellliste.erase(i);
         }
        goto reloop;              
      }
    for(list<st_artmeng>::const_iterator i=bandlagerliste.begin();i!=bandlagerliste.end();++i)
     {
       map_artikel[st_art((*i).aid,datum)].addiere((*i).menge,(*i).aeb);      
     }
//cout << st_art(artikelid,datum) <<"\t"<< map_artikel[st_art(artikelid,datum)] <<"\n";
   } 
cout <<"Anzahl der verschiedenen Artikel: "<<map_artikel.size()<<"\n";
}

list<st_artmeng> get_rohartikel_menge(int artikelid,const AufEintragBase& aeb)
{
  list<st_artmeng> v;
  ArtikelBase A  = ArtikelBase(artikelid);
  ArtikelBaum AB(A);

//cout << "Artikel (" <<artikelid<<")  "<<flush<< ArtikelBezeichnung(artikelid).Bezeichnung()<<" ensteht aus "<<flush;
cout << "Artikel (" <<artikelid<<") ensteht aus "<<flush;
  for (ArtikelBaum::const_iterator ab=AB.begin();ab!=AB.end();++ab)
    {
//cout << " => "<<flush; 
      unsigned int rid = ab->rohartikel;
      fixedpoint<2> menge = ab->menge;
      v.push_back(st_artmeng(rid,menge,aeb));
//cout <<"("<<rid<<")  "<<ArtikelBezeichnung(rid).Bezeichnung()<<"\t"<<flush;
cout <<"("<<rid<<")  ";
    }
cout << "\n";
  return v;
}

void get_all_orders(cH_ppsInstanz instanz)
{
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),(AufStatVal)OPEN);
  allaufids = new SelectedFullAufList(psel);
}


