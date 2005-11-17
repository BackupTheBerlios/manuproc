
#include <Basic/src/i18n.h>
#include <string>

namespace Database
{ enum context { Instanz };
};

std::string dbgettext(std::string const& key, Database::context ctx);
