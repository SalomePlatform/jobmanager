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

#include "BL_JobTab.hxx"
#include "BL_Traces.hxx"

BL::JobTab::JobTab(QWidget *parent, BL::JobsManager_QT * jobs_manager) : QTabWidget(parent)
{
  DEBTRACE("Creating BL::JobTab");
  BL_ASSERT(parent);
  BL_ASSERT(jobs_manager);
  _parent = parent;
  _jobs_manager = jobs_manager;
  _model = NULL;

  createJobSummaryTab();
  createJobFilesTab();

  addTab(_summary_tab, "Job Summary");
  addTab(_files_tab, "Job Files");
}

BL::JobTab::~JobTab()
{
  DEBTRACE("Destroying BL::JobTab");
}

void
BL::JobTab::setModel(QStandardItemModel * model)
{
  DEBTRACE("Call setModel BL::JobTab");
  BL_ASSERT(model);

  _model = model;
}

void
BL::JobTab::createJobSummaryTab()
{
  _summary_tab = new QWidget(this);

  QLabel * job_name_label = new QLabel("Name:");
  _job_name_label_value = new QLabel("");
  QLabel * job_type_label = new QLabel("Type:");
  _job_type_label_value = new QLabel("");
  QLabel * job_state_label = new QLabel("State:");
  _job_state_label_value = new QLabel("");
  QLabel * job_machine_label = new QLabel("Machine:");
  _job_machine_label_value = new QLabel("");
  _job_command_label = new QLabel("Schema or Command:");
  _job_command_label_value = new QLabel("");

  QGroupBox * main_values_box = new QGroupBox("Main values");
  QFormLayout * values_form = new QFormLayout;
  values_form->insertRow(0, job_name_label, _job_name_label_value);
  values_form->insertRow(1, job_type_label, _job_type_label_value);
  values_form->insertRow(2, job_state_label, _job_state_label_value);
  values_form->insertRow(3, job_machine_label, _job_machine_label_value);
  values_form->insertRow(4, _job_command_label, _job_command_label_value);
  main_values_box->setLayout(values_form);

  QLabel * job_nif_label = new QLabel("Number of Input Files:");
  _job_nif_label_value = new QLabel("");
  QLabel * job_nof_label = new QLabel("Number of Output Files:");
  _job_nof_label_value = new QLabel("");
  QLabel * job_bd_label = new QLabel("Execution directory:");
  _job_bd_label_value = new QLabel("");
  QLabel * job_rd_label = new QLabel("Result directory:");
  _job_rd_label_value = new QLabel("");

  QLabel * job_mdt_label = new QLabel("Maximum duration:");
  _job_mdt_label_value = new QLabel("");
  QLabel * job_em_label = new QLabel("Expected memory:");
  _job_em_label_value = new QLabel("");
  QLabel * job_nop_label = new QLabel("Number of processors:");
  _job_nop_label_value = new QLabel("");

  QGroupBox * run_values_box = new QGroupBox("Run values");
  QFormLayout * run_values_form = new QFormLayout;
  run_values_form->insertRow(0, job_nif_label, _job_nif_label_value);
  run_values_form->insertRow(1, job_nof_label, _job_nof_label_value);
  run_values_form->insertRow(2, job_bd_label, _job_bd_label_value);
  run_values_form->insertRow(3, job_rd_label, _job_rd_label_value);
  QFormLayout * other_run_values_form = new QFormLayout;
  other_run_values_form->insertRow(0, job_mdt_label, _job_mdt_label_value);
  other_run_values_form->insertRow(1, job_em_label, _job_em_label_value);
  other_run_values_form->insertRow(2, job_nop_label, _job_nop_label_value);
  QHBoxLayout * box_layout = new QHBoxLayout();
  box_layout->addLayout(run_values_form);
  box_layout->addLayout(other_run_values_form);
  run_values_box->setLayout(box_layout);

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->addWidget(main_values_box);
  mainLayout->addWidget(run_values_box);
  _summary_tab->setLayout(mainLayout);
}

void
BL::JobTab::createJobFilesTab()
{
  _files_tab = new QWidget(this);
  
  _input_files_list = new QListWidget(this);
  _input_files_list->setSelectionMode(QAbstractItemView::NoSelection);
  QGroupBox * input_files_box = new QGroupBox("Input Files");
  QVBoxLayout * input_layout = new QVBoxLayout();
  input_layout->addWidget(_input_files_list);
  input_files_box->setLayout(input_layout);

  _output_files_list = new QListWidget(this);
  _output_files_list->setSelectionMode(QAbstractItemView::NoSelection);
  QGroupBox * output_files_box = new QGroupBox("Output Files");
  QVBoxLayout * output_layout = new QVBoxLayout();
  output_layout->addWidget(_output_files_list);
  output_files_box->setLayout(output_layout);

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->addWidget(input_files_box);
  mainLayout->addWidget(output_files_box);
  _files_tab->setLayout(mainLayout);
}

