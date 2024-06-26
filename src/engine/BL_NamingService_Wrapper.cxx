// Copyright (C) 2021-2024  CEA, EDF
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "BL_NamingService_Wrapper.hxx"

#include "SALOME_NamingService.hxx"
#include "SALOME_Fake_NamingService.hxx"
#include "KernelBasis.hxx"

BL_SALOME_NamingService_Wrapper::BL_SALOME_NamingService_Wrapper()
{
  this->initializeEffectiveNS();
}

BL_SALOME_NamingService_Wrapper::BL_SALOME_NamingService_Wrapper(CORBA::ORB_ptr orb)
{
  this->initializeEffectiveNS();
  this->_effective_ns->init_orb(orb);
}

BL_SALOME_NamingService_Wrapper::BL_SALOME_NamingService_Wrapper(const BL_SALOME_NamingService_Wrapper& other):_effective_ns(other._effective_ns->cloneCoVar())
{
}

void BL_SALOME_NamingService_Wrapper::initializeEffectiveNS()
{
  if(getSSLMode())
    _effective_ns.reset( new SALOME_Fake_NamingService );
  else
    _effective_ns.reset( new SALOME_NamingService );
}
