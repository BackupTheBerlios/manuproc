
#include<gtkmm/messagedialog.h>
#include<Misc/Datum.h>
#include <strstream>
#include<Misc/SQLerror.h>
#include <Misc/compiler_ports.h>

class MyMessage : public Gtk::MessageDialog
{
public:

 MyMessage() : Gtk::MessageDialog("") {}

 MyMessage(const SQLerror &e) :
 	 Gtk::MessageDialog("",Gtk::MESSAGE_ERROR)
 { set(e);
 }
 
 MyMessage(const ManuProC::Datumsfehler &e) :
 	 Gtk::MessageDialog("",Gtk::MESSAGE_ERROR)
 { set(e);
 } 
 
 MyMessage(const std::string &s,Gtk::MessageType mt=Gtk::MESSAGE_INFO) : 
 		Gtk::MessageDialog(s,mt)
 {
 }
 
 static int show_and_wait(const std::string &s, Gtk::Window *parent=0, Gtk::MessageType mt=Gtk::MESSAGE_INFO)
 { MyMessage m(s,mt);
   if (parent) m.set_transient_for(parent);
   m.show();
   return m.run();
 }

public:
 __deprecated void set_Message(const std::string msg) 
   { set_message(msg,true); 
   }
 void set(const std::string msg) 
   { set_message(msg,true); 
   }
 void set(const ManuProC::Datumsfehler &e)
   {
    std::strstream ostr;
    ostr << e;  
    set_message(ostr.str(),true);
   }
 void set(const SQLerror &e)
   { 
      char tmp[100]; 
      std::string _msg;
      
      snprintf(tmp,sizeof tmp,"DB-Error Code:%d %s\n",e.Code(),e.State().c_str());
      _msg=tmp;
      snprintf(tmp,sizeof tmp,"DB-Error Message:%s\n",e.Message().c_str());  
      _msg+=tmp;
      snprintf(tmp,sizeof tmp,"Context:%s\n",e.Context().c_str());  
      _msg+=tmp;
      set_message(_msg,true);
   }
};

