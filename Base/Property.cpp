// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class Property
 \ingroup WAC

 Utility class used to define generic properties
 */

#include "Property.hpp"

//ClassImp(Property);

Property::Property()
{
  name  = "Unnamed";
  type  = Type::EMPTY;
  value.propertyValueInt = 0;
}

Property::Property(const TString & theName, int theValue)
{
  name  = theName;
  type  = Type::INT;
  value.propertyValueInt = theValue;
}
Property::Property(const TString & theName, float   theValue)
{
  name  = theName;
  type  = Type::FLOAT;
  value.propertyValueFloat = theValue;
}

Property::Property(const TString & theName, TString * theValue)
{
  name  = theName;
  type  = Type::STRING;
  value.propertyValueString = theValue;
}

Property::Property(const TString & theName, bool    theValue)
{
  name  = theName;
  type  = Type::BOOL;
  value.propertyValueBool = theValue;
}

Property::~Property()
{

}

void Property::set(int  theValue)
{
  type  = Type::INT;
  value.propertyValueInt = theValue;
}

void Property::set(float theValue)
{
  type  = Type::FLOAT;
  value.propertyValueFloat = theValue;
}

void Property::set(TString * theValue)
{
  type  = Type::STRING;
  value.propertyValueString = theValue;
}

void Property::set(bool theValue)
{
  type  = Type::BOOL;
  value.propertyValueBool = theValue;
}


void Property::setName(const TString& theName)
{
  name = theName;
}

TString Property::getName() const
{
  return name;
}
bool Property::isNamed(const TString & theName) const
{
  return name==theName;
}

int Property::getValueInt()
{
  return value.propertyValueInt;
}

float Property::getValueFloat()
{
  return value.propertyValueFloat;
}

TString Property::getValueString()
{
  return *value.propertyValueString;
}

bool Property::getValueBool()
{
  return value.propertyValueBool;
}

void Property::print(std::ostream & os)
{
  switch (type)
    {
      case Type::INT:
      os << name << " : " <<  value.propertyValueInt; break;
      case Type::FLOAT:
      os << name << " : " <<  value.propertyValueFloat; break;
      case Type::STRING:
      os << name << " : " <<  *value.propertyValueString; break;
      case Type::BOOL:
      os << name << " : " <<  value.propertyValueBool; break;
      case Type::EMPTY:
      os << name << " : Unset."; break;

    }
}

