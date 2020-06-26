// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class GeometryMoments
 \ingroup WAC

 Class defining GeometryMoments
 */

#include "GeometryMoments.hpp"

ClassImp(GeometryMoments);

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////
GeometryMoments::GeometryMoments()
:
counts(0.0),
sum(0.0),
sumR1(0.0),
sumR2(0.0),
sumR4(0.0),
volume(0.0),
avgR(0.0),
rmsR(0.0),
rMax(0)
{
}


void GeometryMoments::reset()
{
  counts     = 0.0;
  sum        = 0.0;
  sumR1      = 0.0;
  sumR2      = 0.0;
  sumR3      = 0.0;
  sumR4      = 0.0;
  volume     = 0.0;
  avgR       = 0.0;
  rmsR       = 0.0;
  rMax       = 0.0;
}

void GeometryMoments::fill(double r, double w)
{
  if (r>rMax) rMax = r;
   double r2  = r*r;
  counts     += 1;
  sum        += r2*w;
  sumR1      += r2*r*w;
  sumR2      += r2*r2*w;
//  sumR3      += r2*r*w;
//  sumR4      += r2*r2*w;
}

void GeometryMoments::calculateMoments(double maxR)
{

  double mean   = sum/counts;
  double meanR1 = sumR1/sum;
  double meanR2 = sumR2/sum;
//  double meanR4 = sumR4/counts;
  double r3Max = rMax*rMax*rMax;
  volume = 4.0*3.1415927*mean*r3Max/3.0;
  avgR   = meanR1; //*r3Max;
  rmsR   = meanR2;
  rmsR   = sqrt(rmsR);
}
