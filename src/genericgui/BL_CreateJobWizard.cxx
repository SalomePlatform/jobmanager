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

#include "BL_CreateJobWizard.hxx"

BL::CreateJobWizard::CreateJobWizard(BL::JobsManager_QT * jobs_manager, BL::SALOMEServices * salome_services)
{
  DEBTRACE("Creating BL::CreateJobWizard");
  BL_ASSERT(jobs_manager);
  BL_ASSERT(salome_services);
  _jobs_manager = jobs_manager;

  job_name = "";
  yacs_file = "";
  command = "";
  batch_directory = "";
  expected_during_time = "";
  expected_memory = "";
  nb_proc = 1;

  result_directory = "";

  machine_choosed = "";

  start_job = false;

  setOptions(QWizard::IndependentPages | QWizard::NoBackButtonOnStartPage);

  setPage(Page_JobName, new BL::CreateJobWizard::JobNamePage(this, _jobs_manager));
  setPage(Page_YACSSchema, new BL::YACSSchemaPage(this));
  setPage(Page_BatchParameters, new BL::BatchParametersPage(this));
  setPage(Page_Files, new BL::FilesPage(this));
  setPage(Page_Command_Main_Definitions, new BL::CreateJobWizard::CommandMainPage(this));
  setPage(Page_Machine, new BL::MachinePage(this, salome_services));
  setPage(Page_Conclusion, new BL::CreateJobWizard::ConclusionPage(this));

  setWindowTitle("Create Job Wizard");
  connect(this, SIGNAL(finished(int)), this, SLOT(end(int)));
}

BL::CreateJobWizard::~CreateJobWizard()
{
  DEBTRACE("Destroying BL::CreateJobWizard");
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
    
    // Command Panel
    QString f_command = field("command").toString();
    command = f_command.toStdString();
    
    // Batch Panel
    QString f_batch_directory = field("batch_directory").toString();
    batch_directory = f_batch_directory.toStdString();

    QString time_hour;
    QString time_min;
    if (field("during_time_hour").toInt() < 10)
      time_hour = "0" + field("during_time_hour").toString();
    else
      time_hour = field("during_time_hour").toString();
    if (field("during_time_min").toInt() < 10)
      time_min = "0" + field("during_time_min").toString();
    else
      time_min = field("during_time_min").toString();
    expected_during_time = time_hour.toStdString() + ":" + time_min.toStdString() + ":00";

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

    // Machine list
    QString f_machine_choosed = field("machine_choosed").toString();
    machine_choosed = f_machine_choosed.toStdString(); 

    start_job = field("start_job").toBool();
  }
}

// Job Name Page
BL::CreateJobWizard::JobNamePage::JobNamePage(QWidget * parent, BL::JobsManager_QT * jobs_manager)
: QWizardPage(parent)
{
  _jobs_manager = jobs_manager;
  setTitle("Create a new job");

  QLabel *label = new QLabel("Enter Job name, you cannot add two jobs with the same name");
  label->setWordWrap(true);
  QLabel * nameLabel = new QLabel("Job Name:");
  QLineEdit * _nameLineEdit = new QLineEdit(this);
  registerField("job_name", _nameLineEdit);

  QLabel * label_type = new QLabel("Choose type of batch job:");
  QGroupBox *groupBox = new QGroupBox();
  _yacs_schema_button = new QRadioButton(tr("YACS Schema"));
  _yacs_schema_button->setChecked(true);
  QRadioButton *radio2 = new QRadioButton(tr("Command"));
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(_yacs_schema_button);
  vbox->addWidget(radio2);
  vbox->addStretch(1);
  groupBox->setLayout(vbox);

  // Layouts
  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(nameLabel, 0, 0);
  layout->addWidget(_nameLineEdit, 0, 1);
  main_layout->insertLayout(-1, layout);
  layout->addWidget(label_type, 2, 0);
  layout->addWidget(groupBox, 3, 0, 1, -1);
  setLayout(main_layout);
}

BL::CreateJobWizard::JobNamePage::~JobNamePage()
{}

bool
BL::CreateJobWizard::JobNamePage::validatePage()
{
  DEBTRACE("Calling validatePage of BL::CreateJobWizard::JobNamePage");
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
    if (_jobs_manager->job_already_exist(job_name.toStdString()) == false)
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
BL::CreateJobWizard::JobNamePage::nextId() const
{
  if (_yacs_schema_button->isChecked()) 
  {
    return BL::CreateJobWizard::Page_YACSSchema;
  } 
  else 
  {
    return BL::CreateJobWizard::Page_Command_Main_Definitions;
  }
}

BL::YACSSchemaPage::YACSSchemaPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Choose YACS Schema");

  QLabel *label = new QLabel("In this step you have to choose what YACS Schema you want to execute");
  label->setWordWrap(true);

  _yacs_file_button = new QPushButton(tr("Choose YACS Schema file"));
  _yacs_file_button->show();
  connect(_yacs_file_button, SIGNAL(clicked()), this, SLOT(choose_file()));

  _yacs_file_text = new QLineEdit(this);
  _yacs_file_text->setText("");
  registerField("yacs_file", _yacs_file_text);
  _yacs_file_text->setReadOnly(true);

  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(_yacs_file_button, 0, 0);
  layout->addWidget(_yacs_file_text, 0, 1);
  main_layout->insertLayout(-1, layout);
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

int 
BL::YACSSchemaPage::nextId() const
{
  return BL::CreateJobWizard::Page_BatchParameters;
}

BL::CreateJobWizard::CommandMainPage::CommandMainPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Define command job");
  QLabel *label = new QLabel("Enter the command that will be executed into the resource");
  label->setWordWrap(true);

  // command
  QLabel * label_command = new QLabel("Command: ");
  QLineEdit * line_command = new QLineEdit(this);
  registerField("command", line_command);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(label_command, 0, 0);
  layout->addWidget(line_command, 0, 1);

  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(label);
  main_layout->insertLayout(-1, layout);
  setLayout(main_layout);
};

