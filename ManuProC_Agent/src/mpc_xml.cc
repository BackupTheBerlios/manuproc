
#include "mpc_agent.hh"
#include <Misc/TagStream.h>
#include <MyMessage.h>
#include <Misc/itos.h>
#include <Misc/FetchIStream.h>
#include "mpc_export.hh"

void mpc_agent::on_senden_clicked()
{
 mpc_export mpx;
 mpx.set_transient_for(*this);
 int ret;
 ret=mpx.run();

}

/*
 std::string file="/dev/stdout";
 if(orderid->sensitive()) return; //nix selectiert
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

*/
