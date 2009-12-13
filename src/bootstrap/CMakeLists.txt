
FILE(GLOB BOOTSTRAP_SOURCES *.h *.cpp)
ADD_EXECUTABLE(bootstrap WIN32 ${BOOTSTRAP_SOURCES})
TARGET_LINK_LIBRARIES(bootstrap ${ENGINE_LIBRARIES} xerces-c_static_3D d3d9 dinput8 d3dx9 dxerr dxguid util graphic platform math)