BL::CreateJobWizard::CommandMainPage::~CommandMainPage()
{}

bool
BL::CreateJobWizard::CommandMainPage::validatePage()
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
BL::CreateJobWizard::CommandMainPage::nextId() const
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
  QLabel * label_directory = new QLabel("Batch directory: ");
  QLineEdit * line_directory = new QLineEdit(this);
  registerField("batch_directory", line_directory);

  // exected during time
  QLabel * label_during_time = new QLabel("Expected during time: ");
  QSpinBox * spin_during_time_hour = new QSpinBox(this);
  QLabel * label_during_time_hour = new QLabel("Hours");
  spin_during_time_hour->setMinimum(0);
  spin_during_time_hour->setMaximum(1000000);
  registerField("during_time_hour", spin_during_time_hour);
  QSpinBox * spin_during_time_min = new QSpinBox(this);
  QLabel * label_during_time_min = new QLabel("Minutes");
  spin_during_time_min->setMinimum(0);
  spin_during_time_min->setMaximum(59);
  registerField("during_time_min", spin_during_time_min);

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
  layout->addWidget(label_during_time, 1, 0);
  layout->addWidget(spin_during_time_hour, 1, 1);
  layout->addWidget(label_during_time_hour, 1, 2);
  layout->addWidget(spin_during_time_min, 1, 3);
  layout->addWidget(label_during_time_min, 1, 4);
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

  int time_hour = field("during_time_hour").toInt();
  int time_min = field("during_time_min").toInt();
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
  QLabel * label_result_directory = new QLabel("Result directory: ");
  QLineEdit * result_directory = new QLineEdit(this);
  registerField("result_directory", result_directory);

  QGridLayout * output_box = new QGridLayout;
  output_box->addWidget(_output_files_button, 0, 0);
  output_box->addWidget(_remove_output_files_button, 0, 1);
  output_box->addWidget(_output_files_list, 1, 0, 1, -1);
  output_box->addWidget(label_result_directory, 2, 0);
  output_box->addWidget(result_directory, 2, 1, 1, -1);
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

  if (result_directory != "" and _output_files_list->count() == 0)
  {
    QMessageBox::warning(NULL, "Result Error", "Please add output files or erase result directory");
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
  return BL::CreateJobWizard::Page_Machine;
}

BL::CreateJobWizard::ConclusionPage::ConclusionPage(QWidget * parent)
: QWizardPage(parent)
{
  setTitle("Job definition is finished");
  QCheckBox * check_box =  new QCheckBox("Start job after creation");
  registerField("start_job", check_box);
  QVBoxLayout * main_layout = new QVBoxLayout;
  main_layout->addWidget(check_box);
  setLayout(main_layout);
};

BL::CreateJobWizard::ConclusionPage::~ConclusionPage()
{}

bool
BL::CreateJobWizard::ConclusionPage::validatePage()
{
  return true;
}

int 
BL::CreateJobWizard::ConclusionPage::nextId() const
{
  return -1;
}

BL::MachinePage::MachinePage(BL::CreateJobWizard * parent, BL::SALOMEServices * salome_services)
: QWizardPage(parent)
{
  _salome_services = salome_services;
  setTitle("Select a Machine");

  QLabel * main_label = new QLabel("In this step you select the machine of your job");
  main_label->setWordWrap(true);

  // input_Machine
  QGroupBox * machine_group_box = new QGroupBox("Machine List");
  _machine_list = new QListWidget();
  _machine_list->setSelectionMode(QAbstractItemView::SingleSelection);
  std::list<std::string> machine_list = _salome_services->getMachineList();
  std::list<std::string>::iterator it;
  for (it = machine_list.begin(); it != machine_list.end(); it++)
  {
    std::string machine = *it;
    _machine_list->addItem(QString(machine.c_str()));
  }
  connect(_machine_list, SIGNAL(itemSelectionChanged()), this, SLOT(machine_itemSelectionChanged()));
  QVBoxLayout * machine_list_layout = new QVBoxLayout();
  machine_list_layout->addWidget(_machine_list);
  machine_group_box->setLayout(machine_list_layout);

  QLabel * machine_label = new QLabel("Machine selected: ");
  _machine_choosed = new QLineEdit();
  _machine_choosed->setText("");
  _machine_choosed->setReadOnly(true);
  registerField("machine_choosed", _machine_choosed);

  QGridLayout * main_layout = new QGridLayout;
  main_layout->addWidget(machine_group_box, 0, 0, 1, -1);
  main_layout->addWidget(machine_label, 1, 0);
  main_layout->addWidget(_machine_choosed, 1, 1);
  setLayout(main_layout);
};

BL::MachinePage::~MachinePage()
{}

bool
BL::MachinePage::validatePage()
{
  QString machine_choosed = field("machine_choosed").toString();
  if (machine_choosed == "")
  {
    QMessageBox::warning(NULL, "Machine Error", "Please choose a machine");
    return false;
  }
  return true;
}

void 
BL::MachinePage::machine_itemSelectionChanged()
{
  _machine_choosed->setReadOnly(false);
  QList<QListWidgetItem *> list = _machine_list->selectedItems();
  QListWidgetItem * item = list.at(0);
  _machine_choosed->setText(item->text());
  _machine_choosed->setReadOnly(true);
}

int 
BL::MachinePage::nextId() const
{
  return BL::CreateJobWizard::Page_Conclusion;
}
