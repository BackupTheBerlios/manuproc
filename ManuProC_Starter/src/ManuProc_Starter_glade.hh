// generated 2006/1/10 11:01:05 CET by jacek@shindo-debian.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/Monotone/ManuProC_Starter/manuproc_starter.glade
// for gtk 2.6.4 and gtkmm 2.4.11
//
// Please modify the corresponding derived classes in ./src/ManuProc_Starter.hh and./src/ManuProc_Starter.cc

#ifndef _MANUPROC_STARTER_GLADE_HH
#  define _MANUPROC_STARTER_GLADE_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/window.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/table.h>
#include <gtkmm/frame.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#include <gtkmm/toolbutton.h>
#else //
#include <gtkmm/button.h>
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#endif //

class ManuProc_Starter_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
        Gtk::Tooltips _tooltips;
protected:
        class Gtk::RadioButton * database_default;
        class Gtk::RadioButton * database_OLD;
        class Gtk::Table * db_table;
        class Gtk::Frame * db_frame;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * pps_start;
#else //
        class Gtk::Button * pps_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * kunden_start;
#else //
        class Gtk::Button * kunden_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * lager_start;
#else //
        class Gtk::Button * lager_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * artikel_start;
#else //
        class Gtk::Button * artikel_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * preise_start;
#else //
        class Gtk::Button * preise_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * fibu_start;
#else //
        class Gtk::Button * fibu_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * fibu_alt_start;
#else //
        class Gtk::Button * fibu_alt_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * taxbird_start;
#else //
        class Gtk::Button * taxbird_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * vertrieb_start;
#else //
        class Gtk::Button * vertrieb_start;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * ooo;
#else //
        class Gtk::Button * ooo;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * preview;
#else //
        class Gtk::Button * preview;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * quit;
#else //
        class Gtk::Button * quit;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * lagerwert;
#else //
        class Gtk::Button * lagerwert;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * offeneposten;
#else //
        class Gtk::Button * offeneposten;
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::ToolButton * translate;
#else //
        class Gtk::Button * translate;
#endif //
        
        ManuProc_Starter_glade();
        
        ~ManuProc_Starter_glade();
private:
        virtual void on_database_toggled() = 0;
        virtual void on_pps_start_clicked() = 0;
        virtual void on_kunden_start_enter() = 0;
        virtual void on_lager_start_clicked() = 0;
        virtual void on_artikel_start_clicked() = 0;
        virtual void on_preise_start_clicked() = 0;
        virtual void on_fibu_start_clicked() = 0;
        virtual void on_fibu_alt_start_clicked() = 0;
        virtual void on_taxbird_start_clicked() = 0;
        virtual void on_vertrieb_start_clicked() = 0;
        virtual void on_ooo_clicked() = 0;
        virtual void on_preview_clicked() = 0;
        virtual void on_quit_clicked() = 0;
        virtual void on_lagerwert_clicked() = 0;
        virtual void on_offeneposten_clicked() = 0;
        virtual void on_translate_clicked() = 0;
};
#endif
