################################################################################################
# Project WAC/Cluster
################################################################################################


ROOT_GENERATE_DICTIONARY(G__Cluster  
Cell.hpp 
CellHistos.hpp  
Cluster.hpp
ClusterHistos.hpp
ClusterModelConfiguration.hpp
ClusterModelEvent.hpp
ClusterModelEventAnalyzer.hpp
ClusterModelEventGenerator.hpp
ClusterModelEventGenerator2.hpp
ClusterModelEventHistos.hpp LINKDEF ClusterLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Cluster SHARED  Cell.cpp
CellHistos.cpp
Cluster.cpp
ClusterHistos.cpp
ClusterModelConfiguration.cpp
ClusterModelEvent.cpp
ClusterModelEventAnalyzer.cpp
ClusterModelEventGenerator.cpp
ClusterModelEventGenerator2.cpp
ClusterModelEventHistos.cpp  G__Cluster.cxx)
target_link_libraries(Cluster Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Cluster  PUBLIC Base Cluster ${EXTRA_INCLUDES} ) 


# RunClusterModelSimulation app
add_executable(RunClusterModelSimulation RunClusterModelSimulation.cpp)
target_include_directories(RunClusterModelSimulation PUBLIC Base Cluster ${EXTRA_INCLUDES})
target_link_libraries(RunClusterModelSimulation Base Cluster ${CMAKE_DL_LIBS})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libCluster.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libCluster_rdict.pcm" DESTINATION /Users/claudeapruneau/opt/WAC/lib)
install(TARGETS Cluster  LIBRARY DESTINATION /Users/claudeapruneau/opt/WAC/lib) 
install(TARGETS RunClusterModelSimulation DESTINATION /Users/claudeapruneau/opt/WAC/bin) 

#install(TARGETS Cluster  LIBRARY DESTINATION "${WAC_LIB}")
#install(TARGETS RunClusterModelSimulation RUNTIME  DESTINATION "${WAC_BIN}") 


