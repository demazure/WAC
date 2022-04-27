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

 Class defining Two Particle Correlation Analyzer Task
 */

#include "TwoPartDiffCorrelationAnalyzer.hpp"
#include "AnalysisConfiguration.hpp"

//////////////////////////////////////////////////////////////
// CTOR
//////////////////////////////////////////////////////////////
TwoPartDiffCorrelationAnalyzer::TwoPartDiffCorrelationAnalyzer(const TString& name, TaskConfiguration* configuration, Event* event,
                                                               EventFilter* ef, ParticleFilter* pf1, ParticleFilter* pf2)
  : Task(name, configuration, event),
    eventFilter(ef),
    particleFilter1(pf1),
    particleFilter2(pf2),
    partName1("U"),
    partName2("U"),
    event_Histos(nullptr),
    particle1_Histos(nullptr),
    particle2_Histos(nullptr),
    pair11_Histos(nullptr),
    pair22_Histos(nullptr),
    pair12_Histos(nullptr),
    pair21_Histos(nullptr),
    pair12_CIHistos(nullptr),
    pair12_CDHistos(nullptr)
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::CTOR(...) Started." << endl;

  if (!eventFilter) {
    if (reportError())
      cout << "TwoPartDiffCorrelationAnalyzer::CTOR(...) eventFilter is null pointer." << endl;
    postTaskError();
    return;
  }

  if (!particleFilter1) {
    if (reportError())
      cout << "TwoPartDiffCorrelationAnalyzer::CTOR(...) particleFilter1 is null pointer." << endl;
    postTaskError();
    return;
  }
  if (!particleFilter2) {
    if (reportError())
      cout << "TwoPartDiffCorrelationAnalyzer::CTOR(...) particleFilter2 is null pointer." << endl;
    postTaskError();
    return;
  }

  TString newName = getName();
  newName += "_";
  newName += eventFilter->getName();
  setName(newName);
  partName1 = particleFilter1->getName();
  partName2 = particleFilter2->getName();
}

//////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////
TwoPartDiffCorrelationAnalyzer::~TwoPartDiffCorrelationAnalyzer()
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::DTOR(...) Started" << endl;
  if (event_Histos != NULL)
    delete event_Histos;
  if (particle1_Histos != NULL)
    delete particle1_Histos;
  if (particle2_Histos != NULL)
    delete particle2_Histos;
  if (pair11_Histos != NULL)
    delete pair11_Histos;
  if (pair22_Histos != NULL)
    delete pair22_Histos;
  if (pair12_Histos != NULL)
    delete pair12_Histos;
  if (pair21_Histos != NULL)
    delete pair21_Histos;
  if (pair12_CIHistos != NULL)
    delete pair12_CIHistos;
  if (pair12_CDHistos != NULL)
    delete pair12_CDHistos;
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::DTOR(...) Completed" << endl;
}

void TwoPartDiffCorrelationAnalyzer::createHistograms()
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::initialize(...) started" << endl;
  AnalysisConfiguration* ac = (AnalysisConfiguration*)getTaskConfiguration();
  LogLevel debugLevel = getReportLevel();

  event_Histos = new EventHistos("Event Histos", ac, debugLevel);

  particle1_Histos = new ParticleHistos(partName1, ac, debugLevel);
  particle2_Histos = new ParticleHistos(partName2, ac, debugLevel);
  if (ac->fillPairs) {
    pair11_Histos = new ParticlePairDerivedDiffHistos(partName1 + partName1, ac, debugLevel);
    pair22_Histos = new ParticlePairDerivedDiffHistos(partName2 + partName2, ac, debugLevel);
    pair12_Histos = new ParticlePairDerivedDiffHistos(partName1 + partName2, ac, debugLevel);
    pair21_Histos = new ParticlePairDerivedDiffHistos(partName2 + partName1, ac, debugLevel);
    if (ac->calculateDerivedHistograms) {
      pair11_Histos->createDerivedHistograms();
      pair22_Histos->createDerivedHistograms();
      pair12_Histos->createDerivedHistograms();
      pair21_Histos->createDerivedHistograms();
      pair12_CIHistos = new ParticlePairCombinedDiffHistos(partName1 + partName2 + "CI", ac, debugLevel);
      pair12_CDHistos = new ParticlePairCombinedDiffHistos(partName1 + partName2 + "CD", ac, debugLevel);
    }
  }
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::createHistograms(...) completed" << endl;
}

