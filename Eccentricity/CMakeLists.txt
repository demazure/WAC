################################################################################################
# Project WAC/Eccentricity
################################################################################################


ROOT_GENERATE_DICTIONARY(G__Eccentricity  GeometryGenerator.hpp  EccentricityCalc.hpp EccentricityVsCent.hpp OverlapFunction.hpp GeometryPoint.hpp GeometryMoments.hpp LINKDEF EccentricityLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(Eccentricity SHARED  
GeometryGenerator.cpp  EccentricityCalc.cpp EccentricityVsCent.cpp OverlapFunction.cpp GeometryPoint.cpp GeometryMoments.cpp  G__Eccentricity.cxx)
target_link_libraries(Eccentricity Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Eccentricity  PUBLIC Base Eccentricity ${EXTRA_INCLUDES} ) 

# RunEccentricity app
add_executable(RunEccentricity RunEccentricity.cpp)
target_include_directories(RunEccentricity PUBLIC Base Eccentricity ${EXTRA_INCLUDES})
target_link_libraries(RunEccentricity Base Eccentricity ${CMAKE_DL_LIBS})
install(TARGETS RunEccentricity RUNTIME DESTINATION /Users/claudeapruneau/opt/WAC/bin) 

# RunEccentricityAA app
add_executable(RunEccentricityAA RunEccentricityAA.cpp)
target_include_directories(RunEccentricityAA PUBLIC Base Eccentricity ${EXTRA_INCLUDES})
target_link_libraries(RunEccentricityAA Base Eccentricity ${CMAKE_DL_LIBS})
install(TARGETS RunEccentricityAA RUNTIME DESTINATION /Users/claudeapruneau/opt/WAC/bin) 


#install(TARGETS Eccentricity  LIBRARY DESTINATION "${WAC_LIB}")
install(TARGETS Eccentricity  LIBRARY DESTINATION /Users/claudeapruneau/opt/WAC/lib) 
