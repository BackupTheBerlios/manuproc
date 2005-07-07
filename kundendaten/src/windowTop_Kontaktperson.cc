#include "windowTop.hh"
#include <Gtk_OStream.h>
#include "MyMessage.h"

windowTop::KontaktColumns::KontaktColumns()
{ add(name); add(name2); add(position); add(ansprech);
}

void windowTop::on_kontakt_personen_box_activate()
{
try{
  kundendaten->newKontaktperson(KontaktPersonenBox->get_value());
  show_kontaktpersonen();
}catch (SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}


void windowTop::saveKundenKontakt()
{
 try{
  if(UpdateSonst!=0)
   {
     std::string s=textPersonenFirmaNotiz->get_buffer()->get_text(textPersonenFirmaNotiz->get_buffer()->begin(),textPersonenFirmaNotiz->get_buffer()->end());
     Kunde::st_ansprech P;
     if(!get_selected_person(P)) return;
     Kunde::st_ansprech A(P.Person,entryPersonenPosition->get_text(),s);
     kundendaten->updateKontaktperson(A);
     // Jetzt noch die gespeicherte Struktur ändern
     Gtk::TreeModel::iterator sel=clistPersonenListe->get_selection()->get_selected();
     Kunde::st_ansprech &B=*static_cast<std::vector<Kunde::st_ansprech>::iterator>((*sel)[kontakt_cols.ansprech]);
     B.position=entryPersonenPosition->get_text();
     B.notiz=s;
     (*sel)[kontakt_cols.position]=B.position;
   }
  }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
 UpdateSonst=Kunde::UpdateBitsSonst(0);
// show_kontaktpersonen();
}



void windowTop::show_kontaktpersonen()
{
  try{
   table_kontaktperson->set_sensitive(true);
   // Personen

   KontaktStore->clear();
//   textPersonenNotiz->delete_text(0,textPersonenNotiz->get_length());
   textPersonenFirmaNotiz->get_buffer()->erase(textPersonenFirmaNotiz->get_buffer()->begin(),textPersonenFirmaNotiz->get_buffer()->end());

   AnsprechPersonen=kundendaten->getPersonen();
   for (std::vector<Kunde::st_ansprech>::iterator i=AnsprechPersonen.begin();i!=AnsprechPersonen.end();++i)
   {
     cH_Kunde p(i->Person);
     Gtk::TreeModel::iterator iter = KontaktStore->append();
     (*iter)[kontakt_cols.name]=p->getName();
     (*iter)[kontakt_cols.name2]=p->getName2();
     (*iter)[kontakt_cols.position]=i->position;
     (*iter)[kontakt_cols.ansprech]=i;
   }
//  for (unsigned int i=0; i<clistPersonenListe->columns().size(); ++i)
//      clistPersonenListe->set_column_auto_resize(i,true) ;
  button_kontakt_loeschen->set_sensitive(false);
 }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}

bool windowTop::get_selected_person(Kunde::st_ansprech& P)
{
  Gtk::TreeModel::iterator s=clistPersonenListe->get_selection()->get_selected();
// if(?) return false;
  std::vector<Kunde::st_ansprech>::iterator i=(*s)[kontakt_cols.ansprech];
  P=*i;
  return true;
}


void windowTop::on_KontaktPersonTelefon_activate(cH_Telefon ct)
{
  Kunde::st_ansprech P;
  if(!get_selected_person(P)) return;
  try{
  KontaktPersonenTel->showTel(kundendaten->getTelefon(P.Person));
  
  }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}


void windowTop::on_clistPersonenListe_selection_changed()
{
  Kunde::st_ansprech P;
  if(!get_selected_person(P))
  { textPersonenFirmaNotiz->get_buffer()->erase(textPersonenFirmaNotiz->get_buffer()->begin(),textPersonenFirmaNotiz->get_buffer()->end());
    button_kontakt_loeschen->set_sensitive(false);
    return;
  }
  KontaktPersonenBox->set_value(P.Person);
  entryPersonenPosition->set_text(P.position);
  KontaktPersonenTel->showTel(kundendaten->getTelefon(P.Person));
  zeige_notiz(P);
  button_kontakt_loeschen->set_sensitive(true);
  KontaktPersonenTel->setKdPer(kundendaten->Id(),P.Person);
}

void windowTop::zeige_notiz(Kunde::st_ansprech &P)
{
  Gtk::TextBuffer::iterator pos=textPersonenFirmaNotiz->get_buffer()->erase(textPersonenFirmaNotiz->get_buffer()->begin(),textPersonenFirmaNotiz->get_buffer()->end());
  textPersonenFirmaNotiz->get_buffer()->insert(pos,P.notiz);
}

void windowTop::on_button_kontakt_loeschen_clicked()
{
 cH_Kunde P=KontaktPersonenBox->get_value();
 Kunde::deleteKontaktperson(kundendaten,P); 
 show_kontaktpersonen();
}
