
ADD_EXECUTABLE (tests
	main.cpp
)

TARGET_LINK_LIBRARIES (tests
	math
	math_tests
	graphic
	graphic_tests
)

TARGET_LINK_LIBRARIES (tests debug
	xerces-c_3D
	cppunitd
)