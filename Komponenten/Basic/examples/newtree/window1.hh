#ifndef _WINDOW1_HH
#  include "window1_glade.hh"
#  define _WINDOW1_HH
#include <SimpleTree.hh>

class window1 : public window1_glade
{	SimpleTree st;
	virtual void add_one();
	virtual void eine_weg();
	virtual void refresh();
	virtual void change1();
	virtual void change2();

public:
	window1();
};
#endif
