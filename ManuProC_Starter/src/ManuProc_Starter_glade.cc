// generated 2005/5/25 13:50:06 CEST by christof@vesta.petig-baender.de
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- manuproc_starter.glade
// for gtk 2.6.7 and gtkmm 2.6.1
//
// Please modify the corresponding derived classes in ./src/ManuProc_Starter.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/compatibility.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "ManuProc_Starter_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/image.h>
#include <gtkmm/stockid.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <gtkmm/separatortoolitem.h>
#endif //
#include <gtkmm/toolbar.h>
#include <gtkmm/box.h>

ManuProc_Starter_glade::ManuProc_Starter_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  
   
   Gtk::Window *ManuProc_Starter = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Image *pps_start_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-execute"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   pps_start = Gtk::manage(new class Gtk::ToolButton(*pps_start_img, "PPS"));
#endif //
   
   Gtk::Image *kunden_start_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-open"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   kunden_start = Gtk::manage(new class Gtk::ToolButton(*kunden_start_img, "Kunden"));
#endif //
   
   Gtk::Image *lager_start_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-paste"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   lager_start = Gtk::manage(new class Gtk::ToolButton(*lager_start_img, "Lager"));
#endif //
   
   Gtk::Image *artikel_start_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-index"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   artikel_start = Gtk::manage(new class Gtk::ToolButton(*artikel_start_img, "Artikel"));
#endif //
   
   Gtk::Image *preise_start_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-justify-fill"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   preise_start = Gtk::manage(new class Gtk::ToolButton(*preise_start_img, "Preislisten"));
#endif //
   
   Gtk::Image *fibu_start_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-sort-ascending"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   fibu_start = Gtk::manage(new class Gtk::ToolButton(*fibu_start_img, "FiBu"));
#endif //
   
   Gtk::Image *vertrieb_start_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-jump-to"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   vertrieb_start = Gtk::manage(new class Gtk::ToolButton(*vertrieb_start_img, "Vertrieb"));
#endif //
   
   Gtk::Image *ooo_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-select-font"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   ooo = Gtk::manage(new class Gtk::ToolButton(*ooo_img, "OpenOffice"));
#endif //
   
   Gtk::Image *preview_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-dialog-error"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   preview = Gtk::manage(new class Gtk::ToolButton(*preview_img, "(vorab)"));
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   
   Gtk::SeparatorToolItem *separatortoolitem1 = Gtk::manage(new class Gtk::SeparatorToolItem());
#endif //
   Gtk::Image *quit_img = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-quit"), Gtk::IconSize(3)));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   quit = Gtk::manage(new class Gtk::ToolButton(*quit_img, "Beenden"));
#endif //
   
   Gtk::Toolbar *toolbar1 = Gtk::manage(new class Gtk::Toolbar());
   Gtk::VBox *vbox1 = Gtk::manage(new class Gtk::VBox(false, 0));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*pps_start);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("PPS", *pps_start_img, Gtk::Toolbar_Helpers::Callback0()));
   pps_start = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*kunden_start);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Kunden", *kunden_start_img, Gtk::Toolbar_Helpers::Callback0()));
   kunden_start = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*lager_start);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Lager", *lager_start_img, Gtk::Toolbar_Helpers::Callback0()));
   lager_start = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*artikel_start);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Artikel", *artikel_start_img, Gtk::Toolbar_Helpers::Callback0()));
   artikel_start = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*preise_start);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Preislisten", *preise_start_img, Gtk::Toolbar_Helpers::Callback0()));
   preise_start = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*fibu_start);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("FiBu", *fibu_start_img, Gtk::Toolbar_Helpers::Callback0()));
   fibu_start = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*vertrieb_start);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Vertrieb", *vertrieb_start_img, Gtk::Toolbar_Helpers::Callback0()));
   vertrieb_start = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*ooo);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("OpenOffice", *ooo_img, Gtk::Toolbar_Helpers::Callback0()));
   ooo = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*preview);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("(vorab)", *preview_img, Gtk::Toolbar_Helpers::Callback0()));
   preview = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*separatortoolitem1);
#else //
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::Space());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->append(*quit);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Beenden", *quit_img, Gtk::Toolbar_Helpers::Callback0()));
   quit = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   pps_start->set_visible_horizontal(true);
   pps_start->set_visible_vertical(true);
   pps_start->set_is_important(false);
   pps_start->set_homogeneous(true);
   pps_start->set_expand(false);
