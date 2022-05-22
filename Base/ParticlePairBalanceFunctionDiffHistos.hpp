//
//  ParticlePairBalanceFunctionDiffHistos.hpp
//  MyMC
//
//  Created by Claude Pruneau on 12/18/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#ifndef ParticlePairBalanceFunctionDiffHistos_hpp
#define ParticlePairBalanceFunctionDiffHistos_hpp
#include "Histograms.hpp"
#include "ParticlePairDerivedDiffHistos.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Two-Particle Correlation Combinations (CI, CD)
//////////////////////////////////////////////////////////////////////////////////////////////////////
class ParticlePairBalanceFunctionDiffHistos : public Histograms
{
 public:
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ParticlePairBalanceFunctionDiffHistos(const TString& name,
                                        AnalysisConfiguration* configuration,
                                        LogLevel debugLevel);
  ParticlePairBalanceFunctionDiffHistos(TFile* inputFile,
                                        const TString& name,
                                        AnalysisConfiguration* configuration,
                                        LogLevel debugLevel);
  virtual ~ParticlePairBalanceFunctionDiffHistos();
  void loadHistograms(TFile* inputFile);
  void createHistograms();
  void calculate(ParticlePairDerivedDiffHistos*,
                 ParticlePairDerivedDiffHistos*,
                 ParticlePairDerivedDiffHistos*,
                 ParticlePairDerivedDiffHistos*);
  // void calculateRR_Q3D(ClusterHistos * ch);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members - Computed
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TH2* h_R2BF_DetaDphi_shft;
  TH2* h_R2BF_DyDphi_shft;

  ClassDef(ParticlePairBalanceFunctionDiffHistos, 1)
};

#endif /* ParticlePairBalanceFunctionDiffHistos_hpp */
