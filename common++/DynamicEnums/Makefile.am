# $Id: Makefile.am,v 1.14 2005/07/04 15:53:38 christof Exp $

includedir=@includedir@/DynamicEnums

include_HEADERS=$(MAINTAINERCLEANFILES)

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
	-rm $(MAINTAINERCLEANFILES)
	make $(MAINTAINERCLEANFILES)

update:
	for i in $(MAINTAINERCLEANFILES); do \
	  mv $$i $$i.orig ; \
	  make $$i ; \
	  if diff -bBq -I '//.* created .*' $$i $$i.orig ; then mv $$i.orig $$i ; fi ; \
	done

CLEANFILES= *~

MAINTAINERCLEANFILES = DynamicEnums.h DynamicDescr.h DynamicConfig.h