//////////////////////////////////////////////////////////////
// load histograms from given files
//////////////////////////////////////////////////////////////
void TwoPartDiffCorrelationAnalyzer::loadHistograms(TFile* inputFile)
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::loadHistograms(...) Starting." << endl;
  /* first load the number of events as from the  cumulated parameter */
  TParameter<Long64_t>* par = (TParameter<Long64_t>*)inputFile->Get("NoOfEvents");
  eventsProcessed = par->GetVal();
  delete par;
  AnalysisConfiguration* analysisConfiguration = (AnalysisConfiguration*)getTaskConfiguration();
  LogLevel debugLevel = getReportLevel();

  particle1_Histos = new ParticleHistos(inputFile, partName1, analysisConfiguration, debugLevel);
  particle2_Histos = new ParticleHistos(inputFile, partName2, analysisConfiguration, debugLevel);
  if (analysisConfiguration->fillPairs) {
    pair11_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName1 + partName1, analysisConfiguration, debugLevel);
    pair22_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName2 + partName2, analysisConfiguration, debugLevel);
    pair12_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName1 + partName2, analysisConfiguration, debugLevel);
    pair21_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName2 + partName1, analysisConfiguration, debugLevel);
    if (analysisConfiguration->calculateDerivedHistograms) {
      pair12_CIHistos = new ParticlePairCombinedDiffHistos(inputFile, partName1 + partName2 + "CI", analysisConfiguration, debugLevel);
      pair12_CDHistos = new ParticlePairCombinedDiffHistos(inputFile, partName1 + partName2 + "CD", analysisConfiguration, debugLevel);
    }
  }
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::loadHistograms(...) Completed." << endl;
}

//////////////////////////////////////////////////////////////
// load the base histograms from given file
//////////////////////////////////////////////////////////////
void TwoPartDiffCorrelationAnalyzer::loadBaseHistograms(TFile* inputFile)
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::loadHistograms(...) Starting." << endl;

  /* first load the number of events as from the  cumulated parameter */
  TParameter<Long64_t>* par = (TParameter<Long64_t>*)inputFile->Get("NoOfEvents");
  eventsProcessed = par->GetVal();
  delete par;
  AnalysisConfiguration* analysisConfiguration = (AnalysisConfiguration*)getTaskConfiguration();
  LogLevel debugLevel = getReportLevel();

  particle1_Histos = new ParticleHistos(inputFile, partName1, analysisConfiguration, debugLevel);
  particle2_Histos = new ParticleHistos(inputFile, partName2, analysisConfiguration, debugLevel);
  if (analysisConfiguration->fillPairs) {
    pair11_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName1 + partName1, analysisConfiguration, debugLevel);
    pair22_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName2 + partName2, analysisConfiguration, debugLevel);
    pair12_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName1 + partName2, analysisConfiguration, debugLevel);
    pair21_Histos = new ParticlePairDerivedDiffHistos(inputFile, partName2 + partName1, analysisConfiguration, debugLevel);
    if (analysisConfiguration->calculateDerivedHistograms) {
      pair11_Histos->createDerivedHistograms();
      pair22_Histos->createDerivedHistograms();
      pair12_Histos->createDerivedHistograms();
      pair21_Histos->createDerivedHistograms();
      pair12_CIHistos = new ParticlePairCombinedDiffHistos(partName1 + partName2 + "CI", analysisConfiguration, debugLevel);
      pair12_CDHistos = new ParticlePairCombinedDiffHistos(partName1 + partName2 + "CD", analysisConfiguration, debugLevel);
    }
  }

  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::loadHistograms(...) Completed." << endl;
}

