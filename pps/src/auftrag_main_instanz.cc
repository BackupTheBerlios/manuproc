#include "auftrag_main.hh"
#include "class_instanz_trees.hh"
#include <Artikel/ArtikelStamm.h>
#include "class_auftrag_tree.hh"
#include <Auftrag/AufEintragZu.h>
#include <unistd.h>
#include "MyMessage.h"
//#include <Instanzen/Produziert.h>
#include <Instanzen/ppsInstanzProduziert.h>


extern MyMessage *meldung;


void auftrag_main::on_kunden_lieferant_activate()
{
  button_neue_anr->grab_focus();
}


void auftrag_main::on_button_neue_anr_clicked()
{
 try { 
    Kunde::ID kid=Kunde::none_id; 
    if(instanz->Lieferschein()) kid = kunden_lieferant->get_value() ;
    else                        kid = ManuProC::DefaultValues::EigeneKundenId ;
    if(kid==Kunde::none_id) 
      { meldung->Show("Keine Lieferantennummer vergeben");
        return;}
    instanz_auftrag = new AuftragFull(Auftrag::Anlegen(instanz->Id()),kid);
    instanz_auftrag->setStatusAuftragFull(OPEN,int(getuid()));
    AuftragBase ab(*instanz_auftrag);
    loadAuftragInstanz(ab);
    searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),'0');
   } catch(SQLerror &e) 
      { std::cerr << e;
        instanz_auftrag=NULL;
        return;
      }
}

void auftrag_main::loadAuftragInstanz(const AuftragBase& auftragbase)
{
  try { AuftragBase ab(auftragbase);
        if(instanz_auftrag) delete instanz_auftrag;
        instanz_auftrag = new AuftragFull(ab);
      } catch(SQLerror &e)
      { std::cerr << e ; instanz_auftrag=NULL; return;} 
        catch (Kalenderwoche::error &e)
      { std::cerr << "KW error\n"; } 
//std::cout <<"SIZE = "<< instanz_auftrag->size()<<'\n';
 show_neuer_auftrag();
 searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),'0');
}


void auftrag_main::on_searchcombo_auftragid_activate()
{
 loadAuftragInstanz(AuftragBase(instanz->Id(),searchcombo_auftragid->Content()));
}


void auftrag_main::tree_neuer_auftrag_leaf_selected(cH_RowDataBase d)
{
//  tree_neuer_auftrag->clear();
  const Data_neuer_auftrag *dt=dynamic_cast<const Data_neuer_auftrag*>(&*d);
  Data_neuer_auftrag *Dna = const_cast<Data_neuer_auftrag*>(dt); 
  AufEintrag IA=Dna->get_AufEintrag();

  // Referenzaufträge müssen gehohlt werden BEVOR der Auftrag gelöscht wird!!!
  std::list<AufEintragZu::st_reflist> ReferenzAufEintragK = AufEintragZu(IA).get_Referenz_listFull(false); // Entsprechenden Kundenauftrag (die 2er sind auch mit dabei) hohlen
  std::list<AufEintragZu::st_reflist> ReferenzAufEintragR = AufEintragZu(IA).get_Referenz_list(IA,false); // direkte Referenen hohlen
  try{
  Transaction tr;
  if(!IA.deleteAuftragEntry())  // Auftrag hat noch Kinder?
   {
     info_label_instanzen->set_text("Auftrag kann nicht gelöscht werden, es gibt noch Kindaufträge"); 
     info_label_instanzen->show();
     return;
   }
  for (std::list<AufEintragZu::st_reflist>::iterator i=ReferenzAufEintragK.begin();i!=ReferenzAufEintragK.end();++i)
   {
     if(i->AEB.Instanz()!=ppsInstanzID::Kundenauftraege) continue;
     i->AEB.setLetztePlanungFuer(instanz->Id());
     i->AEB.calculateProzessInstanz();
   }
  for (std::list<AufEintragZu::st_reflist>::iterator i=ReferenzAufEintragR.begin();i!=ReferenzAufEintragR.end();++i)
   {
    AuftragBase::mengen_t rest = AufEintrag(i->AEB).getRestStk();
    try{ 
       if(i->AEB.Id()==AuftragBase::ungeplante_id)
         {
          AuftragBase::mengen_t mt=i->AEB.updateStkDiffBase__(getuid(),i->Menge);
          assert(mt==i->Menge);
         }
       else if(i->AEB.Id()==AuftragBase::dispo_auftrag_id)
         {
          AuftragBase::mengen_t mt=i->AEB.updateStkDiffBase__(getuid(),-i->Menge);
          assert(mt==-i->Menge);
         }
       else assert(!"never get here\n");
      }catch(SQLerror &e)
      {meldung->Show(e); std::cerr << e<<'\n';}
   }

  tr.commit();
  on_neuladen_activate();
  loadAuftragInstanz(Dna->get_AufEintrag());
 }catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_main::on_button_instanz_get_selection_clicked()
{
  if(!instanz_auftrag) { meldung->Show("Keine Auftragsnummer vergeben");
                         return; }
  if(!Datum_instanz->get_value().valid()) 
      { meldung->Show("Datum ist ungültig");
                         return; }

  std::vector<cH_RowDataBase> V;
  try{ V=maintree_s->getSelectedRowDataBase_vec();
     }catch(TreeBase::notLeafSelected) {meldung->Show("Fehler: Keine Knoten anwählen"); return;}
  ArtikelBase artikel;
  AuftragBase::mengen_t menge=0;
  std::vector<AufEintrag> VAE;
  for(std::vector<cH_RowDataBase>::const_iterator i=V.begin();i!=V.end();++i)
   {
     const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*(*i));
     AufEintrag AE=dt->get_AufEintrag();
     // Alle Ausgewählten Zeilen müssen den selben Artikel beinhalten
     if(artikel==ArtikelBase::none_id) artikel=AE.Artikel();
     else { if(artikel!=AE.Artikel() && togglebutton_geplante_menge->get_active() ) 
       {meldung->Show("Die Artikel der ausgewählten Zeilen sind nicht einheitlich.\n Wenn eine Menge vorgegeben wird, dann müssen sie es sein.");
        return; }}
     menge += AE.getRestStk();
     VAE.push_back(AE);
   }
  bool mengenplanung=false;
  if (togglebutton_geplante_menge->get_active())
   {
     spinbutton_geplante_menge->update();
     AuftragBase::mengen_t m=spinbutton_geplante_menge->get_value_as_int();
     if(VAE.size()>1 && m < menge)
       {meldung->Show("Sind mehrere Zeilen mit demselben Artikel gewählt und es wird eine Menge vorgegeben,\nso muß diese Menge größer sein als die Summer der Mengen der Einzelzeilen.");
        return; }
     menge=m-menge;
     if(menge!=0) mengenplanung=true;
   }   
   // Planen
  try{
    for(std::vector<AufEintrag>::iterator i=VAE.begin();i!=VAE.end();++i)
     {
       AuftragBase::mengen_t m=i->getRestStk();
       if(mengenplanung && i+1 == VAE.end() )  m+=menge;
       i->Planen(getuid(),m,true,*instanz_auftrag,Datum_instanz->get_value());
     }     

  for(std::vector<cH_RowDataBase>::const_iterator i=V.begin();i!=V.end();++i)
   {
#warning Das geht so nicht :-( Vermutung: Das push_back legt eine Kopie an?
#warning wie geht es dann?
     dynamic_cast<Data_auftrag&>(const_cast<RowDataBase&>(**i)).redisplayMenge(maintree_s);
   }
  loadAuftragInstanz(*instanz_auftrag);
  }catch(SQLerror &e) {meldung->Show(e);}
}


