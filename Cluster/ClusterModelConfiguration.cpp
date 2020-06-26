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

 Class defining Task
 */

#include "ClusterModelConfiguration.hpp"
ClassImp(ClusterModelConfiguration);

ClusterModelConfiguration::ClusterModelConfiguration(const TString & name, const TString & type, const TString & version)
:
GeneratorConfiguration(name,type,version)
{
}

ClusterModelConfiguration::~ClusterModelConfiguration()
{}

void ClusterModelConfiguration::printConfiguration(ostream & os)
{
  os
  << " ------------------------------------------------------------------------------------------" << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << "                           modelName: " << getName() << endl
  << "                        modelVersion: " << getVersion() << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << "                     Cell Parameters: " << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << "             cellMultiplicity_Random: " << cellMultiplicity_Random << endl
  << "              cellMultiplicity_Fixed: " << cellMultiplicity_Fixed  << endl
  << "            cellMultiplicity_Minimum: " << cellMultiplicity_Minimum    << endl
  << "            cellMultiplicity_Maximum: " << cellMultiplicity_Maximum    << endl
  << "              cellMultiplicity_Slope: " << cellMultiplicity_Slope  << endl << endl
  << "                     cellBeta_Random: " << cellBeta_Random  << endl
  << "                      cellBeta_Fixed: " << cellBeta_Fixed   << endl
  << "                    cellBeta_Minimum: " << cellBeta_Minimum     << endl
  << "                    cellBeta_Maximum: " << cellBeta_Maximum     << endl << endl
  << "                      cellPhi_Random: " <<  cellPhi_Random  << endl
  << "                       cellPhi_Fixed: " <<  cellPhi_Fixed   << endl
  << "                     cellPhi_Minimum: " <<  cellPhi_Minimum     << endl
  << "                     cellPhi_Maximum: " <<  cellPhi_Maximum     << endl << endl
  << " ------------------------------------------------------------------------------------------" << endl;
  os<< "                  Cluster Parameters: " << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << "          clusterMultiplicity_Random: " << clusterMultiplicity_Random << endl
  << "           clusterMultiplicity_Fixed: " << clusterMultiplicity_Fixed  << endl
  << "             clusterMultiplicity_Min: " << clusterMultiplicity_Minimum    << endl
  << "             clusterMultiplicity_Max: " << clusterMultiplicity_Maximum    << endl
  << "           clusterMultiplicity_Slope: " << clusterMultiplicity_Slope  << endl<< endl

  << "                  clusterMass_Random: " << clusterMass_Random      << endl
  << "                   clusterMass_Fixed: " << clusterMass_Fixed       << endl
  << "                 clusterMass_Minimum: " << clusterMass_Minimum     << endl
  << "                 clusterMass_Maximum: " << clusterMass_Maximum     << endl
  << "                   clusterMass_Slope: " << clusterMass_Slope      << endl << endl

  << "              clusterRapidity_Random: " << clusterRapidity_Random   << endl
  << "               clusterRapidity_Fixed: " << clusterRapidity_Fixed    << endl
  << "                 clusterRapidity_Min: " << clusterRapidity_Minimum      << endl
  << "                 clusterRapidity_Max: " << clusterRapidity_Maximum      << endl
  << "               clusterRapidity_Slope: " << clusterRapidity_Slope    << endl << endl

  << "              clusterMomentum_Random: " << clusterMomentum_Random   << endl
  << "               clusterMomentum_Fixed: " << clusterMomentum_Fixed    << endl
  << "                 clusterMomentum_Min: " << clusterMomentum_Minimum      << endl
  << "                 clusterMomentum_Max: " << clusterMomentum_Maximum      << endl
  << "               clusterMomentum_Slope: " << clusterMomentum_Slope    << endl << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << "                 Particle Parameters: " << endl
  << " ------------------------------------------------------------------------------------------" << endl;
  os<< "         particleMultiplicity_Random: " << particleMultiplicity_Random << endl
  << "          particleMultiplicity_Fixed: " << particleMultiplicity_Fixed  << endl
  << "            particleMultiplicity_Min: " << particleMultiplicity_Minimum    << endl
  << "            particleMultiplicity_Max: " << particleMultiplicity_Maximum    << endl
  << "          particleMultiplicity_Slope: " << particleMultiplicity_Slope  << endl << endl

  << "              particleMomenum_Random: " << particleMomenum_Random << endl
  << "               particleMomenum_Fixed: " << particleMomenum_Fixed  << endl
  << "                 particleMomenum_Min: " << particleMomenum_Minimum    << endl
  << "                 particleMomenum_Max: " << particleMomenum_Maximum    << endl
  << "               particleMomenum_Slope: " << particleMomenum_Slope  << endl << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << " ------------------------------------------------------------------------------------------" << endl;
}
