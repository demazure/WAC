//
//  ClusterModelEventGenerator2.hpp
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef WAC_ClusterModelEventGenerator2
#define WAC_ClusterModelEventGenerator2
#include "ClusterModelEventGenerator.hpp"
using namespace std;


class ClusterModelEventGenerator2 : public ClusterModelEventGenerator
{
public:

  ClusterModelEventGenerator2(const TString &  name,
                              TaskConfiguration * configuration,
                              ClusterModelEvent * event);
  virtual ~ClusterModelEventGenerator2();
  void execute();
  ClassDef(ClusterModelEventGenerator2,0)

  Factory<Particle> * particleFactory;
  Factory<Cluster>  * clusterFactory;
  Factory<Cell>     * cellFactory;

};

#endif /* WAC_ClusterModelEventGenerator2 */

