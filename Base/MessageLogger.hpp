// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_MessageLogger
#define WAC_MessageLogger
#include "TClass.h"
#include <iostream>
using namespace std;
using std::cout;
using std::endl;

class MessageLogger
{
public:

  enum LogLevel   { Unknown, Debug, Info, Warning, Error, Fatal};

  LogLevel  reportLevel;

  MessageLogger(LogLevel selectedLevel=Info) : reportLevel( selectedLevel )
  {   }

  virtual ~MessageLogger()
  { }

  void setReportLevel(LogLevel requiredLevel)
  {
    reportLevel = requiredLevel;
  }

  LogLevel getReportLevel()
  {
    return reportLevel;
  }

  bool reportDebug(ostream & output=cout);
  bool reportInfo(ostream & output=cout);
  bool reportWarning(ostream & output=cout);
  bool reportError(ostream & output=cout);
  bool reportFatal(ostream & output=cout);

  bool reportDebug(TString & className, TString & fctName, TString & taskName, ostream & output=cout);


  ClassDef(MessageLogger,0)
};

#endif /* WAC_MessageLogger */
