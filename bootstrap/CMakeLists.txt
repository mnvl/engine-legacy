
ADD_EXECUTABLE (bootstrap WIN32
	main.cpp
    bind_math.cpp
    bind_platform.cpp
    bind_graphic.cpp
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
    dinput8
    dxguid
)

TARGET_LINK_LIBRARIES (bootstrap debug
	d3dx9d
	xerces-c_static_3D
    lua5.1
    libluabindd
)
