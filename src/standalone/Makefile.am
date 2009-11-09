bin_PROGRAMS = jobmanager_gui

bin_SCRIPTS = start_jobmanager.sh

jobmanager_gui_SOURCES = main.cxx 

jobmanager_gui_CXXFLAGS = $(qt4_cppflags) \
			      -I$(top_srcdir)/src/bases \
			      -I$(top_srcdir)/src/engine \
			      -I$(KERNEL_ROOT_DIR)/include/salome \
			      -I$(top_srcdir)/src/wrappers \
			      -I$(top_srcdir)/src/genericgui

jobmanager_gui_LDFLAGS = $(qt4_ldflags)

jobmanager_gui_LDADD = -lQtGui -lQtCore \
			   $(top_builddir)/src/genericgui/libBL_GenericGui.la \
			   $(top_builddir)/src/engine/libBL_Engine.la \
			   $(top_builddir)/src/wrappers/libBL_Wrappers_Qt.la \
			   @OMNIORB_LIBS@ \
			   -L$(KERNEL_ROOT_DIR)/lib/salome -lSalomeLifeCycleCORBA -lSalomeIDLKernel -lSalomeNS

EXTRA_DIST = start_jobmanager.sh.in
