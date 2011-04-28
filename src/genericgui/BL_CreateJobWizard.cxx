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

#include "BL_CreateJobWizard.hxx"
#include "BL_JobsManager_QT.hxx"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>

BL::CreateJobWizard::CreateJobWizard(BL::JobsManager_QT * jobs_manager, BL::SALOMEServices * salome_services)
{
  DEBTRACE("Creating BL::CreateJobWizard");
  BL_ASSERT(jobs_manager);
  BL_ASSERT(salome_services);
  _jobs_manager = jobs_manager;
  _salome_services = salome_services;

  job_name = "";
  yacs_file = "";
  command = "";
  python_salome_file = "";
  env_file = "";
  batch_directory = "";
  maximum_duration = "";
  expected_memory = "";
  nb_proc = 1;

  result_directory = "";

  resource_choosed = "";
  batch_queue = "";

  start_job = false;
  dump_yacs_state = -1;
  ll_jobtype = "";

  setOptions(QWizard::IndependentPages | QWizard::NoBackButtonOnStartPage);

  // Common pages
  _job_name_page = new BL::JobNamePage(this, _jobs_manager);

  setPage(Page_JobName, _job_name_page);
  setPage(Page_BatchParameters, new BL::BatchParametersPage(this));
  setPage(Page_Files, new BL::FilesPage(this));
  setPage(Page_Resource, new BL::ResourcePage(this, salome_services));
  setPage(Page_Conclusion, new BL::ConclusionPage(this));

  // Specific pages
  setPage(Page_YACSSchema, new BL::YACSSchemaPage(this));
  setPage(Page_Command_Main_Definitions, new BL::CommandMainPage(this));
  setPage(Page_PythonSalome_Main_Definitions, new BL::PythonSalomeMainPage(this));

  setWindowTitle("Create Job Wizard");
  connect(this, SIGNAL(finished(int)), this, SLOT(end(int)));
}

BL::CreateJobWizard::~CreateJobWizard()
{
  DEBTRACE("Destroying BL::CreateJobWizard");
}

void
BL::CreateJobWizard::clone(const std::string & name)
{
  if (_jobs_manager->job_already_exist(name) == true)
  {
    BL::Job * job = _jobs_manager->getJob(name);

    // We can only edit a job in CREATED, ERROR, FAILED and NOT_CREATED
    if (job->getState() == BL::Job::CREATED ||
        job->getState() == BL::Job::ERROR   ||
        job->getState() == BL::Job::FAILED  ||
        job->getState() == BL::Job::NOT_CREATED)
    { 
      setField("job_name", QString(name.c_str()));
      _job_name_page->_check_name = false;
    }

    if (job->getType() == BL::Job::YACS_SCHEMA)
    {
      setField("yacs_file", QString(job->getJobFile().c_str()));
      _job_name_page->_yacs_schema_button->click();
      setField("env_yacs_file", QString(job->getEnvFile().c_str()));
      if (job->getDumpYACSState() != -1)
      {
        QString value;
        value.setNum(job->getDumpYACSState());
        setField("dump_yacs_state", value);
      }
    }
    else if (job->getType() == BL::Job::COMMAND)
    {
      setField("command", QString(job->getJobFile().c_str()));
      _job_name_page->_command_button->click();
      setField("env_command_file", QString(job->getEnvFile().c_str()));
    }
    else if (job->getType() == BL::Job::PYTHON_SALOME)
    {
      setField("PythonSalome", QString(job->getJobFile().c_str()));
      _job_name_page->_python_salome_button->click();
      setField("env_PythonSalome_file", QString(job->getEnvFile().c_str()));
    }

    BL::Job::BatchParam batch_params = job->getBatchParameters();
    setField("batch_directory", QString(batch_params.batch_directory.c_str()));
    QString proc_value;
    proc_value.setNum(batch_params.nb_proc);
    setField("proc_value", proc_value);

    std::size_t pos = batch_params.maximum_duration.find(":");
    std::string hour_str = batch_params.maximum_duration.substr(0, pos);
    int hour; 
    std::istringstream iss_hour(hour_str);
    iss_hour >> hour;
    setField("duration_hour", hour);

    std::string min_str = batch_params.maximum_duration.substr(pos + 1, batch_params.maximum_duration.npos);
    int min; 
    std::istringstream iss_min(min_str);
    iss_min >> min;
    setField("duration_min", min);

    std::string mem_type = batch_params.expected_memory.substr(batch_params.expected_memory.size() - 2, 2);
    if (mem_type == "mb")
      setField("mem_type", 0);
    else
      setField("mem_type", 1);
    std::string mem_value = batch_params.expected_memory.substr(0, batch_params.expected_memory.find(mem_type));
    int mem_val; 
    std::istringstream iss_mem(mem_value);
    iss_mem >> mem_val;
    setField("mem_value", mem_val);

    BL::Job::FilesParam files_params = job->getFilesParameters();

    std::list<std::string>::iterator it = files_params.input_files_list.begin();
    for (; it != files_params.input_files_list.end(); it++)
      _input_files_list->addItem(QString((*it).c_str()));
    it = files_params.output_files_list.begin();
    for (; it != files_params.output_files_list.end(); it++)
      _output_files_list->addItem(QString((*it).c_str()));

    setField("result_directory", QString(files_params.result_directory.c_str()));
    setField("resource_choosed", QString(job->getResource().c_str()));
    setField("batch_queue", QString(job->getBatchQueue().c_str()));
    setField("ll_jobtype", QString(job->getLoadLevelerJobType().c_str()));
  }
}

