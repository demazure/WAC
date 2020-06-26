// Author: Claude Pruneau   05/08/2020

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/
#ifndef WAC_EccentricityCalc
#define WAC_EccentricityCalc
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLine.h"
#include "TArrow.h"
#include "TAxis.h"
#include "GraphConfiguration.hpp"
#include "GeometryGenerator.hpp"

////////////////////////////////////////////////////
// Correlation Dist Width Calculator
////////////////////////////////////////////////////
class EccentricityCalc
{
public:
  
  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  TString systemName;
  // 1D histogram
  TH1 * h_x;
  TH1 * h_y;
  TH1 * h_avg_x;
  TH1 * h_avg_y;
  TH1 * h_avg_x2;
  TH1 * h_avg_y2;
  TH1 * h_avg_x3;
  TH1 * h_avg_y3;
  TH1 * h_avg_x4;
  TH1 * h_avg_y4;
  TH1 * h_avg_xy;

  TH1 * h_var_x;
  TH1 * h_var_y;
  TH1 * h_var_xy;

  TH1 * h_eps_denom;
  TH1 * h_eps_x;
  TH1 * h_eps_y;
  TH1 * h_eps_mod;

  TH2 * h2_yVsX;
  TH2 * h2_eps_xy;

  double counts;
  double mom_x;
  double mom_y;
  double mom_x2;
  double mom_y2;
  double mom_x3;
  double mom_y3;
  double mom_x4;
  double mom_y4;
  double mom_x6;
  double mom_y6;
  double mom_x8;
  double mom_y8;
  double mom_xy;

  double avg_x;
  double avg_y;
  double avg_x2;
  double avg_y2;
  double avg_x3;
  double avg_y3;
  double avg_x4;
  double avg_y4;
  double avg_x6;
  double avg_y6;
  double avg_x8;
  double avg_y8;
  double avg_xy;

  double var_x;
  double var_y;
  double var_xy;

  double eps_denom;
  double eps_x;
  double eps_y;
  double eps_mod;
  double eps_22;
  double eps_24;
  double eps_26;
  double eps_28;
  double psi2;

  bool doHistograms;

  ~EccentricityCalc()
  {
  // no ops.
  }
  
   EccentricityCalc(const TString & systemName,
                    int nX, double minX, double maxX,
                    int nY, double minY, double maxY,
                    int nEcc, double minEcc, double maxEcc,
                    bool _doHistograms);
  void fill(double x, double y, double w);
  void fillAvg();
  void fill(TH2 * h);
  void fillWithNuclei(double b, GeometryGenerator * nucleus1, GeometryGenerator * nucleus2);
  void clear();
  void reset();
  void calculateAverages();
  void save();

  ClassDef(EccentricityCalc,0)
  
};

#endif /* EccentricityCalc_hpp */
