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

#include "BL_Job.hxx"

BL::Job::Job()
{
  DEBTRACE("Creating BL::Job");
  _name = "";
  _yacs_file = "";
  _command = "";
  _env_file = "";
  _batch_params.batch_directory = "";
  _batch_params.maximum_duration = "";
  _batch_params.expected_memory = "";
  _batch_params.nb_proc = 0;
  _files_params.result_directory = "";
  _machine_choosed = "";
  _batch_queue = "";
  _state = BL::Job::CREATED;
  _thread_state = BL::Job::NOTHING;
  _salome_launcher_id = -1;
}

BL::Job::Job(const std::string & name)
{
  DEBTRACE("Creating BL::Job with name : " << name);
  _name = name;
  _yacs_file = "";
  _command = "";
  _env_file = "";
  _batch_params.batch_directory = "";
  _batch_params.maximum_duration = "";
  _batch_params.expected_memory = "";
  _batch_params.nb_proc = 0;
  _files_params.result_directory = "";
  _machine_choosed = "";
  _batch_queue = "";
  _state = BL::Job::CREATED;
  _thread_state = BL::Job::NOTHING;
  _salome_launcher_id = -1;
}

BL::Job::~Job()
{}

void
BL::Job::setName(const std::string & name)
{
  _name = name;
}

std::string
BL::Job::getName()
{
  return _name;
}

void 
BL::Job::setType(BL::Job::JobType type)
{
  _type = type;
}

BL::Job::JobType
BL::Job::getType()
{
  return _type;
}

void 
BL::Job::setYACSFile(std::string & yacs_file)
{
  _yacs_file = yacs_file;
}

std::string & 
BL::Job::getYACSFile()
{
  return _yacs_file;
}

void 
BL::Job::setCommand(std::string & command)
{
  _command = command;
}

std::string & 
BL::Job::getCommand()
{
  return _command;
}

void 
BL::Job::setEnvFile(std::string & env_file)
{
  _env_file = env_file;
}

std::string & 
BL::Job::getEnvFile()
{
  return _env_file;
}

void 
BL::Job::setBatchParameters(BL::Job::BatchParam & param)
{
  _batch_params.batch_directory = param.batch_directory;
  _batch_params.maximum_duration = param.maximum_duration;
  _batch_params.expected_memory = param.expected_memory;
  _batch_params.nb_proc = param.nb_proc;
}

BL::Job::BatchParam & 
BL::Job::getBatchParameters()
{
  return _batch_params;
}

void 
BL::Job::setFilesParameters(BL::Job::FilesParam & param)
{
  _files_params.result_directory = param.result_directory;
  _files_params.input_files_list = param.input_files_list;
  _files_params.output_files_list = param.output_files_list;
}

BL::Job::FilesParam & 
BL::Job::getFilesParameters()
{
  return _files_params;
}

void
BL::Job::setMachine(std::string & machine)
{
  _machine_choosed = machine;
}

std::string &
BL::Job::getMachine()
{
  return _machine_choosed;
}

void
BL::Job::setBatchQueue(std::string & queue)
{
  _batch_queue = queue;
}

std::string &
BL::Job::getBatchQueue()
{
  return _batch_queue;
}

void 
BL::Job::setState(BL::Job::State state)
{
  _state = state;
}

BL::Job::State 
BL::Job::getState()
{
  return _state;
}

void 
BL::Job::setThreadState(BL::Job::ThreadState state)
{
  _thread_state = state;
}

BL::Job::ThreadState 
BL::Job::getThreadState()
{
  return _thread_state;
}

void 
BL::Job::setSalomeLauncherId(int id)
{
  _salome_launcher_id = id;
}

int 
BL::Job::getSalomeLauncherId()
{
  return _salome_launcher_id;
}
