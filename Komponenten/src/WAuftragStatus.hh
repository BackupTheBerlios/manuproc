#ifndef _WAuftragStatus_HH
#  define _WAuftragStatus_HH

#include <gtk--/optionmenu.h>
#include <Auftrag/auftrag_status.h>
#include <vector>

class WAuftragStatus : public Gtk::OptionMenu 
{
   private:
      std::string status;
      void fuelleMenu();
   public:

      WAuftragStatus();
      ~WAuftragStatus(){}

      void set_History(AufStatVal a) {set_history(a);} 
      AufStatVal get_Status();

      SigC::Signal0<void> activate;
      
};
#endif
