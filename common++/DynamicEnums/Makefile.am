# $Id: Makefile.am,v 1.11 2003/07/30 11:15:30 christof Exp $

includedir=@includedir@/DynamicEnums

include_HEADERS=DynamicEnums.h

DynamicEnums.h: DynEnums_default.h
	if [ -x ../Programme/createDynEnums ] ; \
	then ../Programme/createDynEnums >DynamicEnums.h ; \
	else cp DynEnums_default.h DynamicEnums.h ; \
	fi
	
recreate:
	-rm DynamicEnums.h
	make DynamicEnums.h

CLEANFILES= *~

MAINTAINERCLEANFILES = DynamicEnums.h
