################################################################################################
# Project WAC/Base
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

set(SOURCE_FILES
  EventPoolManager.cpp
  )

set(HEADER_FILES
  EventPoolManager.hpp
  )

root_generate_dictionary(G__MixedEvents ${HEADER_FILES} LINKDEF LinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -Wpedantic)
add_library(MixedEvents SHARED ${HEADER_FILES} LinkDef.h ${SOURCE_FILES} G__MixedEvents.cxx)


target_link_libraries(MixedEvents ${WAC_ROOT_LIBRARIES} ${EXTRA_LIBS})
target_include_directories(MixedEvents PUBLIC ${EXTRA_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR})
install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libMixedEvents.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libMixedEvents_rdict.pcm" DESTINATION "$ENV{WAC_LIB}")
install(TARGETS MixedEvents  LIBRARY DESTINATION "$ENV{WAC_LIB}")


