
#include "mpc_agent.hh"
#include <TagStream.h>
#include <MyMessage.h>
#include <itos.h>

void mpc_agent::on_senden_clicked()
{
 std::string file="/dev/stdout";
 if(orderid->sensitive()) return; //nix selectiert
 std::string ord_id=itos(orderid->get_value());


 TagStream ts;
 ts.setEncoding("ISO-8859-1");
 Tag &auftrag=ts.push_back(Tag("ManuProC-Order"));
 auftrag.setAttr("origin","mpc_agent");
 auftrag.setAttr("orderid",ord_id);
 auftrag.setAttr("agent",itos(VERKNR));

 Tag &aufentry=auftrag.push_back(Tag("OrderRow"));
 aufentry.setAttr("article","12345");
 aufentry.setAttr("ean","1122334455667");
 aufentry.setAttr("amount","10");
 Tag &aufentry2=auftrag.push_back(Tag("OrderRow"));
 aufentry2.setAttr("article","12345");
 aufentry2.setAttr("ean","1122334455667");
 aufentry2.setAttr("amount","10");

 std::ofstream of(file.c_str());
 ts.write(of);

}


