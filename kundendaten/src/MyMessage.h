
#include<Message.h>
#include<Aux/SQLerror.h>
#include<Aux/Ausgabe_neu.h>
#include <Aux/itos.h> 

class MyMessage : public Message
{
public:
 void Show(const SQLerror &e)
 { 
  std::string _msg = std::string("DB-Error Code:")+itos(e.Code());
  _msg = _msg + " DB-Error Message:"+e.Message();  
  _msg = _msg + " DB-Error Message:"+e.Context();  
  Message::Show((char *)_msg.c_str());
 }
 void Show(const std::string _msg) {  Message::Show((char *)_msg.c_str());}
};

