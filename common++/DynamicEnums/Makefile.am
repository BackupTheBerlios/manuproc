# $Id: Makefile.am,v 1.12 2004/03/26 13:55:33 jacek Exp $

includedir=@includedir@/DynamicEnums

include_HEADERS=DynamicEnums.h DynamicDescr.h

DynamicEnums.h: DynEnums_default.h
	if [ -x ../Programme/createDynEnums ] ; \
	then ../Programme/createDynEnums >DynamicEnums.h ; \
	else cp DynEnums_default.h DynamicEnums.h ; \
	fi

DynamicDescr.h: DynDescr_default.h
	if [ -x ../Programme/createDynDescr ] ; \
	then ../Programme/createDynDescr >DynamicDescr.h ; \
	else cp DynDescr_default.h DynamicDescr.h ; \
	fi
	
recreate:
	-rm DynamicEnums.h DynamicDescr.h
	make DynamicEnums.h
	make DynamicDescr.h

CLEANFILES= *~

MAINTAINERCLEANFILES = DynamicEnums.h DynamicDescr.h
