// Author: Claude Pruneau   05/08/2020

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/
#ifndef WAC_GeometryPoint
#define WAC_GeometryPoint
#include "TRandom.h"
#include "TMath.h"

////////////////////////////////////////////////////
// A simple point container.
////////////////////////////////////////////////////
class GeometryPoint
{
public:

  double x,y,z, r;
  double phi, theta;
  GeometryPoint();

  void setRThetaPhi(double r, double theta, double phi);
  void setRCosThetaPhi(double _r, double _cosTheta, double _phi);

  ~GeometryPoint()
  {
  // no ops.
  }

  ClassDef(GeometryPoint,0)
  
};

#endif /* GeometryPoint_hpp */
