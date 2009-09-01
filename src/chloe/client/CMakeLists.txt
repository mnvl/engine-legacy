
FILE (GLOB drive_SOURCES *.h *.cpp)
ADD_EXECUTABLE (drive WIN32 ${drive_SOURCES})
TARGET_LINK_LIBRARIES (drive util platform graphic d3d9 d3dx9 dxerr dxguid dinput8 ${ENGINE_LIBRARIES})
