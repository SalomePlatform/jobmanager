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

#include "BL_JobsManager.hxx"

BL::JobsManager::JobsManager(BL::SALOMEServices * salome_services)
{
  DEBTRACE("Creating BL::JobsManager");
  BL_ASSERT(salome_services);
  _salome_services = salome_services;
  _observer = NULL;
}

BL::JobsManager::~JobsManager()
{
  DEBTRACE("Destroying BL::JobsManager");

  // Delete All Jobs !
  _jobs_it = _jobs.begin();
  for(;_jobs_it != _jobs.end();_jobs_it++)
    delete _jobs_it->second;
}

void 
BL::JobsManager::setObserver(BL::Observer * observer)
{
  BL_ASSERT(observer);
  _observer = observer;
}

BL::Job *
BL::JobsManager::createJob(const std::string & name)
{
  DEBTRACE("createJob BL::JobsManager");

  BL::Job * new_job = NULL;

  _jobs_it = _jobs.find(name);
  if (_jobs_it == _jobs.end())
  {
    new_job = new BL::Job(name);

    // Ajout dans la liste
    _jobs[name] = new_job;
  }
  else
    DEBTRACE("createJob Error !!!! Job already exist: " << name);

  return new_job;
}

void
BL::JobsManager::addJobToLauncher(const std::string & name)
{
  DEBTRACE("addJobToLauncher BL::JobsManager");

  _jobs_it = _jobs.find(name);
  if (_jobs_it == _jobs.end())
  {
    // TODO: SHOULD SEND an exeception...
    DEBMSG("[addJobToLauncher] failed, job was not found");
  }
  
  std::string result = "";
  result = _salome_services->create_job(_jobs_it->second);
  if (_observer)
    if (result != "")
    {
      _observer->sendEvent("create_job", "Error", name, result);
      _jobs_it->second->setState(BL::Job::ERROR);
    }
    else
      _observer->sendEvent("create_job", "Ok", name, "");
}

void
BL::JobsManager::removeJob(const std::string & name)
{
  DEBTRACE("removeJob BL::JobsManager");
  _jobs_it = _jobs.find(name);
  if (_jobs_it != _jobs.end())
  {
    std::string result = "";
    if (_jobs_it->second->getSalomeLauncherId() != -1)
      result = _salome_services->delete_job(_jobs_it->second);

    delete _jobs_it->second;
    _jobs.erase(_jobs_it->first);

    if (_observer)
      if (result != "")
	_observer->sendEvent("delete_job", "Error", name, result);
      else
	_observer->sendEvent("delete_job", "Ok", name, "");
  }
  else
    DEBTRACE("removeJob Error !!!! Job does not exist: " << name);
}

BL::Job *
BL::JobsManager::getJob(const std::string & name)
{
  DEBTRACE("getJob BL::JobsManager called");
  return _jobs[name];
}

std::map<std::string, BL::Job *> & 
BL::JobsManager::getJobs()
{
  return _jobs;
}

bool 
BL::JobsManager::job_already_exist(const std::string & name)
{
  _jobs_it = _jobs.find(name);

  if (_jobs_it == _jobs.end())
    return false;
  else
    return true;
}

void
BL::JobsManager::start_job(const std::string & name)
{
  DEBTRACE("BL::JobsManager::start_job called for job: " << name);

  // Check job exits
  _jobs_it = _jobs.find(name);
  if (_jobs_it == _jobs.end())
  {
    DEBTRACE("BL::JobsManager::start_job job unknown: " << name);
    return;
  }
  else
  {
    _thread_mutex.lock();
    BL::Job * job = _jobs_it->second; 
    // Check Job Exec State - multithread protection
    if (job->getThreadState() == BL::Job::NOTHING)
    {
      // Prepare Info for thread
      BL::JobsManager::thread_info * ti = new thread_info();
      ti->object_ptr = this;
      ti->job_name = name;
      job->setState(BL::Job::IN_PROCESS);
      job->setThreadState(BL::Job::STARTING);
      omni_thread::create(BL::JobsManager::starting_job_thread, ti);
    }
    else
    {
      DEBTRACE("BL::JobsManager::start_job nothin to do, job already starting, job name: " << name);
    }
    _thread_mutex.unlock();
  }
}

void 
BL::JobsManager::starting_job_thread(void * object_ptr)
{
  DEBTRACE("starting_job_thread BL::JobsManager called");
  BL::JobsManager::thread_info * ti = reinterpret_cast<BL::JobsManager::thread_info*>(object_ptr);
  BL::JobsManager * object = ti->object_ptr;
  std::string job_name =  ti->job_name;
  BL::Job * job = object->getJob(job_name);

  std::string result = object->_salome_services->start_job(job);

  object->_thread_mutex.lock();
  // End
  if (result == "")
  {
    job->setState(BL::Job::QUEUED);
    job->setThreadState(BL::Job::NOTHING);
    if (object->_observer)
      object->_observer->sendEvent("start_job", "Ok", job_name, "");
  }
  else
  {
    job->setState(BL::Job::ERROR);
    job->setThreadState(BL::Job::NOTHING);
    if (object->_observer)
      object->_observer->sendEvent("start_job", "Error", job_name, result);
  }
  object->_thread_mutex.unlock();
}

