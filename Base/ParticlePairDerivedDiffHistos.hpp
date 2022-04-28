//
// ParticlePairDerivedDiffHistos.hpp
// MyMC
//
// created by Claude Pruneau on 9/23/16.
// Copyright © 2016 Claude Pruneau. All rights reserved.
//

#ifndef WAC_ParticlePairDerivedDiffHistos
#define WAC_ParticlePairDerivedDiffHistos
#include "Histograms.hpp"
#include "ParticleHistos.hpp"
#include "ParticlePairDiffHistos.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Two-Particle Correlation Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////
class ParticlePairDerivedDiffHistos : public ParticlePairDiffHistos
{
 public:
  ParticlePairDerivedDiffHistos(const TString& name,
                                AnalysisConfiguration* configuration,
                                LogLevel debugLevel);
  ParticlePairDerivedDiffHistos(TFile* inputFile,
                                const TString& name,
                                AnalysisConfiguration* configuration,
                                LogLevel debugLevel);
  virtual ~ParticlePairDerivedDiffHistos();
  void loadHistograms(TFile* inputFile);
  void createDerivedHistograms();
  double calculateWeightCorrection(TFile* fRaw, TFile* fCal);
  void calculateDerivedHistograms(ParticleHistos* part1Histos,
                                  ParticleHistos* part2Histos,
                                  double bincorrection);
  void reduce(const TH2* source, TH2* target, int nEtaBins, int nPhiBins);

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members - Computed
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  TH2* h_n1n1_phiEtaPhiEta;
  TH2* h_n1n1_etaEta;
  TH2* h_n1n1_phiPhi;
  TH2* h_n1n1_ptPt;

  TH2* h_pt1pt1_phiEtaPhiEta;
  TH2* h_pt1pt1_etaEta;
  TH2* h_pt1pt1_phiPhi;

  TH2* h_n1n1_DetaDphi;
  TH2* h_pt1pt1_DetaDphi;

  TH2* h_R2_DetaDphi;
  TH2* h_P2_DetaDphi;
  TH2* h_G2_DetaDphi;

  TH2* h_R2_DetaDphi_shft;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_G2_DetaDphi_shft;

  TH2* h_R2bf12_DetaDphi_shft;

  // vs Y ////////
  TH2* h_n1n1_phiYPhiY;
  TH2* h_n1n1_yY;

  TH2* h_pt1pt1_phiYPhiY;
  TH2* h_pt1pt1_yY;

  TH2* h_n1n1_DyDphi;
  TH2* h_pt1pt1_DyDphi;

  TH2* h_R2_DyDphi;
  TH2* h_P2_DyDphi;
  TH2* h_G2_DyDphi;

  TH2* h_R2_DyDphi_shft;
  TH2* h_P2_DyDphi_shft;
  TH2* h_G2_DyDphi_shft;

  TH2* h_R2bf12_DyDphi_shft;

  ClassDef(ParticlePairDerivedDiffHistos, 1)
};

#endif /* ParticlePairDerivedDiffHistos */
