salomeidldir       = $(prefix)/idl
salomeincludedir   = $(includedir)/salome
libdir             = $(prefix)/lib/salome

BUILT_SOURCES = JOBMANAGER_IDLSK.cc
IDL_FILES     = JOBMANAGER_IDL.idl

lib_LTLIBRARIES = libJOBMANAGER_IDL.la
salomeidl_DATA = $(IDL_FILES)

nodist_libJOBMANAGER_IDL_la_SOURCES = JOBMANAGER_IDLSK.cc
nodist_salomeinclude_HEADERS    = JOBMANAGER_IDL.hh

libJOBMANAGER_IDL_la_CXXFLAGS     = -I. @KERNEL_CXXFLAGS@ @OMNIORB_CXXFLAGS@ @OMNIORB_INCLUDES@
libJOBMANAGER_IDL_la_LIBADD       = $(KERNEL_LIBS)

%SK.cc %.hh : %.idl
	$(OMNIORB_IDL) -bcxx @OMNIORB_IDLCXXFLAGS@ -I$(KERNEL_ROOT_DIR)/idl/salome $<

CLEANFILES = *.hh *SK.cc *.py *.hxx *.cxx

EXTRA_DIST = $(IDL_FILES)
