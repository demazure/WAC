################################################################################################
# Project WAC/Pythia
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Pythia PythiaEventGenerator.hpp PythiaConfiguration.hpp LINKDEF PythiaLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(WacPythia SHARED PythiaEventGenerator.cpp PythiaConfiguration.cpp G__Pythia.cxx)
target_link_libraries(WacPythia Base  EGLib PythiaLib EGPythiaLib  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(WacPythia  PUBLIC Base Pythia ${EXTRA_INCLUDES})

add_executable(RunPythiaSimulation RunPythiaSimulation.cpp)
target_include_directories(RunPythiaSimulation PUBLIC Base WacPythia ${EXTRA_INCLUDES})
target_link_libraries(RunPythiaSimulation Base WacPythia  EGLib PythiaLib EGPythiaLib   ${ROOT_LIBRARIES} ${EXTRA_LIBS} ${CMAKE_DL_LIBS})

add_executable(RunPythiaSimulationSingleParticles RunPythiaSimulationSingleParticles.cpp)
target_include_directories(RunPythiaSimulationSingleParticles PUBLIC Base WacPythia ${EXTRA_INCLUDES})
target_link_libraries(RunPythiaSimulationSingleParticles Base WacPythia Base EGLib PythiaLib EGPythiaLib   ${ROOT_LIBRARIES} ${EXTRA_LIBS} ${CMAKE_DL_LIBS})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPythia.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPythia_rdict.pcm" DESTINATION /Users/claudeapruneau/opt/WAC/lib)
install(TARGETS WacPythia  LIBRARY DESTINATION /Users/claudeapruneau/opt/WAC/lib) 
install(TARGETS RunPythiaSimulation DESTINATION /Users/claudeapruneau/opt/WAC/bin) 
install(TARGETS RunPythiaSimulationSingleParticles DESTINATION /Users/claudeapruneau/opt/WAC/bin) 

