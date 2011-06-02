// Copyright (C) 2009-2011  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#ifndef _BL_QMODELMANAGER_HXX_
#define _BL_QMODELMANAGER_HXX_

#include <QtGui>

#include "BL_Job.hxx"
#include "BL_JobsManager_QT.hxx"

namespace BL{

  class QModelManager: virtual public QObject
  {
    Q_OBJECT

    public:
      QModelManager(QObject * parent, BL::JobsManager_QT * jobs_manager);
      virtual ~QModelManager();
      
      QStandardItemModel * getModel();

      void deleteJob(int row);
      void deleteJob(const QString & name);

    public slots:
      void new_job_added(const QString & name);
      void job_state_changed(const QString & name);
      void job_selected(const QModelIndex & index);
    
    private:
      QStandardItemModel * _model;
      BL::JobsManager_QT * _jobs_manager;
      
  };

}

#endif

