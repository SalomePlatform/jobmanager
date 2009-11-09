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

#include "BL_MainWindows_SALOME.hxx"

BL::MainWindows_SALOME::MainWindows_SALOME(const QString & module_name) :
  SalomeApp_Module(module_name),
  LightApp_Module(module_name)
{
  DEBTRACE("Creating BL::MainWindows_SALOME");
  _actionId = 190;
}

void 
BL::MainWindows_SALOME::initialize(SalomeApp_Application * appli)
{
  DEBTRACE("Initialize BL::MainWindows_SALOME");
  BL_ASSERT(appli);

  _appli = appli;
  _svm = new SUIT_ViewManager(_appli->activeStudy(), _appli->desktop(), new SUIT_ViewModel());
  _appli->addViewManager(_svm);
  _viewWin = _svm->createViewWindow();

  if (_viewWin && _appli->desktop())
    _viewWin->resize((int)(_appli->desktop()->width() * 0.6), (int)(_appli->desktop()->height() * 0.6 ));
}

QString  
BL::MainWindows_SALOME::engineIOR() const
{
  DEBTRACE("Entering in BL::SalomeGui::engineIOR");
  return _appli->defaultEngineIOR();
}

void 
BL::MainWindows_SALOME::viewManagers(QStringList& list) const
{
  DEBTRACE("Entering in BL::SalomeGui::viewManagers");
  list.append(_svm->getType());
}


BL::MainWindows_SALOME::~MainWindows_SALOME()
{
  DEBTRACE("Destroying BL::MainWindows_SALOME");
}

QMainWindow * 
BL::MainWindows_SALOME::getDockParent()
{
  return _appli->desktop();
}

QMainWindow * 
BL::MainWindows_SALOME::getTabParent()
{
  return _viewWin;
}

QAction* 
BL::MainWindows_SALOME::createAction(const QString& toolTip,
				     const QIcon& icon,
				     const QString& menu,
				     const QString& status,
				     const int shortCut,
				     QObject* parent,
				     bool checkable,
				     QObject* receiver,
				     const char* member)
{
  return SalomeApp_Module::createAction(getActionId(), toolTip, icon, menu, status, shortCut, parent, checkable, receiver, member);
}

int
BL::MainWindows_SALOME::createTopMenu(const QString & menu_name)
{
  return createMenu(menu_name, -1);
}

void 
BL::MainWindows_SALOME::addActionToMenu(QAction * action, int menu_id)
{
  createMenu(action, menu_id);
}
