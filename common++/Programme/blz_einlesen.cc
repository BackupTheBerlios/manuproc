#include <iostream>
#include <Misc/dbconnect.h>
#include <Misc/Query.h>
#include <Misc/create_parse.h>

// Die Liste der Bankleitzahlen ist von der Bundeszentralbank als Download
// verfügbar. Dieses Programm liest die PC ASCII (eigentlich LATIN1)
// Version ein. Bitte _nicht_ mehrmals benutzen ...

// Format:
// 1
// 100000001        1         Bundesbank                                                BBk                 10591Berlin                       BBk Berlin                 201001MARKF11000911380
// 12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
//          1         2         3         4         5         6         7         8         9        10        11        12        13        14        15        16        17        18

int main()
{ ManuProC::dbconnect();
  Query("set names 'latin1'");
  char buf[256];
  while (std::cin.getline(buf,sizeof buf),std::cin.good())
  { if (strlen(buf)==188+1) try
    { std::string name;
      long blz;
      long index;
      name=std::string(buf+139,buf+166);
      blz=ManuProC::parse<long>(std::string(buf,buf+8));
      index=ManuProC::parse<long>(std::string(buf+183,buf+188));
      if (blz)
        Query("insert into banken (bankindex,blz,name) values (?,?,trim(?))")
          << index << blz << name;
    } catch (...) {}
    else std::cerr << strlen(buf) << '\n';
  }
  int maxindex;
  Query("select max(bankindex) from banken") >> maxindex;
  Query("select setval('banken_bankindex_seq',?)") << maxindex;
  ManuProC::dbdisconnect();
  return 0;
}
