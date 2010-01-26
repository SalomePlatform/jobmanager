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

#include "BL_QModelManager.hxx"

BL::QModelManager::QModelManager(QObject * parent, BL::JobsManager_QT * jobs_manager) : QObject(parent)
{
  DEBTRACE("Creating BL::QModelManager");
  BL_ASSERT(jobs_manager);
  _jobs_manager = jobs_manager;

  _model = new QStandardItemModel(this);
  QStringList headers;
  headers << "Job Name" << "Type" << "State" << "Resource" << "Launcher Id";
  _model->setHorizontalHeaderLabels(headers);
}

BL::QModelManager::~QModelManager()
{
  DEBTRACE("Destroying BL::QModelManager");
}

QStandardItemModel *
BL::QModelManager::getModel()
{
  DEBTRACE("getModel BL::QModelManager called");

  return _model;
}

void
BL::QModelManager::new_job_added(const QString & name)
{
  BL::Job * job = _jobs_manager->getJob(name.toStdString());
  QStandardItem * new_job_name = new QStandardItem(name);
  
  QStandardItem * new_job_type;
  if (job->getType() == BL::Job::YACS_SCHEMA)
    new_job_type = new QStandardItem("YACS_Schema");
  else if (job->getType() == BL::Job::COMMAND)
    new_job_type = new QStandardItem("Command");
  else if (job->getType() == BL::Job::PYTHON_SALOME)
    new_job_type = new QStandardItem("Python_Salome");

  QStandardItem * new_job_state;
  if (job->getState() == BL::Job::CREATED)
    new_job_state = new QStandardItem("Created");
  else if (job->getState() == BL::Job::QUEUED)
    new_job_state = new QStandardItem("Queued");
  else if (job->getState() == BL::Job::RUNNING)
    new_job_state = new QStandardItem("Running");
  else if (job->getState() == BL::Job::PAUSED)
    new_job_state = new QStandardItem("Paused");
  else if (job->getState() == BL::Job::ERROR)
    new_job_state = new QStandardItem("Error");
  else 
    new_job_state = new QStandardItem("Finished");

  QStandardItem * new_job_resource =  new QStandardItem(job->getResource().c_str());
  QString id_str;
  id_str.setNum(job->getSalomeLauncherId());
  QStandardItem * new_job_id =  new QStandardItem(id_str);

  int row = _model->rowCount();
  _model->setItem(row, 0, new_job_name);
  _model->setItem(row, 1, new_job_type);
  _model->setItem(row, 2, new_job_state);
  _model->setItem(row, 3, new_job_resource);
  _model->setItem(row, 4, new_job_id);
}

void
BL::QModelManager::job_state_changed(const QString & name)
{
  DEBTRACE("BL::QModelManager::job_state_changed received");

  BL::Job * job = _jobs_manager->getJob(name.toStdString());
  QList<QStandardItem *> item_list = _model->findItems(name);
  QStandardItem * job_state_item = _model->item(item_list.at(0)->row(), 2);

  if (job->getState() == BL::Job::CREATED)
    job_state_item->setText("Created");
  else if (job->getState() == BL::Job::QUEUED)
    job_state_item->setText("Queued");
  else if (job->getState() == BL::Job::RUNNING)
    job_state_item->setText("Running");
  else if (job->getState() == BL::Job::PAUSED)
    job_state_item->setText("Paused");
  else if (job->getState() == BL::Job::ERROR)
    job_state_item->setText("Error");
  else 
    job_state_item->setText("Finished");
}

void
BL::QModelManager::deleteJob(int row)
{
  _model->removeRow(row);
}

void
BL::QModelManager::job_selected(const QModelIndex & index)
{
  DEBTRACE("BL::QModelManager::job_selected slot");
  QStandardItem * item = _model->itemFromIndex(index);
  int row = item->row();

  // Algo un peu bourrin....
  for (int i = 0; i < _model->rowCount(); i++)
    for (int j = 0; j < _model->columnCount(); j++)
    {
      _model->item(i,j)->setBackground(QBrush(Qt::white));
      _model->item(i,j)->setForeground(QBrush(Qt::black));
    }

  for (int j = 0; j < _model->columnCount(); j++)
  {
    _model->item(row,j)->setBackground(QBrush(Qt::darkBlue));
    _model->item(row,j)->setForeground(QBrush(Qt::white));
  }
}