#endif //
   pps_start_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   kunden_start->set_visible_horizontal(true);
   kunden_start->set_visible_vertical(true);
   kunden_start->set_is_important(false);
   kunden_start->set_homogeneous(true);
   kunden_start->set_expand(false);
#endif //
   kunden_start_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   lager_start->set_visible_horizontal(true);
   lager_start->set_visible_vertical(true);
   lager_start->set_is_important(false);
   lager_start->set_homogeneous(true);
   lager_start->set_expand(false);
#endif //
   lager_start_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   artikel_start->set_visible_horizontal(true);
   artikel_start->set_visible_vertical(true);
   artikel_start->set_is_important(false);
   artikel_start->set_homogeneous(true);
   artikel_start->set_expand(false);
#endif //
   artikel_start_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   preise_start->set_visible_horizontal(true);
   preise_start->set_visible_vertical(true);
   preise_start->set_is_important(false);
   preise_start->set_homogeneous(true);
   preise_start->set_expand(false);
#endif //
   preise_start_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   fibu_start->set_sensitive(false);
   fibu_start->set_visible_horizontal(true);
   fibu_start->set_visible_vertical(true);
   fibu_start->set_is_important(false);
   fibu_start->set_homogeneous(true);
   fibu_start->set_expand(false);
#endif //
   fibu_start_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   vertrieb_start->set_sensitive(false);
   vertrieb_start->set_visible_horizontal(true);
   vertrieb_start->set_visible_vertical(true);
   vertrieb_start->set_is_important(false);
   vertrieb_start->set_homogeneous(true);
   vertrieb_start->set_expand(false);
#endif //
   vertrieb_start_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   ooo->set_sensitive(false);
   ooo->set_visible_horizontal(true);
   ooo->set_visible_vertical(true);
   ooo->set_is_important(false);
   ooo->set_homogeneous(true);
   ooo->set_expand(false);
#endif //
   ooo_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   preview->set_visible_horizontal(true);
   preview->set_visible_vertical(true);
   preview->set_is_important(false);
   preview->set_homogeneous(true);
   preview->set_expand(false);
#endif //
   preview_img->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   separatortoolitem1->set_visible_horizontal(true);
   separatortoolitem1->set_visible_vertical(true);
   separatortoolitem1->set_homogeneous(false);
   separatortoolitem1->set_expand(false);
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   quit->set_visible_horizontal(true);
   quit->set_visible_vertical(true);
   quit->set_is_important(false);
   quit->set_homogeneous(true);
   quit->set_expand(false);
#endif //
   quit_img->show();
   toolbar1->set_tooltips(true);
   toolbar1->set_toolbar_style(Gtk::TOOLBAR_BOTH);
   toolbar1->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   toolbar1->set_show_arrow(false);
#endif //
   vbox1->pack_start(*toolbar1, Gtk::PACK_SHRINK, 0);
   ManuProc_Starter->set_title("ManuProC_Starter");
   ManuProc_Starter->set_modal(false);
   ManuProc_Starter->property_window_position().set_value(Gtk::WIN_POS_NONE);
   ManuProc_Starter->set_resizable(false);
   ManuProc_Starter->property_destroy_with_parent().set_value(false);
   ManuProc_Starter->add(*vbox1);
   pps_start->show();
   kunden_start->show();
   lager_start->show();
   artikel_start->show();
   preise_start->show();
   fibu_start->show();
   vertrieb_start->show();
   ooo->show();
   preview->show();
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   separatortoolitem1->show();
#endif //
   quit->show();
   toolbar1->show();
   vbox1->show();
   ManuProc_Starter->show();
   pps_start->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_pps_start_clicked), false);
   kunden_start->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_kunden_start_enter), false);
   lager_start->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_lager_start_clicked), false);
   artikel_start->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_artikel_start_clicked), false);
   preise_start->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_preise_start_clicked), false);
   fibu_start->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_fibu_start_clicked), false);
   vertrieb_start->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_vertrieb_start_clicked), false);
   ooo->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_ooo_clicked), false);
   preview->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_preview_clicked), false);
   quit->signal_clicked().connect(SigC::slot(*this, &ManuProc_Starter_glade::on_quit_clicked), false);
}

ManuProc_Starter_glade::~ManuProc_Starter_glade()
{  delete gmm_data;
}
