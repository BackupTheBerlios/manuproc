#include "auftrag_main.hh"
#include "class_instanz_trees.hh"
#include <Artikel/ArtikelStamm.h>
#include "class_auftrag_tree.hh"
#include <Auftrag/AufEintragZu.h>
#include <unistd.h>
#include "MyMessage.h"


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
    else                        kid = Kunde::default_id ;
    if(kid==Kunde::none_id) return;
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
  std::vector<cH_RowDataBase> V=maintree_s->getSelectedRowDataBase_vec();
  ArtikelBase artikel;
  AuftragBase::mengen_t menge=0;
  vector<AufEintrag> VAE;
  for(std::vector<cH_RowDataBase>::const_iterator i=V.begin();i!=V.end();++i)
   {
     const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*(*i));
     AufEintrag AE=dt->get_AufEintrag();
     // Alle Ausgewählten Zeilen müssen den selben Artikel beinhalten
     if(artikel==ArtikelBase::none_id) artikel=AE.Artikel();
     else { if(artikel!=AE.Artikel()) 
       {meldung->Show("Die Artikel der ausgewählten Zeilen sind nicht einheitlich");
        return; }}
     menge += AE.getRestStk();
     VAE.push_back(AE);
   }
  if (togglebutton_geplante_menge->get_active())
   {
     spinbutton_geplante_menge->update();
     AuftragBase::mengen_t m=spinbutton_geplante_menge->get_value_as_int();
     if(VAE.size()>1 && m < menge)
       {meldung->Show("Teilmengen für mehrer Zeilen sind nicht möglich");
        return; }
     menge=m;
   }   
   // Planen
  try{
  AufEintragBase::Planen(getuid(),VAE,menge,*instanz_auftrag,Datum_instanz->get_value());
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
/*
  if(!instanz_auftrag) return;
  AuftragBase::mengen_t menge=0;
  if (togglebutton_geplante_menge->get_active())
   {
    gtk_spin_button_update(spinbutton_geplante_menge->gtkobj());
    menge = spinbutton_geplante_menge->get_value_as_int();
    if(menge>selected_AufEintrag.getRestStk()) menge=selected_AufEintrag.getRestStk();
   }   
  else  menge = selected_AufEintrag.getRestStk();
  
  selected_AufEintrag.Planen(getuid(),menge, *instanz_auftrag,Datum_instanz->get_value());

  cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
  dynamic_cast<Data_auftrag&>(const_cast<RowDataBase&>(*dt)).redisplayMenge(maintree_s);
  loadAuftragInstanz(*instanz_auftrag);
*/
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
  if (ev->button==1)
   {
     std::string s="auftrag_drucken -p -a Intern -n "+searchcombo_auftragid->get_text()+" -i "+itos(instanz->Id());
     system(s.c_str());
   } 
  if (ev->button==3); 
   {
     std::string s="auftrag_drucken -a Intern -n "+searchcombo_auftragid->get_text()+" -i "+itos(instanz->Id());
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
//std::cout << j->Menge <<'\t'<<menge<<'\n';
         try{ i->abschreiben(menge,ManuProcEntity::none_id);
              change=true;
            } catch(SQLerror &e){std::cerr <<e<<'\n';}
       }
    }
  }
  if(change) on_neuladen_activate() ;
}


