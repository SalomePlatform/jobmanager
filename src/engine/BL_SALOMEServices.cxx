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

#include "BL_SALOMEServices.hxx"

BL::SALOMEServices::SALOMEServices()
{
  int nbargs = 0; char **args = 0;
  _orb = CORBA::ORB_init (nbargs, args);
  _salome_naming_service = NULL;
  _lcc = NULL;
  _state = false;
}

BL::SALOMEServices::~SALOMEServices()
{
  if (_salome_naming_service)
    delete _salome_naming_service;
  if (_lcc)
    delete _lcc;
}

bool
BL::SALOMEServices::initNS()
{
  bool return_value = true;
  _salome_naming_service = new SALOME_NamingService(_orb);
  _lcc = new SALOME_LifeCycleCORBA(_salome_naming_service);
  CORBA::Object_var obj = _salome_naming_service->Resolve("/SalomeLauncher");
  _salome_launcher = Engines::SalomeLauncher::_narrow(obj);

  if (CORBA::is_nil(_salome_launcher))
  {
    DEBTRACE("SALOME Launcher is not reachable!")
    return_value = false;
  }

  obj = _salome_naming_service->Resolve("/ResourcesManager");
  _resources_manager = Engines::ResourcesManager::_narrow(obj);
  if (CORBA::is_nil(_resources_manager))
  {
    DEBTRACE("SALOME Resource Manager is not reachable !");
    return_value = false;
  }

  _state = return_value;
  return return_value;
}

std::list<std::string> 
BL::SALOMEServices::getMachineList()
{
  std::list<std::string> machine_list;
  
  if (_state)
  {
    Engines::MachineParameters params;
    _lcc->preSet(params);
    Engines::MachineList * machineList = _resources_manager->GetFittingResources(params);
    for (int i = 0; i < machineList->length(); i++)
    {
      const char* aMachine = (*machineList)[i];
      machine_list.push_back(aMachine);
    }
  }

  return machine_list;
}

std::string
BL::SALOMEServices::create_job(BL::Job * job)
{
  DEBTRACE("Begin of create_job");
  std::string ret = "";
  Engines::JobParameters_var job_parameters = new Engines::JobParameters;

  // Job type
  if (job->getType() == BL::Job::COMMAND)
  {
    job_parameters->job_type = CORBA::string_dup("command");
    job_parameters->command = CORBA::string_dup(job->getCommand().c_str());
    job_parameters->env_file = CORBA::string_dup(job->getEnvFile().c_str());
  }
  else if (job->getType() == BL::Job::YACS_SCHEMA)
  {
    job_parameters->job_type = CORBA::string_dup("yacs_file");
    job_parameters->yacs_file = CORBA::string_dup(job->getYACSFile().c_str());
  }

  // Files
  BL::Job::FilesParam files = job->getFilesParameters();
  std::list<std::string>::iterator it;
  int i,j = 0;

  job_parameters->in_files.length(files.input_files_list.size());
  for (it = files.input_files_list.begin() ; it != files.input_files_list.end(); it++)
  {
    job_parameters->in_files[i] = CORBA::string_dup((*it).c_str());
    i++;
  }

  job_parameters->out_files.length(files.output_files_list.size());
  for (it = files.output_files_list.begin() ; it != files.output_files_list.end(); it++)
  {
    job_parameters->out_files[j] = CORBA::string_dup((*it).c_str());
    j++;
  }
  job_parameters->local_directory = CORBA::string_dup("");
  job_parameters->result_directory = CORBA::string_dup(files.result_directory.c_str());

  BL::Job::BatchParam cpp_batch_params =  job->getBatchParameters();
  job_parameters->work_directory = CORBA::string_dup(cpp_batch_params.batch_directory.c_str());

  // Resource
  job_parameters->maximum_during_time = CORBA::string_dup(cpp_batch_params.maximum_during_time.c_str());
  job_parameters->resource_required.hostname = CORBA::string_dup(job->getMachine().c_str());
  job_parameters->resource_required.nb_node = cpp_batch_params.nb_proc;

  // Memory
  CORBA::Long memory;
  std::string ram = cpp_batch_params.expected_memory.substr(0,cpp_batch_params.expected_memory.size()-2);
  std::istringstream iss(ram);
  iss >> memory;
  std::string unity = cpp_batch_params.expected_memory.substr(cpp_batch_params.expected_memory.size()-2, 2);
  if((unity.find("gb") != std::string::npos))
    memory = memory * 1024;
  job_parameters->resource_required.mem_mb = memory;

  // Create Job
  try
  {
    int job_id = _salome_launcher->createJob(job_parameters);
    job->setSalomeLauncherId(job_id);
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in createJob !");
    ret = ex.details.text.in();
  }
  return ret;
}

std::string
BL::SALOMEServices::start_job(BL::Job * job)
{
  std::string ret = "";
  // Launch Job !
  try
  {
    _salome_launcher->launchJob(job->getSalomeLauncherId());
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in launchJob !");
    ret = ex.details.text.in();
  }
  return ret;
}

std::string
BL::SALOMEServices::refresh_job(BL::Job * job)
{
  std::string ret = "";

  // Refresh Job !
  try
  {
    CORBA::String_var result = _salome_launcher->getJobState(job->getSalomeLauncherId());
    ret = result.in();
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in getJobState !");
    ret = ex.details.text.in();
  }
  return ret;
}

std::string
BL::SALOMEServices::delete_job(BL::Job * job)
{
  std::string ret = "";
  // Delete Job !
  try
  {
    _salome_launcher->removeJob(job->getSalomeLauncherId());
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in removeJob !");
    ret = ex.details.text.in();
  }
  return ret;
}

std::string
BL::SALOMEServices::get_results_job(BL::Job * job)
{
  std::string ret = "";

  BL::Job::FilesParam files = job->getFilesParameters();
  CORBA::String_var directory = CORBA::string_dup(files.result_directory.c_str());

  // get job results !
  try
  {
    _salome_launcher->getJobResults(job->getSalomeLauncherId(), directory);
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in refresh_job !");
    ret = ex.details.text.in();
  }
  return ret;
}
