#include "mpc_agent.hh"
#include <FetchIStream.h>
#include <itos.h>
#include <Datum.h>
#include <Transaction.h>
#include <MyMessage.h>

void mpc_agent::on_orderid_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    std::string squery("select aufid from auftrag where vknr=?");

    if(!kunde->get_value(KDBOX_NR).empty())
       squery+=std::string(" and kdnr=?");

    Query qu(squery);

    switch(context)
	{  
         case GTK_SEARCH_OPEN:
	 case GTK_SEARCH_REOPEN:
           {
	    qu << VERKNR;
            if(!kunde->get_value(KDBOX_NR).empty())
	      qu << kunde->get_value(KDBOX_NR);
	    // fall through
	   }

	 case GTK_SEARCH_FETCH:
	   // dont need idle loops here. It is rapid enouth from SQLite.
	   {
	    int aufid;
	    FetchIStream fi=qu.Fetch();
 	    while(fi.good())
	      {fi >> aufid;
	       orderid->add_item(itos(VERKNR)+'_'+itos(aufid),aufid);
	       fi=qu.Fetch();
	      }
	    *cont=false;
	    break;
	   }

	 case GTK_SEARCH_CLOSE:
           {
            break;
           }

	}
   }
  catch (SQLerror &e)
   {  std::cerr << e << '\n';
   }

}



void mpc_agent::clear_order()
{
 orderid->reset();
 orderid->set_sensitive(true);
 label_orderdate->set_text("");
 artikel->reset();
 menge->set_value(0);
 order->clear();
}



void mpc_agent::load_order(int oid)
{
 std::string kdnr;
 ManuProC::Datum datum;

 Query("select kdnr,datum from auftrag where aufid=? and vknr=?")
	<< oid << VERKNR >> kdnr >> datum;
  
 label_orderdate->set_text(std::string("dated ")+datum.c_str());

 if(kunde->get_value(KDBOX_NR)!=kdnr)
   {kunde->set_value(KDBOX_NR,kdnr);
    on_activate_entry(KDBOX_NR);  
   }
 orderid->set_value(itos(VERKNR)+"_"+itos(oid),oid);
 orderid->set_sensitive(false);
}


int mpc_agent::create_new_order(int kdnr)
{
 ManuProC::Datum now(ManuProC::Datum::today());

 int new_aufid;

 try {
 Query("select coalesce(max(aufid),0)+1 from auftrag where vknr=?") 
	<< VERKNR >> new_aufid;

 Query q("insert into auftrag (aufid,kdnr,vknr,datum) values (?,?,?,?)");
 q << new_aufid << kdnr << VERKNR << now.to_iso();
 }
 catch(SQLerror &e)
 {
  MyMessage msg(e);
  msg.set_transient_for(*this);
  msg.run();
  return 0;
 }
 catch(ManuProC::Datumsfehler &d)
 {
  MyMessage msg(d);
  msg.set_transient_for(*this);
  msg.run();
  return 0;
 } 

 return new_aufid;

}



