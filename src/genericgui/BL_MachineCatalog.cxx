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

#include "BL_MachineCatalog.hxx"
#include "BL_Traces.hxx"

BL::MachineCatalog::MachineCatalog(QWidget *parent, BL::SALOMEServices * salome_services) : QWidget(parent)
{
  DEBTRACE("Creating BL::MachineCatalog");
  BL_ASSERT(parent);
  BL_ASSERT(salome_services);
  _parent = parent;
  _salome_services = salome_services;
  
  _refresh_button = new QPushButton("Refresh Machine List");
  _refresh_button->show();
  connect(_refresh_button, SIGNAL(clicked()), this, SLOT(refresh_machine_list()));
  _machine_files_list = new QListWidget(this);
  _machine_files_list->setSelectionMode(QAbstractItemView::NoSelection);
  std::list<std::string> machine_list = _salome_services->getMachineList();
  std::list<std::string>::iterator it;
  for (it = machine_list.begin(); it != machine_list.end(); it++)
  {
    std::string machine = *it;
    _machine_files_list->addItem(QString(machine.c_str()));
  }

  QVBoxLayout * mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(_refresh_button);
  mainLayout->addWidget(_machine_files_list);
  setLayout(mainLayout);
}

BL::MachineCatalog::~MachineCatalog()
{
  DEBTRACE("Destroying BL::MachineCatalog");
}

void
BL::MachineCatalog::refresh_machine_list()
{
  _machine_files_list->clear();
  std::list<std::string> machine_list = _salome_services->getMachineList();
  std::list<std::string>::iterator it;
  for (it = machine_list.begin(); it != machine_list.end(); it++)
  {
    std::string machine = *it;
    _machine_files_list->addItem(QString(machine.c_str()));
  }
}
