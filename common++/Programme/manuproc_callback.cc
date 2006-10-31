#include <Misc/TagStream.h>
#include <unistd.h>
#include <cassert>
#include <iostream>

int main(int argc, char **argv)
{ assert(argc==2);
  TagStream ts(argv[1]);
  
  Tag &root=ts.getContent();
  assert(root.Type()=="tex2prn_completion");
  int first=root.getAttr_def<int>("first_page",1);
  int last=root.getAttr_def<int>("last_page");
  int pages=root.getAttr<int>("pages");
  std::cout << "prosessed: pages " << first << '-' << last << " of " << pages;

  FOR_EACH_CONST_TAG_OF(j,root,"mpc_rng_sent")
  {
std::cout << " RNGID:" << j->getAttr<int>("rngid");
  }

/*
  FOR_EACH_CONST_TAG_OF(j,root,"printer")
  { FOR_EACH_CONST_TAG_OF(i,*j,"print")
    { std::cout << ' ' << i->getAttr<int>("copies") << " copies to printer "
        << i->getAttr("name");
    }
  }
  FOR_EACH_CONST_TAG_OF(i,root,"save")
  { std::cout << " saved to file "
      << i->getAttr("file");
  }
  FOR_EACH_CONST_TAG_OF(i,root,"fax")
  { std::cout << " faxed to number "
      << i->getAttr("number");
  }
  FOR_EACH_CONST_TAG_OF(i,root,"mail")
  { std::cout << " mailed to " << i->getAttr("address") << " with subject "
      << i->getAttr("subject");
  }
  std::cout << '\n';
  // arguments/integer bool string 
  FOR_EACH_CONST_TAG_OF(i,root,"arguments")
  { std::cout << "args: integer " << i->getAttr<int>("integer")
      << " bool " << i->getAttr<bool>("bool")
      << " string " << i->getAttr("string") << '\n';
  }
*/

  return 0;
}



