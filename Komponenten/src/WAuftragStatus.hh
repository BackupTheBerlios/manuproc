#ifndef _WAuftragStatus_HH
#  define _WAuftragStatus_HH

#include <gtk--/optionmenu.h>
#include <Auftrag/auftrag_status.h>
#include <Auftrag/AuftragBase.h>
#include <vector>

class WAuftragStatus : public Gtk::OptionMenu 
{
   private:
//      std::string status;
      void fuelleMenu();
   public:

      WAuftragStatus();
      ~WAuftragStatus(){}
      
      void set_value(AufStatVal a);
      AufStatVal get_value() const;
      SigC::Signal0<void> activate;

// Veraltet!
      void set_History(AufStatVal a) {set_value(a);} 
      AufStatVal get_Status() const { return get_value(); }
      const std::string get_Status_Name() const 
      { return AuftragBase::getStatusStr(get_value()); }
};
#endif
