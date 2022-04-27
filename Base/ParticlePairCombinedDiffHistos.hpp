//
//  ParticlePairCombinedDiffHistos.hpp
//  MyMC
//
//  Created by Claude Pruneau on 12/18/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#ifndef ParticlePairCombinedDiffHistos_hpp
#define ParticlePairCombinedDiffHistos_hpp
#include "Histograms.hpp"
#include "ParticlePairDerivedDiffHistos.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Two-Particle Correlation Combinations (CI, CD)
//////////////////////////////////////////////////////////////////////////////////////////////////////
class ParticlePairCombinedDiffHistos : public Histograms
{
 public:
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ParticlePairCombinedDiffHistos(const TString& name,
                                 AnalysisConfiguration* configuration,
                                 LogLevel debugLevel);
  ParticlePairCombinedDiffHistos(TFile* inputFile,
                                 const TString& name,
                                 AnalysisConfiguration* configuration,
                                 LogLevel debugLevel);
  virtual ~ParticlePairCombinedDiffHistos();
  void loadHistograms(TFile* inputFile);
  void createHistograms();
  void calculate(ParticlePairDerivedDiffHistos* pp,
                 ParticlePairDerivedDiffHistos* mm,
                 ParticlePairDerivedDiffHistos* pm,
                 ParticlePairDerivedDiffHistos* mp,
                 double app, double amm, double apm, double amp);
  // void calculateRR_Q3D(ClusterHistos * ch);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members - Computed
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TH2* h_R2_DetaDphi_shft;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_G2_DetaDphi_shft;
  TH2* h_R2BF_DetaDphi_shft;

  TH2* h_R2_DyDphi_shft;
  TH2* h_P2_DyDphi_shft;
  TH2* h_G2_DyDphi_shft;
  TH2* h_R2BF_DyDphi_shft;

  ClassDef(ParticlePairCombinedDiffHistos, 1)
};

#endif /* ParticlePairCombinedDiffHistos_hpp */