void
BL::CreateJobWizard::setFilesList(QListWidget * input_files_list, QListWidget * output_files_list)
{
  _input_files_list = input_files_list;
  _output_files_list = output_files_list;
}

void
BL::CreateJobWizard::end(int result)
{
  DEBTRACE("End of BL::CreateJobWizard");
  if (result)
  {
    // Job Name Panel
    QString f_job_name = field("job_name").toString();
    job_name = f_job_name.toStdString();

    // YACS Schema Panel
    QString f_yacs_file = field("yacs_file").toString();
    yacs_file = f_yacs_file.toStdString();
    dump_yacs_state = field("dump_yacs_state").toInt();

    // Command Panel
    QString f_command = field("command").toString();
    command = f_command.toStdString();

    // Command Panel
    QString f_python_salome_file = field("PythonSalome").toString();
    python_salome_file = f_python_salome_file.toStdString();

    QString f_env_file;
    if (yacs_file != "")
      f_env_file = field("env_yacs_file").toString();
    else if (command != "")
      f_env_file = field("env_command_file").toString();
    else if (python_salome_file != "")
      f_env_file = field("env_PythonSalome_file").toString();
    env_file = f_env_file.toStdString();

    // Batch Panel
    QString f_batch_directory = field("batch_directory").toString();
    batch_directory = f_batch_directory.toStdString();

    QString time_hour;
    QString time_min;
    if (field("duration_hour").toInt() < 10)
      time_hour = "0" + field("duration_hour").toString();
    else
      time_hour = field("duration_hour").toString();
    if (field("duration_min").toInt() < 10)
      time_min = "0" + field("duration_min").toString();
    else
      time_min = field("duration_min").toString();
    maximum_duration = time_hour.toStdString() + ":" + time_min.toStdString();

    QString mem = field("mem_value").toString();
    int mem_type_i = field("mem_type").toInt();
    QString mem_type("gb");
    if (mem_type_i == 0)
      mem_type = "mb";
    expected_memory = mem.toStdString() + mem_type.toStdString();

    nb_proc = field("proc_value").toInt();

    // Files Panel
    QString f_result_directory = field("result_directory").toString();
    result_directory = f_result_directory.toStdString();
    for (int i = 0; i < _input_files_list->count(); ++i)
    {
      QListWidgetItem * item = _input_files_list->item(i);
      QString item_text = item->text();
      input_files_list.push_back(item_text.toStdString());
    }
    for (int i = 0; i < _output_files_list->count(); ++i)
    {
      QListWidgetItem * item = _output_files_list->item(i);
      QString item_text = item->text();
      output_files_list.push_back(item_text.toStdString());
    }

    // Resource list
    QString f_resource_choosed = field("resource_choosed").toString();
    resource_choosed = f_resource_choosed.toStdString(); 

    // Batch Queue
    QString f_batch_queue = field("batch_queue").toString();
    batch_queue = f_batch_queue.toStdString();

    // LoadLeveler JobType
    BL::ResourceDescr resource_descr = _salome_services->getResourceDescr(resource_choosed);
    std::string batch = resource_descr.batch.c_str();
    if (batch == "ll")
    {
      QString f_ll_jobtype = field("ll_jobtype").toString();
      ll_jobtype = f_ll_jobtype.toStdString();
    }
    else
    {
      ll_jobtype = "";
    }

    start_job = field("start_job").toBool();
  }
}

