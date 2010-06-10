//  Copyright (C) 2009-2010  CEA/DEN, EDF R&D
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

#include "BL_GenericGui.hxx"

BL::GenericGui::GenericGui(BL::MainWindows_Wrap * wrapper) : QObject(wrapper->getDockParent())
{
  DEBTRACE("Creating BL::GenericGui");
  BL_ASSERT(wrapper);
  _wrapper = wrapper;
  _row_selected = -1;
  _job_name_selected = "";

  _dock_parent = _wrapper->getDockParent();

  _salome_services = new BL::SALOMEServices();
  if (_salome_services->initNS() == false)
    DEBMSG("WARNING !!!!! SALOME IS NOT REACHABLE !!!!");
}

BL::GenericGui::~GenericGui()
{
  DEBTRACE("Destroying BL::GenericGui");
  delete _salome_services;
}

void 
BL::GenericGui::createDockWidgets()
{
  DEBTRACE("createDockWidgets BL::GenericGui");
  _jobs_manager = new BL::JobsManager_QT(_dock_parent, this, _salome_services);
  _model_manager = new BL::QModelManager(this, _jobs_manager);
  _model = _model_manager->getModel();

  /* Summary */
  _dw_summary = new QDockWidget(_dock_parent);
  _dw_summary->setWindowTitle("Summary");
  _summary = new BL::Summary(_dw_summary, _jobs_manager);
  _summary->setModel(_model);
  QScrollArea * scroll_widget_summary = new QScrollArea(_dw_summary);
  scroll_widget_summary->setWidget(_summary);
  scroll_widget_summary->setWidgetResizable(true);
  _dw_summary->setWidget(scroll_widget_summary);

  /* ResourceCatalog */
  _dw_resource_catalog = new QDockWidget(_dock_parent);
  _dw_resource_catalog->setWindowTitle("Resource Catalog");
  _resource_catalog = new JM::ResourceCatalog(_dw_resource_catalog, _salome_services);
  QScrollArea * scroll_widget_resource = new QScrollArea(_dw_resource_catalog);
  scroll_widget_resource->setWidget(_resource_catalog);
  scroll_widget_resource->setWidgetResizable(true);
  _dw_resource_catalog->setWidget(scroll_widget_resource);

  /* Main Dock Window */
  _dock_parent->addDockWidget(Qt::RightDockWidgetArea, _jobs_manager);
  _dock_parent->addDockWidget(Qt::RightDockWidgetArea, _dw_summary);
  _dock_parent->addDockWidget(Qt::RightDockWidgetArea, _dw_resource_catalog);
  _dock_parent->splitDockWidget(_jobs_manager, _dw_summary, Qt::Vertical);
  _dock_parent->tabifyDockWidget(_dw_summary, _dw_resource_catalog);

  /* Signals and Slots */
  // JobsManager -> Model
  connect(_jobs_manager, SIGNAL(new_job_added(QString)), _model_manager, SLOT(new_job_added(QString)));
  connect(_jobs_manager, SIGNAL(job_state_changed(QString)), _model_manager, SLOT(job_state_changed(QString)));
  connect(_jobs_manager, SIGNAL(job_state_changed(QString)), this, SLOT(job_state_changed(QString)));
  // Model -> summary
  connect(_model, SIGNAL(rowsInserted(QModelIndex, int, int)), _summary, SLOT(rowsInserted(QModelIndex, int, int)));
  connect(_model, SIGNAL(rowsRemoved(QModelIndex, int, int)), _summary, SLOT(rowsRemoved(QModelIndex, int, int)));
  connect(_model, SIGNAL(itemChanged(QStandardItem*)), _summary, SLOT(itemChanged(QStandardItem*)));
}

