
#include "mpc_agent.hh"
#include <TagStream.h>
#include <MyMessage.h>


void mpc_agent::on_senden_clicked()
{
 std::string file="/dev/stdout";

 TagStream ts;
 ts.setEncoding("ISO-8859-1");
 Tag &data=ts.push_back(Tag("ManuProC-Order"));

 std::ofstream of(file.c_str());
 ts.write(of);

}


