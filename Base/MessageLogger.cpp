// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class MessageLogger
 \ingroup WAC
 */

#include "MessageLogger.hpp"

ClassImp(MessageLogger);


bool MessageLogger::reportDebug(TString className, TString fctName, TString taskName, ostream & output)
{
 if (reportLevel<=Debug)
   {
   output << " <DEBUG> " << className << "(" << taskName << ")::" << fctName << "  ";
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportInfo(TString  className, TString  fctName, TString  taskName, ostream & output)
{
 if (reportLevel<=Info)
   {
   output << " <INFO> " << className << "(" << taskName << ")::" << fctName << "  ";
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportWarning(TString  className, TString  fctName, TString  taskName, ostream & output)
{
 if (reportLevel<=Warning)
   {
   output << " <WARNING> " << className << "(" << taskName << ")::" << fctName << "  ";
   return true;
   }
 else
   return false;
}


bool MessageLogger::reportError(TString  className, TString  fctName, TString  taskName, ostream & output)
{
 if (reportLevel<=Error)
   {
   output << " <ERROR> " << className << "(" << taskName << ")::" << fctName << "  ";
   return true;
   }
 else
   return false;
}

bool MessageLogger::reportFatal(TString  className, TString  fctName, TString  taskName, ostream & output)
{
 if (reportLevel<=Fatal)
   {
   output << " <FATAL> " << className << "(" << taskName << ")::" << fctName << "  ";
   return true;
   }
 else
   return false;
}



bool MessageLogger::reportDebug(ostream & output)
{
  if (reportLevel<=Debug)
    {
    output << " <DEBUG> ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportInfo(ostream & output)
{
  if (reportLevel<=Info)
    {
    output << " <INFO> ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportWarning(ostream & output)
{
  if (reportLevel<=Warning)
    {
    output << " <WARNING> ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportError(ostream & output)
{
  if (reportLevel<=Error)
    {
    output << " <ERROR> ";
    return true;
    }
  else
    return false;
}

bool MessageLogger::reportFatal(ostream & output)
{
  if (reportLevel<=Fatal)
    {
    output << " <FATAL> ";
    return true;
    }
  else
    return false;
}
