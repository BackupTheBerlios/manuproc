// generated 2000/1/28 11:31:10 MET by jacek@mimi.
// using glademm V0.5.5
//
// newer (non customized) versions of this file go to wlagermain.cc_glade
// This file is for your program, I won't touch it again!

#include "wlagermain.hh"
#include<gtk--/main.h>
#include"MyMessage.h"
#include<Aux/Datum.h>

#include <Lager/FertigWarenLager.h>

extern MyMessage *message;

wlagermain::wlagermain() : lagerid(FertigWarenLager::default_lagerid)
{
 fillLagerMenu();
 lagerwahl->get_menu()->deactivate.connect(
	SigC::slot(static_cast<class wlagermain*>(this), 
		&wlagermain::on_lagerwahl_activate));
}


void wlagermain::fillLagerMenu()
{
 int lid;
 std::string lbez; 
 
 Query q("select lagerid,bezeichnung from lager order by lagerid");

 FetchIStream fi=q.Fetch();

 int count=0;
 int defaultidx=0;
 Gtk::OStream t_(lagerwahl);

 while(fi.good())
   {
    fi >> lid >> lbez;
    t_ << lbez;
    t_.flush((gpointer)lid);

    if(lid==FertigWarenLager::default_lagerid)
      defaultidx=count;
    count++;
    
    fi=q.Fetch();
   }
 lagerwahl->set_history(defaultidx);
}

void wlagermain::on_abmelden()
{
  mengespin->grab_focus();
  on_artreset_activate();
}


gint wlagermain::on_delete_event(GdkEventAny*)
{
 on_beenden();
 return true;
}

void wlagermain::on_beenden()
{   
 Gtk::Main::instance()->quit();   
}

void wlagermain::on_mengespin_activate()
{   
 if(einlagern1->get_active())
   on_button_einlagern_clicked();
 else
   if(auslagern1->get_active())
     on_button_auslagern_clicked();
 else
   if(inventur1->get_active())   
     on_button_inventur_clicked();
 else
   return;
   
 artikelbox->grab_focus();  
   
}



bool wlagermain::Check()
{
 if(!artmenge.artikel.Id())
  { message->Message::Show("Bitte zuerst Artikel auswählen");
     mengespin->grab_focus();
     artmenge=st_artmenge();     
     artikelbox->reset();
     return false;
   } 
 
 return true;
}

void wlagermain::on_artbox_activate()
{
 ArtikelBase A=artikelbox->get_value();
 if(!A) 
  {// message->Message::Show("Ungültiger Artikel\n");  
    artmenge=st_artmenge();
    return;
  }


 FertigWaren fw(A,FertigWaren::eManuell,0);
 artmenge=st_artmenge(A,FertigWarenLager(fw,lagerid).Bestand());
 lagerbestand->set_text(itos(artmenge.menge));
 fill_CList(A);
 mengespin->grab_focus();
 mengespin->select_region(0,-1);
}

void wlagermain::on_button_einlagern_clicked()
{
  if(!Check()) return;
  gtk_spin_button_update(mengespin->gtkobj());
  int stk = mengespin->get_value_as_int();
  if( stk <= 0)  
    {message->Show("Bitte Menge eingeben");return;}
    

  FertigWaren fw(artmenge.artikel,FertigWaren::eManuell,stk);
  FertigWarenLager fwl(fw,lagerid);

  try {
   fwl.Einlagern(ProductionContext());
   fw=fwl.getFertigWaren();
//   fw.setUser(user_entry->get_text());
      vfw.push_back(fw);
  }
  catch(SQLerror &e)
    { message->Show(e); }
  catch(LagerError &l)
    { message->Show(l); return; }

  artmenge.menge = fwl.Bestand();
  lagerbestand->set_text(itos(artmenge.menge));      
  show_CList();
  mengespin->set_value(0);  
}

void wlagermain::on_button_auslagern_clicked()
{
  if(!Check()) return;
  gtk_spin_button_update(mengespin->gtkobj());
  int stk = mengespin->get_value_as_int();
  if( stk <= 0)  
    {message->Show("Bitte Menge eingeben");return;}
    
  FertigWaren fw(artmenge.artikel,FertigWaren::eManuell,stk);  
  FertigWarenLager fwl(fw,lagerid);

  try {
    fwl.Auslagern(ProductionContext());
    fw=fwl.getFertigWaren();
//    fw.setUser(user_entry->get_text());
      vfw.push_back(fw);
    }
  catch(SQLerror &e)
    { message->Show(e); }
  catch(LagerError &l)
    { message->Show(l); return; }

  artmenge.menge = fwl.Bestand();
  lagerbestand->set_text(itos(artmenge.menge));      
  show_CList();
  mengespin->set_value(0);
}

