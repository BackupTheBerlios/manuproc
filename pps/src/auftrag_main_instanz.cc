#include "auftrag_main.hh"
#include "class_instanz_trees.hh"
#include <Artikel/ArtikelStamm.h>

void auftrag_main::on_button_neue_anr_clicked()
{
 if(!an_instanz->Id()) {cerr << "Erst eine Instanz wählen, an die der Auftrag gehen soll\n"; 
   return; }
 try { 
    instanz_auftrag = new Auftrag(an_instanz->Id(),1,9999);
    AuftragBase ab(*instanz_auftrag);
    loadAuftrag(ab);
    searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),'0');
   } catch(SQLerror &e) 
      { cerr << e;
        instanz_auftrag=NULL;
        return;
      }
}

void auftrag_main::loadAuftrag(const AuftragBase& auftragbase)
{
  try { AuftragBase ab(auftragbase);
        if(instanz_auftrag) delete instanz_auftrag;
        instanz_auftrag = new Auftrag(ab);
        instanz_aufeintrag =new AuftragFull(ab);
      } catch(SQLerror &e)
      { cerr << e ; instanz_auftrag=NULL; return;} 
        catch (Kalenderwoche::error &e)
      { cerr << "KW error\n"; } 
 datavec_instanz_auftrag.clear();
 for (AuftragFull::const_iterator i=instanz_aufeintrag->begin();i!=instanz_aufeintrag->end();++i)
  {
    datavec_instanz_auftrag.push_back(new Data_neuer_auftrag(
      this,ArtikelBase(i->ArtikelID()),i->getStueck(),
      i->getLieferdatum(),i->getEntryStatus(),i->getZnr()));  
  }
 tree_neuer_auftrag->clear();
 tree_neuer_auftrag->setDataVec(datavec_instanz_auftrag);

 searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),'0');
 
 instanz_auftrag_status->set_History(instanz_auftrag->getStatus());
 instanz_eintrag_status->hide();
 label_eintragstatus->hide();
}


void auftrag_main::on_searchcombo_auftragid_activate()
{
 loadAuftrag(AuftragBase(an_instanz->Id(),searchcombo_auftragid->Content()));
}

void auftrag_main::on_instanz_auftrag_status_activate()
{
 if (instanz_auftrag)
  {  
    instanz_auftrag->setStatusAuftrag(instanz_auftrag_status->get_Status());
    loadAuftrag(*instanz_auftrag);
  }
}
void auftrag_main::on_instanz_eintrag_status_activate()
{
 if (instanz_auftrag)
  {  
    instanz_aufeintrag->setStatusEntryZnr(selected_instanz_znr,instanz_eintrag_status->get_Status());
    loadAuftrag(*instanz_auftrag);
  }
}

gint auftrag_main::on_eventbox_instanz_button_press_event(GdkEventButton *event)
{
 menu_an_instanz->popup(event->button,event->time);
 return true;
}

void auftrag_main::an_instanz_selected(ppsInstanz::ppsInstId insnr)
{
 an_instanz =cH_ppsInstanz(insnr);
 label_an_instanz->set_text(an_instanz->get_Name());
//cout<< an_instanz.get_Name()<<"\t"<<an_instanz.Id()<<"\n";
}

void auftrag_main::instanz_tree_herkunft_leaf_selected(cH_RowDataBase d)
{
  if(!instanz_auftrag) return;
  const Data_instanz_herkunft *dt=dynamic_cast<const Data_instanz_herkunft*>(&*d);
  AufStatVal status = instanz_auftrag_status->get_Status();
  int znr = get_next_entry_znr(*instanz_auftrag);
  datavec_instanz_auftrag.push_back(new Data_neuer_auftrag(this,dt->ArtBase(),dt->Menge(),
         dt->get_Datum(),status,znr));  

  if (instanz_auftrag)
   {
    instanz_auftrag->insertNewEntry(dt->Menge(),dt->get_Datum(),dt->ArtBase().Id());
   }  

  tree_neuer_auftrag->clear();
  tree_neuer_auftrag->setDataVec(datavec_instanz_auftrag);
}

void auftrag_main::tree_neuer_auftrag_leaf_selected(cH_RowDataBase d)
{
  const Data_neuer_auftrag *dt=dynamic_cast<const Data_neuer_auftrag*>(&*d);
  selected_instanz_znr=dt->getZnr();
  label_eintragstatus->show();
  instanz_eintrag_status->show();
  instanz_eintrag_status->set_History(dt->Status());
}


void auftrag_main::instanz_tree_inhalt_setzen(int artid,int laenge_m,Petig::Datum datum)
{
   std::vector<cH_RowDataBase> datavec;
   ArtikelBase A(artid);
   ArtikelBaum AB(A);
   if(instanz->Id() != 8)
    for (ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
     {
      ArtikelStamm AS(i->rohartikel);
      an_instanz_selected(AS.BestellenBei()->Id());
      datavec.push_back(new Data_instanz_herkunft(   
         cH_ppsInstanz(AS.BestellenBei()),i->rohartikel,i->menge,datum));
     }
   else
    {
      // Die Schussfaeden sind doch wohl an dieser Stelle unwichtig, oder?
      an_instanz_selected(ppsInstanz::INST_SCHAER);
      Schussfaeden schussfaeden;
      Fadenliste fadenliste;
      get_faeden(artid,schussfaeden,fadenliste);
      for (Fadenliste::const_iterator i=fadenliste.sumbegin();i!=fadenliste.sumend();++i)
       {
//        const double km_m=0.001;
//        double menge = laenge_m * km_m *i->get_Gewicht_kg_pro_km_Faden() ;
        cH_ppsInstanz instanz(ppsInstanz::INST_SCHAER);
        datavec.push_back(new Data_instanz_herkunft(   
           instanz,i->getMaterial(),laenge_m,datum,"m"));
       }
    }
   instanz_tree_herkunft->setDataVec(datavec);
}

void auftrag_main::get_faeden(int artid,Schussfaeden& schussfaeden,
      Fadenliste& fadenliste)
{
 Bindungsliste bindungsliste;
 ArtikelBase artbase(artid);
 try {                  
    schussfaeden.Load(artbase);
    bindungsliste.Load();
    fadenliste.Load(artbase,bindungsliste);
   }
 catch(SQLerror &e) {std::cerr << e << '\n';}
}


void auftrag_main::instanz_tree_titel_setzen()
{
 vector<std::string> s;
 s.push_back("Instanz");
 s.push_back("Artikel");
 s.push_back("Menge");
 instanz_tree_herkunft->setTitles(s);
}

void auftrag_main::neuer_auftrag_tree_titel_setzen()
{
 vector<std::string> s;
 s.push_back("Artikel");
 s.push_back("Menge");
 s.push_back("Datum");
 s.push_back("Status");
 tree_neuer_auftrag->setTitles(s);
}
