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

lib_LTLIBRARIES = libBL_Engine.la

libBL_Engine_la_SOURCES = BL_JobsManager.hxx BL_JobsManager.cxx \
			  BL_Job.hxx BL_Job.cxx \
			  BL_SALOMEServices.hxx BL_SALOMEServices.cxx \
			  BL_Observer.hxx

libBL_Engine_la_CXXFLAGS = -I$(top_srcdir)/src/bases \
			   -I$(KERNEL_ROOT_DIR)/include/salome

libBL_Engine_la_LIBADD = $(top_builddir)/src/bases/libBL_Bases.la

