// Author: Claude Pruneau   05/08/2020

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/
#ifndef WAC_EccentricityVsCent
#define WAC_EccentricityVsCent
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1.h"
#include "TLine.h"
#include "TArrow.h"
#include "TAxis.h"
#include "GraphConfiguration.hpp"
#include "EccentricityCalc.hpp"

////////////////////////////////////////////////////
// Correlation Dist Width Calculator
////////////////////////////////////////////////////
class EccentricityVsCent
{
public:
  
  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  TString systemName;

  TH1 * h_b;
  TH1 * h_var_x_vsB;
  TH1 * h_var_y_vsB;
  TH1 * h_var_xy_vsB;

  TH1 * h_eps_x_vsB;
  TH1 * h_eps_y_vsB;
  TH1 * h_eps_mod_vsB;

  TH1 * h_eps22_vsB;
  TH1 * h_eps24_vsB;
  TH1 * h_eps26_vsB;
  TH1 * h_eps28_vsB;
  TH1 * h_psi2_vsB;

//  // 2D histogram
  TH2 * h2_eps_x_vsB;
  TH2 * h2_eps_y_vsB;
  TH2 * h2_eps_mod_vsB;
  TH2 * h2_psi2_vsB;

  //
//  TH2 * h2_eps22_vsB;
//  TH2 * h2_eps24_vsB;
//  TH2 * h2_eps26_vsB;

  ~EccentricityVsCent()
  {
  // no ops.
  }
  
  EccentricityVsCent(const TString & theSystemName,
                     int nX, double minX, double maxX,
                     int nY, double minY, double maxY,
                     int nB, double minB, double maxB);
  void fill(double b, EccentricityCalc * eccCalc);
  void fill(int  iB, EccentricityCalc * eccCalc);
  void normalize();
  void clear();
  void save();

  ClassDef(EccentricityVsCent,0)
  
};

#endif /* EccentricityVsCent_hpp */
