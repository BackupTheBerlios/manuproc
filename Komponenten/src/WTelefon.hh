#ifndef _WID_TELEFON_HH
#  define _WID_TELEFON_HH

#include <gtk--/spinbutton.h>
#include <gtk--/optionmenu.h>
#include <gtk--/table.h>
#include <gtk--/frame.h>
#include <SimpleTree.hh>
#include <Kunde/Telefon.h>
 

class WTelefon : public Gtk::Table
{
 private:
   class Gtk::OptionMenu *option_menu;
   class Gtk::SpinButton *spinbutton_land; 
   class Gtk::SpinButton *spinbutton_vorwahl; 
   class Gtk::SpinButton *spinbutton_nummer; 
   class SimpleTree *tree;

   std::list<cH_Telefon> TelList;

   void setTitels();
   void showTel();

   void spinbutton_land_activate();
   void spinbutton_vorwahl_activate();
   void spinbutton_nummer_activate();

   bool getSelectedTel(cH_Telefon &T) const;
   void on_buttonNumLoeschen_clicked();

   static gint try_grab_focus(GtkWidget *w,gpointer gp);
 public:
   WTelefon();
   void setNumber(int land,int vorwahl,int nummer);
   void setNumber(int vorwahl,int nummer) {setNumber(49,vorwahl,nummer); }
   void setNumber(Telefon::st_nummer T) {setNumber(T.land,T.vorwahl,T.nummer);}
   void showTel(std::list<cH_Telefon> VT);

   int getLand() const;
   int getVorwahl() const;
   int getNummer() const;
   Telefon::st_nummer get_value() const;
   void clear() const;

   SigC::Signal0<void> activate;
};

#include <rowdata.h>
#include <Aux/EntryValueIntString.h>

class Data_Tel :  public RowDataBase
{
      cH_Telefon T;
   public:
      Data_Tel(cH_Telefon t) : T(t){}
      enum Spalten {NUMMER,ART};
      virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
       {
         switch((Spalten)seqnr)
          {
            case NUMMER :  return cH_EntryValueIntString(T->NummerStr());
            case ART    :  
               std::string s=T->ArtString();
               if(T->isPrivat()) s+=" (privat)";
cout << T->ArtString()<<' '<<T->isPrivat()<<' '<<s<<'\n';
               return cH_EntryValueIntString(s);
          }
         return cH_EntryValueIntString();
       }
   cH_Telefon getTelefon() const {return T;}
};

class cH_Data_Tel : public Handle<const Data_Tel>
{
 public:
   cH_Data_Tel(const Data_Tel *r) : Handle<const Data_Tel>(r) {}
};
 

#endif
