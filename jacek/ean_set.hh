
#include <string>
#include <vector>
#include <Auftrag/AuftragBase.h>
#include <Misc/SQLerror.h>
#include <Artikel/ArtikelBezeichnung.h>

#define DEF_SIGNIFIKANZ		1
#define EAN_SIGNIFIKANZ		2


class ean_set
{
 std::vector<cH_ArtikelBezeichnung> eans;
 int cols,rows;
 
public:
 ean_set(const AuftragBase::ID aid) throw(SQLerror);
 ean_set(const std::string _ean, int _count, const ArtikelTyp at);
 ean_set() : cols(0),rows(0) {}

 bool cell_out(std::ostream &o,cH_ArtikelBezeichnung ab,bool last_col);
 void row_out(std::ostream &o, std::vector<std::string>::const_iterator i);
 void begin_table(std::ostream &o) const;
 void TeX_out(std::ostream &o); 
 
};


