//
//  ClusterFactory.cpp
//  MyMC
//
//  Created by Claude Pruneau on 2/20/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//

#include "ClusterFactory.hpp"
ClassImp(ClusterFactory);

/////////////////////////////////////////////////////////////
 // CTOR.
 /////////////////////////////////////////////////////////////
ClusterFactory::ClusterFactory(int initialCapacity)
 :
 Factory(initialCapacity)
 {
   objects = new Cluster*[capacity];
 for (int k=0; k<capacity; k++)
   {
   objects[k] = new Cluster();
   }
 }

 /////////////////////////////////////////////////////////////
 // DTOR.
 /////////////////////////////////////////////////////////////
 ClusterFactory::~ClusterFactory()
 {
 delete[] objects;
 }

 /////////////////////////////////////////////////////////////
 // Returns the next object.
 /////////////////////////////////////////////////////////////
 Cluster * ClusterFactory::getObject()
 {
   if (index<capacity)
     {
     //cout << " ================== Cluster Factory index=" << index << endl;

     return objects[index++];
     }
   else
     {
     cout << "-FATAL- ClusterFactory::getObject() Object stored is exhausted" << endl;
     return 0;
     }
 }

 /////////////////////////////////////////////////////////////
 // Returns the object at the given index.
 /////////////////////////////////////////////////////////////
 Cluster * ClusterFactory::getObject(long index)
 {
 if (index>=0 && index<capacity)
   {
   return objects[index];
   }
 else
   {
   cout << "-FATAL- ParticleFactory::getObject() Object stored is exhausted" << endl;
   return 0;
   }
 }