void 
BL::GenericGui::createCentralWidget()
{
  DEBTRACE("createCentralWidget BL::GenericGui");

  _tab_parent  = _wrapper->getTabParent();
  /* Tab Central Widget */
  /* Buttons */
  _buttons = new BL::Buttons(_tab_parent);
  _buttons->setCreateButtonSlot(this, SLOT(create_job()));
  _buttons->setEditCloneButtonSlot(this, SLOT(edit_clone_job()));
  _buttons->setStartButtonSlot(this, SLOT(start_job()));
  _buttons->setReStartButtonSlot(this, SLOT(restart_job()));
  _buttons->setDeleteButtonSlot(this, SLOT(delete_job()));
  _buttons->setRefreshButtonSlot(this, SLOT(refresh_job()));
  _buttons->setGetResultsButtonSlot(this, SLOT(get_results_job()));

  /* Jobs Table */
  _jobs_table = new BL::JobsTable(_tab_parent);
  _jobs_table->setModel(_model);

  /* Job Tab */
  _job_tab = new BL::JobTab(_tab_parent, _jobs_manager);
  _job_tab->setModel(_model);

  // Central Widget
  QWidget * central_widget = new QWidget(_tab_parent);
  QSplitter * tab_central_widget = new QSplitter(Qt::Vertical, _tab_parent);
  tab_central_widget->addWidget(_jobs_table);
  tab_central_widget->addWidget(_job_tab);
  tab_central_widget->setChildrenCollapsible(false);
  QVBoxLayout * central_layout = new QVBoxLayout;
  central_layout->addWidget(_buttons);
  central_layout->addWidget(tab_central_widget);
  central_widget->setLayout(central_layout);

  // Adding a scroll area for low resolution
  QScrollArea * scroll_central_widget = new QScrollArea(_tab_parent);
  scroll_central_widget->setWidget(central_widget);
  scroll_central_widget->setWidgetResizable(true);
  _tab_parent->setCentralWidget(scroll_central_widget);

  /* Signals and Slots */
  // Model -> JobTab
  connect(_model, SIGNAL(itemChanged(QStandardItem*)), _job_tab, SLOT(itemChanged(QStandardItem*)));
  // TableView -> X
  connect(_jobs_table, SIGNAL(clicked(QModelIndex)), _job_tab, SLOT(job_selected(QModelIndex)));
  connect(_jobs_table, SIGNAL(clicked(QModelIndex)), this, SLOT(job_selected(QModelIndex)));
  connect(_jobs_table, SIGNAL(clicked(QModelIndex)), _model_manager, SLOT(job_selected(QModelIndex)));
  // Delete Job -> X
  connect(this, SIGNAL(job_deleted(QString)), _job_tab, SLOT(reset(QString)));
}

void
BL::GenericGui::deleteDockWidget()
{
  // Specific for SALOME...
  _dock_parent->removeDockWidget(_dw_summary);
  delete _dw_summary;
  _dock_parent->removeDockWidget(_dw_resource_catalog);
  delete _dw_resource_catalog;
  _dock_parent->removeDockWidget(_jobs_manager);
  delete _jobs_manager;
}

void 
BL::GenericGui::showDockWidgets(bool isVisible)
{
  DEBTRACE("BL::GenericGui::showDockWidgets " << isVisible);
  if (_jobs_manager) _jobs_manager->setVisible(isVisible);
  if (_dw_summary) _dw_summary->setVisible(isVisible);
  if (_dw_resource_catalog) _dw_resource_catalog->setVisible(isVisible);
}