// Job Name Page
BL::JobNamePage::JobNamePage(QWidget * parent, BL::JobsManager_QT * jobs_manager)
: QWizardPage(parent)
{
  _jobs_manager = jobs_manager;
  _check_name = true;
  setTitle("Create a new job");

  QLabel *label = new QLabel("Enter Job name, you cannot add two jobs with the same name");
  label->setWordWrap(true);
  QLabel * nameLabel = new QLabel("Job Name:");
  QLineEdit * _nameLineEdit = new QLineEdit(this);
  registerField("job_name", _nameLineEdit);

  QLabel * label_type = new QLabel("Choose type of batch job:");
  QGroupBox *groupBox = new QGroupBox("Type of job");
  _yacs_schema_button = new QRadioButton(tr("YACS Schema"));
  _yacs_schema_button->setChecked(true);
  _command_button = new QRadioButton(tr("Command"));
  _python_salome_button = new QRadioButton(tr("Python script in SALOME"));
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(_yacs_schema_button);
  vbox->addWidget(_command_button);
  vbox->addWidget(_python_salome_button);
  vbox->addStretch(1);
  groupBox->setLayout(vbox);

  QGroupBox * explanationBox = new QGroupBox("Explanation");
  QVBoxLayout *explanationvbox = new QVBoxLayout;
  _explanation = new QLabel();
  _explanation->setWordWrap(true);
  explanationvbox->addWidget(_explanation);
  explanationvbox->addStretch(1);
  explanationBox->setLayout(explanationvbox);

  // Layouts
  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(nameLabel, 0, 0);
  layout->addWidget(_nameLineEdit, 0, 1);
  main_layout->insertLayout(-1, layout);
  layout->addWidget(label_type, 2, 0);
  layout->addWidget(groupBox, 3, 0, 1, -1);
  layout->addWidget(explanationBox, 4, 0, 1, -1);
  setLayout(main_layout);

  connect(_yacs_schema_button, SIGNAL(clicked(bool)), this, SLOT(yacs_schema_button(bool)));
  connect(_command_button, SIGNAL(clicked(bool)), this, SLOT(command_button(bool)));
  connect(_python_salome_button, SIGNAL(clicked(bool)), this, SLOT(python_salome_button(bool)));

  // Default button
  yacs_schema_button(true);
}

BL::JobNamePage::~JobNamePage()
{}

void
BL::JobNamePage::yacs_schema_button(bool checked)
{
  if (checked)
    _explanation->setText("This job permits to launch a YACS schema into a SALOME application");
}

void
BL::JobNamePage::command_button(bool checked)
{
  if (checked)
    _explanation->setText("This job permits to launch a script into a distributed resource. This script is not launched into a SALOME application");
}

void
BL::JobNamePage::python_salome_button(bool checked)
{
  if (checked)
    _explanation->setText("This job permits to launch a python script into a SALOME application");
}

bool
BL::JobNamePage::validatePage()
{
  DEBTRACE("Calling validatePage of BL::JobNamePage");
  bool return_value;
  QString job_name = field("job_name").toString();

  // Check if job name is empty
  if (job_name == "")
  {
    QMessageBox::warning(NULL, "Job Name Error", "Job name entered is empty, enter a job name or cancel the wizard");
    return_value = false;
  }

  // Check if job name already exists
  else {
    if (_jobs_manager->job_already_exist(job_name.toStdString()) == false or _check_name == false)
    {
      return_value = true;
    }
    else
    {
      QMessageBox::critical(NULL, "Job Name Error", "Job name entered is already used");
      QString value("");
      setField("job_name", value);
      return_value = false;
    }
  }

  return return_value;
}

int
BL::JobNamePage::nextId() const
{
  if (_yacs_schema_button->isChecked())
  {
    return BL::CreateJobWizard::Page_YACSSchema;
  } 
  else if (_command_button->isChecked())
  {
    return BL::CreateJobWizard::Page_Command_Main_Definitions;
  }
  else
  {
    return BL::CreateJobWizard::Page_PythonSalome_Main_Definitions;
  }
}

