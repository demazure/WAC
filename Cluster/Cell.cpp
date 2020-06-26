// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
/**
 \class Cell
 \ingroup WAC

 */

#include "Cell.hpp"
ClassImp(Cell);

Cell::Cell()
:
beta(0.0),
betaX(0.0),
betaY(0.0),
betaZ(0.0),
phi(0.0),
nClusters(0),
nParticles(0),
clusters(0)
 {
 clusters = new Cluster*[nClustersMax];
 }

 ////////////////////////////////////////////////////
 // DTOR
 ////////////////////////////////////////////////////
 Cell::~Cell()
 {
 }

 ////////////////////////////////////////////////////
 // Reset this cell
 ////////////////////////////////////////////////////
 void Cell::reset()
 {
 beta              = 0;
 phi               = 0;
 betaX             = 0;
 betaY             = 0;
 betaZ             = 0;
 nClusters         = 0;
 nParticles        = 0;
 }

 /////////////////////////////////////////////////////////////
 // Get the cluster (within this cell) at the given index
 /////////////////////////////////////////////////////////////
 Cluster * Cell::getClusterAt(int index)
 {
 if (index>=0 && index<nClusters)
   {
   return clusters[index];
   }
 else
   return 0;
 }

 /////////////////////////////////////////////////////////////
 // Add the given cluster to this cell
 /////////////////////////////////////////////////////////////
 void Cell::add(Cluster * cluster)
 {
// cout << "                 nClusters = " << nClusters << endl;
 clusters[nClusters] = cluster;
 nClusters++;
// cout << "                 nClusters = " << nClusters << endl;
 nParticles += cluster->nParticles;
 }

 void Cell::boost(double beta_x, double beta_y, double beta_z)
 {
 // boost the momentum of the whole cell vector
 // p.Boost(beta_x, beta_y, beta_z);

 // boost all clusters within the cell
 for (int iCluster=0; iCluster<nClusters; iCluster++)
   {
   clusters[iCluster]->boost(beta_x, beta_y, beta_z);
   }
 }

void Cell::boostZ(double beta_z)
{
// boost the momentum of the whole cell vector
// along the z axis
// boost all clusters within the cell
  for (int iCluster=0; iCluster<nClusters; iCluster++)
    {
    clusters[iCluster]->boostZ(beta_z);
    }
}


 /////////////////////////////////////////////////////////////
 // Print properties of this cell at the given output
 /////////////////////////////////////////////////////////////
 void Cell::printProperties(ostream & output)
 {
 output
 << "===========================================" << endl
 << "      Cell# " << endl
 << "      beta: " << beta << endl
 << "       phi: " << phi << endl
 << "     betaX: " << betaX << endl
 << "     betaY: " << betaY << endl
 << "     betaZ: " << betaZ << endl
 << " nClusters: " << nClusters << endl
 << "nParticles: " << nParticles << endl;
 for (int iCluster=0; iCluster<nClusters; iCluster++)
   {
   output
   << "===========================================" << endl
   << "       Cluster# " << iCluster << endl;
   clusters[iCluster]->printProperties(output);
   }
 }

int Cell::nClustersMax = 100;

int Cell::factorySize = 5000;
Factory<Cell> * Cell::factory = 0;
Factory<Cell> * Cell::getFactory()
{
  if (!factory)
    {
    factory = new Factory<Cell>();
    factory->initialize(factorySize);
    }
   return factory;
}
