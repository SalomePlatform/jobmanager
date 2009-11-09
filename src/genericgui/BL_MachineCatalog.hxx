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
//

#ifndef _BL_MACHINECATALOG_HXX_
#define _BL_MACHINECATALOG_HXX_

#include <QtGui>

#include "BL_SALOMEServices.hxx"

#include <string>
#include <list>

namespace BL 
{
  class MachineCatalog: public QWidget
  {
    Q_OBJECT

    public:
      MachineCatalog(QWidget *parent, BL::SALOMEServices * salome_services);
      virtual ~MachineCatalog();

    public slots:
      void refresh_machine_list();

    protected:
      QWidget* _parent;
      BL::SALOMEServices * _salome_services;

      QPushButton * _refresh_button;
      QListWidget * _machine_files_list;
  };
}

#endif


