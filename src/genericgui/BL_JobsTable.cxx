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

#include "BL_JobsTable.hxx"
#include "BL_Traces.hxx"

BL::JobsTable::JobsTable(QWidget *parent) : QTableView(parent)
{
  DEBTRACE("Creating BL::JobsTable");
  BL_ASSERT(parent);
  _parent = parent;

  // Init
  setShowGrid(false);
  setCornerButtonEnabled(false);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setAlternatingRowColors(true);
  setSortingEnabled(true);

  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  QHeaderView * header_view = verticalHeader();
  header_view->setClickable(false);
}

BL::JobsTable::~JobsTable()
{
  DEBTRACE("Destroying BL::JobsTable");
}

bool
BL::JobsTable::selectCurrent()
{
  QModelIndex idx = currentIndex();
  if (idx.row() > -1)
    if (!isMultipleSelected())
    {
      DEBTRACE("SELECT CURRENT ACTIVATION !!!");
      setCurrentIndex(idx);
      activated(idx);
      return true;
    }
  return false;
}

void
BL::JobsTable::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
  DEBTRACE("current changed");
  DEBTRACE("current row: " << current.row());
  if (current.row() > -1)
    if (!isMultipleSelected())
    {
      DEBTRACE("CURRENT CHANGED ACTIVATION !!!");
      setCurrentIndex(current);
      activated(current);
    }
}

bool
BL::JobsTable::isMultipleSelected()
{
  QModelIndexList list_of_selected = selectedIndexes();
  int length = list_of_selected.length();
  length = length / model()->columnCount();
  DEBTRACE("Number of selected rows: " << length);
  if (length > 1)
    return true;
  else
    return false;
}

QModelIndexList
BL::JobsTable::getSelectedIndexes()
{
  return selectedIndexes();
}
