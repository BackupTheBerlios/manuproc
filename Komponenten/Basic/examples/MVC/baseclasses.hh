// generated 2002/7/7 22:56:07 CEST by christof@puck.petig-baender.de
// using glademm V1.1.0d_cvs
//

#if !defined(_MVCBASECLASSES_HH)
#define _MVCBASECLASSES_HH 

// these objects need to get initialized BEFORE window1_glade is created
// glade-- --baseclass takes care of this
class window1_base
{protected:
        MVC<int> myint;
        MVC<std::string> mystring;
public: 
	window1_base() : myint(0) {}
};
#endif //