void auftrag_main::instanz_auftrag_anlegen(AufEintrag& AE)
// entspricht 'on_leaf_selected' im Hauptbaum
{
  Datum_instanz->set_value(AE.getLieferdatum());
  try{
  cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
  dynamic_cast<Data_auftrag&>(const_cast<RowDataBase&>(*dt)).redisplayMenge(maintree_s);
  }catch(TreeBase::multipleRowsSelected) {}
}

void auftrag_main::show_neuer_auftrag()
{
  if (!instanz_auftrag) return;
  tree_neuer_auftrag->clear();
  std::vector<cH_RowDataBase> datavec;
  for (AuftragFull::const_iterator i=instanz_auftrag->begin();i!=instanz_auftrag->end();++i)
     datavec.push_back(new Data_neuer_auftrag(this,*i));  
  tree_neuer_auftrag->setDataVec(datavec);
}

void auftrag_main::neuer_auftrag_tree_titel_setzen()
{
 std::vector<std::string> s;
 s.push_back("Kunde");
 s.push_back("Artikel");
 s.push_back("Menge");
 s.push_back("Datum");
 tree_neuer_auftrag->setTitles(s);
}

gint auftrag_main::on_button_instanz_print_clicked(GdkEventButton *ev)
{
  std::string EI="Intern";
  if(instanz->ExterneBestellung()) EI="Extern";
  if (ev->button==1)
   {
     std::string s="auftrag_drucken -p -a "+EI+" -n "+searchcombo_auftragid->get_text()+" -i "+itos(instanz->Id());
     system(s.c_str());
   } 
  if (ev->button==3); 
   {
     std::string s="auftrag_drucken -a "+EI+" -n "+searchcombo_auftragid->get_text()+" -i "+itos(instanz->Id());
     system(s.c_str());
   }
 return false;
}

void auftrag_main::on_togglebutton_geplante_menge_toggled()
{
 if (togglebutton_geplante_menge->get_active())
  {
   spinbutton_geplante_menge->show();
   spinbutton_geplante_menge->grab_focus();
   spinbutton_geplante_menge->select_region(0,-1);
  }
 else
   spinbutton_geplante_menge->hide();  
}

void auftrag_main::on_button_Kunden_erledigt_clicked()
{
 if(!allaufids) 
   { SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),(AufStatVal)OPEN);
     allaufids = new SelectedFullAufList(psel);
   }
 bool change=false;
 for(SelectedFullAufList::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end(); ++i)
  {
   std::list<AufEintragZu::st_reflist> RL=AufEintragZu(*i).get_Referenz_listFull(false);
   for(std::list<AufEintragZu::st_reflist>::const_iterator j=RL.begin();j!=RL.end();++j)
    {
      AufEintrag AEB(j->AEB);
      if(AEB.getEntryStatus()==CLOSED)
       {
         AuftragBase::mengen_t menge = i->getRestStk();
         try{ 
              i->Produziert(menge,ManuProcEntity<>::none_id);
              change=true;
            } catch(SQLerror &e){std::cerr <<e<<'\n';}
       }
    }
  }
  if(change) on_neuladen_activate() ;
}


