
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
 Tag &data=ts.push_back(Tag("ManuProC-Order"));
 data.setAttr("origin","mpc_agent");
 data.setAttr("orderid",ord_id);
 data.push_back(Tag("test")); 

 std::ofstream of(file.c_str());
 ts.write(of);

}


