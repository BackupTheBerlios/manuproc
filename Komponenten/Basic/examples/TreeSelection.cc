#include <iostream>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <SimpleTree.hh>
#include <treebase_data.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

class window1 : public Gtk::Window
{  
	SimpleTree st;
	Gtk::VBox vb;
	Gtk::Button e;
        void leaf_sel(cH_RowDataBase r);
public:
        window1();
};

int main(int argc, char **argv)
{  Gtk::Main m(&argc, &argv);
   window1 *window1 = new class window1();
   m.run(*window1);
   delete window1;
   return 0;
}

window1::window1() : st(1)
{  vb.pack_start(e);
   vb.pack_start(st);
   add(vb);
   e.show();
   st.show();
   vb.show();
   show();

  std::vector <std::string> v(st.Cols());
   v[0]="Test";
   st.setTitles(v);
   std::vector <cH_RowDataBase> datavec;
   datavec.push_back(new RowDataStrings("X"));
   datavec.push_back(new RowDataStrings("Y"));
   datavec.push_back(new RowDataStrings("Z"));
   st.setDataVec(datavec);
   
   st.signal_leaf_selected().connect(SigC::slot(*this,&window1::leaf_sel));
}

void window1::leaf_sel(cH_RowDataBase r)
{  Handle<const RowDataStrings> hrds=r.cast_dynamic<const RowDataStrings>();
   std::cout << (*hrds)[0] << " selected\n";
}
