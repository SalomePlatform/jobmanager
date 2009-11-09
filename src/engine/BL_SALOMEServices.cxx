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
BL::SALOMEServices::start_job(BL::Job * job)
{
  return start_yacs_job(job);
}

std::string
BL::SALOMEServices::start_yacs_job(BL::Job * job)
{
  std::string ret = "";
  // Prepare Arguments
  const char * fileToExecute = job->getYACSFile().c_str();

  BL::Job::FilesParam files = job->getFilesParameters();
  Engines::FilesList_var filesToExport = new Engines::FilesList;
  filesToExport->length(files.input_files_list.size());
  std::list<std::string>::iterator it;
  int i = 0;
  for (it=files.input_files_list.begin() ; it != files.input_files_list.end(); it++)
  {
    filesToExport[i] = CORBA::string_dup((*it).c_str());
    i++;
  }
  Engines::FilesList_var filesToImport = new Engines::FilesList;
  filesToImport->length(files.output_files_list.size());
  i = 0;
  for (it=files.output_files_list.begin() ; it != files.output_files_list.end(); it++)
  {
    filesToImport[i] = CORBA::string_dup((*it).c_str());
    i++;
  }

  BL::Job::BatchParam cpp_batch_params =  job->getBatchParameters();
  Engines::BatchParameters_var batch_params = new Engines::BatchParameters;
  batch_params->batch_directory = CORBA::string_dup(cpp_batch_params.batch_directory.c_str());
  batch_params->expected_during_time = CORBA::string_dup(cpp_batch_params.expected_during_time.c_str());
  batch_params->mem = CORBA::string_dup(cpp_batch_params.expected_memory.c_str());
  batch_params->nb_proc = cpp_batch_params.nb_proc;

  Engines::MachineParameters_var machine = new Engines::MachineParameters;
  machine->hostname = CORBA::string_dup(job->getMachine().c_str());

  // Start Job !
  try
  {
    int job_id = _salome_launcher->submitSalomeJob(fileToExecute, filesToExport,
						   filesToImport, batch_params, machine);
    job->setSalomeLauncherId(job_id);
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in submitSalomeJob !");
    ret = ex.details.text.in();
  }
  return ret;
}

std::string
BL::SALOMEServices::refresh_job(BL::Job * job)
{
  std::string ret = "";

  Engines::MachineParameters_var machine = new Engines::MachineParameters;
  machine->hostname = CORBA::string_dup(job->getMachine().c_str());

  // Refresh Job !
  try
  {
    CORBA::String_var result = _salome_launcher->queryJob(job->getSalomeLauncherId(), machine);
    std::string res = result.in();
    if (res == "Running")
      return "RUNNING";
    else if (res == "Done")
      return "FINISHED";
    else
      return result.in();
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in refresh_job !");
    ret = ex.details.text.in();
  }
  return ret;
}

std::string
BL::SALOMEServices::delete_job(BL::Job * job)
{
  std::string ret = "";

  Engines::MachineParameters_var machine = new Engines::MachineParameters;
  machine->hostname = CORBA::string_dup(job->getMachine().c_str());

  // Delete Job !
  try
  {
    _salome_launcher->deleteJob(job->getSalomeLauncherId(), machine);
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in refresh_job !");
    ret = ex.details.text.in();
  }
  return ret;
}

std::string
BL::SALOMEServices::get_results_job(BL::Job * job)
{
  std::string ret = "";

  Engines::MachineParameters_var machine = new Engines::MachineParameters;
  machine->hostname = CORBA::string_dup(job->getMachine().c_str());

  BL::Job::FilesParam files = job->getFilesParameters();
  CORBA::String_var directory = CORBA::string_dup(files.result_directory.c_str());

  // get job results !
  try
  {
    _salome_launcher->getResultsJob(directory, job->getSalomeLauncherId(), machine);
  }
  catch (const SALOME::SALOME_Exception & ex)
  {
    DEBTRACE("SALOME Exception in refresh_job !");
    ret = ex.details.text.in();
  }
  return ret;
}
