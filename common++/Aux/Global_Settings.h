#ifndef GLOBALSETTINGSCLASS
#define GLOBALSETTINGSCLASS
#include <string>

class Global_Settings
{
    int userid;
    std::string program,name,wert;       
  public:
    Global_Settings(int u,const std::string& p,const std::string& n);

    std::string get_Wert() const {return wert;}
    void set_Wert(const std::string& wert);
    void update_Date();

    //Neuen Eintrag anlegen :
    Global_Settings(int u,const std::string& p,const std::string& n,const std::string& w);
};
#endif
