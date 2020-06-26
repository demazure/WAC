// Author: Claude Pruneau   05/08/2020

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/
#ifndef WAC_GeometryMoments
#define WAC_GeometryMoments
#include "TRandom.h"
#include "TMath.h"

////////////////////////////////////////////////////
// A simple point container.
////////////////////////////////////////////////////
class GeometryMoments
{
public:

  double counts, sum,sumR1, sumR2, sumR3, sumR4;
  double volume;
  double avgR;
  double rmsR;
  double rMax;

  GeometryMoments();
  void reset();
  void fill(double r, double w);
  void calculateMoments(double maxR=1.0);
  ~GeometryMoments()
  {
  // no ops.
  }

  ClassDef(GeometryMoments,0)
  
};

#endif /* GeometryMoments_hpp */
