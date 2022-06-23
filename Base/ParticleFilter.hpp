// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_ParticleFilter
#define WAC_ParticleFilter
#include "TString.h"
#include "Particle.hpp"

//////////////////////////////////////////////////////////////////////////////////////////
// Single Particle Filter
//
// chargeRequested:
// case -1:    accepts negative only
// case  0:    accepts neutral only
// case  1:    accepts positive only
// case  999:  accepts all
//////////////////////////////////////////////////////////////////////////////////////////

class ParticleFilter
{
 public:
  enum ChargeSelection { AllCharges,
                         Negative,
                         Positive,
                         Charged,
                         Neutral };
  enum SpeciesSelection { AllSpecies,
                          Photon,
                          Lepton,
                          Electron,
                          Muon,
                          Hadron,
                          Pion,
                          Kaon,
                          Baryon,
                          Proton,
                          Lambda,
                          ALambda,
                          AString,
                          MString,
                          BString,
                          Strings };

  ParticleFilter(SpeciesSelection pidRequested,
                 ChargeSelection chargeRequested,
                 double minPt,
                 double maxPt,
                 double minEta,
                 double maxEta,
                 double minY,
                 double maxY);
  virtual ~ParticleFilter();
  bool accept(Particle& particle);
  TString getName();
  TString getTitle();

  static int getAcceptedIndex(std::vector<ParticleFilter*>, Particle& particle);

  //////////////////////////////////////////////////////////////////////////////////////////
  // Data Members
  //////////////////////////////////////////////////////////////////////////////////////////

  SpeciesSelection pidRequested;
  ChargeSelection chargeRequested;
  double min_pt;
  double max_pt;
  double min_eta;
  double max_eta;
  double min_y;
  double max_y;

  ClassDef(ParticleFilter, 1)
};

//////////////////////////////////////////////////////////////////////////////////////////
// accept/reject the given particle based on filter parameter
//////////////////////////////////////////////////////////////////////////////////////////
inline bool ParticleFilter::accept(Particle& particle)
{
  //  enum ChargeSelection   { AllCharges, Negative, Positive, Charged, Neutral };
  //  enum SpeciesSelection  { AllSpecies, Photon, Lepton, Electron, Muon, Hadron, Pion, Kaon, Baryon, Proton, Lambda };

  bool accepting = true;
  double charge = particle.charge;
  switch (chargeRequested) {
    case AllCharges:
      accepting = true;
      break;
    case Negative:
      accepting = (charge < 0);
      break;
    case Positive:
      accepting = (charge > 0);
      break;
    case Charged:
      accepting = (charge != 0);
      break;
    case Neutral:
      accepting = (charge == 0);
      break;
  }
  if (!accepting)
    return false;
  double pid = TMath::Abs(particle.pid);
  switch (pidRequested) {
    case AllSpecies:
      accepting = true;
      break;
    case Photon:
      accepting = (pid == 22);
      break;
    case Lepton:
      accepting = (pid == 11) || (pid == 12) || (pid == 13) || (pid == 14) || (pid == 15) || (pid == 16);
      break;
    case Electron:
      accepting = (pid == 11);
      break;
    case Muon:
      accepting = (pid == 13);
      break;
    case Hadron:
      accepting = (pid == 211) || (pid == 321) || (pid == 2212);
      break;
    case Pion:
      accepting = (pid == 111) || (pid == 211);
      break;
    case Kaon:
      accepting = (pid == 321) || (pid == 311) || (pid == 310) || (pid == 130);
      break;
    case Baryon:
      accepting = (pid == 2212) || (pid == 2112) || (pid == 3122);
      break;
    case Proton:
      accepting = (pid == 2212);
      break;
    case Lambda:
      accepting = (particle.pid == 3122);
      break;
    case ALambda:
      accepting = (particle.pid == -3122);
      break;
    case AString:
      accepting = (pid == 51);
      break;
    case MString:
      accepting = (pid == 52);
      break;
    case BString:
      accepting = (pid == 53);
      break;
    case Strings:
      accepting = (pid == 51) || (pid == 52) || (pid == 53);
      break;
  }

  if (!accepting)
    return false;

  double pt = particle.pt;
  double eta = particle.eta;
  if (pt > min_pt &&
      pt <= max_pt &&
      eta >= min_eta &&
      eta <= max_eta)
    return true;
  else
    return false;
}

inline int ParticleFilter::getAcceptedIndex(std::vector<ParticleFilter*> filters, Particle& particle)
{
  for (uint i = 0; i < filters.size(); ++i) {
    if (filters[i]->accept(particle)) {
      return i;
    }
  }
  return -1;
}

#endif /* WAC_ParticleFilter */
