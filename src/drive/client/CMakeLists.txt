
FILE (GLOB DRIVE_CLIENT_SOURCES *.h *.cpp)
ADD_EXECUTABLE (drive_client WIN32 ${DRIVE_CLIENT_SOURCES})
TARGET_LINK_LIBRARIES (drive_client util platform graphic d3d9 d3dx9 dxerr dxguid dinput8 ${ENGINE_LIBRARIES})
