// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_Cell
#define WAC_Cell
#include "Cluster.hpp"
#include "Factory.hpp"

class Cell
{
public:
  
  Cell();
  ~Cell();
  void reset();;
  Cluster * getClusterAt(int index);
  void add(Cluster * cluster);
  void boost(double beta_x, double beta_y, double beta_z);
  void boostZ(double beta_z);
  void printProperties(ostream & output);

  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  double beta;
  double betaX;
  double betaY;
  double betaZ;
  double phi;
  int nClusters;
  int nParticles;
  Cluster ** clusters;

  static int nClustersMax;
  static int factorySize;
  static Factory<Cell> * factory;
  static Factory<Cell> * getFactory();

  ClassDef(Cell,0)
};


#endif /* WAC_Cell */