void
BL::GenericGui::createActions()
{
  DEBTRACE("CreateActions BL::GenericGui");
  _create_job_action = _wrapper->createAction("Create a Job", QIcon(), "Create a Job", "Create a Job", 0, _dock_parent, false, this, SLOT(create_job()));
  _edit_clone_job_action = _wrapper->createAction("Edit/Clone a Job", QIcon(), "Edit/Clone a Job", "Edit/Clone a Job", 0, _dock_parent, false, this, SLOT(edit_clone_job()));
  _start_job_action = _wrapper->createAction("Start a Job", QIcon(), "Start a Job", "Start a Job", 0, _dock_parent, false, this, SLOT(start_job()));
  _restart_job_action = _wrapper->createAction("Restart a Job", QIcon(), "Restart a Job", "Restart a Job", 0, _dock_parent, false, this, SLOT(restart_job()));
  _delete_job_action = _wrapper->createAction("Delete a Job", QIcon(), "Delete a Job", "Delete a Job", 0, _dock_parent, false, this, SLOT(delete_job()));
  _refresh_job_action = _wrapper->createAction("Refresh Jobs", QIcon(), "Refresh Jobs", "Refresh Jobs", 0, _dock_parent, false, this, SLOT(refresh_job()));
  _get_results_job_action = _wrapper->createAction("Get Job Results", QIcon(), "Get Job Results", "Get Job Results", 0, _dock_parent, false, this, SLOT(refresh_job()));
}

void
BL::GenericGui::createMenus()
{
  DEBTRACE("CreateMenus BL::GenericGui");
  int menu_id = _wrapper->createTopMenu("JobManager");
  _wrapper->addActionToMenu(_create_job_action, menu_id);
  _wrapper->addActionToMenu(_edit_clone_job_action, menu_id);
  _wrapper->addActionToMenu(_start_job_action, menu_id);
  _wrapper->addActionToMenu(_restart_job_action, menu_id);
  _wrapper->addActionToMenu(_delete_job_action, menu_id);
  _wrapper->addActionToMenu(_get_results_job_action, menu_id);
  _wrapper->addActionToMenu(_refresh_job_action, menu_id);
}

void
BL::GenericGui::create_job()
{
  DEBTRACE("Create Job Slot BL::GenericGui");
  _jobs_manager->create_job();
}

void
BL::GenericGui::edit_clone_job()
{
  DEBTRACE("Edit/Clone Job Slot BL::GenericGui");
  _jobs_manager->edit_clone_job(_job_name_selected.toStdString());
}

void
BL::GenericGui::start_job()
{
  DEBTRACE("Start Job Slot BL::GenericGui");
  _jobs_manager->start_job(_job_name_selected.toStdString());
  updateButtonsStates();
}

void
BL::GenericGui::restart_job()
{
  DEBTRACE("Restart Job Slot BL::GenericGui");
  _jobs_manager->restart_job(_job_name_selected.toStdString());
  updateButtonsStates();
}

void
BL::GenericGui::refresh_job()
{
  DEBTRACE("Refresh Job Slot BL::GenericGui");
  _jobs_manager->refresh_jobs();
}

void
BL::GenericGui::get_results_job()
{
  DEBTRACE("Get Results Job Slot BL::GenericGui");
  _jobs_manager->get_results_job(_job_name_selected.toStdString());
}

void
BL::GenericGui::delete_job()
{
  DEBTRACE("Delete Job Slot BL::GenericGui");
  int ret = QMessageBox::warning(NULL, "Delete a job", "Do you really want to delete this job ?",
				 QMessageBox::Ok|QMessageBox::Cancel,
				 QMessageBox::Ok);
  if (ret == QMessageBox::Ok)
  {
    delete_job_internal();
  }
}

void
BL::GenericGui::delete_job_internal()
{
  _jobs_manager->delete_job(_job_name_selected);
  _model_manager->deleteJob(_row_selected);
  emit job_deleted(_job_name_selected);
  _row_selected = -1;
  _job_name_selected = "";
  updateButtonsStates();
}

void
BL::GenericGui::job_selected(const QModelIndex & index)
{
  DEBTRACE("BL::GenericGui::job_selected slot");
  QStandardItem * item = _model->itemFromIndex(index);
  QStandardItem * item_name = _model->item(item->row());
  _row_selected = item->row();
  _job_name_selected = item_name->text();
  updateButtonsStates();
}

