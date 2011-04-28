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

#ifndef _BL_CREATEJOBWIZARD_HXX_
#define _BL_CREATEJOBWIZARD_HXX_

#include <QtGui>

#include "BL_Traces.hxx"
#include "BL_SALOMEServices.hxx"
#include "JM_ResourceCatalog.hxx"

#include <string>
#include <list>

namespace BL{

  class JobsManager_QT;
  class JobNamePage;
  class CreateJobWizard: virtual public QWizard
  {
    Q_OBJECT

    public:
      CreateJobWizard(BL::JobsManager_QT * jobs_manager, BL::SALOMEServices * salome_services);
      virtual ~CreateJobWizard();

      void setFilesList(QListWidget * input_files_list, QListWidget * output_files_list);

      void clone(const std::string & name);

    public slots:
      void end(int result);

    private:
      //Page Name
      QLineEdit * _nameLineEdit;
      BL::JobsManager_QT * _jobs_manager;
      QListWidget * _input_files_list;
      QListWidget * _output_files_list;

      BL::JobNamePage * _job_name_page;

    public:
      // Results
      std::string job_name;

      std::string yacs_file;
      std::string command;
      std::string python_salome_file;

      std::string env_file;

      std::string batch_directory;
      std::string maximum_duration;
      std::string expected_memory;
      int nb_proc;

      std::string result_directory;
      std::list<std::string> input_files_list;
      std::list<std::string> output_files_list;

      std::string resource_choosed;
      std::string batch_queue;

      bool start_job;
      int dump_yacs_state;

    public:
      enum {Page_JobName,
            Page_YACSSchema,
            Page_Command_Main_Definitions,
            Page_PythonSalome_Main_Definitions,
            Page_BatchParameters,
            Page_Files,
            Page_Resource,
            Page_Conclusion};
  };

  class JobNamePage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      JobNamePage(QWidget * parent, BL::JobsManager_QT * jobs_manager);
      virtual ~JobNamePage();

      virtual bool validatePage();
      virtual int nextId() const ;

    public slots:
      void yacs_schema_button(bool checked);
      void command_button(bool checked);
      void python_salome_button(bool checked);

    private:	  
      BL::JobsManager_QT * _jobs_manager;
      QLabel * _explanation;

    public:
      QRadioButton * _yacs_schema_button;
      QRadioButton * _command_button;
      QRadioButton * _python_salome_button;
      bool _check_name;
  };

  class ConclusionPage: virtual public QWizardPage
  {
    public:
      ConclusionPage(QWidget * parent);
      virtual ~ConclusionPage();

      virtual bool validatePage();
      virtual int nextId() const ;
  };

  class BatchParametersPage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      BatchParametersPage(QWidget * parent);
      virtual ~BatchParametersPage();

      virtual bool validatePage();
      virtual int nextId() const;
  };

  class FilesPage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      FilesPage(BL::CreateJobWizard * parent);
      virtual ~FilesPage();

      virtual int nextId() const;
      virtual bool validatePage();

    public slots:
      void choose_input_files();
      void remove_input_files();
      void input_itemSelectionChanged();
      void choose_local_directory();

      void add_output_file();
      void remove_output_files();
      void output_itemSelectionChanged();

    private:
      QPushButton * _input_files_button;
      QPushButton * _remove_input_files_button;
      QListWidget * _input_files_list;
      QLineEdit * _result_directory;

      QPushButton * _output_files_button;
      QPushButton * _remove_output_files_button;
      QListWidget * _output_files_list;
  };

  class ResourcePage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      ResourcePage(BL::CreateJobWizard * parent, BL::SALOMEServices * salome_services);
      virtual ~ResourcePage();

      virtual int nextId() const;
      virtual bool validatePage();

    public slots:
      void itemSelected(QListWidgetItem * item);

    private:
      JM::ResourceCatalog * _resource_list;
      QLineEdit * _resource_choosed;
      BL::SALOMEServices * _salome_services;
  };

  class YACSSchemaPage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      YACSSchemaPage(QWidget * parent);
      virtual ~YACSSchemaPage();

      virtual bool validatePage();
      virtual int nextId() const ;

    public slots:
      void choose_file();
      void choose_env_file();

    private:
      QLineEdit * _yacs_file_text;
      QLineEdit * _line_env_file;
  };

  class CommandMainPage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      CommandMainPage(QWidget * parent);
      virtual ~CommandMainPage();

      virtual bool validatePage();
      virtual int nextId() const ;

    public slots:
      void choose_command_file();
      void choose_env_file();

    private:
      QLineEdit * _line_command;
      QLineEdit * _line_env_file;
  };

  class PythonSalomeMainPage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      PythonSalomeMainPage(QWidget * parent);
      virtual ~PythonSalomeMainPage();

      virtual bool validatePage();
      virtual int nextId() const ;

    public slots:
      void choose_PythonSalome_file();
      void choose_env_file();

    private:
      QLineEdit * _line_PythonSalome;
      QLineEdit * _line_env_file;
  };
}

#endif

