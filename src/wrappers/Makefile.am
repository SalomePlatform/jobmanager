#  Copyright (C) 2009 CEA/DEN, EDF R&D
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

libdir = $(prefix)/lib/salome

lib_LTLIBRARIES = libBL_Wrappers_Qt.la 

if HAS_GUI
lib_LTLIBRARIES += libBL_Wrappers_SALOME.la
endif

libBL_Wrappers_Qt_la_SOURCES = BL_MainWindows_Wrap.hxx \
			       BL_MainWindows_Qt.cxx  BL_MainWindows_Qt.hxx

nodist_libBL_Wrappers_Qt_la_SOURCES =

libBL_Wrappers_Qt_la_CXXFLAGS = $(qt4_cppflags) -I$(top_srcdir)/src/bases

libBL_Wrappers_Qt_la_LDFLAGS = $(qt4_ldflags) 

libBL_Wrappers_Qt_la_LIBADD = -lQtGui -lQtCore $(top_builddir)/src/bases/libBL_Bases.la

###########

libBL_Wrappers_SALOME_la_SOURCES = BL_MainWindows_Wrap.hxx \
				   BL_MainWindows_SALOME.cxx  BL_MainWindows_SALOME.hxx

nodist_libBL_Wrappers_SALOME_la_SOURCES =

libBL_Wrappers_SALOME_la_CXXFLAGS = $(qt4_cppflags) -I$(top_srcdir)/src/bases \
				    $(OMNIORB_INCLUDES) \
				    -I$(KERNEL_ROOT_DIR)/include/salome \
				    -I$(GUI_ROOT_DIR)/include/salome

libBL_Wrappers_SALOME_la_LDFLAGS = $(qt4_ldflags) 

libBL_Wrappers_SALOME_la_LIBADD = -lQtGui -lQtCore $(top_builddir)/src/bases/libBL_Bases.la

