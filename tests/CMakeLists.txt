
FILE (GLOB TEST_SOURCES *.h *.cpp)
ADD_EXECUTABLE (tests ${TEST_SOURCES})

TARGET_LINK_LIBRARIES (tests
	math
	graphic
	util
	platform
)

TARGET_LINK_LIBRARIES (tests debug
	d3d9
	d3dx9d
	dxerr
	xerces-c_static_3D
	cppunitd
)