
#include<gtkmm/messagedialog.h>
#include<SQLerror.h>


class MyMessage : public Gtk::MessageDialog
{
public:

 MyMessage() : Gtk::MessageDialog("") {}

 MyMessage(const SQLerror &e) : Gtk::MessageDialog("")
 {char tmp[100]; 
  std::string _msg;
  
  snprintf(tmp,sizeof tmp,"DB-Error Code:%d\n",e.Code());
  _msg=tmp;
  snprintf(tmp,sizeof tmp,"DB-Error Message:%s\n",e.Message().c_str());  
  _msg+=tmp;
  snprintf(tmp,sizeof tmp,"Context:%s\n",e.Context().c_str());  
  _msg+=tmp;
  set_message(_msg);
 }
 
 MyMessage(const std::string &s) : Gtk::MessageDialog(s)
 {
 }

public:
 void set_Message(const std::string msg) 
   { set_message(msg); 
   }
 
};

