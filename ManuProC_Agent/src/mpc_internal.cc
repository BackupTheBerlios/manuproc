

#include "mpc_agent.hh"
#include <FetchIStream.h>
#include <itos.h>

void mpc_agent::on_orderid_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    std::string squery;

    Query qu("select aufid from auftrag where kdnr=? and vknr=? ");

    switch(context)
	{  
         case GTK_SEARCH_OPEN:
	 case GTK_SEARCH_REOPEN:
           {
	    qu << kunde->get_value(KDBOX_NR) << VERKNR;
	    // fall through
	   }

	 case GTK_SEARCH_FETCH:
	   // dont need idle loops here. Ist rapid enouth from SQLite.
	   {
	    int aufid;
 	    while(qu.good())
	      {qu >> aufid;
	       orderid->add_item(itos(VERKNR)+'_'+itos(aufid),aufid);
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



