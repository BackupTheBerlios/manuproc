#include "mpc_agent.hh"
#include <Misc/FetchIStream.h>
#include <Misc/itos.h>
#include <Misc/Datum.h>
#include <Misc/Transaction.h>
#include <MyMessage.h>
#include <treebase_data.h>
#include <Misc/TagStream.h>

void mpc_agent::on_orderid_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    std::string squery("select aufid from auftrag where vknr=?");

    if(!orderid->get_text().empty())
      squery=squery+" and aufid like '"+orderid->get_text()+"%'";

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
	       orderid->add_item(itos(aufid),aufid);
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


void mpc_agent::artikel_clean()
{
 article_entry->reset();
 width_entry->reset();
 color_entry->reset();
 makeup_entry->reset();
 ean_entry->reset();
 
 article_entry->set_sensitive(true);
 width_entry->set_sensitive(true);
 color_entry->set_sensitive(true);
 makeup_entry->set_sensitive(true); 
 ean_entry->set_sensitive(true);
}

void mpc_agent::clear_order()
{
 orderid->reset();
 orderid->set_sensitive(true);
 label_orderdate->set_text("");
 artikel_clean();
 menge->set_value(0);
 order->clear();
}



void mpc_agent::load_order(int oid)
{
 std::string kdnr;
 ManuProC::Datum datum;

 try{
 Query("select kdnr,datum from auftrag where aufid=? and vknr=?")
	<< oid << VERKNR >> kdnr >> datum;
 }
 catch(SQLerror &e)
 {
  MyMessage msg(e);
  msg.set_transient_for(*this);
  msg.run();
  return;
 } 


 Query qe("select e.artnr||'/'||e.breite||'/'||e.farbe||'/'||e.aufmachung,"
 	"b.bezeichnung, b.ean, e.stueck from auftragentry e left join artikel b"
 	" on (e.artnr=b.artnr and e.breite=b.breite and e.farbe=b.farbe)"
 	" where aufid=? and vknr=?");
 try{
   qe << oid << VERKNR;
 }
 catch(SQLerror &e)
 {
  MyMessage msg(e);
  msg.set_transient_for(*this);
  msg.run();
  return;
 }  

 FetchIStream fs=qe.Fetch();
 std::vector <cH_RowDataBase> dv;
 int count=0;
 while(fs.good())
  {
   std::string artikel,bez,ean;
   int stk;
   fs >> artikel >> bez >> FetchIStream::MapNull(ean,"") >> stk;
   dv.push_back(cH_RowDataStrings(itos(++count),artikel,bez,ean,itos(stk)));
   fs=qe.Fetch();
  }
 order->setDataVec(dv);  

 label_orderdate->set_text(std::string("dated ")+datum.c_str());

 if(kunde->sensitive())
   {kunde->set_value(KDBOX_NR,kdnr);
    on_activate_entry(KDBOX_NR);  
   }
 orderid->set_value(itos(oid),oid);
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


void mpc_agent::on_ean_entry_search(gboolean *cont,GtkSCContext context)
{  
}

void mpc_agent::on_article_entry_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    std::string squery("select distinct artnr from artikel");

    squery=squery+" where 1=? and artnr like '"+article_entry->get_text()+"%'";

    Query qu(squery);

    switch(context)
	{  
         case GTK_SEARCH_OPEN:
	 case GTK_SEARCH_REOPEN:
           {
	    qu << 1; // only to trigger a Query at this place;
	    // fall through
	   }

	 case GTK_SEARCH_FETCH:
	   // dont need idle loops here. It is rapid enouth from SQLite.
	   {
	    std::string artikel;
	    FetchIStream fi=qu.Fetch();
 	    while(fi.good())
	      {fi >> artikel;
	       article_entry->add_item(artikel,artikel);
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



void mpc_agent::on_width_entry_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    std::string squery("select distinct breite from artikel");

    squery=squery+" where 1=? and artnr = '"+article_entry->get_text()+"'";
    squery=squery+" and breite like '"+width_entry->get_text()+"%'";

    Query qu(squery);

    switch(context)
	{  
         case GTK_SEARCH_OPEN:
	 case GTK_SEARCH_REOPEN:
           {
	    qu << 1; // only to trigger a Query at this place;
	    // fall through
	   }

	 case GTK_SEARCH_FETCH:
	   // dont need idle loops here. It is rapid enouth from SQLite.
	   {
	    std::string artikel;
	    FetchIStream fi=qu.Fetch();
 	    while(fi.good())
	      {fi >> artikel;
	       width_entry->add_item(artikel,artikel);
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


void mpc_agent::on_color_entry_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    std::string squery("select distinct farbe from artikel");

    squery=squery+" where 1=? and artnr = '"+article_entry->get_text()+"'";
    squery=squery+" and breite = '"+width_entry->get_text()+"'";
    squery=squery+" and farbe like '"+color_entry->get_text()+"%'";

    Query qu(squery);

    switch(context)
	{  
         case GTK_SEARCH_OPEN:
	 case GTK_SEARCH_REOPEN:
           {
	    qu << 1; // only to trigger a Query at this place;
	    // fall through
	   }

	 case GTK_SEARCH_FETCH:
	   // dont need idle loops here. It is rapid enouth from SQLite.
	   {
	    std::string artikel;
	    FetchIStream fi=qu.Fetch();
 	    while(fi.good())
	      {fi >> artikel;
	       color_entry->add_item(artikel,artikel);
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


void mpc_agent::on_makeup_entry_search(gboolean *cont,GtkSCContext context)
{
 try
   {  
    std::string squery("select distinct aufmachung from artikel");

    squery=squery+" where 1=? and artnr = '"+article_entry->get_text()+"'";
    squery=squery+" and breite = '"+width_entry->get_text()+"'";
    squery=squery+" and farbe = '"+color_entry->get_text()+"'";
    squery=squery+" and aufmachung like '"+makeup_entry->get_text()+"%'";

    Query qu(squery);

    switch(context)
	{  
         case GTK_SEARCH_OPEN:
	 case GTK_SEARCH_REOPEN:
           {
	    qu << 1; // only to trigger a Query at this place;
	    // fall through
	   }

	 case GTK_SEARCH_FETCH:
	   // dont need idle loops here. It is rapid enouth from SQLite.
	   {
	    std::string artikel;
	    FetchIStream fi=qu.Fetch();
 	    while(fi.good())
	      {fi >> artikel;
	       makeup_entry->add_item(artikel,artikel);
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

#include <fstream>

void mpc_agent::export_xml(const std::string file)
{
 std::string ord_id=itos(orderid->get_value());

 TagStream ts;
 ts.setEncoding("ISO-8859-1");
 Tag &auftrag=ts.push_back(Tag("ManuProC-Order"));
 auftrag.setAttr("origin","mpc_agent");
 auftrag.setAttr("orderid",ord_id);
 auftrag.setAttr("agent",itos(VERKNR));
 ManuProC::Datum ad;
 Query("select datum from auftrag where aufid=? and vknr=?")
	<< ord_id << VERKNR >> ad;
 auftrag.setAttr("date",ad.to_iso());
 auftrag.setAttr("customer_no",kunde->get_value(KDBOX_NR));
 auftrag.setAttr("customer",kunde->get_value(KDBOX_NAME)); 


 Query q("select artnr||'/'||breite||'/'||farbe||'/'||aufmachung,"
	"ean,stueck from auftragentry where aufid=? and vknr=?");

 q << ord_id << VERKNR;
 FetchIStream fs=q.Fetch();

 while(fs.good())
   {
    std::string art,ean;
    int stk;
    fs >> art >> FetchIStream::MapNull(ean,"") >> stk;
    Tag &aufentry=auftrag.push_back(Tag("OrderRow"));
    aufentry.setAttr("article",art);
    aufentry.setAttr("ean",ean);
    aufentry.setAttr("amount",itos(stk));
    fs=q.Fetch();
   }
   
 std::ofstream of(file.c_str());
 ts.write(of);

}


