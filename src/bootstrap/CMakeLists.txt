
FILE(GLOB BOOTSTRAP_SOURCES *.h *.cpp)
ADD_EXECUTABLE(bootstrap WIN32 ${BOOTSTRAP_SOURCES})
TARGET_LINK_LIBRARIES(bootstrap ${ENGINE_LIBRARIES} d3d9 dinput8 d3dx9 dxerr dxguid util graphic particle_system platform math spatial)