BL::YACSSchemaPage::YACSSchemaPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Configure YACS Execution");

  QLabel *label = new QLabel("In this step you have to configure your YACS execution");
  label->setWordWrap(true);

  QGroupBox * files_param_box = new QGroupBox("YACS job files");
  QPushButton * yacs_file_button = new QPushButton(tr("Choose YACS Schema file"));
  connect(yacs_file_button, SIGNAL(clicked()), this, SLOT(choose_file()));
  _yacs_file_text = new QLineEdit(this);
  _yacs_file_text->setText("");
  registerField("yacs_file", _yacs_file_text);
  _yacs_file_text->setReadOnly(true);
  QPushButton * command_env_file_button = new QPushButton(tr("Choose an environnement file"));
  connect(command_env_file_button, SIGNAL(clicked()), this, SLOT(choose_env_file()));
  _line_env_file = new QLineEdit(this);
  registerField("env_yacs_file", _line_env_file);
  _line_env_file->setReadOnly(true);
  QGridLayout * files_layout = new QGridLayout;
  files_layout->addWidget(yacs_file_button, 0, 0);
  files_layout->addWidget(_yacs_file_text, 0, 1);
  files_layout->addWidget(command_env_file_button, 1, 0);
  files_layout->addWidget(_line_env_file, 1, 1);
  files_param_box->setLayout(files_layout);

  QGroupBox * spec_param_box = new QGroupBox("YACS specific parameters");
  QLabel * label_dump =  new QLabel("Dump YACS state each secs (0 disable this feature)");
  QLabel * label_dump_warning = new QLabel("(WARNING: can only be used with SALOME >= 6.3.0)");
  QSpinBox * spin_dump = new QSpinBox(this);
  spin_dump->setMinimum(0);
  spin_dump->setMaximum(1000000);
  registerField("dump_yacs_state", spin_dump);
  QGridLayout * specific_layout = new QGridLayout;
  specific_layout->addWidget(label_dump, 0, 0);
  specific_layout->addWidget(spin_dump, 0, 1);
  specific_layout->addWidget(label_dump_warning, 1, 0);
  spec_param_box->setLayout(specific_layout);

  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);
  main_layout->addWidget(files_param_box);
  main_layout->addWidget(spec_param_box);

  setLayout(main_layout);
};

BL::YACSSchemaPage::~YACSSchemaPage()
{}

bool
BL::YACSSchemaPage::validatePage()
{
  bool return_value;
  QString yacs_file = field("yacs_file").toString();
  if (yacs_file != "")
  {
    return_value = true;
  }
  else
  {
    QMessageBox::warning(NULL, "YACS File Error", "Please choose a YACS File");
    return_value = false;
  }
  return return_value;
}

void
BL::YACSSchemaPage::choose_file()
{
  QString yacs_file = QFileDialog::getOpenFileName(this,
						   tr("Open YACS files"), "",
						   tr("XML (*.xml);;All Files (*)"));
  _yacs_file_text->setReadOnly(false);
  _yacs_file_text->setText(yacs_file);
  _yacs_file_text->setReadOnly(true);
}

void
BL::YACSSchemaPage::choose_env_file()
{
  QString env_file = QFileDialog::getOpenFileName(this,
						      tr("Open environnement file"), "",
						      tr("sh (*.sh);;All Files (*)"));
  _line_env_file->setReadOnly(false);
  _line_env_file->setText(env_file);
  _line_env_file->setReadOnly(true);
}


int 
BL::YACSSchemaPage::nextId() const
{
  return BL::CreateJobWizard::Page_BatchParameters;
}

BL::CommandMainPage::CommandMainPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Define command job");
  QLabel *label = new QLabel("Enter the command that will be executed into the resource");
  label->setWordWrap(true);

  // command
  QPushButton * command_file_button = new QPushButton(tr("Choose a command file"));
  connect(command_file_button, SIGNAL(clicked()), this, SLOT(choose_command_file()));
  _line_command = new QLineEdit(this);
  registerField("command", _line_command);
  _line_command->setReadOnly(true);

  QPushButton * command_env_file_button = new QPushButton(tr("Choose an environnement file"));
  connect(command_env_file_button, SIGNAL(clicked()), this, SLOT(choose_env_file()));
  _line_env_file = new QLineEdit(this);
  registerField("env_command_file", _line_env_file);
  _line_env_file->setReadOnly(true);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(command_file_button, 0, 0);
  layout->addWidget(_line_command, 0, 1);
  layout->addWidget(command_env_file_button, 1, 0);
  layout->addWidget(_line_env_file, 1, 1);

  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);
  main_layout->insertLayout(-1, layout);
  setLayout(main_layout);
};

