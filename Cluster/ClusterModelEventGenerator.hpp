// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ClusterModelEventGenerator
#define WAC_ClusterModelEventGenerator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Hierarchy
// - Event
//   - Cell
//     - Cluster
//       - Particle
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Task.hpp"
#include "ClusterModelEvent.hpp"
#include "ClusterModelConfiguration.hpp"
#include "RandomGenerators.hpp"
#include "TGenPhaseSpace.h"
#include "Particle.hpp"

class ClusterModelEventGenerator : public Task
{
public:
  
  ClusterModelEventGenerator(const TString &  name,
                             TaskConfiguration * configuration,
                             ClusterModelEvent * event);
  virtual ~ClusterModelEventGenerator();
  virtual void initialize();
  virtual void reset();
  void execute();

  Event * getClusterModelEvent();

  ////////////////////////////////////////////
  // Data members
  ////////////////////////////////////////////

  ScalarIntRandomGenerator    * cellMultiplicityGenerator;
  ScalarDoubleRandomGenerator * cellBetaGenerator;
  ScalarDoubleRandomGenerator * cellPhiGenerator;
  ScalarIntRandomGenerator    * clusterMultiplicityGenerator;
  ScalarDoubleRandomGenerator * clusterRapidityGenerator;
  ScalarDoubleRandomGenerator * clusterMassGenerator;
  ScalarDoubleRandomGenerator * clusterMomentumGenerator;
  VectorRandomGenerator       * clusterDirectionGenerator;
  VectorRandomGenerator       * particleDirectionGenerator;
  ScalarIntRandomGenerator    * particleMultiplicityGenerator;
  ScalarDoubleRandomGenerator * particleMomenumGenerator;
  int particleIndex;
  TLorentzVector * p1;
  
  double masses[100];
  double massesSq[100];
  
 // static constexpr double pionMass = 0.14;
 double pionMass;

  ClusterModelEvent * cmEvent;
   ClassDef(ClusterModelEventGenerator,0)
};

#endif /* WAC_ClusterModelEventGenerator */

