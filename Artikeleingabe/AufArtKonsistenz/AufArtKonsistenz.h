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

    SelectedFullAufList *allaufids;
	 int count,count_ok;
    void get_Auftraege(ppsInstanz::ppsInstId instanz,AufStatVal status);
    void iterieren(std::ofstream &ofs,const AufEintragBase& AE);
    bool check_ArtikelAuftrag(std::ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AuftragsListe);
//    ArtikelBase get_artikel_for_instanz(const std::list<AufEintragZu::st_reflist>& AuftragsListe,const ppsInstanz::ppsInstId instanz);
    bool AufArtKonsistenz::kundenauftrag_testen(std::ofstream& ofs,const AufEintrag& AE,std::list<AufEintragZu::st_reflist>& AuftragsListe);
    void ueberspringe_instanz(const ppsInstanz::ppsInstId instanz,std::list<AufEintragZu::st_reflist>& AuftragsListe, AufEintrag& AE);
    bool teste_lager(std::ofstream &ofs,const ppsInstanz::ppsInstId instanz,std::list<AufEintragZu::st_reflist>& AuftragsListe,bool& Lager);
    std::list<AufEintragZu::st_reflist> get_deeper_list(const std::list<AufEintragZu::st_reflist>& AuftragsListe);

    enum Fehler{Kunde_mehr_als_eins,Kunde_Ohne,Kunde_Instanz,Kunde_Menge,Kunde_Artikel,Anzahl,Artikel,LagerMenge,LagerLaenge};
    void fehler(std::ofstream &ofs,const Fehler &bug,const AufEintrag& AE,ppsInstanz::ppsInstId instanz=ppsInstanz::None);
    void AufArtKonsistenz::korrektur(std::ofstream &ofs,const AufEintrag& AE,const std::pair<ArtCIterator,ArtCIterator>& MisIt);
    void falscheAnzahl(std::ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf);
    void falscheArtikel(std::ofstream &ofs,const AufEintrag& AE,const std::list<ArtikelBase>& LArt, const std::list<ArtikelBase>& LAuf,const std::pair<ArtCIterator,ArtCIterator>& MisIt);

    void delete_auftrag(std::ofstream &ofs,const AufEintrag& AE,const ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AufList);
    void AufArtKonsistenz::create_auftrag(std::ofstream &ofs,const AufEintrag& AE,ArtikelBaum& artbaum, const std::list<AufEintragZu::st_reflist>& AufList);

 public:
    AufArtKonsistenz();
};

#endif
