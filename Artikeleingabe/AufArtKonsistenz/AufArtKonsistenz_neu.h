#ifndef _AufArtKonsistenz_HH
#  define _AufArtKonsistenz_HH

#include<Instanzen/ppsInstanz.h>
#include<Auftrag/auftrag_status.h>
#include<Auftrag/selFullAufEntry.h>
#include<Artikel/ArtikelBaum.h>
#include<Auftrag/AufEintragZu.h>
#include<Artikel/ArtikelBezeichnung.h>
#include <fstream>

class AufArtKonsistenz
{
    typedef std::list<ArtikelBase>::const_iterator ArtCIterator;
    struct st_korrektur{AufEintragBase AE1; AufEintragBase AE2;
           st_korrektur(AufEintragBase a1,AufEintragBase a2) 
            : AE1(a1),AE2(a2) {}};

	 int count,count_ok;
    bool kundenauftrag_testen(ofstream& ofs,ofstream& ofsH,const AufEintrag& AE);

    bool iterieren(ofstream &ofs,ofstream &ofsH,const AufEintragBase& AE);
    std::list<AufEintragZu::st_reflist> get_zuordnung_for_artikelbaum(const AufEintrag &AE,const ArtikelBaum &artbaum);
    bool loesche_falsche_artikel_in_zuordnung(ofstream &ofs,const AufEintragBase &AEB,const ArtikelBaum& artbaum,const std::list<AufEintragZu::st_reflist>& AZ);  
/*
    bool check_ArtikelAuftrag(ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AuftragsListe);
//    ArtikelBase get_artikel_for_instanz(const std::list<AufEintragZu::st_reflist>& AuftragsListe,const ppsInstanz::ppsInstId instanz);
    bool AufArtKonsistenz::kundenauftrag_testen(ofstream& ofs,const AufEintrag& AE,std::list<AufEintragZu::st_reflist>& AuftragsListe);
    void ueberspringe_instanz(const ppsInstanz::ppsInstId instanz,std::list<AufEintragZu::st_reflist>& AuftragsListe, AufEintrag& AE);
    bool teste_lager(ofstream &ofs,const ppsInstanz::ppsInstId instanz,std::list<AufEintragZu::st_reflist>& AuftragsListe,bool& Lager);
*/
    void get_deeper_list(std::list<AufEintragZu::st_reflist>& AuftragsListe);
    enum Fehler{Kunde_nicht,Kunde_mehr_als_eins,Kunde_Ohne,Kunde_Instanz,FInstanz,
                Kunde_MengeZ,H_Kunde_MengeZ,Kunde_MengeAEB,Kunde_MengeAEBZ,
                H_Kunde_MengeAEB,H_Lager_MengeAEB,
                Kunde_Artikel,Anzahl,Artikel,LagerMenge,LagerLaenge};
    bool fehler(ofstream &ofsK,ofstream &ofsH,const Fehler &bug,const AufEintrag& AE,const AufEintragBase &AEB,int int_wert=0);
    void fehler(ofstream &ofs,const Fehler &bug,const AufEintrag& AE,const AufEintragBase &AEB,int int_wert=0);
/*
    void AufArtKonsistenz::korrektur(ofstream &ofs,const AufEintrag& AE,const pair<ArtCIterator,ArtCIterator>& MisIt);
    void falscheAnzahl(ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf);
    void falscheArtikel(ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf,const pair<ArtCIterator,ArtCIterator>& MisIt);

    void delete_auftrag(ofstream &ofs,const AufEintrag& AE,const ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AufList);
    void AufArtKonsistenz::create_auftrag(ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AufList);
*/
 public:
    AufArtKonsistenz();
};

#endif
