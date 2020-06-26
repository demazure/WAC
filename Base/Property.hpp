// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_Property
#define WAC_Property

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// Configuration                                                       //
//                                                                     //
// Class used to define basic configuration                            //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include <ostream>
#include "TString.h"

//using namespace std;
//using std::ostream;
//using std::endl;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Propeterty
////////////////////////////////////////////////////////////////////////////////////////////////////////
class Property
{
protected:
  union Data
  {
  int     propertyValueInt;
  float   propertyValueFloat;
  TString *propertyValueString;
  bool    propertyValueBool;
  };

  enum Type
  {
  INT, FLOAT, STRING, BOOL, EMPTY
  };

  Data value;
  Type type;
  TString name;

  public:

  Property();
  Property(const TString & name, int     value);
  Property(const TString & name, float   value);
  Property(const TString & name, TString * value);
  Property(const TString & name, bool    value);
  virtual ~Property();

  void set(int     value);
  void set(float   value);
  void set(TString * value);
  void set(bool    value);

  void setName(const TString& name);
  TString getName() const;
  bool isNamed(const TString & name) const;

  int     getValueInt();
  float   getValueFloat();
  TString getValueString();
  bool    getValueBool();

  void print(std::ostream & os);

  //ClassDef(Property,0)
};

#endif /* WAC_Property */
