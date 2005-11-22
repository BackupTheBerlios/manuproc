#include "i18n.h"
#include <Misc/dbconnect.h>

static std::string last_domain;

std::string dbgettext(std::string const& key)
{ if (key.empty()) return key;
  if (ManuProC::get_dbname()!=last_domain)
  { last_domain=ManuProC::get_dbname();
    bindtextdomain(last_domain.c_str(), PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(last_domain.c_str(), "UTF-8");
  }
  return dgettext(last_domain.c_str(), key.c_str());
}
