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

#include "BL_JobsManager_QT.hxx"
#include "BL_GenericGui.hxx"

BL::JobManagerEvent::JobManagerEvent(const std::string & action_i, 
				     const std::string & event_name_i, 
				     const std::string & job_name_i, 
				     const std::string & data_i) : QEvent(QEvent::User)
{
  action = action_i;
  event_name = event_name_i;
  job_name = job_name_i;
  data = data_i;
}

BL::JobManagerEvent::~JobManagerEvent() {}  

BL::JobsManager_QT::JobsManager_QT(QWidget * parent, BL::GenericGui * main_gui, BL::SALOMEServices * salome_services) : 
  QDockWidget(parent), BL::JobsManager(salome_services)
{
  DEBTRACE("Creating BL::JobsManager_QT");
  _main_gui = main_gui;
  setObserver(this);

  // Widget Part

  QWidget * main_widget = new QWidget(this);

  _load_jobs = new QPushButton("Load Jobs");
  _save_jobs = new QPushButton("Save Jobs");
  _load_jobs->setEnabled(false);
  _save_jobs->setEnabled(false);
  _auto_refresh_jobs = new QPushButton("Auto Refresh: no");
  _timer = new QTimer(this);
  _timer->stop();
  connect(_timer, SIGNAL(timeout()), this, SLOT(RefreshJobs()));

  // Menu for auto refresh
  QMenu * refresh_menu = new QMenu(this);
  refresh_menu->addAction("No", this, SLOT(no_auto_refresh()));
  refresh_menu->addAction("10 seconds", this, SLOT(ten_seconds_refresh()));
  refresh_menu->addAction("30 seconds", this, SLOT(thirty_seconds_refresh()));
  refresh_menu->addAction("1 minute", this, SLOT(one_minute_refresh()));
  refresh_menu->addAction("5 minutes", this, SLOT(five_minutes_refresh()));
  refresh_menu->addAction("30 minutes", this, SLOT(thirty_minutes_refresh()));
  refresh_menu->addAction("1 hour", this, SLOT(one_hour_refresh()));
  _auto_refresh_jobs->setMenu(refresh_menu);

  QHBoxLayout * button_layout = new QHBoxLayout();
  button_layout->addWidget(_load_jobs);
  button_layout->addWidget(_save_jobs);
  button_layout->addWidget(_auto_refresh_jobs);

  QGroupBox * message_box = new QGroupBox("Messages");
  _log = new QTextEdit(this);
  _log->setReadOnly(true);
  QVBoxLayout * message_box_layout = new QVBoxLayout(message_box);
  message_box_layout->addWidget(_log);
  message_box->setLayout(message_box_layout);

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->addLayout(button_layout);
  mainLayout->addWidget(message_box);
  main_widget->setLayout(mainLayout);

  QScrollArea * scroll_widget = new QScrollArea(this);
  scroll_widget->setWidget(main_widget);
  scroll_widget->setWidgetResizable(true);
  setWidget(scroll_widget);
  setWindowTitle("Job Manager");
}

BL::JobsManager_QT::~JobsManager_QT()
{
  DEBTRACE("Destroying BL::JobsManager_QT");
}

void
BL::JobsManager_QT::RefreshJobs()
{
  refresh_jobs(); 
}

void
BL::JobsManager_QT::no_auto_refresh()
{
  _auto_refresh_jobs->setText("Auto Refresh: no");
  _timer->stop();
}

void
BL::JobsManager_QT::ten_seconds_refresh()
{
  _auto_refresh_jobs->setText("Auto Refresh: 10s");
  _timer->stop();
  _timer->start(10 * 1000);
}

void 
BL::JobsManager_QT::thirty_seconds_refresh()
{
  _auto_refresh_jobs->setText("Auto Refresh: 30s");
  _timer->stop();
  _timer->start(30 * 1000);
}

