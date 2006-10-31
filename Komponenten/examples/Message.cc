#include <MyMessage.h>
#include <gtkmm/main.h>

int main(int argc, char **argv)
{ Gtk::Main m(argc,argv);
  MyMessage::show_and_wait("test");
}
