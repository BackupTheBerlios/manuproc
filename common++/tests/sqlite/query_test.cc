#include <Misc/FetchIStream.h>
#include <stdio.h>
#include <Misc/dbconnect.h>

int main(void)
{Query::dbconnect();
 Query::Execute("select * from tb");
 Query::dbdisconnect();
}
