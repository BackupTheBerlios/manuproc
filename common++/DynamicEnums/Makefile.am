# $Id: Makefile.am,v 1.13 2005/06/13 09:57:50 christof Exp $

includedir=@includedir@/DynamicEnums

include_HEADERS=DynamicEnums.h DynamicDescr.h DynamicConfig.h

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

DynamicConfig.h:
	if [ -x ../Programme/createDynConfig ] ; \
	then ../Programme/createDynConfig >DynamicConfig.h ; \
	else touch DynamicConfig.h ; \
	fi
	
recreate:
	-rm DynamicEnums.h DynamicDescr.h DynamicConfig.h
	make DynamicEnums.h
	make DynamicDescr.h
	make DynamicConfig.h

CLEANFILES= *~

MAINTAINERCLEANFILES = DynamicEnums.h DynamicDescr.h DynamicConfig.h
