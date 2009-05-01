
ADD_EXECUTABLE (bootstrap WIN32
	main.cpp
)

TARGET_LINK_LIBRARIES (bootstrap
	engine
	math
	d3d9
	dxerr
)

TARGET_LINK_LIBRARIES (bootstrap debug
	d3dx9d
	xerces-c_3D
)

TARGET_LINK_LIBRARIES (bootstrap optimized
	d3dx9d
	xerces-c_3D
)