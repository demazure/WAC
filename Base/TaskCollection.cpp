// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
/**
 \class TaskCollection
 \ingroup WAC

 Class defining Nested Tasks
 */

#include "TaskCollection.hpp"

ClassImp(TaskCollection)

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  TaskCollection::TaskCollection(const TString& name,
                                 TaskConfiguration* configuration,
                                 int nTasksMaxSelected)
  : Task(name, configuration, nullptr),
    nTasksMax(nTasksMaxSelected > 1 ? nTasksMaxSelected : 1),
    nTasks(0),
    tasks(0)
{
  if (reportDebug())
    cout << "TaskCollection::TaskCollection() No ops." << endl;
  tasks = new Task*[nTasksMax];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DTOR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
TaskCollection::~TaskCollection()
{
  if (reportDebug())
    cout << "TaskCollection::~TaskCollection() Started." << endl;
  delete[] tasks;
  if (reportDebug())
    cout << "TaskCollection::~TaskCollection() Completed." << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize task
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskCollection::initialize()
{
  if (reportDebug())
    cout << "TaskCollection::initialize() Started" << endl;
  cout << "TaskCollection::initialize() Initializing " << nTasks << " tasks" << endl;
  for (int iTask = 0; iTask < nTasks; iTask++) {
    if (isTaskOk()) {
      if (!tasks[iTask]) {
        if (reportFatal())
          cout << "TaskCollection::initialize() Null pointer for iTask=" << iTask << endl;
        cout << "TaskCollection::initialize() Null pointer for iTask=" << iTask << endl;
        postTaskFatal();
        return;
      }
    }
    tasks[iTask]->initialize();
  }
  if (reportDebug())
    cout << "TaskCollection::initialize() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execute task
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskCollection::execute()
{
  if (reportDebug())
    cout << "TaskCollection::execute() Started" << endl;
  for (int iTask = 0; iTask < nTasks; iTask++)
    if (isTaskOk())
      tasks[iTask]->execute();
  if (reportDebug())
    cout << "TaskCollection::execute() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Finalize task
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TaskCollection::finalize()
{
  if (reportDebug())
    cout << "TaskCollection::finalize() Started" << endl;
  for (int iTask = 0; iTask < nTasks; iTask++)
    if (isTaskOk())
      tasks[iTask]->finalize();
  if (reportDebug())
    cout << "TaskCollection::finalize() Completed" << endl;
}

////////////////////////////////////////////
// Reset this task
////////////////////////////////////////////
void TaskCollection::reset()
{
  if (reportDebug())
    cout << "TaskCollection::reset() Started" << endl;
  for (int iTask = 0; iTask < nTasks; iTask++)
    if (isTaskOk())
      tasks[iTask]->reset();
  if (reportDebug())
    cout << "TaskCollection::reset() Completed" << endl;
}

void TaskCollection::clear()
{
  if (reportDebug())
    cout << "TaskCollection::clear() Started" << endl;
  for (int iTask = 0; iTask < nTasks; iTask++)
    if (isTaskOk())
      tasks[iTask]->clear();
  if (reportDebug())
    cout << "TaskCollection::clear() Completed" << endl;
}

////////////////////////////////////////////
// Print task configuration
////////////////////////////////////////////
void TaskCollection::printConfiguration(ostream& output)
{
  if (reportInfo()) {
    output << "Task Name : " << getName() << endl;
    for (int iTask = 0; iTask < nTasks; iTask++)
      if (isTaskOk())
        tasks[iTask]->printConfiguration(output);
  }
}

void TaskCollection::addTask(Task* task)
{
  if (!task) {
    if (reportFatal())
      cout << "TaskCollection::addTask(Task *)  Given task pointer is null. Abort." << endl;
    postTaskFatal();
    return;
  }

  if (nTasks >= nTasksMax) {
    if (reportFatal())
      cout << "TaskCollection::addTask(Task *) Cannot add subtask " << task->getName() << " to task " << getName()
           << ". Maximum of " << nTasksMax << " tasks exceeded" << endl;
    postTaskFatal();
  }
  tasks[nTasks++] = task;
  if (reportInfo())
    cout << "TaskCollection::addTask(Task *)  Added task " << task->getName() << " to task " << getName() << endl;
}
