################################################################################################
# Project WAC/Epos
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Epos EposEventReader.hpp LINKDEF EposLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Epos SHARED EposEventReader.cpp G__Epos.cxx)
target_link_libraries(Epos Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Epos  PUBLIC Base Cluster Epos ${EXTRA_INCLUDES})

add_executable(RunEposSimulation RunEposSimulation.cpp)
target_include_directories(RunEposSimulation PUBLIC Base Epos ${EXTRA_INCLUDES})
target_link_libraries(RunEposSimulation Base Epos ${CMAKE_DL_LIBS})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libEpos.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libEpos_rdict.pcm" DESTINATION /Users/claudeapruneau/opt/WAC/lib)
install(TARGETS Epos  LIBRARY DESTINATION /Users/claudeapruneau/opt/WAC/lib) 
install(TARGETS RunEposSimulation DESTINATION /Users/claudeapruneau/opt/WAC/bin) 

