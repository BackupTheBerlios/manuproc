#include "auftrag_main.hh"
#include "class_instanz_trees.hh"
#include <Artikel/ArtikelStamm.h>

void auftrag_main::on_button_neue_anr_clicked()
{
 try { 
    instanz_auftrag = new AuftragFull(Auftrag::Anlegen(instanz->Id()),1,Datum_instanz->get_value().Jahr());
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
        instanz_auftrag =new AuftragFull(ab);
      } catch(SQLerror &e)
      { cerr << e ; instanz_auftrag=NULL; return;} 
        catch (Kalenderwoche::error &e)
      { cerr << "KW error\n"; } 
/*
 datavec_instanz_auftrag.clear();
 for (AuftragFull::const_iterator i=instanz_auftrag->begin();i!=instanz_auftrag->end();++i)
    datavec_instanz_auftrag.push_back(new Data_neuer_auftrag(this,*i));  
 tree_neuer_auftrag->clear();
 tree_neuer_auftrag->setDataVec(datavec_instanz_auftrag);
*/
 show_neuer_auftrag();
 searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),'0');
}


void auftrag_main::on_searchcombo_auftragid_activate()
{
 loadAuftrag(AuftragBase(instanz->Id(),searchcombo_auftragid->Content()));
}

void auftrag_main::tree_neuer_auftrag_leaf_selected(cH_RowDataBase d)
{
//  Data_neuer_auftrag *dt=dynamic_cast<Data_neuer_auftrag*>(&(const_cast<RowDataBase&>(*d)));
  const Data_neuer_auftrag *dt=dynamic_cast<const Data_neuer_auftrag*>(&*d);
  Data_neuer_auftrag *Dna = const_cast<Data_neuer_auftrag*>(dt); 
  (Dna->get_AufEintragBase()).deleteAuftragEntry();
  loadAuftrag(Dna->get_AufEintragBase());
}

void auftrag_main::instanz_leaf_auftrag(AufEintragBase& selected_AufEintrag)
{
  if(!instanz_auftrag) return;
  long menge=0;
  if (togglebutton_geplante_menge->get_active())
   {
    gtk_spin_button_update(spinbutton_geplante_menge->gtkobj());
    menge = spinbutton_geplante_menge->get_value_as_int();
    if(menge>selected_AufEintrag.getRestStk()) menge=selected_AufEintrag.getRestStk();
   }   
  else  menge = selected_AufEintrag.getRestStk();

  selected_AufEintrag.abschreiben(menge);

  int znr = (dynamic_cast<Auftrag*>(instanz_auftrag))->insertNewEntry(menge,
     selected_AufEintrag.getLieferdatum(),selected_AufEintrag.ArtikelID());

  AuftragsEntryZuordnung(selected_AufEintrag,*instanz_auftrag,znr);

  cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
  maintree_s->redisplay(dt,Data_auftrag::METER);         
  maintree_s->redisplay(dt,Data_auftrag::STUECK);         
                  
//  show_neuer_auftrag();
  loadAuftrag(*instanz_auftrag);
}

void auftrag_main::show_neuer_auftrag()
{
  if (!instanz_auftrag) return;
  tree_neuer_auftrag->clear();
  vector<cH_RowDataBase> datavec;
  for (AuftragFull::const_iterator i=instanz_auftrag->begin();i!=instanz_auftrag->end();++i)
     datavec.push_back(new Data_neuer_auftrag(this,*i));  
  tree_neuer_auftrag->setDataVec(datavec);
}

void auftrag_main::neuer_auftrag_tree_titel_setzen()
{
 vector<std::string> s;
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
     std::string s="auftrag_drucken -p -a Auftrag -n "+searchcombo_auftragid->get_text()+" -i "+itos(instanz->Id());
     system(s.c_str());
   } 
  if (ev->button==3); 
   {
     std::string s="auftrag_drucken -a Auftrag -n "+searchcombo_auftragid->get_text()+" -i "+itos(instanz->Id());
     system(s.c_str());
   }
 return false;
}
