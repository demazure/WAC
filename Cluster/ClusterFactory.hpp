// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_ClusterFactory
#define WAC_ClusterFactory
#include <iostream>
#include "Factory.hpp"
#include "Cluster.hpp"
using namespace std;

/////////////////////////////////////////////////////////////
// class ParticleFactory
// Implements a factory/store pattern to serve objects of
// class Cluster.
/////////////////////////////////////////////////////////////
class ClusterFactory: public Factory
{
public:
  

  /////////////////////////////////////////////////////////////
  // CTOR.
  /////////////////////////////////////////////////////////////
  ClusterFactory(int initialCapacity = 1000);
  ~ClusterFactory();
  Cluster * getObject();
  Cluster * getObject(long index);

  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  Cluster ** objects;

    ClassDef(ClusterFactory,0)
};

static ClusterFactory * clusterFactory = new ClusterFactory(1000);


#endif /* WAC_ClusterFactory */
