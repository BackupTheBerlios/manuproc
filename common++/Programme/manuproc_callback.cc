#include <Misc/TagStream.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <Lieferschein/Rechnung.h>
#include <Lieferschein/Lieferschein.h>
#include <Auftrag/Auftrag.h>
#include <Aux/dbconnect.h>


int main(int argc, char **argv)
{ assert(argc==2);
  TagStream ts(argv[1]);
  
  Tag &root=ts.getContent();
  assert(root.Type()=="tex2prn_completion");
  int first=root.getAttr_def<int>("first_page",1);
  int last=root.getAttr_def<int>("last_page");
  int pages=root.getAttr<int>("pages");
  std::cout << "prosessed: pages " << first << '-' << last << " of " << pages;


// Rechnung
  FOR_EACH_CONST_TAG_OF(j,root,"mpc_rng_sent")
  {
   ManuProcEntity<>::ID rngid(j->getAttr<int>("document_id"));
   std::cout << " RNGID:" << rngid << "\n";
   std::string dest;
   bool sent=false;

   FOR_EACH_CONST_TAG_OF(j,root,"mail")
     {dest = j->getAttr("address");
      if(!(j->getAttr("cc_address")).empty())
        {if(!dest.empty()) dest += ", "; 
	 else dest+=j->getAttr("cc_address");
	}
      sent=!(dest.empty());
     }
   if(dest.empty())
     FOR_EACH_CONST_TAG_OF(j,root,"fax")
      {dest = j->getAttr("number");
       sent=!(dest.empty());
      }

   if(sent)
   {
    try {
      ManuProC::Connection conn;
      conn.setDbase("");
      conn.setHost("");
      ManuProC::dbconnect(conn);
    Rechnung r(rngid);
    r.Set_sent_at();
    r.Set_sent_to(dest);
    ManuProC::dbdisconnect();
    }
    catch (SQLerror &e)
     {  std::cerr << e << '\n';
      return 1;
     }
   }
  }


//Auftrag
  FOR_EACH_CONST_TAG_OF(j,root,"mpc_ord_sent")
  {
   ManuProcEntity<>::ID abid(j->getAttr<int>("document_id"));
   std::cout << " ABID:" << abid << "\n";
   std::string dest;
   bool sent=false;

   FOR_EACH_CONST_TAG_OF(j,root,"mail")
     {dest = j->getAttr("address");
      if(!(j->getAttr("cc_address")).empty())
        {if(!dest.empty()) dest += ", "; 
	 else dest+=j->getAttr("cc_address");
	}
      sent=!(dest.empty());
     }
   if(dest.empty())
     FOR_EACH_CONST_TAG_OF(j,root,"fax")
      {dest = j->getAttr("number");
       sent=!(dest.empty());
      }

   if(sent)
   {
    try {
      ManuProC::Connection conn;
      conn.setDbase("");
      conn.setHost("");
      ManuProC::dbconnect(conn);
    Auftrag a(AuftragBase(ppsInstanzID::Kundenauftraege,abid));
    a.Set_sent_at();
    a.Set_sent_to(dest);
    ManuProC::dbdisconnect();
    }
    catch (SQLerror &e)
     { std::cerr << e << '\n';
       return 1;
     }
    }
  }


// Lieferschein
FOR_EACH_CONST_TAG_OF(j,root,"mpc_deliv_sent")
  {
   ManuProcEntity<>::ID lsid(j->getAttr<int>("document_id"));
   std::cout << " LSID:" << lsid << "\n";
   std::string dest;
   bool sent=false;

   FOR_EACH_CONST_TAG_OF(j,root,"mail")
     {dest = j->getAttr("address");
      if(!(j->getAttr("cc_address")).empty())
        {if(!dest.empty()) dest += ", "; 
	 else dest+=j->getAttr("cc_address");
	}
      sent=!(dest.empty());
     }
   if(dest.empty())
     FOR_EACH_CONST_TAG_OF(j,root,"fax")
      {dest = j->getAttr("number");
       sent=!(dest.empty());
      }

   if(sent)
   {
    try {
      ManuProC::Connection conn;
      conn.setDbase("");
      conn.setHost("");
      ManuProC::dbconnect(conn);
    Lieferschein l(LieferscheinBase(ppsInstanzID::Kundenauftraege,lsid));
    l.Set_sent_at();
    l.Set_sent_to(dest);
    ManuProC::dbdisconnect();
    }
    catch (SQLerror &e)
     { std::cerr << e << '\n';
       return 1;
     }
    }
  }

  return 0;
}



