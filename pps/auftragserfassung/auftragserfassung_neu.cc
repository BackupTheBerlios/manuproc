#include "auftragserfassung_neu.hh"
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


int main()
{
cout << "Bevor dieses Programm gestertet wird muß 'clean_db' und 'cxx/Programme/Instanzen' laufen.\n";

  Petig::PrintUncaughtExceptions();
 try {
  Petig::dbconnect(Petig::Connection("","petigdb"));

  cH_ppsInstanz instanz_from(ppsInstanz::INST_KNDAUF) ;
  get_all_orders(instanz_from);

  create_new_auftrag();

  Petig::dbdisconnect("petigdb");
   } catch (SQLerror &e)
  {  std::cerr << e << '\n';
  }
}

void create_new_auftrag()
{
  // Neue Auftrag für alle Instanzen anlegen
  std::vector<cH_ppsInstanz>  VI=cH_ppsInstanz::get_all_instanz();
  for(vector<AufEintrag>::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
   {
     ArtikelBase artikelid       = (*i).ArtId();
     Petig::Datum datum  = (*i).getLieferdatum();
     AuftragBase::mengen_t offene_stueck  = (*i).getRestStk();
//cout <<"Artikel: " <<cH_ArtikelBezeichnung(artikelid)->Bezeichnung()<<'\n';
//     i->InstanzAuftraegeAnlegen(artikelid,(*i).ZNr(),datum,OPEN,offene_stueck);

     AuftragBase AB(ArtikelStamm(artikelid).BestellenBei());
cout << cH_ArtikelBezeichnung(artikelid)->Bezeichnung()<<'\n';
     AB.tryUpdateEntry(offene_stueck,datum,artikelid,OPEN,*i,i->ZNr());
             
   }

}

list<st_artmeng> get_rohartikel_menge(int artikelid,const AufEintrag& aeb)
{
  list<st_artmeng> v;
  ArtikelBase A  = ArtikelBase(artikelid);
  ArtikelBaum AB(A);

//cout << "Artikel (" <<artikelid<<") ensteht aus "<<flush;
  for (ArtikelBaum::const_iterator ab=AB.begin();ab!=AB.end();++ab)
    {
      unsigned int rid = ab->rohartikel.Id();
      fixedpoint<2> menge = ab->menge;
      v.push_back(st_artmeng(rid,menge,aeb));
//cout <<"("<<rid<<")  ";
    }
//cout << "\n";
  return v;
}

void get_all_orders(cH_ppsInstanz instanz)
{
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),(AufStatVal)OPEN);
  allaufids = new SelectedFullAufList(psel);
}