void
BL::JobsManager::refresh_jobs()
{
  DEBTRACE("refresh_jobs BL::JobsManager called");

  omni_thread::create(BL::JobsManager::refresh_job, this);
}

void
BL::JobsManager::refresh_job(void * object_ptr)
{
  DEBTRACE("refresh_job BL::JobsManager called");
  BL::JobsManager * object = reinterpret_cast<BL::JobsManager*>(object_ptr);

  //iterate on all jobs
  _jobs_map::iterator jobs_it;
  jobs_it = object->_jobs.begin();
  for(;jobs_it != object->_jobs.end();jobs_it++)
  {
    BL::Job * job = jobs_it->second;
    if (job->getSalomeLauncherId() != -1)
    {
      object->_thread_mutex.lock();
      if (job->getThreadState() == BL::Job::NOTHING)
      {
	BL::Job::State job_state = job->getState();
	if (job_state != BL::Job::FINISHED or job_state != BL::Job::ERROR)
	{
	  std::string result = object->_salome_services->refresh_job(job);
	  if (result == "CREATED")
	  {
	    if (job_state != BL::Job::CREATED)
	    {
	      job->setState(BL::Job::CREATED);
	      if (object->_observer)
		object->_observer->sendEvent("refresh_job", "Ok", job->getName(), "new state");
	    }
	  }
	  else if (result == "QUEUED")
	  {
	    if (job_state != BL::Job::QUEUED)
	    {
	      job->setState(BL::Job::QUEUED);
	      if (object->_observer)
		object->_observer->sendEvent("refresh_job", "Ok", job->getName(), "new state");
	    }
	  }
	  else if (result == "IN_PROCESS")
	  {
	    if (job_state != BL::Job::IN_PROCESS)
	    {
	      job->setState(BL::Job::IN_PROCESS);
	      if (object->_observer)
		object->_observer->sendEvent("refresh_job", "Ok", job->getName(), "new state");
	    }
	  }
	  else if (result == "RUNNING")
	  {
	    if (job_state != BL::Job::RUNNING)
	    {
	      job->setState(BL::Job::RUNNING);
	      if (object->_observer)
		object->_observer->sendEvent("refresh_job", "Ok", job->getName(), "new state");
	    }
	  }
	  else if (result == "PAUSED")
	  {
	    if (job_state != BL::Job::PAUSED)
	    {
	      job->setState(BL::Job::PAUSED);
	      if (object->_observer)
		object->_observer->sendEvent("refresh_job", "Ok", job->getName(), "new state");
	    }
	  }
	  else if (result == "FINISHED")
	  {
	    if (job_state != BL::Job::FINISHED)
	    {
	      job->setState(BL::Job::FINISHED);
	      if (object->_observer)
		object->_observer->sendEvent("refresh_job", "Ok", job->getName(), "new state");
	    }
	  }
	  else if (result == "ERROR")
	  {
	    if (job_state != BL::Job::ERROR)
	    {
	      job->setState(BL::Job::ERROR);
	      if (object->_observer)
		object->_observer->sendEvent("refresh_job", "Ok", job->getName(), "new state");
	    }
	  }
	  else
	  {
	    // Error using launcher...
	    if (object->_observer)
	      object->_observer->sendEvent("refresh_job", "Error", job->getName(), result);
	  }
	}
      }
      object->_thread_mutex.unlock();
    }
  }
}

void
BL::JobsManager::get_results_job(const std::string & name)
{
  DEBTRACE("get_results_job BL::JobsManager called");
   
  // Check job exits
  _jobs_it = _jobs.find(name);
  if (_jobs_it == _jobs.end())
  {
    DEBTRACE("BL::JobsManager::get_results_job job unknown: " << name);
    return;
  }
  else
  {
    BL::Job * job = _jobs_it->second; 
    if (job->getState() == BL::Job::FINISHED)
    {
      // Prepare Info for thread
      BL::JobsManager::thread_info * ti = new thread_info();
      ti->object_ptr = this;
      ti->job_name = name;
      omni_thread::create(BL::JobsManager::get_results_job_thread, ti);
    }
    else
    {
      DEBTRACE("BL::JobsManager::get_results_job job bad job state !");
      return;
    }
  }
}

void 
BL::JobsManager::get_results_job_thread(void * object_ptr)
{
  DEBTRACE("get_results_job_thread BL::JobsManager called");
  BL::JobsManager::thread_info * ti = reinterpret_cast<BL::JobsManager::thread_info*>(object_ptr);
  BL::JobsManager * object = ti->object_ptr;
  std::string job_name =  ti->job_name;
  BL::Job * job = object->getJob(job_name);

  object->_thread_mutex_results.lock();
  std::string result = object->_salome_services->get_results_job(job);

  // End
  if (result == "")
  {
    if (object->_observer)
      object->_observer->sendEvent("get_results_job", "Ok", job_name, "");
  }
  else
  {
    if (object->_observer)
      object->_observer->sendEvent("get_results_job", "Error", job_name, result);
  }
  object->_thread_mutex_results.unlock();
}
