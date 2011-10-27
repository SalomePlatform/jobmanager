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

bin_PROGRAMS = jobmanager_gui

bin_SCRIPTS = start_jobmanager.sh

jobmanager_gui_SOURCES = main.cxx 

jobmanager_gui_CXXFLAGS = $(qt4_cppflags) \
			      -I$(top_srcdir)/src/bases \
			      -I$(top_srcdir)/src/engine \
			      -I$(KERNEL_ROOT_DIR)/include/salome \
                               $(OMNIORB_INCLUDES) $(OMNIORB_CXXFLAGS) \
			       -I../../idl \
			      -I$(top_srcdir)/src/wrappers \
			      -I$(top_srcdir)/src/genericgui

jobmanager_gui_LDFLAGS = $(qt4_ldflags)

jobmanager_gui_LDADD = -lQtGui -lQtCore \
			   $(top_builddir)/src/genericgui/libBL_GenericGui.la \
			   $(top_builddir)/src/engine/libBL_Engine.la \
			   $(top_builddir)/src/wrappers/libBL_Wrappers_Qt.la \
			   @OMNIORB_LIBS@ \
			   $(top_builddir)/idl/libJOBMANAGER_IDL.la \
			   -L$(KERNEL_ROOT_DIR)/lib/salome -lSalomeLifeCycleCORBA -lSalomeIDLKernel -lSalomeNS

EXTRA_DIST = start_jobmanager.sh.in
