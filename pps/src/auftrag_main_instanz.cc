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
if(instanz_auftrag)
  try { AuftragBase ab(auftragbase);
        if(instanz_auftrag) delete instanz_auftrag;
        instanz_auftrag = new AuftragFull(ab);
      } catch(SQLerror &e)
      { cerr << e ; instanz_auftrag=NULL; return;} 
        catch (Kalenderwoche::error &e)
      { cerr << "KW error\n"; } 
cout <<"SIZE = "<< instanz_auftrag->size()<<'\n';
 show_neuer_auftrag();
 searchcombo_auftragid->setContent(instanz_auftrag->getAuftragidToStr(),'0');
}


void auftrag_main::on_searchcombo_auftragid_activate()
{
 loadAuftrag(AuftragBase(instanz->Id(),searchcombo_auftragid->Content()));
}

void auftrag_main::tree_neuer_auftrag_leaf_selected(cH_RowDataBase d)
{
  const Data_neuer_auftrag *dt=dynamic_cast<const Data_neuer_auftrag*>(&*d);
  Data_neuer_auftrag *Dna = const_cast<Data_neuer_auftrag*>(dt); 
  AufEintragBase IA=Dna->get_AufEintragBase();

  IA.deleteAuftragEntry();

  list<pair<AufEintragBase2,long> > ReferenzAufEintrag = IA.get_Referenz_AufEintragBase2();
  int tmp_menge = IA.getStueck();
  for (list<pair<AufEintragBase2,long> >::iterator i=ReferenzAufEintrag.begin();i!=ReferenzAufEintrag.end();++i)
   {
    if(tmp_menge<=0) break;
    // Prozess setzen, aber nur, wenn er nicht schon früher für diesen 
    // Auftrag gesetzt wurde. WARNUNG WAS IST MIT TEILAUFTRÄGEN????
    if(AufEintragBase(i->first).getProzess() != instanz->get_Prozess())
     {
       i->first.setVerarbeitung(instanz->get_Prozess());
       tmp_menge -= i->second;
     }
   }
  ReferenzAufEintrag = IA.get_Referenz_AufEintragBase2(false); // direkte Referenen hohlen
  tmp_menge = IA.getStueck();
  for (list<pair<AufEintragBase2,long> >::iterator i=ReferenzAufEintrag.begin();i!=ReferenzAufEintrag.end();++i)
   {
    if(tmp_menge<=0) break;
    long rest = AufEintragBase(i->first).getRestStk();
    i->first.abschreiben( tmp_menge<rest ? -tmp_menge : -rest );
    tmp_menge -= i->second;
   }
//???  IA.get_Referenz_AufEintragBase2().abschreiben(-IA.getStueck());
//???  IA.get_Referenz_AufEintragBase2(true).setVerarbeitung(cH_Prozess(Prozess::None));

  on_neuladen_activate();
  loadAuftrag(Dna->get_AufEintragBase());
}

void auftrag_main::instanz_leaf_auftrag(AufEintragBase& selected_AufEintrag)
// entspricht 'on_leaf_selected' im Hauptbaum
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

  list<pair<AufEintragBase2,long> > ReferenzAufEintrag = selected_AufEintrag.get_Referenz_AufEintragBase2();
  int tmp_menge = menge;
  for (list<pair<AufEintragBase2,long> >::iterator i=ReferenzAufEintrag.begin();i!=ReferenzAufEintrag.end();++i)
   {
    if(tmp_menge<=0) break;
    // Prozess setzen, aber nur, wenn er nicht schon früher für diesen 
    // Auftrag gesetzt wurde. WARNUNG WAS IST MIT TEILAUFTRÄGEN????
    if(AufEintragBase(i->first).getProzess() != instanz->get_Prozess())
     {
       i->first.setVerarbeitung(instanz->get_Prozess());
       tmp_menge -= i->second;
     }
   }

  int znr = (dynamic_cast<Auftrag*>(instanz_auftrag))->insertNewEntry(menge,
       selected_AufEintrag.getLieferdatum(),selected_AufEintrag.ArtId());

  AuftragsEntryZuordnung(selected_AufEintrag,menge,*instanz_auftrag,znr);

  cH_RowDataBase dt(maintree_s->getSelectedRowDataBase());
  maintree_s->redisplay(dt,Data_auftrag::METER);         
  maintree_s->redisplay(dt,Data_auftrag::STUECK);         
                  
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
