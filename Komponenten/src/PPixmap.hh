#ifndef PPIXMAP_HH
#define PPIXMAP_HH

#include <gtk--/pixmap.h>
#include <Picture2.h>
#include <vector>
#include <map>

class PPixmap 
{
   static std::vector<Picture2> vpix;
   static std::map<char,Picture2> mappix;

 public: 
   PPixmap();
   
   Gtk::Pixmap *getPixProz(double wieviel,double von); 
   Gtk::Pixmap *getPixProz(double prozent); 
   Gtk::Pixmap *getPixMap(char c);
};



#endif