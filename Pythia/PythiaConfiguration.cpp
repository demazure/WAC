// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
/**
 \class Property
 \ingroup WAC

 Utility class used to definePythiaConfiguration
 */

#include "PythiaConfiguration.hpp"

ClassImp(PythiaConfiguration);

PythiaConfiguration::PythiaConfiguration(int _beam,
                                         int _target,
                                         double _energy,
                                         int _nOptions,
                                         TString** _options)
  : TaskConfiguration("PYTHIA", "GEN", "1.0"),
    beam(_beam),
    target(_target),
    energy(_energy),
    nOptions(_nOptions),
    options(_options)
{
}

////////////////////////////////////////////////////
// Print this configuration to the given stream
////////////////////////////////////////////////////
void PythiaConfiguration::printConfiguration(ostream& os)
{
  printTaskConfiguration(os);
  os
    << "    PYTHIA   Parameters: " << endl
    << " ------------------------------------------------------------------------------------------" << endl;
  os << "   Beam: " << beam << endl;
  os << " Target: " << target << endl;
  os << " Energy: " << energy << endl;
  for (int i = 0; i < nOptions; i++) {
    os << *options[i] << endl;
  }
}