void 
BL::JobsManager_QT::one_minute_refresh()
{
  _auto_refresh_jobs->setText("Auto Refresh: 1min");
  _timer->stop();
  _timer->start(1 * 60 * 1000);
}

void 
BL::JobsManager_QT::five_minutes_refresh()
{
  _auto_refresh_jobs->setText("Auto Refresh: 5min");
  _timer->stop();
  _timer->start(5 * 60 * 1000);
}

void 
BL::JobsManager_QT::thirty_minutes_refresh()
{
  _auto_refresh_jobs->setText("Auto Refresh: 30min");
  _timer->stop();
  _timer->start(30 * 60 * 1000);
}

void 
BL::JobsManager_QT::one_hour_refresh()
{
  _auto_refresh_jobs->setText("Auto Refresh: 1hour");
  _timer->stop();
  _timer->start(1 * 60 * 60 * 1000);
}

void 
BL::JobsManager_QT::restart_job(const std::string & name)
{
  DEBTRACE("Restart job with name: " << name);
  BL::CreateJobWizard wizard(this, _salome_services);
  wizard.clone(name);
  wizard.end(1);
  wizard.job_name = name;
  wizard.start_job = true;
  _main_gui->delete_job_internal();
  create_job_with_wizard(wizard);
}

void 
BL::JobsManager_QT::edit_clone_job(const std::string & name)
{
  BL::CreateJobWizard wizard(this, _salome_services);
  wizard.clone(name);
  wizard.exec();

  // Check if the job has the same name
  if (name == wizard.job_name)
  {
    DEBTRACE("Job " << name << " has been changed");
    _main_gui->delete_job_internal();
  }

  if (wizard.job_name != "")
  {
    create_job_with_wizard(wizard);
  }
  else
  {
    DEBTRACE("User cancel Create Job Wizard");
  }
}

void
BL::JobsManager_QT::create_job()
{
    BL::CreateJobWizard wizard(this, _salome_services);
    wizard.exec();
    if (wizard.job_name != "")
    {
      create_job_with_wizard(wizard);
    }
    else
    {
       DEBTRACE("User cancel Create Job Wizard");
    }
}

void 
BL::JobsManager_QT::create_job_with_wizard(BL::CreateJobWizard & wizard)
{
  BL::Job * new_job = createJob(wizard.job_name);
  if (wizard.yacs_file != "")
  {
    // YACS schema job
    new_job->setType(BL::Job::YACS_SCHEMA);
    new_job->setJobFile(wizard.yacs_file);
  }
  else if (wizard.command != "")
  {
    // Command Job
    new_job->setType(BL::Job::COMMAND);
    new_job->setJobFile(wizard.command);
  }
  else if (wizard.python_salome_file != "")
  {
    // Command Job
    new_job->setType(BL::Job::PYTHON_SALOME);
    new_job->setJobFile(wizard.python_salome_file);
  }

  // For all jobs
  new_job->setEnvFile(wizard.env_file);
  BL::Job::BatchParam param;
  param.batch_directory = wizard.batch_directory;
  param.maximum_duration = wizard.maximum_duration;
  param.expected_memory = wizard.expected_memory;
  param.nb_proc = wizard.nb_proc;
  new_job->setBatchParameters(param);
  BL::Job::FilesParam files_params;
  files_params.result_directory = wizard.result_directory;
  files_params.input_files_list = wizard.input_files_list;
  files_params.output_files_list = wizard.output_files_list;
  new_job->setFilesParameters(files_params);
  new_job->setResource(wizard.resource_choosed);
  new_job->setBatchQueue(wizard.batch_queue);

  // End
  addJobToLauncher(wizard.job_name);
  emit new_job_added(QString::fromStdString(wizard.job_name));
  if (wizard.start_job)
    start_job(wizard.job_name);
}

void
BL::JobsManager_QT::delete_job(QString job_name)
{
  BL::JobsManager::removeJob(job_name.toStdString());
}

