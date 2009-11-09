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

#ifndef _BL_JOBSMANAGER_HXX_
#define _BL_JOBSMANAGER_HXX_

#include "BL_Job.hxx"
#include "BL_SALOMEServices.hxx"
#include "BL_Traces.hxx"
#include "BL_Observer.hxx"

#include <map>
#include <string>

#include <omnithread.h>

namespace BL{

  class JobsManager
  {
    public:
      JobsManager(BL::SALOMEServices * salome_services);
      virtual ~JobsManager();

      void setObserver(BL::Observer * observer);

      BL::Job * addNewJob(const std::string & name);
      void removeJob(const std::string & name);

      BL::Job * getJob(const std::string & name);
      std::map<std::string, BL::Job *> & getJobs();

      bool job_already_exist(const std::string & name);

      virtual void start_job(const std::string & name);
      virtual void refresh_jobs();
      virtual void get_results_job(const std::string & name);

      static void starting_job_thread(void * object_ptr);
      static void get_results_job_thread(void * object_ptr);
      static void refresh_job(void * object_ptr);

      struct thread_info
      {
	BL::JobsManager * object_ptr;
	std::string job_name;
      };

    protected:
      BL::SALOMEServices * _salome_services;
      BL::Observer * _observer;

    private:
      typedef std::map<std::string, BL::Job *> _jobs_map;
      _jobs_map _jobs;
      _jobs_map::iterator _jobs_it;

      omni_mutex _thread_mutex;
      omni_mutex _thread_mutex_results;
  };

}

#endif

