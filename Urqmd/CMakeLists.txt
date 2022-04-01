cmake_minimum_required(VERSION 3.10)

project(UrQMDParser VERSION 1.0 
                    DESCRIPTION "Parses particle files to produce a file stored TTree and viceversa"
                    LANGUAGES CXX)

# finding the ROOT package
find_package(ROOT 6.14 CONFIG REQUIRED)
# for ROOT < 6.20
# include("${ROOT_DIR}/modules/RootNewMacros.cmake")
add_definitions(${ROOT_CXX_FLAGS})

# classes dictionary 
ROOT_GENERATE_DICTIONARY(G__DictUrQMDEvent UrQMDEvent.h LINKDEF DictLinkDef.h)

add_library(urqmd2ttree SHARED UrQMD2TTree.C UrQMDEvent.h UrQMDEvent.cxx G__DictUrQMDEvent.cxx)
target_link_libraries(urqmd2ttree PUBLIC ROOT::Core ROOT::Tree ROOT::Net)
target_include_directories(urqmd2ttree  PUBLIC Urqmd ${EXTRA_INCLUDES} )

add_executable(runUrQMDParser runUrQMDParser.C)
target_link_libraries(runUrQMDParser PUBLIC urqmd2ttree)

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libDictUrQMDEvent.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libDictUrQMDEvent_rdict.pcm" DESTINATION "$ENV{WAC_LIB}")
install(TARGETS urqmd2ttree LIBRARY DESTINATION "$ENV{WAC_LIB}")
install(TARGETS runUrQMDParser RUNTIME DESTINATION "$ENV{WAC_BIN}")

