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

#include "JM_ResourceCatalog.hxx"
#include "BL_Traces.hxx"
#include "JM_SalomeResource.hxx"

JM::ResourceCatalog::ResourceCatalog(QWidget *parent, BL::SALOMEServices * salome_services) : QWidget(parent)
{
  DEBTRACE("Creating JM::ResourceCatalog");
  BL_ASSERT(parent);
  BL_ASSERT(salome_services);
  _parent = parent;
  _salome_services = salome_services;
  
  _refresh_button = new QPushButton("Refresh Resource List");
  _refresh_button->show();
  connect(_refresh_button, SIGNAL(clicked()), this, SLOT(refresh_resource_list()));
  _resource_files_list = new QListWidget(this);
  _resource_files_list->setSelectionMode(QAbstractItemView::SingleSelection);
  connect(_resource_files_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(item_choosed(QListWidgetItem*)));
  std::list<std::string> resource_list = _salome_services->getResourceList();
  std::list<std::string>::iterator it;
  for (it = resource_list.begin(); it != resource_list.end(); it++)
  {
    std::string resource = *it;
    _resource_files_list->addItem(QString(resource.c_str()));
  }

  QVBoxLayout * mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(_refresh_button);
  mainLayout->addWidget(_resource_files_list);
  setLayout(mainLayout);
}

JM::ResourceCatalog::~ResourceCatalog()
{
  DEBTRACE("Destroying JM::ResourceCatalog");
}

void
JM::ResourceCatalog::refresh_resource_list()
{
  _resource_files_list->clear();
  std::list<std::string> resource_list = _salome_services->getResourceList();
  std::list<std::string>::iterator it;
  for (it = resource_list.begin(); it != resource_list.end(); it++)
  {
    std::string resource = *it;
    _resource_files_list->addItem(QString(resource.c_str()));
  }
}

void
JM::ResourceCatalog::item_choosed(QListWidgetItem * item)
{
  DEBTRACE("JM::ResourceCatalog::item_choosed");
  JM::SalomeResource * resource_widget = new JM::SalomeResource(this, 
								_salome_services,
								item->text().toStdString(),
								false);
  resource_widget->exec();
  delete resource_widget;
}
