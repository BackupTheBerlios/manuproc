#ifndef PPIXMAP_HH
#define PPIXMAP_HH

#include <gtk--/pixmap.h>
#include <Picture2.h>
#include <vector>

class PPixmap //: public HandleContent
{
   static vector<Picture2> vpix;
   static vector<Picture2> vpix_;

 public: 
   PPixmap();
   
   Gtk::Pixmap *getPixProz(double wieviel,double von); 
   Gtk::Pixmap *getPixProz(double prozent); 
   Gtk::Pixmap *getPixProz(); 
};



#endif