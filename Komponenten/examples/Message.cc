#include <MyMessage.h>
#include <gtkmm/main.h>
#include <gtkmm/label.h>

int main(int argc, char **argv)
{ Gtk::Main m(argc,argv);
  MyMessage::show_and_wait("test");
  
  Gtk::Window w;
  Gtk::Label l("X");
  l.show();
  w.add(l);
  w.show();
  MyMessage::show_and_wait("test X",l.get_toplevel());
}