BL::CommandMainPage::~CommandMainPage()
{}

void
BL::CommandMainPage::choose_command_file()
{
  QString command_file = QFileDialog::getOpenFileName(this,
						      tr("Open command file"), "",
						      tr("sh (*.sh);;All Files (*)"));
  _line_command->setReadOnly(false);
  _line_command->setText(command_file);
  _line_command->setReadOnly(true);
}

void
BL::CommandMainPage::choose_env_file()
{
  QString env_file = QFileDialog::getOpenFileName(this,
						      tr("Open environnement file"), "",
						      tr("sh (*.sh);;All Files (*)"));
  _line_env_file->setReadOnly(false);
  _line_env_file->setText(env_file);
  _line_env_file->setReadOnly(true);
}

bool
BL::CommandMainPage::validatePage()
{
  QString command = field("command").toString();
  if (command == "")
  {
    QMessageBox::warning(NULL, "Command Error", "Please enter a command");
    return false;
  }

  return true;
}

int 
BL::CommandMainPage::nextId() const
{
  return BL::CreateJobWizard::Page_BatchParameters;
}

BL::BatchParametersPage::BatchParametersPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Enter Batch Parameters");

  QLabel *label = new QLabel("In this step you define the parameters of your job");
  label->setWordWrap(true);
  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);

  // batch_directory
  QLabel * label_directory = new QLabel("Remote work directory: ");
  QLineEdit * line_directory = new QLineEdit(this);
  registerField("batch_directory", line_directory);

  // exected during time
  QLabel * label_duration = new QLabel("Maximum during time: ");
  QSpinBox * spin_duration_hour = new QSpinBox(this);
  QLabel * label_duration_hour = new QLabel("Hours");
  spin_duration_hour->setMinimum(0);
  spin_duration_hour->setMaximum(1000000);
  registerField("duration_hour", spin_duration_hour);
  QSpinBox * spin_duration_min = new QSpinBox(this);
  QLabel * label_duration_min = new QLabel("Minutes");
  spin_duration_min->setMinimum(0);
  spin_duration_min->setMaximum(59);
  registerField("duration_min", spin_duration_min);

  // memory
  QLabel * label_memory = new QLabel("Memory per nodes expected: ");
  QSpinBox * spin_memory = new QSpinBox(this);
  spin_memory->setMinimum(0);
  spin_memory->setMaximum(1000000);
  registerField("mem_value", spin_memory);
  QComboBox * combo_memory = new QComboBox(this);
  combo_memory->addItem("MB");
  combo_memory->addItem("GB");
  combo_memory->setCurrentIndex(1);
  registerField("mem_type", combo_memory);

  // proc
  QLabel * label_proc = new QLabel("Number of proc expected: ");
  QSpinBox * spin_proc = new QSpinBox(this);
  spin_proc->setMinimum(1);
  spin_proc->setMaximum(1000000);
  registerField("proc_value", spin_proc);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(label_directory, 0, 0);
  layout->addWidget(line_directory, 0, 1, 1, -1);
  layout->addWidget(label_duration, 1, 0);
  layout->addWidget(spin_duration_hour, 1, 1);
  layout->addWidget(label_duration_hour, 1, 2);
  layout->addWidget(spin_duration_min, 1, 3);
  layout->addWidget(label_duration_min, 1, 4);
  layout->addWidget(label_memory, 2, 0);
  layout->addWidget(spin_memory, 2, 1);
  layout->addWidget(combo_memory, 2, 2);
  layout->addWidget(label_proc, 3, 0);
  layout->addWidget(spin_proc, 3, 1);

  main_layout->insertLayout(-1, layout);

  setLayout(main_layout);
};

BL::BatchParametersPage::~BatchParametersPage()
{}

bool
BL::BatchParametersPage::validatePage()
{
  QString batch_directory = field("batch_directory").toString();
  if (batch_directory == "")
  {
    QMessageBox::warning(NULL, "Batch Directory Error", "Please enter a batch directory");
    return false;
  }

  int time_hour = field("duration_hour").toInt();
  int time_min = field("duration_min").toInt();
  if (time_hour == 0 and time_min == 0)
  {
    QMessageBox::warning(NULL, "Time Error", "Please enter an expected during time");
    return false;
  }

  int mem = field("mem_value").toInt();
  if (mem == 0)
  {
    QMessageBox::warning(NULL, "Memory Error", "Please enter an expected memory");
    return false;
  }

  return true;
}

