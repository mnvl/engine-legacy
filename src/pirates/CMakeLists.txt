
FILE(GLOB PIRATES_SOURCES *.h *.cpp)
ADD_EXECUTABLE(pirates WIN32 ${PIRATES_SOURCES})
TARGET_LINK_LIBRARIES(pirates ${ENGINE_LIBRARIES} d3d9 dinput8 d3dx9 dxerr dxguid util graphic math spatial particle_system)
