#include "windowTop.hh"
#include <Gtk_OStream.h>
#include "MyMessage.h"

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
     std::string s=textPersonenFirmaNotiz->get_chars(0,textPersonenFirmaNotiz->get_length());
     Kunde::st_ansprech P;
     if(!get_selected_person(P)) return;
     Kunde::st_ansprech A(P.Person,entryPersonenPosition->get_text(),s);
     kundendaten->updateKontaktperson(A);
     // Jetzt noch die gespeicherte Struktur ändern
     Kunde::st_ansprech &B=*(static_cast<Kunde::st_ansprech*>(clistPersonenListe->selection().begin()->get_data()));
     B.position=entryPersonenPosition->get_text();
     B.notiz=s;
     Gtk::CList::SelectionList::iterator b = clistPersonenListe->selection().begin();
     Gtk::CList::SelectionList::iterator e = clistPersonenListe->selection().end();
     if(b==e) assert(!"Interner Fehler: bitte beim Support melden");
#warning nach dem Editieren wird in der clist die Position noch nicht richtig gesetzt
//     b[2];
//     Gtk::CList::Row *row;
//     b->Row(*row);
//     int row=b->get_row_num();
          
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

   clistPersonenListe->clear();
//   textPersonenNotiz->delete_text(0,textPersonenNotiz->get_length());
   textPersonenFirmaNotiz->delete_text(0,textPersonenFirmaNotiz->get_length());
   Gtk::OStream os(clistPersonenListe);

   AnsprechPersonen=kundendaten->getPersonen();
   for (std::vector<Kunde::st_ansprech>::iterator i=AnsprechPersonen.begin();i!=AnsprechPersonen.end();++i)
      {
       cH_Kunde p(i->Person);
         os <<p->getName()<<"\t"<<p->getName2()<<"\t"
            <<i->position<<"\n";
         os.flush(gpointer(&*i));
      }
  for (unsigned int i=0; i<clistPersonenListe->columns().size(); ++i)
      clistPersonenListe->set_column_auto_resize(i,true) ;
  button_kontakt_loeschen->set_sensitive(false);
 }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}

bool windowTop::get_selected_person(Kunde::st_ansprech& P)
{
  Gtk::CList::SelectionList::iterator b = clistPersonenListe->selection().begin();
  Gtk::CList::SelectionList::iterator e = clistPersonenListe->selection().end();
  if (b==e)  {//Message *m=manage(new Message());
//     m->Show("Keine Person ausgewählt");
     return false;}
  P=*(static_cast<Kunde::st_ansprech*>(clistPersonenListe->selection().begin()->get_data()));
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
  { textPersonenFirmaNotiz->delete_text(0,textPersonenFirmaNotiz->get_length());
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
  std::string N=P.notiz;
//  textPersonenNotiz->delete_text(0,textPersonenNotiz->get_length());
  textPersonenFirmaNotiz->delete_text(0,textPersonenFirmaNotiz->get_length());
  gint pos2=0;
//  textPersonenNotiz->insert_text(P.Person->Notiz().c_str(), P.Person->Notiz().size(),&pos);
  textPersonenFirmaNotiz->insert_text(N.c_str(), N.size(),&pos2);
}

void windowTop::on_button_kontakt_loeschen_clicked()
{
 cH_Kunde P=KontaktPersonenBox->get_value();
 Kunde::deleteKontaktperson(kundendaten,P); 
 show_kontaktpersonen();
}
