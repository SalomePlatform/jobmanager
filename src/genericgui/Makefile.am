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

libdir = $(prefix)/lib/salome

lib_LTLIBRARIES = libBL_GenericGui.la

libBL_GenericGui_la_SOURCES = BL_GenericGui.hxx BL_GenericGui.cxx \
			      BL_JobsTable.hxx BL_JobsTable.cxx \
			      BL_JobTab.hxx BL_JobTab.cxx \
			      BL_Buttons.hxx BL_Buttons.cxx \
			      BL_JobsManager_QT.hxx BL_JobsManager_QT.cxx \
			      BL_QModelManager.hxx BL_QModelManager.cxx \
			      BL_CreateJobWizard.hxx BL_CreateJobWizard.cxx \
			      BL_Summary.hxx BL_Summary.cxx \
			      JM_ResourceCatalog.hxx JM_ResourceCatalog.cxx \
			      JM_SalomeResource.hxx JM_SalomeResource.cxx \
			      JM_EditSalomeResource.hxx JM_EditSalomeResource.cxx

MOC_FILES =                          BL_GenericGui_moc.cxx BL_JobsTable_moc.cxx \
				     BL_JobTab_moc.cxx  \
				     BL_Buttons_moc.cxx BL_JobsManager_QT_moc.cxx \
				     BL_QModelManager_moc.cxx BL_CreateJobWizard_moc.cxx \
				     BL_Summary_moc.cxx JM_ResourceCatalog_moc.cxx \
				     JM_SalomeResource_moc.cxx JM_EditSalomeResource_moc.cxx

nodist_libBL_GenericGui_la_SOURCES = $(MOC_FILES)

libBL_GenericGui_la_CXXFLAGS = $(qt4_cppflags) \
			       -I$(top_srcdir)/src/bases \
			       -I$(top_srcdir)/src/engine \
			       -I$(KERNEL_ROOT_DIR)/include/salome \
			       $(OMNIORB_INCLUDES) \
			       -I../../idl \
			       -I$(top_srcdir)/src/wrappers

libBL_GenericGui_la_LDFLAGS = $(qt4_ldflags) 

libBL_GenericGui_la_LIBADD = -lQtGui -lQtCore \
			     $(top_builddir)/src/bases/libBL_Bases.la \
			     $(top_builddir)/src/engine/libBL_Engine.la

SUFFIXES = .hxx _moc.cxx .qrc _qrc.cxx

.hxx_moc.cxx :
	$(QT_MOC) -p . -o $@ $<

.qrc_qrc.cxx :
	$(QT_RCC) -name $(*F)  $< -o $@

clean-local-qt :
	rm -f *_moc.cxx *_qrc.cxx ui_*.h

clean-local: clean-local-qt

