#include "PPixmap.hh"

#include <../pixmap/prozent000.xpm>
#include <../pixmap/prozent010.xpm>
#include <../pixmap/prozent020.xpm>
#include <../pixmap/prozent030.xpm>
#include <../pixmap/prozent040.xpm>
#include <../pixmap/prozent050.xpm>
#include <../pixmap/prozent060.xpm>
#include <../pixmap/prozent070.xpm>
#include <../pixmap/prozent080.xpm>
#include <../pixmap/prozent090.xpm>
//#include <../pixmap/prozent100.xpm>
#include <../pixmap/Geplant_small.xpm>
#include <../pixmap/fragezeichen_small.xpm>
#include <Aux/itos.h>

vector<Picture2> PPixmap::vpix;
vector<Picture2> PPixmap::vpix_;

PPixmap::PPixmap()
{
  if(vpix.empty())
   {
     vpix.push_back(Picture2(prozent000_xpm));
     vpix.push_back(Picture2(prozent010_xpm));
     vpix.push_back(Picture2(prozent020_xpm));
     vpix.push_back(Picture2(prozent030_xpm));
     vpix.push_back(Picture2(prozent040_xpm));
     vpix.push_back(Picture2(prozent050_xpm));
     vpix.push_back(Picture2(prozent060_xpm));
     vpix.push_back(Picture2(prozent070_xpm));
     vpix.push_back(Picture2(prozent080_xpm));
     vpix.push_back(Picture2(prozent090_xpm));
     vpix.push_back(Picture2(Geplant_small_xpm));
     vpix_.push_back(Picture2(fragezeichen_small_xpm));
   }
}

Gtk::Pixmap *PPixmap::getPixProz(double x)
{
  if (x<0)   {assert(!"PPixmap: Wert unter 0");} 
  if (x>1) {assert(!"PPixmap: Wert unter 1");} 

  if(x<.10)   return manage(new Gtk::Pixmap(vpix[0].std::map(),vpix[0].mask()));
  if(x<.20)   return manage(new Gtk::Pixmap(vpix[1].std::map(),vpix[1].mask()));
  if(x<.30)   return manage(new Gtk::Pixmap(vpix[2].std::map(),vpix[2].mask()));
  if(x<.40)   return manage(new Gtk::Pixmap(vpix[3].std::map(),vpix[3].mask()));
  if(x<.50)   return manage(new Gtk::Pixmap(vpix[4].std::map(),vpix[4].mask()));
  if(x<.60)   return manage(new Gtk::Pixmap(vpix[5].std::map(),vpix[5].mask()));
  if(x<.70)   return manage(new Gtk::Pixmap(vpix[6].std::map(),vpix[6].mask()));
  if(x<.80)   return manage(new Gtk::Pixmap(vpix[7].std::map(),vpix[7].mask()));
  if(x<.90)   return manage(new Gtk::Pixmap(vpix[8].std::map(),vpix[8].mask()));
  if(x<1.0)   return manage(new Gtk::Pixmap(vpix[9].std::map(),vpix[9].mask()));
  if(x==1.)   return manage(new Gtk::Pixmap(vpix[10].std::map(),vpix[10].mask()));
  assert(!"PPixmap kann nichts zurückgeben");
  abort();
}

Gtk::Pixmap *PPixmap::getPixProz(double wieviel,double von)
{
  if(von==0) return getPixProz();
  double x= wieviel/von;
//std::cout << wieviel<<" / "<<von<<" = "<<x<<'\n';
  return getPixProz(x);
}

Gtk::Pixmap *PPixmap::getPixProz()
{
  return manage(new Gtk::Pixmap(vpix_[0].std::map(),vpix_[0].mask()));
}

