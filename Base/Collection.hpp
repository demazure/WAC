// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_Collection
#define WAC_Collection

// ====================================================
//
// Collection
//
// Generic base class for Collection services.
// Does not own a store
//
// ====================================================
#include <iostream>
#include "TObject.h"

using namespace std;

template < class T >
class Collection : public TObject
{
protected:
  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  long nObjectCapacity;
  long nObject;
  T ** objects;

  public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  Collection(int initialCapacity=10)
  :
  TObject(),
  nObjectCapacity(0),
  nObject(0),
  objects(0)
  {
  initialize(initialCapacity);
  }

  Collection(const Collection & source)
  :
  TObject(source),
  nObjectCapacity(0),
  nObject(0),
  objects(0)
  {
  initialize(source.nObjectCapacity);
  copy(source.nObject,source.objects);
  }


  virtual ~Collection()
  {
  delete[] objects;
  }

  Collection & operator=(const Collection source)
  {
  if (this==&source) return *this;
  delete[] objects;
  initialize(source.nObjectCapacity);
  copy(source.nObject,source.objects);
  return *this;
  }

  T * operator[](long index)
  {
  if (index<0) return objects[0];
  if (index>=nObject) return objects[nObject-1];
  return objects[index];
  }

  // Returns the nObjectCapacity of this store/Collection
  long getCollectionCapacity() const
  {
    return nObjectCapacity;
  }

  // Returns the number of object currently
  // in use.
  long getCollectionSize() const
  {
    return nObject;
  }

T * add(T * object)
  {
  if (nObject >= nObjectCapacity)
    {
    T ** work = objects;
    initialize(2*nObjectCapacity);
    copy(nObject,work);
    delete[] work;
    }
  objects[nObject] = object;
  nObject++;
  return object;
  }

  T * getObjectAt(long index)
  {
  if (index<0) return objects[0];
  if (index>=nObject) return objects[nObject-1];
  return objects[index];
  }

protected:

  void initialize(int initialCapacity)
  {
    nObjectCapacity = initialCapacity;
    objects = new T*[nObjectCapacity];
  }


void copy(long n, T ** source)
  {
  for (int iObject=0; iObject<n; iObject++)
    {
    //objects[iObject] = new T(*source[iObject]);
    objects[iObject] = (T*) source[iObject]->Clone();
    }
  nObject = n;
  }

  ClassDef(Collection,0)
};

#endif /* WAC_Collection */
