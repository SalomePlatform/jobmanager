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

#ifndef _BL_CREATEJOBWIZARD_HXX_
#define _BL_CREATEJOBWIZARD_HXX_

#include <QtGui>

#include "BL_Traces.hxx"
#include "BL_JobsManager_QT.hxx"
#include "BL_SALOMEServices.hxx"

#include <string>
#include <list>

namespace BL{

  class JobsManager_QT;
  class CreateJobWizard: virtual public QWizard
  {
    Q_OBJECT

    public:
      CreateJobWizard(BL::JobsManager_QT * jobs_manager, BL::SALOMEServices * salome_services);
      virtual ~CreateJobWizard();

      void setFilesList(QListWidget * input_files_list, QListWidget * output_files_list);

    public slots:
      void end(int result);

    private:
      //Page Name
      QLineEdit * _nameLineEdit;
      BL::JobsManager_QT * _jobs_manager;
      QListWidget * _input_files_list;
      QListWidget * _output_files_list;

    public:
      // Results
      std::string job_name;
      
      std::string yacs_file;

      std::string command;
      std::string env_file;

      std::string batch_directory;
      std::string maximum_during_time;
      std::string expected_memory;
      int nb_proc;

      std::string result_directory;
      std::list<std::string> input_files_list;
      std::list<std::string> output_files_list;

      std::string machine_choosed;

      bool start_job;

    public:
      enum {Page_JobName, Page_YACSSchema, Page_BatchParameters, Page_Files, 
	Page_Command_Main_Definitions, Page_Machine, Page_Conclusion};

      class JobNamePage: virtual public QWizardPage
      {
	public:
	  JobNamePage(QWidget * parent, BL::JobsManager_QT * jobs_manager);
	  virtual ~JobNamePage();

	  virtual bool validatePage();
	  virtual int nextId() const ;

	private:	  
	  BL::JobsManager_QT * _jobs_manager;
	  QRadioButton * _yacs_schema_button;
      };



      class ConclusionPage: virtual public QWizardPage
      {
	public:
	  ConclusionPage(QWidget * parent);
	  virtual ~ConclusionPage();

	  virtual bool validatePage();
	  virtual int nextId() const ;
      };
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

    private:
      QPushButton * _yacs_file_button;
      QLineEdit * _yacs_file_text;
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

      void add_output_file();
      void remove_output_files();
      void output_itemSelectionChanged();

    private:
      QPushButton * _input_files_button;
      QPushButton * _remove_input_files_button;
      QListWidget * _input_files_list;

      QPushButton * _output_files_button;
      QPushButton * _remove_output_files_button;
      QListWidget * _output_files_list;
  };

  class MachinePage: virtual public QWizardPage
  {
    Q_OBJECT

    public:
      MachinePage(BL::CreateJobWizard * parent, BL::SALOMEServices * salome_services);
      virtual ~MachinePage();

      virtual int nextId() const;
      virtual bool validatePage();

    public slots:
      void machine_itemSelectionChanged();

    private:
      QListWidget * _machine_list;
      QLineEdit * _machine_choosed;
      BL::SALOMEServices * _salome_services;
  };
}

#endif

