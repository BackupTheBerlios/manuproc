#include <Misc/FetchIStream.h>
#include <stdio.h>
#include <Misc/dbconnect.h>

int main(void)
{ManuProC::dbconnect();
 Query::Execute("select * from tb");
 ManuProC::dbdisconnect();
}
