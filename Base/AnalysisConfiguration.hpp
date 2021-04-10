// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_AnalysisConfiguration
#define WAC_AnalysisConfiguration
#include "TMath.h"
#include "TaskConfiguration.hpp"
#include "ParticleFilter.hpp"
#include "ParticlePairFilter.hpp"
#include "CanvasConfiguration.hpp"
#include "GraphConfiguration.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration of a given analysis
////////////////////////////////////////////////////////////////////////////////////////////////////////
class AnalysisConfiguration : public TaskConfiguration
{
public:
  
  AnalysisConfiguration(const TString & name,
                        const TString & type,
                        const TString & version);
  AnalysisConfiguration(const AnalysisConfiguration & source);
  virtual ~AnalysisConfiguration(){}

  AnalysisConfiguration & operator=(const AnalysisConfiguration & source);

  void printConfiguration(ostream & os);
  int getIxEtaPhi(double eta, double phi);
  int getIxYPhi(double y, double phi);
  double getDphiShifted(double dphi);

  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  TString histoBaseName;

  // event wise parameters
  int nBins_nPartTot;      double min_nPartTot;      double max_nPartTot;
  int nBins_nCell;         double min_nCell;         double max_nCell;
  int nBins_nPartPerCell;  double min_nPartPerCell;  double max_nPartPerCell;

  // single particle
  int nBins_pt;  double min_pt;  double max_pt;  double range_pt;
  int nBins_eta; double min_eta; double max_eta; double range_eta;
  int nBins_y;   double min_y;   double max_y;   double range_y;
  int nBins_phi; double min_phi; double max_phi; double range_phi;
  int nBins_phiEta;
  int nBins_phiEtaPt;
  int nBins_phiY;
  int nBins_phiYPt;

  // pair multiplicity bin correction
  float binCorrPM;
  float binCorrMM;
  float binCorrPP;

  // pairs q3d
  int nBins_DeltaPlong;  double min_DeltaPlong;  double max_DeltaPlong; double range_DeltaPlong;
  int nBins_DeltaPside;  double min_DeltaPside;  double max_DeltaPside; double range_DeltaPside;
  int nBins_DeltaPout;   double min_DeltaPout;   double max_DeltaPout;  double range_DeltaPout;
  int nBins_Dphi;        double min_Dphi;        double max_Dphi;       double width_Dphi;
  int nBins_Dphi_shft;   double min_Dphi_shft;   double max_Dphi_shft;
  int nBins_Deta;        double min_Deta;        double max_Deta;       double width_Deta;
  int nBins_Dy;          double min_Dy;          double max_Dy;         double width_Dy;

  bool fillPairs;
  bool fill3D;
  bool fill6D;
  bool fillQ3D;
  bool fillY;

  bool nuDynVsMult;
  bool nuDynVsCent;
  int nBins_mult;  double min_mult; double max_mult;
  int nBins_cent;  double min_cent; double max_cent;

ClassDef(AnalysisConfiguration,0)
};

inline int AnalysisConfiguration::getIxEtaPhi(double eta, double phi) {

  if(!(eta<min_eta || eta>max_eta)) {
    int iEta = int( double(nBins_eta)*(eta-min_eta)/range_eta );
    int iPhi = int( double(nBins_phi)*(phi-min_phi)/range_phi );

    if (iEta>=0 && iPhi>=0 && iEta<nBins_eta && iPhi<nBins_phi)
      {
      return nBins_phi*iEta + iPhi;
      }
    return -1;
  }
  return -1;
}

inline int AnalysisConfiguration::getIxYPhi(double y, double phi) {

  if(!(y<min_y || y>max_y))
    {
    int iY = int( double(nBins_y)*(y-min_y)/range_y );
    int iPhi = int( double(nBins_phi)*(phi-min_phi)/range_phi );

    if (iY>=0 && iPhi>=0 && iY<nBins_y && iPhi<nBins_phi)
      {
      return nBins_phi*iY + iPhi;
      }
    return -1;
    }
  return -1;
}

double const  kPI        = TMath::Pi();
double const  kTWOPI     = TMath::TwoPi();

inline double AnalysisConfiguration::getDphiShifted(double dphi) {
  while (dphi >= max_Dphi_shft) dphi -= kTWOPI;
  while (dphi <  min_Dphi_shft) dphi += kTWOPI;
  return dphi;
}

#endif /* WAC_AnalysisConfiguration */
