// generated 2004/3/12 22:15:24 CET by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to mpc_agent.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gtkmm/main.h>
#include <Misc/dbconnect.h>
#include <Misc/FetchIStream.h>
#include "mpc_agent.hh"
#include <MyMessage.h>
#include <Misc/itos.h>
#include "kunden_selector.hh"
#include <SearchComboContent.h>
#include "yes_no_dialog.hh"
#include "mpc_export.hh"

mpc_agent *mpca;

int main(int argc, char **argv)
{  
#if defined(ENABLE_NLS)
   bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
   bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
   textdomain (GETTEXT_PACKAGE);
#endif //ENABLE_NLS
   
   ManuProC::Connection conn;
   Gtk::Main m(&argc, &argv);


   try {

      conn.setDbase("mpc.data");
      ManuProC::dbconnect(conn);  

      mpca=manage(new mpc_agent());

      m.run();

      Petig::dbdisconnect();
   } catch (SQLerror &e)
   {  std::cerr << e << '\n';
      return 1;
   }

   return 0;
}


mpc_agent::mpc_agent()
{
 kunde->set_label(KDBOX_NR,"Cust.No.");
 kunde->set_label(KDBOX_NAME,"Name");
 kunde->set_label(KDBOX_ORT,"Location");

 std::vector<std::string> v(order->Cols());

 v[OD_ROW]="row no.";
 v[OD_ARTICLE]="article";
 v[OD_NAME]="name";
 v[OD_EAN]="EAN";
 v[OD_AMOUNT]="amount";

 order->setTitles(v);
 orderid->set_always_fill(true);
 orderid->set_start_on_idle(true);
 orderid->set_autoexpand(true);
 label_orderid_prefix->set_text(itos(VERKNR)+label_orderid_prefix->get_text());

 article_entry->set_always_fill(true);
 article_entry->set_autoexpand(true);
 width_entry->set_always_fill(true);
 width_entry->set_start_on_idle(true);
 width_entry->set_autoexpand(true);
 color_entry->set_always_fill(true);
 color_entry->set_start_on_idle(true);
 color_entry->set_autoexpand(true);
 makeup_entry->set_always_fill(true);
 makeup_entry->set_start_on_idle(true);
 makeup_entry->set_autoexpand(true);
 ean_entry->set_always_fill(true);
 ean_entry->set_autoexpand(true);

 set_title(get_title()+" - Ag.No.("+itos(VERKNR)+")");
}


void mpc_agent::on_beenden_activate()
{
  Gtk::Main::instance()->quit();
}

void mpc_agent::on_kunde_activate()
{

}


void mpc_agent::on_activate_entry(int enr)
{
 std::string name,ort;
 std::string search_column,query;
 int kundennr;
 
 switch(enr)
   {
    case KDBOX_NR : search_column="kundennr"; break;
    case KDBOX_NAME : search_column="name"; break;
    case KDBOX_ORT : search_column="ort"; break;
   }

 try{
 std::string query("select kundennr,name,ort from kunden where ");
 query+=search_column+" like '"+kunde->get_value(enr)+"%'";
       
 Query q(query);

 if(q.Result()==100)
   {
    MyMessage msg("Customer not found",Gtk::MESSAGE_ERROR);
    msg.set_transient_for(*this);
    msg.run();
    return;
   }


 if(q.LinesAffected()>1)
   {std::string kn;
    kunden_selector ks(q,&kn,&name,&ort);
    ks.set_transient_for(*this);
    int ret;
    ret=ks.run();
    kundennr=atoi(kn.c_str());
    if(ret==Gtk::RESPONSE_CANCEL)
      return;
   }
 else
    q >> kundennr >> name >> ort;

 }
 catch(SQLerror &e)
 {
  MyMessage msg(e);
  msg.set_transient_for(*this);
  msg.run();
  return;
 }

 kunde->set_value(KDBOX_NR,itos(kundennr));
 kunde->set_value(KDBOX_NAME,name);
 kunde->set_value(KDBOX_ORT,ort);
   
 kunde->set_sensitive(false);
 neu->set_sensitive(true);
 orderid->grab_focus();
}


void mpc_agent::on_clear_activate()
{  
 kunde->reset();
 kunde->set_sensitive(true);
 kunde->grab_focus();
 neu->set_sensitive(false);
 clear_order();
}


void mpc_agent::on_artikel_aktivate()
{  
}

void mpc_agent::on_artikel_activate_entry(int enr)
{  
}

void mpc_agent::on_menge_editing_done()
{  
 artikel_ok->grab_focus();
}

void mpc_agent::on_artikel_ok_clicked()
{  
 if(article_entry->sensitive() ||
    width_entry->sensitive() ||
    color_entry->sensitive() ||
    makeup_entry->sensitive())
  {
   MyMessage msg("select an complete article before you press OK");
   msg.set_transient_for(*this);
   msg.run();
   return;
  }	
  
 if(!(menge->get_value_as_int()>0))
  {
   MyMessage msg("amount must be greater then 0");
   msg.set_transient_for(*this);
   msg.run();
   return;
  }


 if(orderid->sensitive())
  {
   MyMessage msg("select or create one order first");
   msg.set_transient_for(*this);
   msg.run();
   return;
  }   


 Query q("insert into auftragentry (aufid,vknr,artnr,breite,farbe,aufmachung,"
 	"ean,stueck,preis) values (?,?,?,?,?,?,?,?,null)");

  std::cout << orderid->Content() <<"\n";

 try{
     q << orderid->Content() << VERKNR
 	<< article_entry->Content()
 	<< width_entry->Content()
 	<< color_entry->Content()
 	<< makeup_entry->Content()
 	<< Query::NullIf(ean_entry->Content())
 	<< menge->get_value_as_int();
 	
 }
 catch(SQLerror &e)
  {
   MyMessage msg(e);
   msg.set_transient_for(*this);
   msg.run();
   return;
  }    	
 catch(SearchComboContent<std::string>::ContentError c)
  {
   MyMessage msg(c.what());
   msg.set_transient_for(*this);
   msg.run();
   return;
  }    	

 load_order(orderid->Content());   
 on_artikel_cancel_clicked();
 
}