void
BL::JobTab::job_selected(const QModelIndex & index)
{
  DEBTRACE("BL::JobTab::job_selected slot");
  QStandardItem * item = _model->itemFromIndex(index);
  QStandardItem * item_name = _model->item(item->row());
  if (item)
  {
    BL::Job * job = _jobs_manager->getJob(item_name->text().toStdString());

    _job_name_label_value->setText(QString(job->getName().c_str()));

    if (job->getState() == BL::Job::CREATED)
      _job_state_label_value->setText("Created");
    else if (job->getState() == BL::Job::IN_PROCESS)
      _job_state_label_value->setText("In Process");
    else if (job->getState() == BL::Job::QUEUED)
      _job_state_label_value->setText("Queued");
    else if (job->getState() == BL::Job::RUNNING)
      _job_state_label_value->setText("Running");
    else if (job->getState() == BL::Job::PAUSED)
      _job_state_label_value->setText("Paused");
    else if (job->getState() == BL::Job::ERROR)
      _job_state_label_value->setText("Error");
    else 
      _job_state_label_value->setText("Finished");

    if (job->getType() == BL::Job::YACS_SCHEMA)
    {
      _job_command_label->setText("Schema:");
      _job_command_label_value->setText(QString(job->getYACSFile().c_str()));
      _job_type_label_value->setText("YACS_Schema");
    }
    else
    {
      _job_command_label->setText("Command:");
      _job_command_label_value->setText(QString(job->getCommand().c_str()));
      _job_type_label_value->setText("Command");
    }

    _job_machine_label_value->setText(QString(job->getMachine().c_str()));

    BL::Job::BatchParam batch_params = job->getBatchParameters();

    BL::Job::FilesParam files_params = job->getFilesParameters();
    int nif = files_params.input_files_list.size();
    _job_nif_label_value->setText(QVariant(nif).toString());
    int nof = files_params.output_files_list.size();
    _job_nof_label_value->setText(QVariant(nof).toString());
    _job_bd_label_value->setText(QString(batch_params.batch_directory.c_str()));
    _job_rd_label_value->setText(QString(files_params.result_directory.c_str()));

    _job_mdt_label_value->setText(QString(batch_params.maximum_duration.c_str()));
    _job_em_label_value->setText(QString(batch_params.expected_memory.c_str()));
    _job_nop_label_value->setText(QVariant(batch_params.nb_proc).toString());

    std::list<std::string>::iterator it;
    for (it = files_params.input_files_list.begin(); it != files_params.input_files_list.end(); it++)
    {
      std::string file = *it;
      _input_files_list->addItem(QString(file.c_str()));
    }
    for (it = files_params.output_files_list.begin(); it != files_params.output_files_list.end(); it++)
    {
      std::string file = *it;
      _output_files_list->addItem(QString(file.c_str()));
    }

  }
  else
    DEBTRACE ("itemFromIndex returns 0 !");
}

void 
BL::JobTab::itemChanged(QStandardItem * item)
{
  DEBTRACE("BL::JobTab::itemChanged slot");

  QStandardItem * item_name = _model->item(item->row());
  BL::Job * job = _jobs_manager->getJob(item_name->text().toStdString());

  if (_job_name_label_value->text() == QString(job->getName().c_str()))
  {
    if (job->getState() == BL::Job::CREATED)
      _job_state_label_value->setText("Created");
    else if (job->getState() == BL::Job::IN_PROCESS)
      _job_state_label_value->setText("In Process");
    else if (job->getState() == BL::Job::QUEUED)
      _job_state_label_value->setText("Queued");
    else if (job->getState() == BL::Job::RUNNING)
      _job_state_label_value->setText("Running");
    else if (job->getState() == BL::Job::PAUSED)
      _job_state_label_value->setText("Paused");
    else if (job->getState() == BL::Job::ERROR)
      _job_state_label_value->setText("Error");
    else 
      _job_state_label_value->setText("Finished");
  }
}

void
BL::JobTab::reset(QString job_name)
{
  _job_name_label_value->setText("");
  _job_type_label_value->setText("");
  _job_state_label_value->setText("");
  _job_machine_label_value->setText("");
  _job_nif_label_value->setText("");
  _job_nof_label_value->setText("");
  _job_bd_label_value->setText("");
  _job_rd_label_value->setText("");
  _job_mdt_label_value->setText("");
  _job_em_label_value->setText("");
  _job_nop_label_value->setText("");
  _job_command_label->setText("Schema or Command:");
  _job_command_label_value->setText("");

  _input_files_list->clear();
  _output_files_list->clear();
}
