# $Id: Makefile.am,v 1.9 2002/09/27 09:14:41 thoma Exp $

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

CLEANFILES= *~ DynamicEnums.h
