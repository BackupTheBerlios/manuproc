
#include <Aux/FetchIStream.h>
#include <Aux/FILEstream.h>
#include <Aux/dbconnect.h>
#include <Aux/itos.h>
#include "scan_list.hh"

#define COLUMNS 	6
#define ROWS		20

int tex_table::rows_left;

int main(int argc, char *argv[])
{

 ManuProC::Connection conn; 
 conn.setDbase("");
 conn.setHost("");
 ManuProC::dbconnect(conn);  

 scan_list sl(argv[1]);

 
 FILE *f=popen("tex2prn -2 -G","w");
 oFILEstream os(f);

 sl.TeX_out(os);

 FILE *f2=popen("cat > ./out.tex","w");
 oFILEstream os2(f2);

 sl.TeX_out(os2);

}


void scan_list::TeX_out(std::ostream &o)
{
 o << "\\documentclass[a4paper]{article}\n"
   << "\\usepackage{tabularx,longtable,vmargin}\n"
   << "\\setmarginsrb{0.3in}{0.3in}{0.3in}{0.3in}{0pt}{0pt}{0pt}{0pt}\n";
 
 o << "\\begin{document}\n"
   << "\\input ean13\n";

 std::map<std::string,tex_table>::const_iterator i=tables.begin();
 std::map<std::string,tex_table>::const_iterator i_end=tables.end();
 

 tex_table::set_rows_on_page(ROWS);
 while(i!=i_end)
    {(*i).second.tex_table_out(o); 
     ++i;
    }

 o << "\\end{document}\n"; 
}


scan_list::scan_list(const std::string _q)
	: query(_q)
{
 init_tables(); 
}


void scan_list::init_tables()
{
 std::string artikel,breite,farbe,aufm,ean;

 Query q("select artikel,breite,farbe,aufmachung,ean from"
	" artbez_3_1 where id in ("+query+")");
 
 FetchIStream fi=q.Fetch();

 while(fi.good())
  {
   fi >> FetchIStream::MapNull(artikel) 
   	>> FetchIStream::MapNull(breite) 
   	>> FetchIStream::MapNull(farbe) 
   	>> FetchIStream::MapNull(aufm) 
   	>> FetchIStream::MapNull(ean);

   if(tables.find(artikel)==tables.end())
     {tables[artikel]=tex_table(artikel,COLUMNS,query);
     }
        
   tables[artikel].add_cell(breite,farbe,ean);
   fi=q.Fetch();
  }

}

void tex_table::cell_out(std::ostream &o, const std::string c, 
		const std::string w) const
{
 std::cout << artnr << w << c <<"\n";

 std::map<std::string, c_to_ean>::const_iterator i=tablecols.find(w);
 c_to_ean::const_iterator s=(*i).second.find(c);

 if(s!=(*i).second.end())
   o << " & \\font\\ocrb=ocrb7 \\barheight=0.65cm \\X=0.25mm "
     << " \\EAN " << (*s).second;
 else
   o << " & "; 
}

void tex_table::row_out(std::ostream &o, const std::string c, 
	const std::map<std::string, tex_table::c_to_ean>::const_iterator from) const
{
 std::map<std::string, tex_table::c_to_ean>::const_iterator it=from;

 o << "\\rule[0mm]{0mm}{12mm} \\Large " << c;

 for(int i=0; i<columns-1; i++)
   {
    if(it != tablecols.end())
       {
        cell_out(o,c,(*it).first);
        ++it;
       }
    else
      o << "&";
   }

 o << "\\\\ \\hline\n";
}




void tex_table::add_cell(const std::string w, 
	const std::string c, const std::string ean)
{
 std::cout << artnr << w << c << ean <<"\n";

 if(tablecols.find(w)==tablecols.end())
     tablecols[w]=std::map<std::string,std::string>();

 (tablecols[w])[c]=ean;

}


tex_table::tex_table(const std::string a, int _cols, const std::string query) : columns(_cols), artnr(a) 
{
 Query q("select distinct farbe from artbez_3_1 where "
 	" id in ("+query+") and artikel=? order by farbe");
 	
 q << artnr;
 
 q.FetchArray(colors);
}


void tex_table::begin_table(std::ostream &o,
	const std::map<std::string, c_to_ean>::const_iterator br) const
{
 o << "\\begin{tabularx}{\\linewidth}{lccccc}\\\\\n"
   << "\\multicolumn{"+itos(columns)+"}{c}{\\LARGE Artikel "+artnr+"}\\\\[2ex]\n";

 o << "Breite $\\rightarrow$";
 
 std::map<std::string, c_to_ean>::const_iterator i=br;
 
 for(int c=0; c<columns-1; c++)
   {if(i!=tablecols.end())
      {o << " & \\Large "<<(*i).first<<"mm ";
       ++i;
      }
    else
       o << "&";
   }
   
 o << "\\\\\n";
 o << "\\multicolumn{"+itos(columns)+"}{l}{Farbe $\\downarrow$}\\\\\n";
 
}

void tex_table::new_page(std::ostream &o) const
{
 o << "\\end{tabularx}\n\n"
   << "\\newpage\n";
}

void tex_table::tex_table_out(std::ostream &o) const
{
 std::map<std::string, c_to_ean>::const_iterator it_breite=tablecols.begin();  
 std::map<std::string, c_to_ean>::const_iterator it_end=tablecols.end();   
 
 while(it_breite!=it_end)
   {
    if(rows_left<2)
       {new_page(o); rows_left=ROWS;
        rows_left--;
       }
    else
      rows_left-=2;
      
    begin_table(o,it_breite);   

   
    std::vector<std::string>::const_iterator it_colors=colors.begin();
    for(it_colors=colors.begin(); it_colors!=colors.end(); ++it_colors)
      {
       if(rows_left<1)
         {new_page(o); 
          rows_left=ROWS;
          begin_table(o,it_breite);   
	  rows_left--;
	 }
       row_out(o,*it_colors,it_breite);
       rows_left--;
      }

    for(int c=0; c<columns-1; c++)
      if(it_breite!=it_end) 
        ++it_breite;

    if(rows_left>=2 && it_breite!=it_end)          
      o << "\\end{tabularx}\n";
   }	
    
 o << "\\end{tabularx}\n";

}



