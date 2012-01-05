
FILE(GLOB BOOTSTRAP_SOURCES *.h *.cpp)
ADD_EXECUTABLE(test_lua ${BOOTSTRAP_SOURCES})
TARGET_LINK_LIBRARIES(test_lua ${ENGINE_LIBRARIES} d3d9 d3dx9 dxerr util graphic math spatial particle_system)
ADD_CONFIGURATION_DEPENDENT_TEST(lua test_lua)

