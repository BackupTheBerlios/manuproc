#include "auftrag_main.hh"
#include <Lager/JumboLager.h>
#include <Lager/RohwarenLager.h>
#include "class_auftrag_lager.hh"

void auftrag_main::lager_zeigen()
{
  lager_ueberschrift();
  std::vector<cH_RowDataBase> datavec_frei,datavec_verplant;
  SelectedFullAufList AuftragsListe=lager_auftraege();
/*
  std::vector<LagerInhalt> LI; 

#ifdef PETIG_EXTENSIONS
  if(instanz==ppsInstanzID::Bandlager)
   {
     JumboLager JL;
     LI=JL.LagerInhalt();
   }
  else if(instanz==ppsInstanzID::Rohwarenlager)
   {
     RohwarenLager RL;
     LI=RL.LagerInhalt();
     RL.LagerInhaltSum(LI);
   }
#endif
*/
/*
  for(std::vector<LagerInhalt>::const_iterator i=LI.begin();i!=LI.end();++i)
   {
     std::list<st_tree_lager> L=auftrags_lager_mengen(i->Artikel(),AuftragsListe);        
     if(L.empty())
        datavec.push_back(new Data_Lager(*i,AufEintrag(),0,0,this));     
     else if(++L.begin()==L.end()) // schneller als L.size()==1 
        datavec.push_back(new Data_Lager(*i,L.begin()->AE,L.begin()->verplant,L.begin()->bestellt,this));     
     else
       {for(std::list<st_tree_lager>::const_iterator j=L.begin();j!=L.end();++j)
           datavec.push_back(new Data_Lager(*i,j->AE,j->verplant,j->bestellt,this,true));     
       }
   }
*/
//cout << AuftragsListe.size()<<'\n';
  for(SelectedFullAufList::const_iterator i=AuftragsListe.begin();i!=AuftragsListe.end();++i)
   {
//     LagerInhalt LI(i->ArtId(), 0, 0, i->getRestStk(),0);
//cout << *i<<'\n';
     if(i->Id() == AuftragBase::dispo_auftrag_id) // Freie Menge
        datavec_frei.push_back(new Data_Lager(*i,this));     
     else if(i->Id() == AuftragBase::plan_auftrag_id) // Verplante Menge
        datavec_verplant.push_back(new Data_Lager(*i,this));     
//     else {cout << *i <<'\n';assert(!"Diesen Auftrag hätte es nicht geben sollen\n");}
   }
//  tree_lager->set_NewNode(&Data_Lager_Node::create);
  tree_lager_frei->setDataVec(datavec_frei);
  tree_lager_verplant->setDataVec(datavec_verplant);
}


/*
std::list<auftrag_main::st_tree_lager> auftrag_main::auftrags_lager_mengen(ArtikelBase artikel,SelectedFullAufList& AuftragsListe)
{
  std::list<st_tree_lager> LagerAuftragListe;
  
  for(SelectedFullAufList::const_iterator i=AuftragsListe.begin();i!=AuftragsListe.end();++i)
   {
     if(artikel==i->ArtId())
      {
        if(i->Id()==0) 
           LagerAuftragListe.push_back(st_tree_lager(*i,0,i->getStueck()));
        else // diese Aufträge sind 'vorgemerkt'
           LagerAuftragListe.push_back(st_tree_lager(*i,i->getStueck(),0));
      }
   }    
  for(std::list<st_tree_lager>::const_iterator i=LagerAuftragListe.begin();i!=LagerAuftragListe.end();++i)
     AuftragsListe.remove(i->AE);
  return LagerAuftragListe;
}
*/

SelectedFullAufList auftrag_main::lager_auftraege()
{
  // holt geplante (id=1) und ungeplante (id=0) Aufträge
  SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),OPEN);
  return SelectedFullAufList(psel);
}


void auftrag_main::on_lager_leaf_selected(cH_RowDataBase d)
{
 try{
   const Data_Lager *dt=dynamic_cast<const Data_Lager*>(&*d);
//   selected_AufEintrag = &dt->get_KundenAufEintrag();
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
 ct.push_back("Artikel");
 ct.push_back("Menge");
 ct.push_back("Datum");   

 tree_lager_frei->setTitles(ct);
 tree_lager_verplant->setTitles(ct);
/*
 ct.push_back("Stueck (Rest)");

#ifdef PETIG_EXTENSIONS 
 if(instanz==ppsInstanzID::Bandlager)
    ct.push_back("Menge (m)"); 
 else if(instanz==ppsInstanzID::Rohwarenlager)
    ct.push_back("Menge (kg)"); 
 else 
#endif
    ct.push_back("?");
 ct.push_back("Verplant");
 ct.push_back("Bestellt");
 ct.push_back("Frei");
 tree_lager->setTitles(ct);   
 int cols=tree_lager->columns().size();
 tree_lager->set_column_justification(cols-1, GTK_JUSTIFY_RIGHT);
 tree_lager->set_column_justification(cols-2, GTK_JUSTIFY_RIGHT);
 tree_lager->set_column_justification(cols-3, GTK_JUSTIFY_RIGHT);
 tree_lager->set_column_justification(cols-4, GTK_JUSTIFY_RIGHT);
*/
}

