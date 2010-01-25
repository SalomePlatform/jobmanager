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

#include "BL_Buttons.hxx"

BL::Buttons::Buttons(QWidget * parent) : QGroupBox(parent)
{
  DEBTRACE("Creating BL::Buttons");

  setTitle("Actions");

  _create_button = new QPushButton("Create a Job", this);
  _edit_clone_button = new QPushButton("Edit/Clone a Job", this);
  _start_button = new QPushButton("Start a Job", this);
  _delete_button = new QPushButton("Delete a Job", this);
  _get_results_button = new QPushButton("Get Job Results", this);
  _refresh_button = new QPushButton("Refresh Jobs", this);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(_create_button);
  mainLayout->addWidget(_edit_clone_button);
  mainLayout->addWidget(_start_button);
  mainLayout->addWidget(_delete_button);
  mainLayout->addWidget(_get_results_button);
  mainLayout->addWidget(_refresh_button);
  mainLayout->setSpacing(0); // Space between buttons

  setLayout(mainLayout);

  //QSizePolicy poli;
  //poli.setControlType(QSizePolicy::PushButton);
  //poli.setVerticalPolicy(QSizePolicy::Fixed);
  //poli.setHorizontalPolicy(QSizePolicy::Fixed);
  //setSizePolicy(poli);
}

BL::Buttons::~Buttons()
{
  DEBTRACE("Destroying BL::Buttons");
}

void
BL::Buttons::setCreateButtonSlot(QObject * receiver, const char * name)
{
  connect(_create_button, SIGNAL(clicked()), receiver, name);
}

void
BL::Buttons::setEditCloneButtonSlot(QObject * receiver, const char * name)
{
  connect(_edit_clone_button, SIGNAL(clicked()), receiver, name);
}

void
BL::Buttons::setStartButtonSlot(QObject * receiver, const char * name)
{
  connect(_start_button, SIGNAL(clicked()), receiver, name);
}

void
BL::Buttons::setDeleteButtonSlot(QObject * receiver, const char * name)
{
  connect(_delete_button, SIGNAL(clicked()), receiver, name);
}

void
BL::Buttons::setRefreshButtonSlot(QObject * receiver, const char * name)
{
  connect(_refresh_button, SIGNAL(clicked()), receiver, name);
}

void 
BL::Buttons::setGetResultsButtonSlot(QObject * receiver, const char * name)
{
  connect(_get_results_button, SIGNAL(clicked()), receiver, name);
}

void 
BL::Buttons::enable_edit_clone_button()
{
  _edit_clone_button->setEnabled(true);
}

void 
BL::Buttons::disable_edit_clone_button()
{
  _edit_clone_button->setEnabled(false);
}

void 
BL::Buttons::enable_start_button()
{
  _start_button->setEnabled(true);
}

void 
BL::Buttons::disable_start_button()
{
  _start_button->setEnabled(false);
}

void 
BL::Buttons::enable_delete_button()
{
  _delete_button->setEnabled(true);
}

void 
BL::Buttons::disable_delete_button()
{
  _delete_button->setEnabled(false);
}

void 
BL::Buttons::enable_get_results_button()
{
  _get_results_button->setEnabled(true);
}

void 
BL::Buttons::disable_get_results_button()
{
  _get_results_button->setEnabled(false);
}
