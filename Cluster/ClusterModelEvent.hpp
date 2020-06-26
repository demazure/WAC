// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ClusterModelEvent
#define WAC_ClusterModelEvent
#include "Cell.hpp"
#include "Cluster.hpp"
#include "Particle.hpp"
#include "Event.hpp"

class ClusterModelEvent : public Event
{
protected:

  ClusterModelEvent();

public:
  

  virtual ~ClusterModelEvent();
  virtual void clear();
  virtual void reset();
  void add(Cell *cell);
  Cell * getCellAt(int index);
  Cluster * getClusterAt(int index);
  virtual void printProperties(ostream & output);

  //////////////////////////
  // Data Members
  //////////////////////////
  long index;
  int  nCells;
  int  nClusters;
  // int  nParticles; from base class

  static int nCellsMax;

  static ClusterModelEvent * getClusterModelEvent();

  ClassDef(ClusterModelEvent,0)


};



#endif /* WAC_ClusterModelEvent */
