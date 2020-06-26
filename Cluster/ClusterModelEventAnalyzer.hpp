// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_ClusterModelEventAnalyzer
#define WAC_ClusterModelEventAnalyzer
#include "Task.hpp"
#include "EventFilter.hpp"
#include "ClusterModelEvent.hpp"
#include "ClusterModelEventHistos.hpp"
#include "CellHistos.hpp"
#include "ClusterHistos.hpp"

class ClusterModelEventAnalyzer : public Task
{
public:

  ClusterModelEventAnalyzer(const TString &  name,
                            TaskConfiguration * configuration,
                            ClusterModelEvent * event,
                            EventFilter       * eventFilter,
                            ParticleFilter    * particleFilter);
  virtual ~ClusterModelEventAnalyzer();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void saveHistograms(TFile * outputFile);
  virtual void execute();
  virtual void scale(double factor);

   ClusterModelEvent * getClusterModelEvent();
  
  //////////////////////////////////////////////////////////////
  // Data Members
  //////////////////////////////////////////////////////////////
  ClusterModelEvent * cmEvent;
  EventFilter    * cmEventFilter;
  ParticleFilter * particleFilter;

  CellHistos    * cellHistos;
  ClusterHistos * clusterHistos;
//ClusterModelEventHistos * 
  ClassDef(ClusterModelEventAnalyzer,0)
};

#endif /* WAC_ClusterModelEventAnalyzer */