//////////////////////////////////////////////////////////////
// save histograms to given files
//////////////////////////////////////////////////////////////
void TwoPartDiffCorrelationAnalyzer::saveHistograms(TFile* outputFile)
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::saveHistograms(...) Saving Event histograms to file." << endl;
  if (!outputFile) {
    if (reportError())
      cout << "TwoPartDiffCorrelationAnalyzer::saveHistograms(...) outputFile is a null  pointer." << endl;
    postTaskError();
    return;
  }
  outputFile->cd();

  /* first save the number of events as a cumulated parameter */
  TParameter<Long64_t>("NoOfEvents", eventsProcessed, '+').Write();
  AnalysisConfiguration* analysisConfiguration = (AnalysisConfiguration*)getTaskConfiguration();
  if (!analysisConfiguration) {
    if (reportError())
      cout << "TwoPartDiffCorrelationAnalyzer::saveHistograms(...) analysisConfiguration is a null  pointer." << endl;
    postTaskError();
    return;
  }

  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::saveHistograms(...) saving singles." << endl;

  // event_Histos  ->saveHistograms(outputFile);
  particle1_Histos->saveHistograms(outputFile);
  particle2_Histos->saveHistograms(outputFile);

  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::saveHistograms(...) saving pairs -- perhaps." << endl;

  if (analysisConfiguration->fillPairs) {
    if (reportDebug())
      cout << "TwoPartDiffCorrelationAnalyzer::saveHistograms(...) saving calculated histograms." << endl;

    pair11_Histos->saveHistograms(outputFile);
    pair22_Histos->saveHistograms(outputFile);
    pair12_Histos->saveHistograms(outputFile);
    pair21_Histos->saveHistograms(outputFile);
    pair12_CIHistos->saveHistograms(outputFile);
    pair12_CDHistos->saveHistograms(outputFile);
  }
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::saveHistograms(...) Completed." << endl;
}

//////////////////////////////////////////////////////////////
// add histograms to an external list
//////////////////////////////////////////////////////////////
void TwoPartDiffCorrelationAnalyzer::addHistogramsToExtList(TList* list, bool all)
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::addHistogramsToExtList(...) Saving Event histograms to file." << endl;

  /* first add the number of events as a cumulated parameter */
  list->Add(new TParameter<Long64_t>("NoOfEvents", eventsProcessed, '+'));
  AnalysisConfiguration* analysisConfiguration = (AnalysisConfiguration*)getTaskConfiguration();

  // event_Histos  ->saveHistograms(outputFile);
  particle1_Histos->addHistogramsToExtList(list, all);
  particle2_Histos->addHistogramsToExtList(list, all);
  if (analysisConfiguration->fillPairs) {
    pair11_Histos->addHistogramsToExtList(list, all);
    pair22_Histos->addHistogramsToExtList(list, all);
    pair12_Histos->addHistogramsToExtList(list, all);
    pair21_Histos->addHistogramsToExtList(list, all);
    pair12_CIHistos->addHistogramsToExtList(list, all);
    pair12_CDHistos->addHistogramsToExtList(list, all);
  }

  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::addHistogramsToExtList(...) Completed." << endl;
}

void TwoPartDiffCorrelationAnalyzer::execute()
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::analyze(...) Starting" << endl;
  if (event != NULL) {
    if (reportDebug())
      cout << "TwoPartDiffCorrelationAnalyzer::analyze(...) analyzing " << event->nParticles << " particles" << endl;
  } else {
    if (reportError())
      cout << "TwoPartDiffCorrelationAnalyzer::analyze(...) event pointer is NULL. Abort." << endl;
    postTaskError();
    return;
  }

  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::analyze(...) check if event is acceptable" << endl;
  if (!eventFilter->accept(*event))
    return;
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::analyze(...) acceptable event" << endl;

  AnalysisConfiguration* analysisConfiguration = (AnalysisConfiguration*)getTaskConfiguration();

  if (!analysisConfiguration) {
    if (reportError())
      cout << "TwoPartDiffCorrelationAnalyzer::analyze(...) analysisConfiguration null pointer" << endl;
    postTaskError();
    return;
  }

  bool accept11;
  bool accept21;
  bool accept12;
  bool accept22;

  /* before filtering let's build the particle indexes to hurry up the process */
  for (int iParticle = 0; iParticle < event->nParticles; iParticle++) {
    if (reportDebug())
      cout << "TwoPartDiffCorrelationAnalyzer::analyze(...) particle: " << iParticle << endl;
    Particle* particle = event->getParticleAt(iParticle);
    if (reportDebug())
      particle->printProperties(cout);
    particle->ixEtaPhi = analysisConfiguration->getIxEtaPhi(particle->eta, particle->phi);
    particle->ixYPhi = analysisConfiguration->getIxYPhi(particle->y, particle->phi);

    /* make benefit to report debug information if required */
    accept11 = particleFilter1->accept(*particle);
    accept21 = particleFilter2->accept(*particle);
    accept12 = particleFilter1->accept(*particle);
    accept22 = particleFilter2->accept(*particle);
    if (reportDebug()) {
      cout << "  accept11:" << accept11 << endl;
      cout << "  accept21:" << accept21 << endl;
      cout << "  accept12:" << accept12 << endl;
      cout << "  accept22:" << accept22 << endl;
    }
  }

  for (int iParticle1 = 0; iParticle1 < event->nParticles; iParticle1++) {
    Particle& particle1 = *event->getParticleAt(iParticle1);
    accept11 = particleFilter1->accept(particle1);
    accept21 = particleFilter2->accept(particle1);

    if (accept11)
      particle1_Histos->fill(particle1, 1.0);
    if (accept21)
      particle2_Histos->fill(particle1, 1.0);
    if (analysisConfiguration->fillPairs) {
      for (int iParticle2 = 0; iParticle2 < event->nParticles; iParticle2++) {
        if (iParticle1 == iParticle2)
          continue;
        Particle& particle2 = *event->getParticleAt(iParticle2);
        accept12 = particleFilter1->accept(particle2);
        accept22 = particleFilter2->accept(particle2);

        /* TODO: to incorporate <pT> in eta,phi for DpTDpT extraction */
        int globalpseudobin = pair11_Histos->getGlobalDeltaEtaDeltaPhiIndex(particle1, particle2);
        int globalrapiditybin = pair11_Histos->getGlobalDeltaRapidityDeltaPhiIndex(particle1, particle2);
        if (accept11 && accept12)
          pair11_Histos->fill(globalpseudobin, globalrapiditybin, particle1, particle2, 1.0, 1.0);
        if (accept21 && accept22)
          pair22_Histos->fill(globalpseudobin, globalrapiditybin, particle1, particle2, 1.0, 1.0);
        if (accept11 && accept22)
          pair12_Histos->fill(globalpseudobin, globalrapiditybin, particle1, particle2, 1.0, 1.0);
        if (accept21 && accept12)
          pair21_Histos->fill(globalpseudobin, globalrapiditybin, particle1, particle2, 1.0, 1.0);
      }
    }
  }
  eventsProcessed++;
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::execute() Completed" << endl;
}

