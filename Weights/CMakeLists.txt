################################################################################################
# Project WAC/Lednicky
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Weight weight.h LINKDEF WeightLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################

add_compile_options(-fPIC)

add_library(Weights SHARED weight.cxx G__Weight.cxx)
target_link_libraries(Weights FortranLib ${EXTRA_LIBS} )
target_include_directories(Weights PUBLIC Weights ${EXTRA_INCLUDES})


SET(Fortran_src FsiTools.F FsiWeightLednicky5.F)
ADD_LIBRARY(FortranLib STATIC ${Fortran_src})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libWeight.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libWeight_rdict.pcm" DESTINATION "$ENV{WAC_LIB}")
install(TARGETS Weights  LIBRARY DESTINATION "$ENV{WAC_LIB}")

