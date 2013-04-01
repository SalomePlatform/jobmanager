# Copyright (C) 2009-2013  CEA/DEN, EDF R&D
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

OMNIORB_IDLCXXFLAGS = @OMNIORB_IDLCXXFLAGS@ -I$(KERNEL_ROOT_DIR)/idl/salome

%SK.cc %.hh : %.idl
	$(OMNIORB_IDL) -bcxx $(OMNIORB_IDLCXXFLAGS) $<

# E.A. : IDLPYFLAGS is just to compile with cmake (parsed in am2cmake).

IDLPYFLAGS = @IDLPYFLAGS@ -I$(KERNEL_ROOT_DIR)/idl/salome

CLEANFILES = *.hh *SK.cc *.py *.hxx *.cxx

EXTRA_DIST = $(IDL_FILES)
