# generated 2001/2/22 9:59:33 CET by thoma@ig23.
# using glademm V0.5_11f_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new

bin_PROGRAMS = kundendaten

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

BUILT_SOURCES = optionmenu_bank_bei_sql.cc windowTop_sql.cc

kundendaten_SOURCES = \
	windowTop.cc \
   windowTop_con_act_foc.cc \
   $(BUILT_SOURCES) \
	kundendaten.cc\
   window_Dialog_Bestaetigen.cc \
   window_Dialog_Bestaetigen_glade.cc \
   window_neue_bank_anlegen.cc \
   window_neue_bank_anlegen_glade.cc \
	windowTop_glade.cc \
   window_neue_Kundennummer.cc \
   window_neue_Kundennummer_glade.cc \
	dialogError_glade.cc \
	dialogError.cc \
   windowTop_Details.cc \
   windowTop_Kontaktperson.cc \
   windowTop_kunde.cc \
   windowTop_Gruppen.cc
#   windowTop_Person.cc 


noinst_HEADERS = \
	windowTop_glade.hh \
	windowTop.hh \
   window_Dialog_Bestaetigen.hh \
   window_Dialog_Bestaetigen_glade.hh \
   window_neue_bank_anlegen.hh \
   window_neue_bank_anlegen_glade.hh \
   window_neue_Kundennummer.hh \
   window_neue_Kundennummer_glade.hh \
	dialogError_glade.hh \
	dialogError.hh


AM_CXXFLAGS = @KOMPONENTEN2_INCLUDES@ 

AM_LDFLAGS= @KOMPONENTEN2_LDFLAGS@ 

kundendaten_LDADD = @LIBS@ @KOMPONENTEN2_LIBS@