void wlagermain::on_button_inventur_clicked()
{
  if(!Check()) return;
  gtk_spin_button_update(mengespin->gtkobj());
  int stk = mengespin->get_value_as_int();
  if( stk < 0)  
    {message->Show("Bitte Menge eingeben");return;}

  FertigWaren fw(artmenge.artikel,FertigWaren::eInventur,stk);
  FertigWarenLager fwl(fw,lagerid);
  
  try { fwl.Inventur();
	fw=fwl.getFertigWaren();
//      fw.setUser(user_entry->get_text());
      vfw.push_back(fw);
   }
  catch(SQLerror &e)
    { message->Show(e); }      
    
  artmenge.menge = fwl.Bestand();  
  lagerbestand->set_text(itos(artmenge.menge));  
  show_CList();
  mengespin->set_value(0);
}



void wlagermain::on_artreset_activate()
{
    mengespin->set_value(0);
    mengespin->grab_focus();
    artmenge=st_artmenge();
    artikelbox->reset();
    lagerbestand->set_text("");
    buchung_clist->clear();
}

void wlagermain::on_mengespin_changed()
{
}

void wlagermain::append_to_CList(const FertigWaren &fw,Gtk::OStream &os)
{
 std::string aktion;
 switch(fw.Aktion())
 	{case FertigWaren::eWareneingang : aktion = "Wareneingang"; break;
	 case FertigWaren::eLieferschein : aktion = "Lieferung"; break;
 	 case FertigWaren::eManuell : aktion = "Manuell"; break;
 	 case FertigWaren::eInventur : aktion = "Inventur"; break;
 	 default : aktion = "?"; 
 	}
 	
 char zeit[10];
 snprintf(zeit,sizeof zeit,"%02d:%02d:%02d",fw.Zeit().Stunde(),
 		fw.Zeit().Minute(),fw.Zeit().Sekunde());	
 	
 char lfrsid[10];
 if(fw.Lfrsid()==ManuProcEntity<>::none_id) *lfrsid=0;
 else snprintf(lfrsid,sizeof lfrsid,"%06ld", fw.Lfrsid());
 	
 os 	<< fw.User() <<  "\t" 
 	<< aktion << "\t"
 	<< fw.Zeit().Datum()<<" - "<<zeit << "\t";
 
 if(fw.Aktion()==FertigWaren::eInventur)
   {
    os << "(" << fw.Stk() << ")" << "\t"
    << fw.Bestand() << "\n";
   }
 else
   {
    os <<  fw.Stk() << "\t\t";
    if(fw.Aktion()==FertigWaren::eLieferschein)
      os << "L " << lfrsid << "\n";
    else
      if(fw.Aktion()==FertigWaren::eWareneingang)
      os << "W " << lfrsid << "\n";
    else
      os << lfrsid << "\n";
   }



}


void wlagermain::on_button_reload_clicked()
{
 on_artbox_activate();
}


void wlagermain::show_CList()
{
 buchung_clist->freeze();
 buchung_clist->clear();
 
 Gtk::OStream os(buchung_clist); 	
 
 for(std::vector<FertigWaren>::reverse_iterator i=vfw.rbegin(); i!=vfw.rend(); ++i)
   {
   append_to_CList(*i,os);
   }

 buchung_clist->set_column_auto_resize(2,true);
 buchung_clist->thaw();
}


void wlagermain::fill_CList(const ArtikelBase &a)
{
 vfw.erase(vfw.begin(),vfw.end());
 
 FertigWarenLager fwl(lagerid);

 std::string tabelle=fwl.Tabelle();

 std::string qu=std::string("select menge,coalesce(bestand,0),datum,aktion,"
	" coalesce(f.pid,0),")+
	"coalesce(lfrsid,"+itos(ManuProcEntity<>::none_id)+
	"),artikelid from "+
 	tabelle+" f "
	" where artikelid="+itos(a.Id())+" order by datum";

// 	"fw_lager_buchung f left join personal p on (p.pid=f.pid) "
//	" where artikelid="+itos(a.Id())+" order by datum";
 	
 Query(qu).FetchArray(vfw);	
 show_CList();
}


void wlagermain::on_einlagern_activate()
{  
}

void wlagermain::on_auslagern_activate()
{  
}

void wlagermain::on_inventur_activate()
{  
}

void wlagermain::on_keineaktion_activate()
{  
}

void wlagermain::on_lagerwahl_activate()
{
 lagerid=int(lagerwahl->get_menu()->get_active()->get_user_data());

std::cout << lagerid << "lagerid:\n";

 
 on_button_reload_clicked();

}