void 
BL::JobsManager_QT::sendEvent(const std::string & action, 
			      const std::string & event_name, 
			      const std::string & job_name, 
			      const std::string & data)
{
  DEBTRACE("sendEvent BL::JobsManager_QT");

  // Sending a QEvent to go back to main thread
  BL::JobManagerEvent * event = new JobManagerEvent(action, event_name, job_name, data);
  QApplication::postEvent(this, event);
}

bool 
BL::JobsManager_QT::event(QEvent * e)
{
  QDockWidget::event(e);
  JobManagerEvent * event = dynamic_cast<JobManagerEvent*>(e);
  if (!event) return false;

  DEBTRACE("BL::JobsManager_QT Receiving event : " 
	   << event->action << " " 
	   << event->event_name << " "
	   << event->job_name << " "
	   << event->data);

  if (event->action == "create_job")
  {
    if (event->event_name == "Ok")
    {
      QString str((event->job_name).c_str());
      write_normal_text("Job " + str + " created\n");
    }
    else
    {
      QString str((event->job_name).c_str());
      write_error_text("Error in creating job: " + str + "\n");
      write_error_text("*** ");
      write_error_text((event->data).c_str());
      write_error_text(" ***\n");
    }
  }
  else if (event->action == "start_job")
  {
    if (event->event_name == "Ok")
    {
      QString str((event->job_name).c_str());
      write_normal_text("Job " + str + " queued\n");
    }
    else
    {
      QString str((event->job_name).c_str());
      write_error_text("Error in starting job: " + str + "\n");
      write_error_text("*** ");
      write_error_text((event->data).c_str());
      write_error_text(" ***\n");
    }
    emit job_state_changed(QString((event->job_name).c_str()));
  }
  else if (event->action == "refresh_job")
  {
    if (event->event_name == "Ok")
    {
      QString str((event->job_name).c_str());
      write_normal_text("Job " + str + " state changed\n");
      emit job_state_changed(QString((event->job_name).c_str()));
    }
    else
    {
      QString str((event->job_name).c_str());
      write_error_text("Error in refreshing job: " + str + "\n");
      write_error_text("*** ");
      write_error_text((event->data).c_str());
      write_error_text(" ***\n");
    }
  }
  else if (event->action == "delete_job")
  {
    if (event->event_name == "Ok")
    {
      QString str((event->job_name).c_str());
      write_normal_text("Job " + str + " deleted\n");
    }
    else
    {
      QString str((event->job_name).c_str());
      write_error_text("Warning delete job: " + str + " maybe not complete, exception catch in SALOME Launcher service\n");
      write_error_text("*** ");
      write_error_text((event->data).c_str());
      write_error_text(" ***\n");
    }
  }
  else if (event->action == "get_results_job")
  {
    if (event->event_name == "Ok")
    {
      QString str((event->job_name).c_str());
      write_normal_text("Results of Job " + str + " are get\n");
    }
    else
    {
      QString str((event->job_name).c_str());
      write_error_text("Warning for results of job: " + str + " maybe not complete, exception catch in SALOME Launcher service\n");
      write_error_text("*** ");
      write_error_text((event->data).c_str());
      write_error_text(" ***\n");
    }
  }
  else
  {
    QString str((event->action).c_str());
    write_error_text("Unknown type of event received:" + str + "\n");
  }
  return true;
}

void 
BL::JobsManager_QT::write_normal_text(const QString & text)
{
  _log->setReadOnly(false);
  QTextCursor cursor = _log->textCursor();
  QTextCharFormat text_format;
  text_format.setForeground(Qt::darkBlue);
  cursor.insertText(text, text_format);
  _log->setTextCursor(cursor);
  _log->setReadOnly(true);
}

void 
BL::JobsManager_QT::write_error_text(const QString & text)
{
  QTextCursor cursor = _log->textCursor();
  QTextCharFormat text_format;
  text_format.setForeground(Qt::red);
  cursor.insertText(text, text_format);
  _log->setTextCursor(cursor);
}
