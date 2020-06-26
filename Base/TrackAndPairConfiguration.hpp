// ////////////////////////////////////////////////////////////////////////////////////////
//  TrackAndPairConfiguration.hpp
//  Created by Victor Gonzalez on 02/10/19 on an original idea from Claude Pruneau.
//  Copyright © 2017-2019 Claude Pruneau. All rights reserved.
// ////////////////////////////////////////////////////////////////////////////////////////
#ifndef WAC_TrackAndPairConfiguration
#define WAC_TrackAndPairConfiguration
#include <ostream>
#include "TString.h"
#include "TaskConfiguration.hpp"

// Base class for configuring effects on individual tracks or pairs once generated

class TrackAndPairConfiguration : public TaskConfiguration
{
public:

  TrackAndPairConfiguration(const TString & name, int version);
  virtual ~TrackAndPairConfiguration();
  virtual void printConfiguration(ostream & os);

   ClassDef(TrackAndPairConfiguration,0)
};


#endif /* WAC_TrackAndPairConfiguration */
