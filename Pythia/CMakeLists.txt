################################################################################################
# Project WAC/Pythia
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Pythia PythiaEventGenerator.hpp PythiaConfiguration.hpp LINKDEF PythiaLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(WacPythia SHARED PythiaEventGenerator.cpp PythiaConfiguration.cpp G__Pythia.cxx)
target_link_libraries(WacPythia Base  PYTHIA8 EGPYTHIA8  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(WacPythia  PUBLIC Base Pythia ${EXTRA_INCLUDES})

add_executable(RunPythiaSimulation RunPythiaSimulation.cpp)
target_include_directories(RunPythiaSimulation PUBLIC Base WacPythia ${EXTRA_INCLUDES})
target_link_libraries(RunPythiaSimulation Base WacPythia  EG PYTHIA8 EGPYTHIA8 ${ROOT_LIBRARIES} ${EXTRA_LIBS} ${CMAKE_DL_LIBS})

add_executable(RunPythiaSimulationSingleParticles RunPythiaSimulationSingleParticles.cpp)
target_include_directories(RunPythiaSimulationSingleParticles PUBLIC Base WacPythia ${EXTRA_INCLUDES})
target_link_libraries(RunPythiaSimulationSingleParticles Base WacPythia Base EG PYTHIA8 EGPYTHIA8 ${ROOT_LIBRARIES} ${EXTRA_LIBS} ${CMAKE_DL_LIBS})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPythia.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPythia_rdict.pcm" DESTINATION "$ENV{WAC_LIB}")
install(TARGETS WacPythia  LIBRARY DESTINATION "$ENV{WAC_LIB}") 
install(TARGETS RunPythiaSimulation DESTINATION "$ENV{WAC_BIN}") 
install(TARGETS RunPythiaSimulationSingleParticles DESTINATION "$ENV{WAC_BIN}") 

