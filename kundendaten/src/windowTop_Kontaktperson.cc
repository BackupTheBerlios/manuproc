#include "windowTop.hh"
#include <Gtk_OStream.h>
#include "MyMessage.h"

void windowTop::on_kontakt_personen_box_activate()
{
  kundendaten->newKontaktperson(KontaktPersonenBox->get_value());
  show_kontaktpersonen();
}


void windowTop::saveKundenKontakt()
{
 try{
  if(UpdateSonst!=0)
   {
     std::string s=textPersonenFirmaNotiz->get_chars(0,textPersonenFirmaNotiz->get_length());
     Kunde::st_ansprech P;
     if(!get_selected_person(P)) return;
     Kunde::st_ansprech A(P.Person->Id(),entryPersonenPosition->get_text(),s);
     kundendaten->updateKontaktperson(A);
     // Jetzt noch die gespeicherte Struktur ändern
     Kunde::st_ansprech &B=*(static_cast<Kunde::st_ansprech*>(clistPersonenListe->selection().begin()->get_data()));
     B.position=entryPersonenPosition->get_text();
     B.notiz=s;
     Gtk::CList::SelectionList::iterator b = clistPersonenListe->selection().begin();
     Gtk::CList::SelectionList::iterator e = clistPersonenListe->selection().end();
     if(b==e) assert(!"Kann nicht sein");
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
   labelPersonenKdNrEintrag->set_text(itos(kundendaten->Id()));
   labelPersonenFirmaEintrag->set_text(kundendaten->firma());

   clistPersonenListe->clear();
   textPersonenNotiz->delete_text(0,textPersonenNotiz->get_length());
   textPersonenFirmaNotiz->delete_text(0,textPersonenFirmaNotiz->get_length());
   Gtk::OStream os(clistPersonenListe);

   AnsprechPersonen=kundendaten->getPersonen();
   for (std::vector<Kunde::st_ansprech>::iterator i=AnsprechPersonen.begin();i!=AnsprechPersonen.end();++i)
      {
         os <<i->Person->Name()<<"\t"<<i->Person->Vorname()<<"\t"
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


void windowTop::on_KontaktPersonTelefon_activate()
{
  Kunde::st_ansprech P;
  if(!get_selected_person(P)) return;
  try{
  Telefon::newTelefon(kundendaten->Id(),P.Person->Id(),KontaktPersonenTel->get_value());
  KontaktPersonenTel->showTel(P.Person->getTelefon());
  }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
}


void windowTop::on_clistPersonenListe_select_row(gint row, gint column, GdkEvent *event)
{
  Kunde::st_ansprech P;
  if(!get_selected_person(P)) return;
  KontaktPersonenBox->set_value(P.Person->Id());
  entryPersonenPosition->set_text(P.position);
  KontaktPersonenTel->showTel(P.Person->getTelefon());
  zeige_notiz(P);
  button_kontakt_loeschen->set_sensitive(true);
}

void windowTop::on_clistPersonenListe_unselect_row(gint row, gint column, GdkEvent *event)
{
   textPersonenNotiz->delete_text(0,textPersonenNotiz->get_length());
   textPersonenFirmaNotiz->delete_text(0,textPersonenFirmaNotiz->get_length());
   button_kontakt_loeschen->set_sensitive(false);
}

void windowTop::zeige_notiz(Kunde::st_ansprech &P)
{
  std::string N=P.notiz;
  textPersonenNotiz->delete_text(0,textPersonenNotiz->get_length());
  textPersonenFirmaNotiz->delete_text(0,textPersonenFirmaNotiz->get_length());
  gint pos=0,pos2=0;
  textPersonenNotiz->insert_text(P.Person->Notiz().c_str(), P.Person->Notiz().size(),&pos);
  textPersonenFirmaNotiz->insert_text(N.c_str(), N.size(),&pos2);
}

void windowTop::on_button_kontakt_loeschen_clicked()
{
 cH_Person P=KontaktPersonenBox->get_value();
 Kunde::deleteKontaktperson(kundendaten,P); 
 show_kontaktpersonen();
}