void mpc_agent::on_artikel_cancel_clicked()
{  
 menge->set_value(0);
 article_entry->reset();
 width_entry->reset(); 
 color_entry->reset();
 makeup_entry->reset(); 
 ean_entry->reset();
 artikel_notebook->set_current_page(0);
 article_entry->set_sensitive(true);
 width_entry->set_sensitive(true); 
 color_entry->set_sensitive(true);
 makeup_entry->set_sensitive(true); 
 article_entry->grab_focus();
}



void mpc_agent::on_neu_clicked()
{
 MyMessage msg;

 if(kunde->sensitive()==true)
    {msg.set_transient_for(*this);
     msg.set_Message("select one customer first");
     msg.run();
     return;
    }
 clear_order();
 try {
 int oi=0;
 oi=create_new_order(atoi(kunde->get_value(KDBOX_NR).c_str()));
 load_order(oi);   
 }
 catch(SQLerror &e)
 {
  MyMessage msg(e);
  msg.set_transient_for(*this);
  msg.run();
  return;
 } 

}



void mpc_agent::on_orderid_activate()
{  
 load_order(orderid->get_value());
}


void mpc_agent::on_customer_search_clicked()
{
 std::string query("select kundennr,name,ort from kunden");

 try {       
 Query q(query);

 std::string kundennr,name,ort;
 kunden_selector ks(q,&kundennr,&name,&ort);
 ks.set_transient_for(*this);
 int ret;
 ret=ks.run();
 if(ret==Gtk::RESPONSE_CANCEL)
      return;

 kunde->set_value(KDBOX_NR,kundennr);
 kunde->set_value(KDBOX_NAME,name);
 kunde->set_value(KDBOX_ORT,ort);
 }
 catch(SQLerror &e)
 {
  MyMessage msg(e);
  msg.set_transient_for(*this);
  msg.run();
  return;
 }
  
 kunde->set_sensitive(false);
 neu->set_sensitive(true); 
 clear_order();
}




void mpc_agent::on_article_entry_activate()
{
 width_entry->grab_focus();
 article_entry->set_sensitive(false);
}

void mpc_agent::on_ean_entry_activate()
{  
 menge->grab_focus();
 ean_entry->set_sensitive(false);
}

void mpc_agent::on_width_entry_activate()
{  
 color_entry->grab_focus();
 width_entry->set_sensitive(false);
}


void mpc_agent::on_color_entry_activate()
{  
 makeup_entry->grab_focus();
 color_entry->set_sensitive(false);
}

void mpc_agent::on_makeup_entry_activate()
{  
 std::string ean;
 Query("select ean from artikel where artnr=? and breite=?"
	" and farbe=? and aufmachung=?") 
	<< article_entry->Content()
	<< width_entry->Content()
	<< color_entry->Content()
	<< makeup_entry->Content()
	>> FetchIStream::MapNull(ean,"");

 ean_entry->set_value(ean,ean);

 menge->grab_focus();
 makeup_entry->set_sensitive(false);
}


void mpc_agent::on_artikel_del_clicked()
{
 int ret;
 yes_no_dialog ynd;
 ynd.set_transient_for(*this);
 ret=ynd.run();

 if(orderid->sensitive()==true) return; // no order selected
 
 if(ret==Gtk::RESPONSE_NO) return;
 if(ret==Gtk::RESPONSE_YES)
   { 
    std::vector<cH_RowDataBase> selrows;
    selrows=order->getSelectedRowDataBase_vec();
    std::vector<cH_RowDataBase>::const_iterator ci=selrows.begin();
    while(ci!=selrows.end())
      {
       std::cout << (*ci)->Value(1,0)->getStrVal();
       try {
       Query("delete from auftragentry where aufid=? and vknr=? and"
	" artnr||'/'||breite||'/'||farbe||'/'||aufmachung = ?")
	<< orderid->Content() << VERKNR << (*ci)->Value(1,0)->getStrVal();	
       }
       catch(SQLerror &e)
       {
        MyMessage msg(e);
        msg.set_transient_for(*this);
        msg.run();
        return;
       } 

      ++ci;
      }
    load_order(orderid->Content());   
    artikel_del->set_sensitive(false);
   }
}


void mpc_agent::on_order_leaf_selected(cH_RowDataBase leaf)
{  
 artikel_del->set_sensitive(true);
}

void mpc_agent::on_order_leaf_unselected()
{  
 artikel_del->set_sensitive(false);
}

void mpc_agent::on_senden_clicked()
{
 if(orderid->sensitive()) return; //nix selectiert
 mpc_export mpx;
 mpx.set_transient_for(*this);
 int ret;
 ret=mpx.run();
}