int 
BL::BatchParametersPage::nextId() const
{
  return BL::CreateJobWizard::Page_Files;
}

BL::FilesPage::FilesPage(BL::CreateJobWizard * parent)
: QWizardPage(parent)
{
  setTitle("Enter Input and Output Files");

  QLabel * main_label = new QLabel("In this step you define input and output files of your job");
  main_label->setWordWrap(true);

  // input_files
  QGroupBox * input_group_box = new QGroupBox("Input Files");
  _input_files_button = new QPushButton("Add input files");
  _input_files_button->show();
  connect(_input_files_button, SIGNAL(clicked()), this, SLOT(choose_input_files()));
  _remove_input_files_button = new QPushButton("Remove input files");
  _remove_input_files_button->show();
  _remove_input_files_button->setEnabled(false);
  connect(_remove_input_files_button, SIGNAL(clicked()), this, SLOT(remove_input_files()));
  _input_files_list = new QListWidget();
  _input_files_list->setSelectionMode(QAbstractItemView::MultiSelection);
  connect(_input_files_list, SIGNAL(itemSelectionChanged()), this, SLOT(input_itemSelectionChanged()));

  QGridLayout * input_box = new QGridLayout;
  input_box->addWidget(_input_files_button, 0, 0);
  input_box->addWidget(_remove_input_files_button, 0, 1);
  input_box->addWidget(_input_files_list, 1, 0, 1, -1);
  input_group_box->setLayout(input_box);

  // output_files
  QGroupBox * output_group_box = new QGroupBox("Output Files");
  _output_files_button = new QPushButton("Add output file");
  _output_files_button->show();
  connect(_output_files_button, SIGNAL(clicked()), this, SLOT(add_output_file()));
  _remove_output_files_button = new QPushButton("Remove output files");
  _remove_output_files_button->show();
  _remove_output_files_button->setEnabled(false);
  connect(_remove_output_files_button, SIGNAL(clicked()), this, SLOT(remove_output_files()));
  _output_files_list = new QListWidget();
  _output_files_list->setSelectionMode(QAbstractItemView::MultiSelection);
  _output_files_list->setEditTriggers(QAbstractItemView::DoubleClicked);
  connect(_output_files_list, SIGNAL(itemSelectionChanged()), this, SLOT(output_itemSelectionChanged()));

  // Results Directory
  QPushButton * button_result = new QPushButton("Local Result directory");
  connect(button_result, SIGNAL(clicked()), this, SLOT(choose_local_directory()));
  _result_directory = new QLineEdit(this);

  // Default result directory is home directory (if we found it)
  // First try -> HOME
  if (getenv("HOME"))
    _result_directory->setText(getenv("HOME"));
  else {
    // Second try -> getpwuid
    struct passwd * pass_struct = getpwuid(getuid());
    if (pass_struct)
      _result_directory->setText(pass_struct->pw_dir);
  }
  registerField("result_directory", _result_directory);

  QGridLayout * output_box = new QGridLayout;
  output_box->addWidget(_output_files_button, 0, 0);
  output_box->addWidget(_remove_output_files_button, 0, 1);
  output_box->addWidget(_output_files_list, 1, 0, 1, -1);
  output_box->addWidget(button_result, 2, 0);
  output_box->addWidget(_result_directory, 2, 1, 1, -1);
  output_group_box->setLayout(output_box);

  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(main_label);
  main_layout->addWidget(input_group_box);
  main_layout->addWidget(output_group_box);
  setLayout(main_layout);

  parent->setFilesList(_input_files_list, _output_files_list);
};

BL::FilesPage::~FilesPage()
{}

bool
BL::FilesPage::validatePage()
{
  QString result_directory = field("result_directory").toString();
  
  for (int i = 0; i < _output_files_list->count(); ++i)
  {
    QListWidgetItem * item = _output_files_list->item(i);
    if (item->text() == "TO EDIT!")
    {
      QMessageBox::warning(NULL, "Ouput Files Error", "Some output files are not defined !");
      return false;
    }
  }

  if (result_directory == "" and _output_files_list->count() != 0)
  {
    QMessageBox::warning(NULL, "Result Directory Error", "Please enter a result directory or remove output files");
    return false;
  }

  return true;
}

