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
//#include <../pixmap/Geplant_small.xpm>
#include <../pixmap/P_gruen.xpm>
#include <../pixmap/V_gruen.xpm>
#include <../pixmap/Geliefert_small.xpm>
#include <../pixmap/fragezeichen_small.xpm>
#include <Aux/itos.h>

std::vector<Picture2> PPixmap::vpix;
std::map<char,Picture2> PPixmap::mappix;

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
     vpix.push_back(Picture2(P_gruen_xpm));
//     vpix.push_back(Picture2(Geplant_small_xpm));

     mappix['?']=Picture2(fragezeichen_small_xpm);
     mappix['G']=Picture2(Geliefert_small_xpm);
//     mappix['P']=Picture2(P_gruen_xpm);
     mappix['V']=Picture2(V_gruen_xpm);
   }
}

Gtk::Pixmap *PPixmap::getPixProz(double x)
{
  if (x<0)   {assert(!"PPixmap: Wert unter 0");} 
  if (x>1) {assert(!"PPixmap: Wert unter 1");} 

  if(x<.10)   return manage(new Gtk::Pixmap(vpix[0].pmap(),vpix[0].mask()));
  if(x<.20)   return manage(new Gtk::Pixmap(vpix[1].pmap(),vpix[1].mask()));
  if(x<.30)   return manage(new Gtk::Pixmap(vpix[2].pmap(),vpix[2].mask()));
  if(x<.40)   return manage(new Gtk::Pixmap(vpix[3].pmap(),vpix[3].mask()));
  if(x<.50)   return manage(new Gtk::Pixmap(vpix[4].pmap(),vpix[4].mask()));
  if(x<.60)   return manage(new Gtk::Pixmap(vpix[5].pmap(),vpix[5].mask()));
  if(x<.70)   return manage(new Gtk::Pixmap(vpix[6].pmap(),vpix[6].mask()));
  if(x<.80)   return manage(new Gtk::Pixmap(vpix[7].pmap(),vpix[7].mask()));
  if(x<.90)   return manage(new Gtk::Pixmap(vpix[8].pmap(),vpix[8].mask()));
  if(x<1.0)   return manage(new Gtk::Pixmap(vpix[9].pmap(),vpix[9].mask()));
  if(x==1.)   return manage(new Gtk::Pixmap(vpix[10].pmap(),vpix[10].mask()));
  assert(!"PPixmap kann nichts zurückgeben");
  abort();
}

Gtk::Pixmap *PPixmap::getPixProz(double wieviel,double von)
{
  if(von==0) return getPixMap('?');
  double x= wieviel/von;
//std::cout << wieviel<<" / "<<von<<" = "<<x<<'\n';
  return getPixProz(x);
}

Gtk::Pixmap *PPixmap::getPixMap(char c)
{
  return manage(new Gtk::Pixmap(mappix[c].pmap(),mappix[c].mask()));
}