//////////////////////////////////////////////////////////////
// calculate Derived Histograms
//////////////////////////////////////////////////////////////
void TwoPartDiffCorrelationAnalyzer::calculateDerivedHistograms()
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::calculateDerivedHistograms() Starting" << endl;
  AnalysisConfiguration* analysisConfiguration = (AnalysisConfiguration*)getTaskConfiguration();
  if (analysisConfiguration->fillPairs) {
    particle1_Histos->calculateAverages();
    particle2_Histos->calculateAverages();
    pair11_Histos->calculateDerivedHistograms(particle1_Histos, particle1_Histos, analysisConfiguration->binCorrPP);
    pair22_Histos->calculateDerivedHistograms(particle2_Histos, particle2_Histos, analysisConfiguration->binCorrMM);
    pair12_Histos->calculateDerivedHistograms(particle1_Histos, particle2_Histos, analysisConfiguration->binCorrPM);
    pair21_Histos->calculateDerivedHistograms(particle1_Histos, particle2_Histos, analysisConfiguration->binCorrMP);
    pair12_CIHistos->calculate(pair11_Histos, pair22_Histos, pair12_Histos, pair21_Histos, 0.25, 0.25, 0.25, 0.25);
    pair12_CDHistos->calculate(pair11_Histos, pair22_Histos, pair12_Histos, pair21_Histos, -0.25, -0.25, 0.25, 0.25);
  } else {
    if (reportDebug())
      cout << "TwoPartDiffCorrelationAnalyzer::calculateDerivedHistograms() Nothing to do, fill pairs not ordered" << endl;
  }
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::calculateDerivedHistograms() Completed" << endl;
}

//////////////////////////////////////////////////////////////
// Scale all filled histograms by the given factor
// Derived histograms are *NOT* scaled
// They are not by construction
//////////////////////////////////////////////////////////////
void TwoPartDiffCorrelationAnalyzer::scaleHistograms(double factor)
{
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::scaleHistograms(..) Scale all primary histograms by " << factor << endl;
  particle1_Histos->scale(factor);
  particle2_Histos->scale(factor);
  AnalysisConfiguration* analysisConfiguration = (AnalysisConfiguration*)getTaskConfiguration();
  if (analysisConfiguration->fillPairs) {
    pair11_Histos->scale(factor);
    pair22_Histos->scale(factor);
    pair12_Histos->scale(factor);
  }
  if (reportDebug())
    cout << "TwoPartDiffCorrelationAnalyzer::scale(..) Completed" << endl;
}

ClassImp(TwoPartDiffCorrelationAnalyzer)
