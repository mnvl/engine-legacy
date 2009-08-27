
FILE (GLOB CHLOE_SOURCES *.h *.cpp)
ADD_EXECUTABLE (chloe WIN32 ${CHLOE_SOURCES})
TARGET_LINK_LIBRARIES (chloe util platform graphic d3d9 d3dx9 dxerr dxguid dinput8 ${ENGINE_LIBRARIES})
