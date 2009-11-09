//  Copyright (C) 2009 CEA/DEN, EDF R&D
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

#include "BL_SalomeGui.hxx"

BL::SalomeGui::SalomeGui() : MainWindows_SALOME("JobManager"), 
			     SalomeApp_Module("JobManager"), 
			     LightApp_Module("JobManager")
{
  DEBTRACE("Creating BL::SalomeGui");
  _gengui = NULL;
}

BL::SalomeGui::~SalomeGui()
{
  DEBTRACE("Destroying BL::SalomeGui");
}

void 
BL::SalomeGui::initialize(CAM_Application* app)
{
   DEBTRACE("Entering in initialize");
   SalomeApp_Module::initialize(app); // MANDATORY -> Otherwise SISEGV...
   BL::MainWindows_SALOME::initialize(getApp());

   _gengui = new BL::GenericGui(this);
   _gengui->createActions();
   _gengui->createMenus();
   _gengui->updateButtonsStates();
}

bool 
BL::SalomeGui::activateModule(SUIT_Study* theStudy)
{
  DEBTRACE("Entering in BL::SalomeGui::activateModule");

  bool bOk = SalomeApp_Module::activateModule(theStudy);
  setMenuShown(true);

  _gengui->showDockWidgets(true);
  return bOk;
}

void 
BL::SalomeGui::windows(QMap<int, int>& theMap) const
{
  DEBTRACE("Entering in BL::SalomeGui::windows");
  theMap.clear();
  theMap.insert(SalomeApp_Application::WT_ObjectBrowser, Qt::LeftDockWidgetArea);
}

bool 
BL::SalomeGui::deactivateModule(SUIT_Study* theStudy)
{
  DEBTRACE("Entering in BL::SalomeGui::deactivateModule");

  setMenuShown(false);
  _gengui->showDockWidgets(false);
  bool bOk = SalomeApp_Module::deactivateModule(theStudy);
  return bOk;
}

// --- Export the module
extern "C"
{
  CAM_Module* createModule()
  {
    return new BL::SalomeGui();
  }
}
