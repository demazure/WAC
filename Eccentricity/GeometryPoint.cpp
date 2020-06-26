// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class GeometryPoint
 \ingroup WAC

 Class defining GeometryPoint
 */

#include "GeometryPoint.hpp"

ClassImp(GeometryPoint);

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////
GeometryPoint::GeometryPoint()
:
x(0.0),y(0.0),z(0.0), r(0.0), phi(0.0), theta(0.0)
{
}

void GeometryPoint::setRThetaPhi(double _r, double _theta, double _phi)
{
  r     = _r;
  theta = _theta;
  phi   = _phi;
  double cosTheta = cos(theta);
  double sinTheta = sin(theta);
  x = r*sinTheta*cos(phi);
  y = r*sinTheta*sin(phi);
  z = r*cosTheta;
}

void GeometryPoint::setRCosThetaPhi(double _r, double _cosTheta, double _phi)
{
  r     = _r;
  phi   = _phi;
  double sinTheta = sqrt(1-_cosTheta*_cosTheta);
  theta = acos(_cosTheta);
  x = r*sinTheta*cos(phi);
  y = r*sinTheta*sin(phi);
  z = r*_cosTheta;
}
