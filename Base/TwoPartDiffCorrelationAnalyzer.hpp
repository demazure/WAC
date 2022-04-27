// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef TwoPartDiffCorrelationAnalyzer_hpp
#define TwoPartDiffCorrelationAnalyzer_hpp

#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "EventHistos.hpp"
#include "ParticleFilter.hpp"
#include "ParticlePairFilter.hpp"
#include "ParticleHistos.hpp"
#include "ParticlePairDiffHistos.hpp"
#include "ParticlePairDerivedDiffHistos.hpp"
#include "ParticlePairCombinedDiffHistos.hpp"

class TwoPartDiffCorrelationAnalyzer : public Task
{
 public:
  //////////////////////////////////////////////////////////////
  // CTOR
  //////////////////////////////////////////////////////////////
  TwoPartDiffCorrelationAnalyzer(const TString& name,
                                 TaskConfiguration* configuration,
                                 Event* event,
                                 EventFilter* eventFilter,
                                 ParticleFilter* particleFilter1,
                                 ParticleFilter* particleFilter2);
  virtual ~TwoPartDiffCorrelationAnalyzer();
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile* inputFile);
  virtual void loadBaseHistograms(TFile* inputFile);
  virtual void saveHistograms(TFile* outputFile);
  virtual void addHistogramsToExtList(TList* list, bool all = false);
  virtual void scaleHistograms(double factor);
  virtual void calculateDerivedHistograms();

  //////////////////////////////////////////////////////////////
  // Data Members
  //////////////////////////////////////////////////////////////

  EventFilter* eventFilter;
  ParticleFilter* particleFilter1;
  ParticleFilter* particleFilter2;

  TString partName1;
  TString partName2;

  EventHistos* event_Histos;
  ParticleHistos* particle1_Histos;
  ParticleHistos* particle2_Histos;
  ParticlePairDerivedDiffHistos* pair11_Histos;
  ParticlePairDerivedDiffHistos* pair22_Histos;
  ParticlePairDerivedDiffHistos* pair12_Histos;
  ParticlePairDerivedDiffHistos* pair21_Histos;
  ParticlePairCombinedDiffHistos* pair12_CIHistos;
  ParticlePairCombinedDiffHistos* pair12_CDHistos;

  ClassDef(TwoPartDiffCorrelationAnalyzer, 1)
};

#endif /* TwoPartDiffCorrelationAnalyzer_hpp */
