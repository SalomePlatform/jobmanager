// Copyright (C) 2009-2019  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//


#ifndef _BL_MAINWINDOWS_WRAP_HXX_
#define _BL_MAINWINDOWS_WRAP_HXX_

#include "BL_Traces.hxx"

#include <QString>

class QAction;
class QIcon;
class QMainWindow;
class QObject;

namespace BL
{
  class MainWindows_Wrap
  {
    
    public:
      MainWindows_Wrap() {}
      virtual ~MainWindows_Wrap() {}

      virtual QMainWindow * getDockParent() = 0;
      virtual QMainWindow * getTabParent()  = 0;

      virtual QAction* createAction(const QString& toolTip,
				    const QIcon& icon,
				    const QString& menu,
				    const QString& status,
				    const int accel,
				    QObject* parent =0,
				    bool checkable = false,
				    QObject* receiver =0,
				    const char* member =0,
                                    const QString& shortCut=QString()) = 0;

      virtual int createTopMenu(const QString & menu_name) = 0;
      virtual void addActionToMenu(QAction * action, int menu_id) = 0;

  };
}

#endif
