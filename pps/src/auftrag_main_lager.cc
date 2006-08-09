#include "auftrag_main.hh"
#include <Lager/JumboLager.h>
#include <Lager/RohwarenLager.h>
#include "class_auftrag_lager.hh"
#include <Misc/i18n.h>

void auftrag_main::lager_zeigen()
{
  lager_ueberschrift();
  std::vector<cH_RowDataBase> datavec_frei,datavec_verplant;
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),OPEN,AuftragBase::none_id);
//  SelectedFullAufList AuftragsListe=lager_auftraege();
  SelectedFullAufList AuftragsListe(psel);//=lager_auftraege();
  for(SelectedFullAufList::const_iterator i=AuftragsListe.begin();i!=AuftragsListe.end();++i)
   {
     if(i->Id() == AuftragBase::dispo_auftrag_id) // Freie Menge
        datavec_frei.push_back(new Data_Lager(*i,this));     
     else if(i->Id() == AuftragBase::plan_auftrag_id) // Verplante Menge
        datavec_verplant.push_back(new Data_Lager(*i,this));     
   }
  tree_lager_frei->setDataVec(datavec_frei);
  tree_lager_verplant->setDataVec(datavec_verplant);
}

/*
SelectedFullAufList auftrag_main::lager_auftraege()
{
  // holt geplante (id=1) und ungeplante (id=0) Aufträge
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),OPEN);
  return SelectedFullAufList(psel);
}
*/

void auftrag_main::on_lager_leaf_selected(cH_RowDataBase d)
{
 try{
   const Data_Lager *dt=dynamic_cast<const Data_Lager*>(&*d);
   selected_AufEintrag = &dt->getAufEintrag();
   show_something_for(*selected_AufEintrag);
  }catch(std::exception &e) {std::cerr<<e.what();}
}

void auftrag_main::on_lager_node_selected(const TreeRow &node)
{
}

void auftrag_main::on_lager_unselect_row(gint row, gint column, GdkEvent *event)
{
 show_frame_instanzen_material();
}

void auftrag_main::lager_ueberschrift()
{
 std::vector<std::string> ct;
 ct.push_back(_("Artikel"));
 ct.push_back(_("Menge"));
 ct.push_back(_("Datum"));

 tree_lager_frei->setTitles(ct);
 tree_lager_verplant->setTitles(ct);
}

