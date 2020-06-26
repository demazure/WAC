// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class CanvasConfiguration
 \ingroup WAC

 Utility class used to define the parameters of a root canvas
 */

#include "CanvasCollection.hpp"

ClassImp(CanvasCollection);

////////////////////////////////////////////////////
// CTOR
////////////////////////////////////////////////////
CanvasConfiguration::CanvasConfiguration(bool log, int style, int dimension)
:
x(20),
y(20),
width(700),
height(700),
leftMargin(0.15),
topMargin(0.03),
rightMargin(0.03),
bottomMargin(0.15),
logx(0),
logy(0),
logz(0),
theta(35.0),
phi(25.0)
{
  switch (dimension)
  {
    default:
    case 0: break;
    case 1:
    switch (style)
    {
      default:
      case 0:
      leftMargin   = 0.15;
      topMargin    = 0.03;
      rightMargin  = 0.14;
      bottomMargin = 0.15;
      break;
      case 1:
      leftMargin   = 0.15;
      topMargin    = 0.03;
      rightMargin  = 0.05;
      bottomMargin = 0.15;
      break;
    }
    if (log)
      {
      logy = 1;
      }
    break;
    case 2:

    switch (style)
    {
        default:
      case 0:
      leftMargin   = 0.15;
      topMargin    = 0.03;
      rightMargin  = 0.14;
      bottomMargin = 0.15;
      break;
      case 1:
      leftMargin   = 0.15;
      topMargin    = 0.03;
      rightMargin  = 0.05;
      bottomMargin = 0.15;
         break;
    }
    if (log)
      {
      logz = 1;
      }
    break;
  }
}

////////////////////////////////////////////////////
// COPY CTOR
////////////////////////////////////////////////////
CanvasConfiguration::CanvasConfiguration(const CanvasConfiguration & canvasConfig)
:
x(canvasConfig.x),
y(canvasConfig.y),
width(canvasConfig.width),
height(canvasConfig.height),
leftMargin(canvasConfig.leftMargin),
topMargin(canvasConfig.topMargin),
rightMargin(canvasConfig.rightMargin),
bottomMargin(canvasConfig.bottomMargin),
logx(canvasConfig.logx),
logy(canvasConfig.logy),
logz(canvasConfig.logz),
theta(canvasConfig.theta),
phi(canvasConfig.phi)
{}

////////////////////////////////////////////////////
// Assignment operator
////////////////////////////////////////////////////
CanvasConfiguration & CanvasConfiguration::operator=(const CanvasConfiguration & canvasConfig)
{
  x = canvasConfig.x;
  y = canvasConfig.y;
  width  = canvasConfig.width;
  height = canvasConfig.height;
  leftMargin = canvasConfig.leftMargin;
  topMargin  = canvasConfig.topMargin;
  rightMargin = canvasConfig.rightMargin;
  bottomMargin = canvasConfig.bottomMargin;
  logx  = canvasConfig.logx;
  logy  = canvasConfig.logy;
  logz  = canvasConfig.logz;
  theta = canvasConfig.theta;
  phi   = canvasConfig.phi;

  return *this;
}

CanvasConfiguration::~CanvasConfiguration()
{

}
