
ADD_EXECUTABLE (bootstrap WIN32
	main.cpp
)

TARGET_LINK_LIBRARIES (bootstrap
	math
	platform
	util
	graphic
)

TARGET_LINK_LIBRARIES (bootstrap
	d3d9
	dxerr
)

TARGET_LINK_LIBRARIES (bootstrap debug
	d3dx9d
	xerces-c_static_3D
    lua5.1
    libluabindd
)
