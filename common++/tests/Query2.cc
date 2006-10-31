#include <Misc/Query.h>
#include <Misc/dbconnect.h>

int main()
{
  Query::Row r;
  ManuProC::dbconnect_nt(ManuProC::Connection("","template1"));
  PreparedQuery pq("select * from pg_type where typname like ?");
  { Query q(pq);
    q << "b%";
    while ((q>>r).good())
      std::cout << r.Fetch<std::string>() << "\n";
  }
  ManuProC::dbdisconnect();
  ManuProC::dbconnect_nt(ManuProC::Connection("","template1"));
  { Query q(pq);
    q << "i%";
    while ((q>>r).good())
      std::cout << r.Fetch<std::string>() << "\n";
  }
  ManuProC::dbdisconnect();
}
