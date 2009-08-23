
FILE (GLOB STARMASTERSC_SOURCES *.h *.cpp)
ADD_EXECUTABLE (starmastersc WIN32 ${STARMASTERSC_SOURCES})
TARGET_LINK_LIBRARIES (starmastersc util platform graphic d3d9 d3dx9 dxerr dxguid dinput8 ${ENGINE_LIBRARIES})
