// generated 2001/5/11 9:10:29 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to auftrag.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gnome--/main.h>

#include "termsplidial.hh"
#include "auftragliste.hh"
#include "auftrag_main.hh"
#include "auftrag_bearbeiten.hh"
#include "auftrag_lieferschein.hh"
#include "auftrag_rechnung.hh"

int main(int argc, char **argv)
{   
   
   Gnome::Main m(PACKAGE, VERSION, argc, argv);
manage(new class termsplidial());
manage(new class auftragliste());
manage(new class auftrag_main());
manage(new class auftrag_bearbeiten());
manage(new class auftrag_lieferschein());
manage(new class auftrag_rechnung());
   m.run();
   return 0;
}
