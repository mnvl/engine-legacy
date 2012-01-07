
FILE(GLOB PIRATES_SOURCES *.h *.cc)
ADD_EXECUTABLE(pirates WIN32 ${PIRATES_SOURCES})
TARGET_LINK_LIBRARIES(pirates util graphic math spatial particle_system ${ENGINE_LIBRARIES} d3d9 dinput8 d3dx9 dxerr dxguid)
