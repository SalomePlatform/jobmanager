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

#ifndef _BL_JOB_HXX_
#define _BL_JOB_HXX_

#include "BL_Traces.hxx"

#include <string>
#include <list>

namespace BL{

  class Job
  {
    public:
      Job();
      Job(const std::string & name);
      virtual ~Job();

      void setName(const std::string & name);
      std::string getName();

      enum JobType {YACS_SCHEMA, COMMAND};
      void setType(BL::Job::JobType type);
      BL::Job::JobType getType();

      void setYACSFile(std::string & yacs_file);
      std::string & getYACSFile();

      void setCommand(std::string & command);
      std::string & getCommand();
      void setEnvFile(std::string & env_file);
      std::string & getEnvFile();

      struct BatchParam
      {
	std::string batch_directory;
	std::string maximum_duration;
	std::string expected_memory;
	int nb_proc;
      };
      void setBatchParameters(BL::Job::BatchParam & param);
      BL::Job::BatchParam & getBatchParameters();

      struct FilesParam
      {
	std::string result_directory;
	std::list<std::string> input_files_list;
	std::list<std::string> output_files_list;
      };
      void setFilesParameters(BL::Job::FilesParam & param);
      BL::Job::FilesParam & getFilesParameters();

      void setResource(std::string & resource);
      std::string & getResource();

      void setBatchQueue(std::string & queue);
      std::string & getBatchQueue();

      enum State {CREATED, IN_PROCESS, QUEUED, RUNNING, PAUSED, FINISHED, ERROR};
      void setState(BL::Job::State state);
      BL::Job::State getState();

      enum ThreadState {NOTHING, STARTING}; 
      void setThreadState(BL::Job::ThreadState state);
      BL::Job::ThreadState getThreadState();

      void setSalomeLauncherId(int id);
      int getSalomeLauncherId();


    private:
      BL::Job::State _state;
      BL::Job::ThreadState _thread_state;

      std::string _name;

      BL::Job::JobType _type;
      std::string _yacs_file;
      std::string _command;
      std::string _env_file;
  
      BL::Job::BatchParam _batch_params;
      BL::Job::FilesParam _files_params;
      std::string _resource_choosed;
      std::string _batch_queue;

      int _salome_launcher_id;
  };

}

#endif
