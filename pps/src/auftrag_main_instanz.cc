#include "auftrag_main.hh"
#include "class_instanz_trees.hh"
#include <Artikel/ArtikelStamm.h>
#include "class_auftrag_tree.hh"
#include <Auftrag/AufEintragZu.h>
#include <unistd.h>
#include "MyMessage.h"
//#include <Instanzen/Produziert.h>
#include <Misc/relops.h>
#include <Misc/i18n.h>

extern MyMessage *meldung;


void auftrag_main::on_OptMenu_Instanz_Bestellen_activate()
{
//std::cout << OptMen_Instanz_Bestellen->get_value()->Id() << "\n";
 akt_instanz->set_value(OptMen_Instanz_Bestellen->get_value());
}


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
      { meldung->Show(_("Keine Lieferantennummer vergeben"));
        return;}
    instanz_auftrag = new AuftragFull(Auftrag::Anlegen(instanz->Id()),kid);
    if(instanz->ExterneBestellung())
      instanz_auftrag->setStatusAuftragFull(UNCOMMITED);
    else
      instanz_auftrag->setStatusAuftragFull(OPEN);
    AuftragBase ab(*instanz_auftrag);
    loadAuftragInstanz(ab);
    searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),instanz_auftrag->Id());
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
 searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),instanz_auftrag->Id());
 kunden_lieferant->set_value(instanz_auftrag->getKundennr()) ;
}


void auftrag_main::on_searchcombo_auftragid_activate()
{
 loadAuftragInstanz(AuftragBase(instanz->Id(),searchcombo_auftragid->Content()));
}


