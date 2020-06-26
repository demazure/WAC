// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining ClusterModelEvent
 */

#include "ClusterModelEvent.hpp"

//extern Factory<Particle> * particleFactory;
//extern Factory<Particle> * getParticleFactory();

ClassImp(Factory<Cluster>);
//Factory<Cluster> * clusterFactory = nullptr;
//extern Factory<Cluster> * getClusterFactory(int nMax=100)
//{
//  if (!clusterFactory)
//    {
//    clusterFactory = new Factory<Cluster>();
//    clusterFactory->initialize(nMax);
//    }
//  return clusterFactory;
//}

ClassImp(Factory<Cell>);
//Factory<Cell> * cellFactory = nullptr;
//extern Factory<Cell> * getCellFactory(int nMax=100)
//{
//   if (!cellFactory)
//     {
//     cellFactory = new Factory<Cell>();
//     cellFactory->initialize(nMax);
//     }
//   return cellFactory;
//}



ClassImp(ClusterModelEvent);


//////////////////////////
// CTOR
//////////////////////////
ClusterModelEvent::ClusterModelEvent()
:
Event(),
index(0),
nCells(0),
nClusters(0)
{
}

//////////////////////////
// DTOR
//////////////////////////
ClusterModelEvent::~ClusterModelEvent()
{
}

////////////////////////////////////////////////////
// Call before restarting simulation
////////////////////////////////////////////////////
void ClusterModelEvent::clear()
{
  index      = 0;
  nCells     = 0;
  nClusters  = 0;
  //nParticles = 0;
//  getCellFactory()->reset();
//  getClusterFactory()->reset();
  Cell::getFactory()->reset();
  Cluster::getFactory()->reset();
  Event::clear();
  //getParticleFactory()->reset();
}

////////////////////////////////////////////////////
// Call before generating new ClusterModelEvent
////////////////////////////////////////////////////
void ClusterModelEvent::reset()
{
  index++;
  nCells     = 0;
  nClusters  = 0;
  //nParticles = 0;
  //getCellFactory()->reset();
  Cell::getFactory()->reset();
  Cluster::getFactory()->reset();
   Event::reset();
 // getParticleFactory()->reset();
}

////////////////////////////////////////////////////
// Add given cell to current ClusterModelEvent
////////////////////////////////////////////////////
void ClusterModelEvent::add(Cell *cell)
{
  nCells++;
  nClusters  += cell->nClusters;
  nParticles += cell->nParticles;
}

////////////////////////////////////////////////////
// Get the cell at the given index
////////////////////////////////////////////////////
Cell * ClusterModelEvent::getCellAt(int index)
{
  if (index>=0 && index<nCells)
    {
    return Cell::getFactory()->getObjectAt(index);
    }
  else
    return 0;
}


Cluster * ClusterModelEvent::getClusterAt(int index)
{
  if (index>=0 && index<nClusters)
    {
    return Cluster::getFactory()->getObjectAt(index);
    }
  else
    return 0;
}


///////////////////////////////////////////////////////
// Print properties of this ClusterModelEvent at the given output
///////////////////////////////////////////////////////
void ClusterModelEvent::printProperties(ostream & output)
{
  output << "----------------------------------------------------" << endl;
  output << "ClusterModelEvent : " << index << endl;
  output << "           nCells : " << nCells << endl;
  output << "        nClusters : " << nClusters << endl;
  output << "       nParticles : " << nParticles << endl;

  for (int iCell=0; iCell<nCells; iCell++)
    {
    getCellAt(iCell)->printProperties(output);
    }
}

 ClusterModelEvent * ClusterModelEvent::getClusterModelEvent()
{
  if (!event)
    {
    event = new ClusterModelEvent();
    }
  return (ClusterModelEvent*) event;
}

int ClusterModelEvent::nCellsMax = 100;
