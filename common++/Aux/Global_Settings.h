#ifndef GLOBALSETTINGSCLASS
#define GLOBALSETTINGSCLASS
#include <string>

class Global_Settings
{
    int userid;
    std::string program,name,wert;       
  public:
    Global_Settings(int userid,const std::string& program,
                const std::string& name);

    std::string get_Wert() const {return wert;}
    void set_Wert(const std::string& wert);
    void update_Date();

    //Neuen Eintrag anlegen :
      //Wenn 'wert' == "now()" dann wird ein Timestamp eingetragen
    Global_Settings(int userid,const std::string& program,
                const std::string& name,const std::string& wert);
};
#endif