void
BL::FilesPage::choose_input_files()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
					      tr("Add input files"), "",
					      tr("All Files (*)"));
  for (int i = 0; i < files.size(); ++i) 
  {
    if (_input_files_list->findItems(files.at(i), Qt::MatchFixedString).size() == 0)
      _input_files_list->addItem(files.at(i));
  }
}

void
BL::FilesPage::choose_local_directory()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose local result directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

  if (dir != "")
    _result_directory->setText(dir);
}

void 
BL::FilesPage::remove_input_files()
{
  QList<QListWidgetItem *> list = _input_files_list->selectedItems();
  for (int i = 0; i < list.size(); ++i)
  {
    int row = _input_files_list->row( list.at(i) );
    delete _input_files_list->takeItem(row);
  }
}

void 
BL::FilesPage::input_itemSelectionChanged()
{
  if (_input_files_list->selectedItems().size() > 0)
    _remove_input_files_button->setEnabled(true);
  else
    _remove_input_files_button->setEnabled(false);
}

void
BL::FilesPage::add_output_file()
{
  QListWidgetItem * new_item = new QListWidgetItem("TO EDIT!");
  new_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
  _output_files_list->addItem(new_item);
}

void 
BL::FilesPage::remove_output_files()
{
  QList<QListWidgetItem *> list = _output_files_list->selectedItems();
  for (int i = 0; i < list.size(); ++i)
  {
    int row = _output_files_list->row( list.at(i) );
    delete _output_files_list->takeItem(row);
  }
}

void 
BL::FilesPage::output_itemSelectionChanged()
{
  if (_output_files_list->selectedItems().size() > 0)
    _remove_output_files_button->setEnabled(true);
  else
    _remove_output_files_button->setEnabled(false);
}

int 
BL::FilesPage::nextId() const
{
  return BL::CreateJobWizard::Page_Resource;
}

BL::ConclusionPage::ConclusionPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Job definition is finished");
  QCheckBox * check_box =  new QCheckBox("Start job after creation");
  registerField("start_job", check_box);
  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(check_box);
  setLayout(main_layout);
};

BL::ConclusionPage::~ConclusionPage()
{}

bool
BL::ConclusionPage::validatePage()
{
  return true;
}

int 
BL::ConclusionPage::nextId() const
{
  return -1;
}

BL::ResourcePage::ResourcePage(BL::CreateJobWizard * parent, BL::SALOMEServices * salome_services)
: QWizardPage(parent)
{
  _salome_services = salome_services;
  setTitle("Select a Resource");

  QLabel * main_label = new QLabel("In this step you select the resource of your job");
  main_label->setWordWrap(true);

  // input_Resource
  QGroupBox * resource_group_box = new QGroupBox("Resource List");
  _resource_list = new JM::ResourceCatalog(this, _salome_services);
  connect(_resource_list->getQListWidget(), SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem*)));

  QVBoxLayout * resource_list_layout = new QVBoxLayout();
  resource_list_layout->addWidget(_resource_list);
  resource_group_box->setLayout(resource_list_layout);

  QLabel * resource_label = new QLabel("Resource selected: ");
  _resource_choosed = new QLineEdit();
  _resource_choosed->setText("");
  _resource_choosed->setReadOnly(true);
  registerField("resource_choosed", _resource_choosed);

  QLabel * bqLabel = new QLabel("Batch Queue (optional):");
  QLineEdit * _bqLineEdit = new QLineEdit(this);
  registerField("batch_queue", _bqLineEdit);

  _ll_label = new QLabel("LoadLeveler JobType:", this);
  _ll_value = new QLineEdit(this);
  registerField("ll_jobtype", _ll_value);
  _ll_label->hide();
  _ll_value->hide();

  _main_layout = new QGridLayout;
  _main_layout->addWidget(resource_group_box, 0, 0, 1, -1);
  _main_layout->addWidget(resource_label, 1, 0);
  _main_layout->addWidget(_resource_choosed, 1, 1);
  _main_layout->addWidget(bqLabel, 2, 0);
  _main_layout->addWidget(_bqLineEdit, 2, 1);
  setLayout(_main_layout);

};

BL::ResourcePage::~ResourcePage()
{}

void
BL::ResourcePage::initializePage()
{
  if (field("ll_jobtype").toString() != "")
  {
    _main_layout->addWidget(_ll_label, 3, 0);
    _main_layout->addWidget(_ll_value, 3, 1);
    _ll_label->show();
    _ll_value->show();
  }
}

