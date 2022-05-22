//
//  ParticlePairBalanceFunctionDiffHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/18/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "ParticlePairBalanceFunctionDiffHistos.hpp"

ParticlePairBalanceFunctionDiffHistos::ParticlePairBalanceFunctionDiffHistos(const TString& name,
                                                                             AnalysisConfiguration* configuration,
                                                                             LogLevel debugLevel)
  : Histograms(name, configuration, 100, debugLevel),
    h_R2BF_DetaDphi_shft(nullptr),
    h_R2BF_DyDphi_shft(nullptr)
{
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::CTOR() Started." << endl;
  initialize();
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::CTOR() Completed." << endl;
}

ParticlePairBalanceFunctionDiffHistos::ParticlePairBalanceFunctionDiffHistos(TFile* inputFile,
                                                                             const TString& collectionName,
                                                                             AnalysisConfiguration* analysisConfiguration,
                                                                             LogLevel debugLevel)
  : Histograms(collectionName, analysisConfiguration, 100, debugLevel),
    h_R2BF_DetaDphi_shft(nullptr),
    h_R2BF_DyDphi_shft(nullptr)
{
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::CTOR(TFile * inputFile,...) Started." << endl;
  loadHistograms(inputFile);
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::CTOR(TFile * inputFile,...) Completed." << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////////////////////////////////////////////
ParticlePairBalanceFunctionDiffHistos::~ParticlePairBalanceFunctionDiffHistos()
{
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::DTOR() No ops." << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// load histograms
//////////////////////////////////////////////////////////////////////////////////////////////////////
void ParticlePairBalanceFunctionDiffHistos::loadHistograms(TFile* inputFile)
{
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::loadHistograms(...) strted" << endl;

  TString bn = getHistoBaseName();

  h_R2BF_DetaDphi_shft = loadH2(inputFile, bn + TString("R2BF_DetaDphi_shft"));

  if (configuration->fillY) {
    h_R2BF_DyDphi_shft = loadH2(inputFile, bn + TString("R2BF_DyDphi_shft"));
  }
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::loadHistograms(...) completed" << endl;
  /* the histograms are not owned */
  bOwnTheHistograms = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize histograms of this class
//////////////////////////////////////////////////////////////////////////////////////////////////////
void ParticlePairBalanceFunctionDiffHistos::createHistograms()
{
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::initialize() started" << endl;
  TString name;

  AnalysisConfiguration& ac = *getConfiguration();
  TString bn = getHistoBaseName();

  h_R2BF_DetaDphi_shft = createHistogram(bn + TString("R2BF_DetaDphi_shft"), ac.nBins_Deta, ac.min_Deta, ac.max_Deta, ac.nBins_Dphi, ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta#eta", "#Delta#varphi", "BF_{2}", scaled, saved, plotted, printed);

  if (ac.fillY) {
    h_R2BF_DyDphi_shft = createHistogram(bn + TString("R2BF_DyDphi_shft"), ac.nBins_Dy, ac.min_Dy, ac.max_Dy, ac.nBins_Dphi, ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta y", "#Delta#varphi", "BF_{2}", scaled, saved, plotted, printed);
  }

  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::initialize() completed" << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// calculate combinations of histograms with given coefficients
//////////////////////////////////////////////////////////////////////////////////////////////////////
void ParticlePairBalanceFunctionDiffHistos::calculate(ParticlePairDerivedDiffHistos* bfpm,
                                                      ParticlePairDerivedDiffHistos* bfpp,
                                                      ParticlePairDerivedDiffHistos* bfmp,
                                                      ParticlePairDerivedDiffHistos* bfmm)
{
  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::calculate() started" << endl;

  h_R2BF_DetaDphi_shft->Reset();
  h_R2BF_DetaDphi_shft->Add(bfpp->h_R2bf12_DetaDphi_shft, bfmm->h_R2bf12_DetaDphi_shft, -0.5, -0.5);
  h_R2BF_DetaDphi_shft->Add(bfpm->h_R2bf12_DetaDphi_shft, 0.5);
  h_R2BF_DetaDphi_shft->Add(bfmp->h_R2bf12_DetaDphi_shft, 0.5);

  if (configuration->fillY) {
    h_R2BF_DetaDphi_shft->Reset();
    h_R2BF_DyDphi_shft->Add(bfpp->h_R2bf12_DyDphi_shft, bfmm->h_R2bf12_DyDphi_shft, -0.5, -0.5);
    h_R2BF_DyDphi_shft->Add(bfpm->h_R2bf12_DyDphi_shft, 0.5);
    h_R2BF_DyDphi_shft->Add(bfmp->h_R2bf12_DyDphi_shft, 0.5);
  }

  if (reportDebug())
    cout << "ParticlePairBalanceFunctionDiffHistos::calculate() completed" << endl;
}

ClassImp(ParticlePairBalanceFunctionDiffHistos)
