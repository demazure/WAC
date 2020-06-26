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

 Class defining Task
 */

#include "EventLoop.hpp"
ClassImp(EventLoop);

EventLoop::EventLoop()
:
TaskCollection("EventLoop",nullptr,10)
{
  if (reportDebug())  cout << "EventLoop::CTOR Started" << endl;
}

EventLoop::~EventLoop()
{
   if (reportDebug())  cout << "EventLoop::DTOR Started" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Run the event loop as setup
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventLoop::run(long nEvents, int nReport)
{
  if (reportInfo()) cout << "EventLoop::run(...) Running for nEvents: " << nEvents << endl;
  postTaskOk();
  initialize();
  if (!isTaskOk())
    {
    if (reportWarning()) cout << "EventLoop::run(...) Initialization failed. Abort." << endl;
    return;
    }
  for (long iEvent=0; iEvent<nEvents; iEvent++)
    {
    if (isTaskOk()) reset();
    if (isTaskOk()) execute();
    if (isTaskOk() && iEvent%nReport==0 )
      {
      if (reportInfo()) cout << "EventLoop::run() Completed event # " << iEvent << endl;
      }
    }
  if (isTaskOk()) finalize();

  if (reportInfo())  cout << "EventLoop::run(...) Completed nEvents: " << nEvents << endl;
  if (reportInfo())  cout << "EventLoop::run(...) Task finished with status : " << getTaskStatusName() << endl;
  if (reportDebug()) cout << "EventLoop::run(...) Completed." << endl;
}
