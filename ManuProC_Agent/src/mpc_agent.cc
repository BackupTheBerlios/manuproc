// generated 2004/3/12 22:15:24 CET by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to mpc_agent.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gtkmm/main.h>
#include <dbconnect.h>
#include <FetchIStream.h>
#include "mpc_agent.hh"
#include <MyMessage.h>
#include <itos.h>
#include "kunden_selector.hh"

int main(int argc, char **argv)
{  
#if defined(ENABLE_NLS)
   bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
   bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
   textdomain (GETTEXT_PACKAGE);
#endif //ENABLE_NLS
   
   ManuProC::Connection conn;
   Gtk::Main m(&argc, &argv);

   mpc_agent *mpc;

   try {

      conn.setDbase("mpc.data");
      ManuProC::dbconnect(conn);  

      mpc=new mpc_agent();

      m.run();

      Petig::dbdisconnect();
   } catch (SQLerror &e)
   {  std::cerr << e << '\n';
      free(mpc);
      return 1;
   }

   free(mpc);
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
 v[OD_AMOUNT]="amount";

 order->setTitles(v);
 orderid->set_always_fill(true);
 orderid->set_start_on_idle(true);
 orderid->set_autoexpand(true);
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
}

void mpc_agent::on_artikel_ok_clicked()
{  
}

void mpc_agent::on_artikel_cancel_clicked()
{  
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


void mpc_agent::on_orderid_entry_editing_done()
{  
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