bool
BL::ResourcePage::validatePage()
{
  QString resource_choosed = field("resource_choosed").toString();
  if (resource_choosed == "")
  {
    QMessageBox::warning(NULL, "Resource Error", "Please choose a resource");
    return false;
  }

  BL::ResourceDescr resource_descr = _salome_services->getResourceDescr(resource_choosed.toStdString());
  std::string batch = resource_descr.batch.c_str();
  if (batch == "ll")
  {
    QString ll_jobtype = field("ll_jobtype").toString();
    if (ll_jobtype == "")
    {
      QMessageBox::warning(NULL, "LoadLeveler Error", "Please define a LoadLeveler JobType");
      return false;
    }
  }
  return true;
}

void 
BL::ResourcePage::itemSelected(QListWidgetItem * item)
{
  _resource_choosed->setReadOnly(false);
  _resource_choosed->setText(item->text());
  _resource_choosed->setReadOnly(true);

  //Specific parameters for LoadLeveler
  BL::ResourceDescr resource_descr = _salome_services->getResourceDescr(item->text().toStdString());
  std::string batch = resource_descr.batch.c_str();
  if (batch == "ll")
  {
    _main_layout->addWidget(_ll_label, 3, 0);
    _main_layout->addWidget(_ll_value, 3, 1);
    _ll_label->show();
    _ll_value->show();
  }
  else
  {
    _main_layout->removeWidget(_ll_value);
    _main_layout->removeWidget(_ll_label);
    _ll_label->hide();
    _ll_value->hide();
  }
}

int 
BL::ResourcePage::nextId() const
{
  return BL::CreateJobWizard::Page_Conclusion;
}

BL::PythonSalomeMainPage::PythonSalomeMainPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Define a Python script in SALOME job");
  QLabel *label = new QLabel("Enter the Python script that will be executed into the resource");
  label->setWordWrap(true);

  // PythonSalome
  QPushButton * PythonSalome_file_button = new QPushButton(tr("Choose a Python file"));
  connect(PythonSalome_file_button, SIGNAL(clicked()), this, SLOT(choose_PythonSalome_file()));
  _line_PythonSalome = new QLineEdit(this);
  registerField("PythonSalome", _line_PythonSalome);
  _line_PythonSalome->setReadOnly(true);

  QPushButton * PythonSalome_env_file_button = new QPushButton(tr("Choose an environnement file"));
  connect(PythonSalome_env_file_button, SIGNAL(clicked()), this, SLOT(choose_env_file()));
  _line_env_file = new QLineEdit(this);
  registerField("env_PythonSalome_file", _line_env_file);
  _line_env_file->setReadOnly(true);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(PythonSalome_file_button, 0, 0);
  layout->addWidget(_line_PythonSalome, 0, 1);
  layout->addWidget(PythonSalome_env_file_button, 1, 0);
  layout->addWidget(_line_env_file, 1, 1);

  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);
  main_layout->insertLayout(-1, layout);
  setLayout(main_layout);
};

BL::PythonSalomeMainPage::~PythonSalomeMainPage()
{}

void
BL::PythonSalomeMainPage::choose_PythonSalome_file()
{
  QString PythonSalome_file = QFileDialog::getOpenFileName(this,
						      tr("Open Python script file"), "",
						      tr("py (*.py);;All Files (*)"));
  _line_PythonSalome->setReadOnly(false);
  _line_PythonSalome->setText(PythonSalome_file);
  _line_PythonSalome->setReadOnly(true);
}

void
BL::PythonSalomeMainPage::choose_env_file()
{
  QString env_file = QFileDialog::getOpenFileName(this,
						      tr("Open environnement file"), "",
						      tr("sh (*.sh);;All Files (*)"));
  _line_env_file->setReadOnly(false);
  _line_env_file->setText(env_file);
  _line_env_file->setReadOnly(true);
}

bool
BL::PythonSalomeMainPage::validatePage()
{
  QString PythonSalome = field("PythonSalome").toString();
  if (PythonSalome == "")
  {
    QMessageBox::warning(NULL, "Python script in SALOME Error", "Please enter a Python script");
    return false;
  }

  return true;
}

int 
BL::PythonSalomeMainPage::nextId() const
{
  return BL::CreateJobWizard::Page_BatchParameters;
}
