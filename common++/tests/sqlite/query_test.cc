#include <Misc/Query.h>
#include <stdio.h>
#include <Misc/dbconnect.h>

int main(void)
{ManuProC::dbconnect();
 int a;
 std::string s;
 Query("select * from tb") >> a >> s;
 std::cout << a << s;
 ManuProC::dbdisconnect();
}
