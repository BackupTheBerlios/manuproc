
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <Artikel/ArtikelBase.h>

class tex_table
{
 int columns;
 std::string artnr;
 std::vector<std::string> colors;

 static int rows_left;

 typedef std::map<std::string, std::string> c_to_ean;
 
	// maps width to map of colors which in turn holds the ean
 std::map<std::string, c_to_ean > tablecols;
 
 void cell_out(std::ostream &o, const std::string c, const std::string w) const;
 void row_out(std::ostream &o, const std::string c,
	const std::map<std::string, tex_table::c_to_ean>::const_iterator from) const;
 

 void begin_table(std::ostream &o,
	const std::map<std::string, c_to_ean>::const_iterator br) const;

 void new_page(std::ostream &o) const;

public:
 static void set_rows_on_page(int r) {rows_left=r;}
 void tex_table_out(std::ostream &o) const;

 tex_table():columns(0){}
 tex_table(const std::string a, int _cols, const std::string query);
 void add_cell(const std::string w, const std::string c, const std::string ean);

};







class scan_list
{
 std::string query;
 int listnr;

 std::map<std::string,tex_table> tables;  // maps artnr to one tex_table 

 void init_tables();

public:

 scan_list(int lnr){}
 scan_list(const std::string q);

 void TeX_out(std::ostream &o);
};