void auftrag_main::tree_neuer_auftrag_leaf_selected(cH_RowDataBase d)
{
#if 0
//  tree_neuer_auftrag->clear();
  const Data_neuer_auftrag *dt=dynamic_cast<const Data_neuer_auftrag*>(&*d);
  Data_neuer_auftrag *Dna = const_cast<Data_neuer_auftrag*>(dt); 
  AufEintrag IA=Dna->get_AufEintrag();

  // Referenzaufträge müssen geholt werden BEVOR der Auftrag gelöscht wird!!!
  std::list<AufEintragZu::st_reflist> ReferenzAufEintragK = AufEintragZu(IA).get_Referenz_listFull(false); // Entsprechenden Kundenauftrag (die 2er sind auch mit dabei) hohlen
  std::list<AufEintragZu::st_reflist> ReferenzAufEintragR = AufEintragZu(IA).get_Referenz_list(IA,false,false); // direkte Referenen hohlen
  try{
  Transaction tr;
  if(!IA.deleteAuftragEntry())  // Auftrag hat noch Kinder?
   {
     meldung->Show(_("Auftrag kann nicht gelöscht werden, es gibt noch Kindaufträge")); 
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
          AuftragBase::mengen_t mt=i->AEB.updateStkDiffBase__(i->Menge);
          assert(mt==i->Menge);
         }
       else if(i->AEB.Id()==AuftragBase::dispo_auftrag_id)
         {
          AuftragBase::mengen_t mt=i->AEB.updateStkDiffBase__(-i->Menge);
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
#endif
}

void auftrag_main::on_button_instanz_get_selection_clicked()
{
  if(!instanz_auftrag) { meldung->Show(_("Keine Auftragsnummer vergeben"));
                         return; }
  if(!Datum_instanz->get_value().valid()) 
      { meldung->Show(_("Datum ist ungültig"));
                         return; }
  try{ 

  std::vector<cH_RowDataBase> VAE=ausgewaehlte_artikel();
  if(VAE.empty()) {meldung->Show(_("Nichts ausgewählt")); return;}
  if(!alle_ausgewaehlten_artikel_gleich(VAE))
   {
    for(std::vector<cH_RowDataBase>::iterator i=VAE.begin();i!=VAE.end();++i)
     {
      const Data_auftrag &dt=dynamic_cast<const Data_auftrag&>(**i);
      AufEintrag &AE=const_cast<Data_auftrag&>(dt).get_AufEintrag();
      if(!OptMen_Instanz_Bestellen->is_visible())
         AE.Planen(AE.getRestStk(),*instanz_auftrag,Datum_instanz->get_value());
//      else
//         AE.ProduktionsPlanung(AE.getRestStk(),*instanz_auftrag,Datum_instanz->get_value(),OptMen_Instanz_Bestellen->get_value());
//         AE.ProduktionsPlanung(OptMen_Instanz_Bestellen->get_value(),
//                               AE.getRestStk(),Datum_instanz->get_value());
     }     
   }
  else
   {
     spinbutton_geplante_menge->update();
     AuftragBase::mengen_t spinmenge=spinbutton_geplante_menge->get_value_as_int();
     AuftragBase::mengen_t mindestmenge=restmengensumme_aller_ausgewaehlten_artikel(VAE);
     if(spinmenge<mindestmenge && VAE.size()>1)
       {meldung->Show(_("Sind mehrere Zeilen gewählt muß die Bestellmenge mindestens so groß sein wie die Summer der Restmengen."));
        return; }
     for(std::vector<cH_RowDataBase>::iterator i=VAE.begin();i!=VAE.end();++i)
      {
       const Data_auftrag &dt=dynamic_cast<const Data_auftrag&>(**i);
       AufEintrag &AE=const_cast<Data_auftrag&>(dt).get_AufEintrag();
       AuftragBase::mengen_t M=AE.getRestStk();       
       spinmenge -= M;
       assert(spinmenge>=0 || VAE.size()==1);
       if(i+1 == VAE.end() )  {M+=spinmenge; spinmenge=0;}
  
       if(!OptMen_Instanz_Bestellen->is_visible())
         AE.Planen(M,*instanz_auftrag,Datum_instanz->get_value());
//       else
//         AE.ProduktionsPlanung(M,*instanz_auftrag,Datum_instanz->get_value(),OptMen_Instanz_Bestellen->get_value());
//         AE.ProduktionsPlanung(OptMen_Instanz_Bestellen->get_value(),
//                               M,Datum_instanz->get_value());
      }     
   }  

  loadAuftragInstanz(*instanz_auftrag);
  for(std::vector<cH_RowDataBase>::iterator i=VAE.begin();i!=VAE.end();++i)
   {
     const Data_auftrag &dt=dynamic_cast<const Data_auftrag&>(**i);
     dt.redisplayMenge(maintree_s);
   }
  maintree_s->get_selection()->unselect_all();

   }catch(SimpleTree::notLeafSelected) 
     {meldung->Show(_("Nichts gewählt")); }
}

bool auftrag_main::alle_ausgewaehlten_artikel_gleich(std::vector<cH_RowDataBase> L)
{
  if(L.empty()) L=ausgewaehlte_artikel();
  assert(!L.empty());
  const Data_auftrag &dt=dynamic_cast<const Data_auftrag&>(*(L.front()));
  ArtikelBase artikel(dt.get_AufEintrag().Artikel());
  for(std::vector<cH_RowDataBase>::const_iterator i=L.begin();i!=L.end();++i)
   {
     const Data_auftrag &dt=dynamic_cast<const Data_auftrag&>(**i);
     if(artikel!=dt.get_AufEintrag().Artikel())  return false;
   }
  return true;
}

AuftragBase::mengen_t auftrag_main::restmengensumme_aller_ausgewaehlten_artikel(std::vector<cH_RowDataBase> L)
{
  if(L.empty()) L=ausgewaehlte_artikel();
  assert(!L.empty());
  AuftragBase::mengen_t M=0;
  for(std::vector<cH_RowDataBase>::const_iterator i=L.begin();i!=L.end();++i)
   {
     const Data_auftrag &dt=dynamic_cast<const Data_auftrag&>(**i);
     M+=dt.get_AufEintrag().getRestStk();
   }
  return M;
}


std::vector<cH_RowDataBase> auftrag_main::ausgewaehlte_artikel()
{
  std::vector<cH_RowDataBase> V;
  try{ V=maintree_s->getSelectedRowDataBase_vec();
     }catch(SimpleTree::notLeafSelected) 
     {meldung->Show(_("Fehler: Keine Knoten anwählen")); }
  return V;
}


void auftrag_main::instanz_auftrag_anlegen(AufEintrag& AE)
// entspricht 'on_leaf_selected' im Hauptbaum
{
  try{
   if(maintree_s->getSelectedRowDataBase_vec().size()==1)
    {
      spinbutton_geplante_menge->set_value(AE.getRestStk().as_int());
      Datum_instanz->set_value(AE.getLieferdatum());
      frame_mengen_eingabe->show();
    }
   else // mehrere Zeilen gewählt
    {
      if(alle_ausgewaehlten_artikel_gleich())       
       {
         spinbutton_geplante_menge->update();
         int m=spinbutton_geplante_menge->get_value_as_int();
         spinbutton_geplante_menge->set_value(m+AE.getRestStk().as_int());
       }
      else frame_mengen_eingabe->hide() ;   
      if(Datum_instanz->get_value()>AE.getLieferdatum())
         Datum_instanz->set_value(AE.getLieferdatum());
    }
  }catch(SimpleTree::multipleRowsSelected) {}
}

void auftrag_main::on_leaf_unselected()
{ 
#warning deselektierte Zeilen wieder abziehen ?
// try{
//   const Data_auftrag *dt=dynamic_cast<const Data_auftrag*>(&*d);
//   AufEintrag &AE=dt->get_AufEintrag();
//   spinbutton_geplante_menge->update();
//   int m=spinbutton_geplante_menge->get_value_as_int();
//   spinbutton_geplante_menge->set_value(m-AE.getRestStk().as_int());
//  }catch(std::exception &e) {std::cerr<<e.what();}
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
 s.push_back(_("Kunde"));
 s.push_back(_("Artikel"));
 s.push_back(_("Menge"));
 s.push_back(_("Datum"));
 tree_neuer_auftrag->setTitles(s);
}

void auftrag_main::on_button_instanz_print_clicked()
{
  std::string EI="Intern";
  if(instanz->ExterneBestellung()) EI="Extern";
  std::string s="auftrag_drucken -Y0,0,1 -a"+EI
    +" -n"+itos(searchcombo_auftragid->Content())+" -i"+itos(instanz->Id());
  system(s.c_str());
}

/*
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
*/

void auftrag_main::on_button_Kunden_erledigt_clicked()
{
/* nicht implementiert
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
              i->ProduziertNG(menge,ManuProcEntity<>::none_id);
              change=true;
            } catch(SQLerror &e){std::cerr <<e<<'\n';}
       }
    }
  }
  if(change) on_neuladen_activate() ;
*/
}


