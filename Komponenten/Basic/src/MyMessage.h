
#include<gtkmm/messagedialog.h>
#include<Misc/Datum.h>
#include <strstream>
#include<Misc/SQLerror.h>

class MyMessage : public Gtk::MessageDialog
{
public:

 MyMessage() : Gtk::MessageDialog("") {}

 MyMessage(const SQLerror &e) :
 	 Gtk::MessageDialog("",Gtk::MESSAGE_ERROR)
 {char tmp[100]; 
  std::string _msg;
  
  snprintf(tmp,sizeof tmp,"DB-Error Code:%d\n",e.Code());
  _msg=tmp;
  snprintf(tmp,sizeof tmp,"DB-Error Message:%s\n",e.Message().c_str());  
  _msg+=tmp;
  snprintf(tmp,sizeof tmp,"Context:%s\n",e.Context().c_str());  
  _msg+=tmp;
  set_message(_msg,true);
 }
 
 MyMessage(const ManuProC::Datumsfehler &e) :
 	 Gtk::MessageDialog("",Gtk::MESSAGE_ERROR)
 {
  std::strstream ostr;
  ostr << e;  
  set_message(ostr.str(),true);
 } 
 
 MyMessage(const std::string &s,Gtk::MessageType mt=Gtk::MESSAGE_INFO) : 
 		Gtk::MessageDialog(s,mt)
 {
 }

public:
 void set_Message(const std::string msg) 
   { set_message(msg,true); 
   }
 
};

