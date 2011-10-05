# Copyright (C) 2009-2011  CEA/DEN, EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

include $(top_srcdir)/src/bases/make_begin.am

MOC_FILES = BL_SalomeGui_moc.cxx

lib_LTLIBRARIES = libJOBMANAGER.la

dist_libJOBMANAGER_la_SOURCES = BL_SalomeGui.hxx BL_SalomeGui.cxx

nodist_libJOBMANAGER_la_SOURCES = $(MOC_FILES)

libJOBMANAGER_la_CXXFLAGS = $(qt4_cppflags) \
				$(BOOST_CPPFLAGS) \
				$(OMNIORB_INCLUDES) \
			        -I$(top_builddir)/idl \
			    -I. \
				-I$(KERNEL_ROOT_DIR)/include/salome \
				-I$(GUI_ROOT_DIR)/include/salome \
				-I$(top_srcdir)/src/genericgui \
				-I$(top_srcdir)/src/wrappers \
				-I$(top_srcdir)/src/engine \
				-I$(top_srcdir)/src/bases

libJOBMANAGER_la_LDFLAGS  = -L$(GUI_ROOT_DIR)/lib/salome \
				-L$(KERNEL_ROOT_DIR)/lib/salome \
				$(qt4_ldflags)

libJOBMANAGER_la_LIBADD  = $(qt4_libs) \
			   $(top_builddir)/idl/libJOBMANAGER_IDL.la \
			   @OMNIORB_LIBS@ \
			       $(top_builddir)/src/bases/libBL_Bases.la \
			       $(top_builddir)/src/engine/libBL_Engine.la \
			       $(top_builddir)/src/genericgui/libBL_GenericGui.la \
			       $(top_builddir)/src/wrappers/libBL_Wrappers_SALOME.la \
			       -lSalomeApp

salomeinclude_HEADERS = BL_SalomeGui.hxx \
			JOBMANAGER_version.h

# resources files
LIBICONS = jobmanager.png
ICONS = resources/jobmanager.png

nodist_salomeres_SCRIPTS = resources/SalomeApp.xml

dist_salomeres_DATA = ${ICONS}
#ICONS = $(LIBICONS:%=resources/%)

# --------------------------------------------
# *.h --> *_moc.cxx
# --------------------------------------------

SUFFIXES = .hxx _moc.cxx .qrc _qrc.cxx

.hxx_moc.cxx :
	$(QT_MOC) -p . -o $@ $<

.qrc_qrc.cxx :
	$(QT_RCC) -name $(*F)  $< -o $@

#.ui.h :
#	$(QT_UIC) -o $@ $<


clean-local-qt :
	rm -f *_moc.cxx *_qrc.cxx


clean-local: clean-local-qt
	rm -f YACS_msg_en.qm YACS_msg_fr.qm
