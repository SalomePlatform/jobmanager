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

#ifndef _BL_SALOMESERVICES_HXX_
#define _BL_SALOMESERVICES_HXX_

#include "BL_Traces.hxx"
#include "BL_Job.hxx"

#include "SALOME_NamingService.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "SALOME_ContainerManager.hh"

#include <map>
#include <list>
#include <string>

namespace BL{

  struct ResourceDescr
  {
    std::string name;
    std::string hostname;
    std::string protocol;
    std::string username;
    std::string applipath;
    std::list<std::string> componentList;

    std::string OS;
    unsigned int mem_mb;
    unsigned int cpu_clock;
    unsigned int nb_node;
    unsigned int nb_proc_per_node;
    std::string batch;
    std::string mpiImpl;
    std::string iprotocol;
  };

  class SALOMEServices
  {
    public:
      SALOMEServices();
      virtual ~SALOMEServices();

      bool initNS();

      std::list<std::string> getResourceList();
      BL::ResourceDescr getResourceDescr(const std::string& name);
      void addResource(BL::ResourceDescr & new_resource);
      void removeResource(const std::string & name);

      std::string create_job(BL::Job * job);
      std::string start_job(BL::Job * job);
      std::string refresh_job(BL::Job * job);
      std::string delete_job(BL::Job * job);
      std::string get_results_job(BL::Job * job);

    private:
      CORBA::ORB_var _orb;
      SALOME_NamingService * _salome_naming_service;
      SALOME_LifeCycleCORBA * _lcc;
      Engines::SalomeLauncher_var _salome_launcher;
      Engines::ResourcesManager_var _resources_manager;

      bool _state;
  };

}

#endif


