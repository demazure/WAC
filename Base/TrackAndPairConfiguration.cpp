// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining TrackAndPairConfiguration
 */

#include "TrackAndPairConfiguration.hpp"

ClassImp(TrackAndPairConfiguration);

TrackAndPairConfiguration::TrackAndPairConfiguration(const TString & name, int version)
: TaskConfiguration(name,"TrackAndPairConfiguration",version)
{ }

TrackAndPairConfiguration::~TrackAndPairConfiguration()
{ }

void TrackAndPairConfiguration::printConfiguration(ostream & os)
{
  os
  << " ------------------------------------------------------------------------------------------" << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << "                             Name: " << getName() << endl
  << "                          Version: " << getVersion() << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << " ------------------------------------------------------------------------------------------" << endl;
}


