

#include "mpc_agent.hh"
#include <Transaction.h>


void mpc_agent::on_orderid_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    static Transaction tr("",false);

    std::string squery;

    switch(newsearch)
      {  
         case GTK_SEARCH_OPEN:
           {tr.open();
            squery="select aufid from auftrag where  
	    Query(squery) <<


