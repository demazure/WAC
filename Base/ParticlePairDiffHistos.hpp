// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining ParticlePairHistos
 */

#ifndef WAC_ParticlePairDiffHistos
#define WAC_ParticlePairDiffHistos

#include "Histograms.hpp"

class ParticlePairDiffHistos : public Histograms
{
 public:
  ParticlePairDiffHistos(const TString& name,
                         AnalysisConfiguration* configuration,
                         LogLevel debugLevel);
  ParticlePairDiffHistos(TFile* inputFile,
                         const TString& name,
                         AnalysisConfiguration* configuration,
                         LogLevel debugLevel);
  virtual ~ParticlePairDiffHistos();
  void initialize();
  int getGlobalDeltaEtaDeltaPhiIndex(Particle& p1, Particle& p2);
  int getGlobalDeltaRapidityDeltaPhiIndex(Particle& p1, Particle& p2);
  void fill(Particle& particle1, Particle& particle2, double weight1, double weight2, double pTavg1 = 0.0, double pTavg2 = 0.0);
  void loadHistograms(TFile* inputFile);

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  ////////////////////////////////////////////////////////////////////////////
  TH2* h_n2_ptPt;

  TH2* h_n2_DetaDphi;
  TH2* h_ptpt_DetaDphi;
  TH2* h_dptdpt_DetaDphi;

  TH2* h_n2_DyDphi;
  TH2* h_ptpt_DyDphi;
  TH2* h_dptdpt_DyDphi;

  ClassDef(ParticlePairDiffHistos, 1)
};

inline int ParticlePairDiffHistos::getGlobalDeltaEtaDeltaPhiIndex(Particle& p1, Particle& p2)
{
  return h_n2_DetaDphi->GetBin(configuration->getDeltaEtaIndex(p1, p2) + 1, configuration->getDeltaPhiIndex(p1, p2) + 1);
}

inline int ParticlePairDiffHistos::getGlobalDeltaRapidityDeltaPhiIndex(Particle& p1, Particle& p2)
{
  if (configuration->fillY) {
    return h_n2_DyDphi->GetBin(configuration->getDeltaRapidityIndex(p1, p2) + 1, configuration->getDeltaPhiIndex(p1, p2) + 1);
  } else {
    return -1;
  }
}

#endif /* ParticlePairDiffHistos  */
