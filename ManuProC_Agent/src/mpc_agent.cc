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

}


void mpc_agent::on_beenden_activate()
{
  Gtk::Main::instance()->quit();
}

void mpc_agent::on_kunde_activate()
{
 std::cout << "Kunde\n";
}


void mpc_agent::on_activate_entry(int enr)
{
 std::cout << "entry" << enr << "\n";
 std::string name,ort;


 try{
 Query q("select name,ort from kunden where kundennr=?");
 if(q.Result()==100)
   {
    MyMessage msg("Customer not found");
    msg.set_transient_for(*this);
    msg.run();
    return;
   }
 q << atoi(kunde->get_value(enr).c_str());
 q >> name >> ort;
 }
 catch(SQLerror &e)
 {
  MyMessage msg(e);
  msg.set_transient_for(*this);
  msg.run();
 }
   
 kunde->set_value(enr+1,name);
 kunde->set_value(enr+2,ort);

}

