#include "MyMessage.h"
#include <Aux/itos.h>
#include "windowTop.hh"
#include <Gtk_OStream.h>


void windowTop::show_privatpersonen()
{
  Kunde::st_ansprech P;
  if(get_selected_person(P)) show_neue_personen_daten(P.Person)  ;
}

void windowTop::on_personen_box_activate()
{
   person=personenbox->get_value();
   show_neue_personen_daten(person);
}


void windowTop::on_buttonPersonNeu_clicked()
{   
  try{
   int nv=Person::createPerson();
   person=cH_Person(nv);
   personenbox->set_value(nv); 
   entryPersonenDatenName->grab_focus();
  }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
  clear_PersonenEntrys();
}

void windowTop::clear_PersonenEntrys()
{
  entryPersonenDatenName->set_text("");
  entryPersonenDatenVorname->set_text("");
  scc_anrede->clear();
  PersonenTel->clear();
  textPersonenPrivatNotiz->delete_text(0,textPersonenPrivatNotiz->get_length());
}



void windowTop::on_buttonPersonLoeschen_clicked()
{
  cH_Person P(personenbox->get_value()); 
  if(P->Id()!=Person::none_id)
   {   
     try{
        Person::delPerson(P->Id());
        Telefon::delPersonsTelefon(P->Id());
        if(kundendaten->Id()!=Kunde::none_id) show_kontaktpersonen();
    }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
   }
}

void windowTop::on_PersonenPrivatTelefon_activate()
{
  cH_Person P(personenbox->get_value());
  try{
  Telefon::newTelefon(Kunde::none_id,P->Id(),PersonenTel->get_value());
  }catch(SQLerror &e) { MyMessage *m=manage(new MyMessage()); m->Show(e);}
  PersonenTel->showTel(P->getTelefon());
  PersonenTel->clear();
}


void windowTop::show_neue_personen_daten(cH_Person &P)
{
   person=P;
   personenbox->set_value(P->Id());
   entryPersonenDatenName->set_text(P->Name());
   entryPersonenDatenVorname->set_text(P->Vorname());
   if(P->GebDatum().valid())
     geburtstag->set_value(P->GebDatum());
   else
     geburtstag->set_value(Petig::Datum().today());     

   scc_anrede->setContent(P->Anrede()->Name(),P->Anrede()->Id());
   std::string N=person->Notiz(); // zwischenspeichern, weil die nächste Zeile
                                 // ein 'changed' Signal sendet und damit die 
                                 // Notiz gelöscht wird.
   textPersonenPrivatNotiz->delete_text(0,textPersonenPrivatNotiz->get_length());
   gint pos=0;
   textPersonenPrivatNotiz->insert_text(N.c_str(),N.size(),&pos);
   PersonenTel->showTel(P->getTelefon());
}

void windowTop::on_entryPersonenDatenName_activate()
{
 entryPersonenDatenVorname->grab_focus();
}
void windowTop::on_entryPersonenDatenVorname_activate()
{
 geburtstag->grab_focus();
}
void windowTop::geburtstag_activate()
{
 scc_anrede->grab_focus();
// comboentryPersonenDatenAnrede->grab_focus();
}
void windowTop::on_comboentryPersonenDatenAnrede_activate()
{
// entryPersonenPosition->grab_focus();
}