void 
BL::GenericGui::job_state_changed(const QString & name)
{
  if (name == _job_name_selected)
    updateButtonsStates();
}
void
BL::GenericGui::updateButtonsStates()
{
  DEBTRACE("BL::GenericGui::updateButtonsStates slot");

  // First case: no job selected
  if (_job_name_selected == "" and _row_selected == -1)
  {
    _start_job_action->setEnabled(false);
    _delete_job_action->setEnabled(false);
    _get_results_job_action->setEnabled(false);
    _buttons->disable_start_button();
    _buttons->disable_delete_button();
    _buttons->disable_get_results_button();
    _edit_clone_job_action->setEnabled(false);
    _buttons->disable_edit_clone_button();
    _restart_job_action->setEnabled(false);
    _buttons->disable_restart_button();
  }
  else if (_job_name_selected != "" and _row_selected != -1)
  {
    BL::Job * job = _jobs_manager->getJob(_job_name_selected.toStdString());
    BL::Job::State job_state = job->getState();

    _edit_clone_job_action->setEnabled(true);
    _buttons->enable_edit_clone_button();
    switch (job_state)
    {
      case BL::Job::CREATED:
	_start_job_action->setEnabled(true);
	_delete_job_action->setEnabled(true);
	_get_results_job_action->setEnabled(false);
	_buttons->enable_start_button();
	_buttons->enable_delete_button();
	_buttons->disable_get_results_button();
	_restart_job_action->setEnabled(false);
	_buttons->disable_restart_button();
	break;

      case BL::Job::IN_PROCESS:
	_start_job_action->setEnabled(false);
	_buttons->disable_start_button();
	_delete_job_action->setEnabled(false);
	_buttons->disable_delete_button();
	_get_results_job_action->setEnabled(false);
	_buttons->disable_get_results_button();
	_restart_job_action->setEnabled(false);
	_buttons->disable_restart_button();
	break;

      case BL::Job::QUEUED:
	_start_job_action->setEnabled(false);
	_buttons->disable_start_button();
	_delete_job_action->setEnabled(true);
	_buttons->enable_delete_button();
	_get_results_job_action->setEnabled(false);
	_buttons->disable_get_results_button();
	_restart_job_action->setEnabled(false);
	_buttons->disable_restart_button();
	break;

      case BL::Job::RUNNING:
	_start_job_action->setEnabled(false);
	_buttons->disable_start_button();
	_delete_job_action->setEnabled(true);
	_buttons->enable_delete_button();
	_get_results_job_action->setEnabled(false);
	_buttons->disable_get_results_button();
	_restart_job_action->setEnabled(false);
	_buttons->disable_restart_button();
	break;

      case BL::Job::PAUSED:
	_start_job_action->setEnabled(false);
	_buttons->disable_start_button();
	_delete_job_action->setEnabled(true);
	_buttons->enable_delete_button();
	_get_results_job_action->setEnabled(false);
	_buttons->disable_get_results_button();
	_restart_job_action->setEnabled(false);
	_buttons->disable_restart_button();
	break;

      case BL::Job::ERROR:
	_start_job_action->setEnabled(false);
	_buttons->disable_start_button();
	_delete_job_action->setEnabled(true);
	_buttons->enable_delete_button();
	_get_results_job_action->setEnabled(false);
	_buttons->disable_get_results_button();
	_restart_job_action->setEnabled(true);
	_buttons->enable_restart_button();
	break;

      case BL::Job::FINISHED:
	_start_job_action->setEnabled(false);
	_buttons->disable_start_button();
	_delete_job_action->setEnabled(true);
	_buttons->enable_delete_button();
	_get_results_job_action->setEnabled(true);
	_buttons->enable_get_results_button();
	_restart_job_action->setEnabled(true);
	_buttons->enable_restart_button();
	break;
    }
  }
  else
  {
    DEBTRACE("ERROR in updateButtonsStates !!!!!!");
  }
}
