# $Id: Makefile.am,v 1.3 2002/04/30 09:49:06 christof Exp $

CLEANFILES= *~ 

includedir=@includedir@/DynamicEnums

include_HEADERS=DynamicEnums.h

DynamicEnums.h: DynEnums_default.h
	if [ -x ../Programme/createDynEnums ] ; \
	then ../Programme/createDynEnums >DynamicEnums.h ; \
	else cp DynEnums_default.h DynamicEnums.h ; \
	fi
	
recreate:
	rm DynamicEnums.h
	make DynamicEnums.h

CLEANFILES= *~ DynamicEnums.h
