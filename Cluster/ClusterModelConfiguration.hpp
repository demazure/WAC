// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ClusterModelConfiguration
#define WAC_ClusterModelConfiguration
#include "GeneratorConfiguration.hpp"

class ClusterModelConfiguration : public GeneratorConfiguration
{
public:

  ClusterModelConfiguration(const TString & name, const TString & type, const TString & version);
  ~ClusterModelConfiguration();
  virtual void printConfiguration(ostream & os);

  //////////////////////////////////////////////
  //  cell generation parameters
  //////////////////////////////////////////////

  int    cellMultiplicity_Random;
  int    cellMultiplicity_Fixed;
  int    cellMultiplicity_Minimum;
  int    cellMultiplicity_Maximum;
  double cellMultiplicity_Slope;

  int    cellBeta_Random;
  double cellBeta_Fixed;
  double cellBeta_Minimum;
  double cellBeta_Maximum;

  int    cellPhi_Random;
  double cellPhi_Fixed;
  double cellPhi_Minimum;
  double cellPhi_Maximum;

  int    flowPlanes_Option;

  //////////////////////////////////////////////
  //  cluster generation parameters
  //////////////////////////////////////////////
  int    clusterMultiplicity_Random;
  int    clusterMultiplicity_Fixed;
  int    clusterMultiplicity_Minimum;
  int    clusterMultiplicity_Maximum;
  double clusterMultiplicity_Slope;

  int    clusterMass_Random;
  double clusterMass_Fixed;
  double clusterMass_Minimum;
  double clusterMass_Maximum;
  double clusterMass_Slope;

  int    clusterRapidity_Random;
  double clusterRapidity_Fixed;
  double clusterRapidity_Minimum;
  double clusterRapidity_Maximum;
  double clusterRapidity_Slope;

  int    clusterMomentum_Random;
  double clusterMomentum_Fixed;
  double clusterMomentum_Minimum;
  double clusterMomentum_Maximum;
  double clusterMomentum_Slope;

  //////////////////////////////////////////////
  //  particle generation parameters
  //////////////////////////////////////////////
  int    particleMultiplicity_Random;
  int    particleMultiplicity_Fixed;
  int    particleMultiplicity_Minimum;
  int    particleMultiplicity_Maximum;
  double particleMultiplicity_Slope;

  int    particleMomenum_Random;
  double particleMomenum_Fixed;
  double particleMomenum_Minimum;
  double particleMomenum_Maximum;
  double particleMomenum_Slope;
  
  double minMassFactor;

     ClassDef(ClusterModelConfiguration,0)
  
};


#endif /* WAC_ClusterModelConfiguration */
