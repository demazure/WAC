// Author: Claude Pruneau   05/08/2020

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/
#ifndef WAC_OverlapFunction
#define WAC_OverlapFunction
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TArrow.h"
#include "TAxis.h"
#include "GraphConfiguration.hpp"
#include "EccentricityCalc.hpp"
#include "EccentricityVsCent.hpp"

////////////////////////////////////////////////////
// Correlation Dist Width Calculator
////////////////////////////////////////////////////
class OverlapFunction
{
public:

  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  TString systemName;
  int nB;
  double maxB;
  double dB;
  int    nX;
  double maxX;
  double radius;
  TH1 * h_overlap;
  TH1 * h_probability;
  //TH2 * h_thick;
  TH2 ** h_thicknessMaps;
  EccentricityCalc   * eccentricityCalc;
  EccentricityVsCent * eccentricityVsCent;
  bool doThicknessMaps;

  ~OverlapFunction()
  {
  // no ops.
  }

  OverlapFunction(const TString & _systemName,
                  int _nB, double _maxB,
                  int _nX, double _maxX,
                  double radius,
                  EccentricityCalc * eccentricityCalc,
                  EccentricityVsCent * eccentricityVsCent,
                  bool doThickMaps);
  void calculateOverlap(TH2* proj1, TH2* proj2);
  void calculateProbability(double sigma);
  void save();

  ClassDef(OverlapFunction,0)

};

#endif /* OverlapFunction_hpp */
