#ifndef _WID_TELEFON_HH
#  define _WID_TELEFON_HH

#include "WTelefon_glade.hh"
#include <Kunde/Telefon.h>
#include <Kunde/Kunde.h>
#include <list> 

class WTelefon : public WTelefon_glade
{	friend class WTelefon_glade;
 private:
  ManuProcEntity::ID kundennr;
  ManuProcEntity::ID persnr;

//   std::list<cH_Telefon> TelList;

   void setTitels();
   void showTel();

   void anderer_typ();
   void land_activate();
   void vorwahl_activate();
   void nummer_activate();
   void durchwahl_activate();
   void text_activate();

   bool getSelectedTel(cH_Telefon &T) const;
   void on_buttonNumLoeschen_clicked();
   void felder_anpassen(TelArt art);

   static gint try_grab_focus(GtkWidget *w,gpointer gp);
   
   std::list<cH_Telefon> TelList;

 public:
   WTelefon();
   
   void set_value(const cH_Telefon &v);
   cH_Telefon get_value() const;
   
   void showTel(std::list<cH_Telefon> VT);
   void setKdPer(const ManuProcEntity::ID kid, const ManuProcEntity::ID pid)
	{kundennr=kid, persnr=pid;}

   void clear(bool withtree=true) const;

   SigC::Signal1<void,cH_Telefon> add;
   SigC::Signal1<void,cH_Telefon> remove;
};


#endif
