################################################################################################
# Project WAC/plotters
################################################################################################


#ROOT_GENERATE_DICTIONARY(G__Cluster  Cell.hpp LINKDEF ClusterLinkDef.h)
#add_library(Cluster SHARED  Cell.cpp G__Cluster.cxx)
#target_link_libraries(Cluster Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
#target_include_directories(Cluster  PUBLIC Base Cluster ${EXTRA_INCLUDES} ) 


add_executable(PlotNuDynHistos PlotNuDynHistos.cpp)
target_include_directories(PlotNuDynHistos PUBLIC Base   ${EXTRA_INCLUDES})
target_link_libraries(PlotNuDynHistos Base  ${CMAKE_DL_LIBS})

#install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libCluster.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libCluster_rdict.pcm" DESTINATION /Users/claudeapruneau/opt/WAC/lib)
#install(TARGETS Cluster  LIBRARY DESTINATION /Users/claudeapruneau/opt/WAC/lib) 
install(TARGETS PlotNuDynHistos DESTINATION /Users/claudeapruneau/opt/WAC/bin) 

