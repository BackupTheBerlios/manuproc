
#include "ean_set.hh"
#include <getopt.h>
#include <Misc/dbconnect.h>
#include <fstream>
#include <Misc/FILEstream.h>
#include <Misc/FetchIStream.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Artikel/ArtikelEAN.h>
#include <Artikel/ArtikelTyp.h>
#include <Misc/EntryValueBase.h>

#define COLUMNS		5
#define ROWS		13


const static struct option options[]=
{
 { "aufnr",   required_argument,        NULL, 'n' },
 { "artean",   required_argument,  NULL, 'a' },
 { "count",   required_argument,  NULL, 'c' }, 
 { "arttyp",   required_argument,  NULL, 'T' }, 
 { "texfile",   required_argument,      NULL, 't' },
 { "view",   no_argument,      NULL, 'v' },
 { NULL,      0,       NULL, 0 }
};

void usage(std::string n)
{
 std::cout << "$Id: drucken_provision.cc,v 1.2 2003/05/23 16:49:16 jacek Exp\n"
              "USAGE:" << n << " -n <Nr> -a <EAN> -t <file> -c<count> -T<arttyp>\n"
                "\n\t-t\t nur TeX file erzeugen\n"
                "\t-n\t Auftr.-Nummer \n"
                "\t-a\t Artikel-EAN \n"
                "\t-c\t Anzahl der Etiketten\n"
                "\t-T\t Artikeltyp\n"
		"\t-v\t biew only \n";
            exit(1);

}

ean_set::ean_set(const AuftragBase::ID aufid) throw(SQLerror)
: cols(COLUMNS), rows(ROWS)
{
 
 Query q("select e.artikelid,e.bestellt from auftragentry e where instanz=?"
       " and auftragid=? order by artikelbezeichnung(artikelid)");
 q << ppsInstanzID::Kundenauftraege << aufid;
 
 FetchIStream fi=q.Fetch();

 while(fi.good())
   {
    ArtikelBase::ID aid;
    int bestellt;
    fi >> aid >> bestellt;
    ArtikelBase a(aid);
    cH_ArtikelBezeichnung ab(a);
    cH_ExtBezSchema ebz=ab->getExtBezSchema();
    for(int i=0; i<bestellt; i++)
      eans.push_back(ab);
    fi=q.Fetch();
   }

}

ean_set::ean_set(const std::string _ean, int _count, const ArtikelTyp at)
: cols(COLUMNS), rows(ROWS)
{
 ArtikelEAN ae(_ean, at);
 cH_ArtikelBezeichnung ab(ae);
 cH_ExtBezSchema ebz=ab->getExtBezSchema();

 for(int i=0; i<_count; i++)
    eans.push_back(ab);
}


int main(int argc, char *argv[])
{
 int opt;
 int aufid,count=5;
 std::string texfile;
 std::string ean_toprint;
 int at=ArtikelTyp::default_ID;
 bool toTeX=false, view=false;

 if(argc==1) usage(argv[0]);

 while ((opt=getopt_long(argc,argv,"t:vn:a:c:T:",options,NULL))!=EOF)
  { switch (opt)
    { 
       case 'a' : ean_toprint=optarg; break;
       case 'T' : at=atoi(optarg); break;
       case 'c' : count=atoi(optarg); break;
       case 'n' : aufid=atoi(optarg);break;
       case 't' : toTeX=true; texfile=optarg; break;
       case 'v' : view=true;break;
       case '?': 
       default :
       usage(argv[0]); exit(0);
    }
  }       


 ManuProC::Connection conn;
 conn.setDbase("");
 conn.setHost(""); 
 ManuProC::dbconnect(conn);

 ean_set es;
 
 if(ean_toprint.empty())
   es = ean_set(aufid);
 else
   es = ean_set(ean_toprint,count,at);  

 std::string cmd("tex2prn -2 ");

 if(toTeX) cmd=std::string("cat > ./")+texfile;
 else
   if(view) cmd+=" -G ";

 FILE *f=popen(cmd.c_str(),"w");
 oFILEstream os(f);

 es.TeX_out(os);

 ManuProC::dbdisconnect();

 return 0;
}

// bool retvalue indicates wether or not the cell was printed
bool ean_set::cell_out(std::ostream &o,cH_ArtikelBezeichnung  ab,bool last_col)
{
 std::string bez=ab->Bezeichnung(DEF_SIGNIFIKANZ,'/');
 std::string ean=ab->Bezeichnung(EAN_SIGNIFIKANZ);
 
 if(ean.empty()) return false;
 
 o << "{\\rule[-2.6ex]{0mm}{21.1mm} ";
 o << "\\raisebox{1.2cm}[0pt][0pt]{\\makebox[0mm][s]{"
   << bez << "}} ";
 if(!ean.empty())  
   o << "\\font\\ocrb=ocrb8 \\barheight=0.65cm \\X=0.29mm "
      << " \\EAN " << ean <<"}";
 else o << "}";     
// else o << "\\rule[0mm]{39.1mm}{0cm} }";
     
 if(last_col) {o << "\\\\";
      o << "\n\\hline\n";
      }
 
 else o << " & ";
 return true;
 
}



void ean_set::begin_table(std::ostream &o) const
{
 std::string tabformat;
 tabformat="|";
 for(int i=0; i<cols; i++)
  { tabformat+="X";
   tabformat+="|";
  }
  
 o << "\\begin{tabularx}{19.75cm}{"+tabformat+"}\\\\\n";
 
 o << "\\hline\n";
}


void ean_set::TeX_out(std::ostream &o)
{
 o << "\\documentclass[a4paper]{article}\n"
   << "\\usepackage{tabularx,longtable,vmargin,helvetic}\n"
//   << "\\setmarginsrb{9.75mm}{9.5mm}{0.0in}{0.0in}{0pt}{0pt}{0pt}{0pt}\n";
   << "\\setmarginsrb{0.0mm}{10.7mm}{0.0mm}{10.7mm}{0pt}{0pt}{0pt}{0pt}\n";
  
 o << "\\pagestyle{empty}\n";
  
 o << "\\begin{document}\n"
   << "\\input ean13\n";   

 o << "\\renewcommand{\\arraystretch}{0}\n"
   << "\\renewcommand{\\rmdefault}{phv}\n"
   << "\\rm\\small";

 std::vector<cH_ArtikelBezeichnung>::const_iterator ei=eans.begin();
 
 int c=0,r=0;
 begin_table(o);
 
 for(;ei!=eans.end();++ei)
   {
    if(cell_out(o,*ei,c==(cols-1))==false) continue;
    
    c++;
    if(c==cols)
      {c=0;
       r++;
       if(r==rows)
         {r=0;
          o << "\\end{tabularx}\n\n";
          o  << "\\newpage\n";
          begin_table(o); 
         }
      }
   }
   
 if(c>0 && c<cols)
   {for(int i=c; i<(cols-1); i++)
      o << "&";  
    o << "\\\\\\hline\n";  
   }

 o << "\\end{tabularx}\n\n";
 
 o << "\\end{document}\n";
}












