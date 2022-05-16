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
#include "EventPool.hpp"

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
  template <typename ParticleType1, typename ParticleType2>
  int getGlobalDeltaEtaDeltaPhiIndex(ParticleType1& p1, ParticleType2& p2);
  template <typename ParticleType1, typename ParticleType2>
  int getGlobalDeltaRapidityDeltaPhiIndex(ParticleType1& p1, ParticleType2& p2);
  template <typename ParticleType1, typename ParticleType2>
  void fill(ParticleType1& particle1, ParticleType2& particle2, double weight1, double weight2, double pTavg1 = 0.0, double pTavg2 = 0.0);
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

template <typename ParticleType1, typename ParticleType2>
inline int ParticlePairDiffHistos::getGlobalDeltaEtaDeltaPhiIndex(ParticleType1& p1, ParticleType2& p2)
{
  return h_n2_DetaDphi->GetBin(configuration->getDeltaEtaIndex(p1, p2) + 1, configuration->getDeltaPhiIndex(p1, p2) + 1);
}

template <typename ParticleType1, typename ParticleType2>
inline int ParticlePairDiffHistos::getGlobalDeltaRapidityDeltaPhiIndex(ParticleType1& p1, ParticleType2& p2)
{
  if (configuration->fillY) {
    return h_n2_DyDphi->GetBin(configuration->getDeltaRapidityIndex(p1, p2) + 1, configuration->getDeltaPhiIndex(p1, p2) + 1);
  } else {
    return -1;
  }
}

template <typename ParticleType1, typename ParticleType2>
void ParticlePairDiffHistos::fill(ParticleType1& particle1, ParticleType2& particle2, double weight1, double weight2, double pTavg1, double pTavg2)
{
  int globaletabinno = getGlobalDeltaEtaDeltaPhiIndex(particle1, particle2);
  h_n2_ptPt->Fill(particle1.pt, particle2.pt, weight1 * weight2);
  h_n2_DetaDphi->AddBinContent(globaletabinno, weight1 * weight2);
  h_ptpt_DetaDphi->AddBinContent(globaletabinno, weight1 * particle1.pt * weight2 * particle2.pt);
  h_dptdpt_DetaDphi->AddBinContent(globaletabinno, (weight1 * particle1.pt - pTavg1) * (weight2 * particle2.pt - pTavg2));
  h_n2_DetaDphi->SetEntries(h_n2_ptPt->GetEntries());
  h_ptpt_DetaDphi->SetEntries(h_n2_ptPt->GetEntries());
  h_dptdpt_DetaDphi->SetEntries(h_n2_ptPt->GetEntries());

  if (configuration->fillY) {
    int globalybinno = getGlobalDeltaRapidityDeltaPhiIndex(particle1, particle2);
    h_n2_DyDphi->AddBinContent(globalybinno, weight1 * weight2);
    h_ptpt_DyDphi->AddBinContent(globalybinno, weight1 * particle1.pt * weight2 * particle2.pt);
    h_dptdpt_DyDphi->AddBinContent(globalybinno, (weight1 * particle1.pt - pTavg1) * (weight2 * particle2.pt - pTavg2));
    h_n2_DyDphi->SetEntries(h_n2_ptPt->GetEntries());
    h_ptpt_DyDphi->SetEntries(h_n2_ptPt->GetEntries());
    h_dptdpt_DyDphi->SetEntries(h_n2_ptPt->GetEntries());
  }
}

#endif /* ParticlePairDiffHistos  */
