// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_EventLoop
#define WAC_EventLoop
#include "TaskCollection.hpp"

class EventLoop : public TaskCollection
{
 public:
  EventLoop();
  virtual ~EventLoop();
  virtual void run(long nEvents = 10, int nReport = 10);

  ClassDef(EventLoop, 0)
};

#endif /* WAC_EventLoop */
