// Copyright (C) 2009-2012  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#include "JM_EditSalomeResource.hxx"
#include "BL_Traces.hxx"

JM::EditSalomeResource::EditSalomeResource(QWidget *parent, BL::SALOMEServices * salome_services,
                                           const std::string & resource_name) : QDialog(parent)
{
  DEBTRACE("Creating JM::EditSalomeResource");
  BL_ASSERT(parent);
  BL_ASSERT(salome_services);
  _parent = parent;
  _salome_services = salome_services;
  _resource_name = resource_name;

  //setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  // Widget code

  // Part 1
  QGroupBox * main_groupBox = new QGroupBox("Main values");
  QLabel * name_label = new QLabel("Name:");
  _name_line = new QLineEdit(this);
  QLabel * hostname_label = new QLabel("Hostname:");
  _hostname_line = new QLineEdit(this);
  QLabel * username_label = new QLabel("Username:");
  _username_line = new QLineEdit(this);
  QLabel * applipath_label = new QLabel("Applipath:");
  _applipath_line = new QLineEdit(this);

  QLabel * protocol_label = new QLabel("Protocol:");
  _protocol_line = new QComboBox(this);
  _protocol_line->addItem("ssh");
  _protocol_line->addItem("rsh");
  _protocol_line->addItem("srun");
  _protocol_line->addItem("pbsdsh");
  _protocol_line->addItem("blaunch");
  _protocol_line->setCurrentIndex(-1);

  QLabel * componentList_label = new QLabel("Component List:");
  _add_button = new QPushButton("Add");
  _remove_button = new QPushButton("Remove");
  _remove_button->setEnabled(false);
  QWidget * component_widget = new QWidget(this);
  _componentList = new QListWidget(this);
  _componentList->setSelectionMode(QAbstractItemView::MultiSelection);
  QGridLayout * input_box = new QGridLayout(this);
  input_box->addWidget(_add_button, 0, 0);
  input_box->addWidget(_remove_button, 0, 1);
  input_box->addWidget(_componentList, 1, 0, 1, -1);
  component_widget->setLayout(input_box);
  connect(_add_button, SIGNAL(clicked()), this, SLOT(add_component()));
  connect(_remove_button, SIGNAL(clicked()), this, SLOT(remove_components()));
  connect(_componentList, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

  QLabel * working_directory_label = new QLabel("Working Directory:");
  _working_directory = new QLineEdit(this);
  QLabel * is_cluster_head_label = new QLabel("Is Cluster Head:");
  _is_cluster_head = new QPushButton(this);
  _is_cluster_head->setCheckable(true);
  connect(_is_cluster_head, SIGNAL(toggled(bool)), this, SLOT(toggle_is_cluster_head(bool)));
  toggle_is_cluster_head(false); // Default is false

  QGridLayout * m_layout = new QGridLayout;
  m_layout->addWidget(name_label, 0, 0);
  m_layout->addWidget(_name_line, 0, 1);
  m_layout->addWidget(hostname_label, 1, 0);
  m_layout->addWidget(_hostname_line, 1, 1);
  m_layout->addWidget(protocol_label, 2, 0);
  m_layout->addWidget(_protocol_line, 2, 1);
  m_layout->addWidget(username_label, 3, 0);
  m_layout->addWidget(_username_line, 3, 1);
  m_layout->addWidget(applipath_label, 4, 0);
  m_layout->addWidget(_applipath_line, 4, 1);
  m_layout->addWidget(componentList_label, 5, 0);
  m_layout->addWidget(component_widget, 5, 1);
  m_layout->addWidget(is_cluster_head_label, 6, 0);
  m_layout->addWidget(_is_cluster_head, 6, 1);
  m_layout->addWidget(working_directory_label, 7, 0);
  m_layout->addWidget(_working_directory, 7, 1);
  main_groupBox->setLayout(m_layout);

  // Part 2
  QGroupBox * config_groupBox = new QGroupBox("Configuration values");
  QLabel * os_label = new QLabel("OS:");
  _os_line = new QLineEdit(this);

  QLabel * mem_mb_label = new QLabel("Memory (mb):");
  _mem_mb_line = new QSpinBox(this);
  _mem_mb_line->setMinimum(0);
  _mem_mb_line->setMaximum(1000000);
  _mem_mb_line->setValue(0);
  QLabel * cpu_clock_label = new QLabel("CPU Clock:");
  _cpu_clock_line = new QSpinBox(this);
  _cpu_clock_line->setMinimum(0);
  _cpu_clock_line->setMaximum(1000000);
  _cpu_clock_line->setValue(0);
  QLabel * nb_node_label = new QLabel("Nb node:");
  _nb_node_line = new QSpinBox(this);
  _nb_node_line->setMinimum(1);
  _nb_node_line->setMaximum(1000000);
  _nb_node_line->setValue(1);
  QLabel * nb_proc_per_node_label = new QLabel("Nb proc/node:");
  _nb_proc_per_node_line = new QSpinBox(this);
  _nb_proc_per_node_line->setMinimum(1);
  _nb_proc_per_node_line->setMaximum(1000000);
  _nb_proc_per_node_line->setValue(1);

  QLabel * iprotocol_label = new QLabel("Internal protocol:");
  _iprotocol_line = new QComboBox(this);
  _iprotocol_line->addItem("ssh");
  _iprotocol_line->addItem("rsh");
  _iprotocol_line->addItem("srun");
  _iprotocol_line->addItem("pbsdsh");
  _iprotocol_line->addItem("blaunch");
  _iprotocol_line->setCurrentIndex(-1);

  QLabel * batch_label = new QLabel("Batch:");
  _batch_line = new QComboBox(this);
  _batch_line->addItem("pbs");
  _batch_line->addItem("lsf");
  _batch_line->addItem("sge");
  _batch_line->addItem("ssh");
  _batch_line->addItem("ccc");
  _batch_line->addItem("slurm");
  _batch_line->addItem("ll");
  _batch_line->addItem("vishnu");
  _batch_line->setCurrentIndex(-1);

  QLabel * mpiImpl_label = new QLabel("MPI impl:");
  _mpiImpl_line = new QComboBox(this);
  _mpiImpl_line->addItem("lam");
  _mpiImpl_line->addItem("mpich1");
  _mpiImpl_line->addItem("mpich2");
  _mpiImpl_line->addItem("openmpi");
  _mpiImpl_line->addItem("slurmmpi");
  _mpiImpl_line->addItem("prun");
  _mpiImpl_line->setCurrentIndex(-1);

  QGridLayout * c_layout = new QGridLayout;
  c_layout->addWidget(os_label, 0, 0);
  c_layout->addWidget(_os_line, 0, 1);
  c_layout->addWidget(mem_mb_label, 1, 0);
  c_layout->addWidget(_mem_mb_line, 1, 1);
  c_layout->addWidget(cpu_clock_label, 2, 0);
  c_layout->addWidget(_cpu_clock_line, 2, 1);
  c_layout->addWidget(nb_node_label, 3, 0);
  c_layout->addWidget(_nb_node_line, 3, 1);
  c_layout->addWidget(nb_proc_per_node_label, 4, 0);
  c_layout->addWidget(_nb_proc_per_node_line, 4, 1);
  c_layout->addWidget(batch_label, 5, 0);
  c_layout->addWidget(_batch_line, 5, 1);
  c_layout->addWidget(mpiImpl_label, 6, 0);
  c_layout->addWidget(_mpiImpl_line, 6, 1);
  c_layout->addWidget(iprotocol_label, 7, 0);
  c_layout->addWidget(_iprotocol_line, 7, 1);
  config_groupBox->setLayout(c_layout);

  // Part 3
  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                      | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  // Main Layout
  QVBoxLayout * mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(main_groupBox);
  mainLayout->addWidget(config_groupBox);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle("Edit/Add a resource");
  if (_resource_name != "")
    get_infos();
}

JM::EditSalomeResource::~EditSalomeResource()
{
  DEBTRACE("Destroying JM::EditSalomeResource");
}

void
JM::EditSalomeResource::get_infos()
{
  BL::ResourceDescr resource_descr = _salome_services->getResourceDescr(_resource_name);

  _name_line->setText(QString(resource_descr.name.c_str()));
  _hostname_line->setText(QString(resource_descr.hostname.c_str()));
  _username_line->setText(QString(resource_descr.username.c_str()));
  _applipath_line->setText(QString(resource_descr.applipath.c_str()));
  _os_line->setText(QString(resource_descr.OS.c_str()));
  _working_directory->setText(QString(resource_descr.working_directory.c_str()));
  _is_cluster_head->setChecked(resource_descr.is_cluster_head);

  std::string protocol = resource_descr.protocol.c_str();
  if (protocol == "ssh")
    _protocol_line->setCurrentIndex(0);
  else if(protocol == "rsh")
    _protocol_line->setCurrentIndex(1);
  else if(protocol == "srun")
    _protocol_line->setCurrentIndex(2);
  else if(protocol == "pbsdsh")
    _protocol_line->setCurrentIndex(3);
  else if(protocol == "blaunch")
    _protocol_line->setCurrentIndex(4);
  else
    _protocol_line->setCurrentIndex(-1);

  std::string iprotocol = resource_descr.iprotocol.c_str();
  if (iprotocol == "ssh")
    _iprotocol_line->setCurrentIndex(0);
  else if (iprotocol == "rsh")
    _iprotocol_line->setCurrentIndex(1);
  else if (iprotocol == "srun")
    _iprotocol_line->setCurrentIndex(2);
  else if (iprotocol == "pbsdsh")
    _iprotocol_line->setCurrentIndex(3);
  else if (iprotocol == "blaunch")
    _iprotocol_line->setCurrentIndex(4);
  else
    _iprotocol_line->setCurrentIndex(-1);

  std::string batch = resource_descr.batch.c_str();
  if (batch == "pbs")
    _batch_line->setCurrentIndex(0);
  else if (batch == "lsf")  
    _batch_line->setCurrentIndex(1);
  else if (batch == "sge")  
    _batch_line->setCurrentIndex(2);
  else if (batch == "ssh")
    _batch_line->setCurrentIndex(3);
  else if (batch == "ccc")
    _batch_line->setCurrentIndex(4);
  else if (batch == "slurm")
    _batch_line->setCurrentIndex(5);
  else if (batch == "ll")
    _batch_line->setCurrentIndex(6);
  else if (batch == "vishnu")
    _batch_line->setCurrentIndex(7);
  else
    _batch_line->setCurrentIndex(-1);

  std::string mpiImpl = resource_descr.mpiImpl.c_str();
  if (mpiImpl == "lam")
    _mpiImpl_line->setCurrentIndex(0);
  else if (mpiImpl == "mpich1")  
    _mpiImpl_line->setCurrentIndex(1);
  else if (mpiImpl == "mpich2")  
    _mpiImpl_line->setCurrentIndex(2);
  else if (mpiImpl == "openmpi")
    _mpiImpl_line->setCurrentIndex(3);
  else if (mpiImpl == "slurmmpi")
    _mpiImpl_line->setCurrentIndex(4);
  else if (mpiImpl == "prun")
    _mpiImpl_line->setCurrentIndex(5);
  else  
    _mpiImpl_line->setCurrentIndex(-1);

  int value = resource_descr.mem_mb;
  if (value > 0)
    _mem_mb_line->setValue(value);
  value = resource_descr.cpu_clock;
  if (value > 0)
    _cpu_clock_line->setValue(value);
  value = resource_descr.nb_node;
  if (value > 1)
    _nb_node_line->setValue(value);
  value = resource_descr.nb_proc_per_node;
  if (value > 1)
    _nb_proc_per_node_line->setValue(value);

   std::list<std::string>::iterator it = resource_descr.componentList.begin();
   for(; it != resource_descr.componentList.end(); it++)
     _componentList->addItem(QString((*it).c_str()));
   
  // Better if string's length is bigger than the widget
  _name_line->setCursorPosition(0);
  _hostname_line->setCursorPosition(0);
  _username_line->setCursorPosition(0);
  _applipath_line->setCursorPosition(0);
  _os_line->setCursorPosition(0);
  _working_directory->setCursorPosition(0);
}


void 
JM::EditSalomeResource::itemSelectionChanged()
{
  if (_componentList->selectedItems().size() > 0)
    _remove_button->setEnabled(true);
  else
    _remove_button->setEnabled(false);
}

void 
JM::EditSalomeResource::add_component()
{
  bool ok;
  QString text = QInputDialog::getText(this, "Add a component",
                                       "Component name:", QLineEdit::Normal,
                                       "", &ok);
  if (ok && !text.isEmpty())
    _componentList->addItem(text);
}

void 
JM::EditSalomeResource::remove_components()
{
  QList<QListWidgetItem *> list = _componentList->selectedItems();
  for (int i = 0; i < list.size(); ++i)
  {
    int row = _componentList->row( list.at(i) );
    delete _componentList->takeItem(row);
  }
}

void
JM::EditSalomeResource::accept()
{
  BL::ResourceDescr resource;

  // Text
  resource.name = _name_line->text().toStdString();
  resource.hostname = _hostname_line->text().toStdString();
  resource.username = _username_line->text().toStdString();
  resource.applipath = _applipath_line->text().toStdString();
  resource.OS = _os_line->text().toStdString();
  resource.working_directory = _working_directory->text().toStdString();
  if (_is_cluster_head->isChecked())
    resource.is_cluster_head = true;
  else
    resource.is_cluster_head = false;

  // Components
  int count = _componentList->count();
  for (int i = 0; i < count; i++)
    resource.componentList.push_back(_componentList->item(i)->text().toStdString());

  // ComboBox
  resource.protocol = _protocol_line->currentText().toStdString();
  resource.iprotocol = _iprotocol_line->currentText().toStdString();
  resource.batch = _batch_line->currentText().toStdString();
  if (resource.batch == "ssh")
    resource.batch = "ssh_batch";
  resource.mpiImpl = _mpiImpl_line->currentText().toStdString();

  // QSpinBox
  resource.mem_mb = _mem_mb_line->value();
  resource.cpu_clock = _cpu_clock_line->value();
  resource.nb_node = _nb_node_line->value();
  resource.nb_proc_per_node = _nb_proc_per_node_line->value();

  if (resource.name != "" &&
      resource.hostname != "" &&
      resource.protocol != "")
  {
    _salome_services->addResource(resource);
    QDialog::accept();
  }
  else
  {
    QMessageBox::warning(NULL, "Values missing", "name, hostname and protocol are mandatory! Cancel or add values!");
  }
}

void
JM::EditSalomeResource::toggle_is_cluster_head(bool checked)
{
  if (checked)
    _is_cluster_head->setText("true");
  else
    _is_cluster_head->setText("false");
}
