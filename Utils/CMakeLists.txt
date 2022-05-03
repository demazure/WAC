################################################################################################
# Project WAC/Pythia
################################################################################################

add_executable(statUncertain statUncertain.cpp)
target_include_directories(statUncertain PUBLIC Base ${EXTRA_INCLUDES})
target_link_libraries(statUncertain Base ${WAC_ROOT_LIBRARIES} ${EXTRA_LIBS} ${CMAKE_DL_LIBS})

install(TARGETS statUncertain DESTINATION "$ENV{WAC_BIN}")
