
ADD_EXECUTABLE (tests
	${ENGINE_SOURCE_DIR}/graphic/test_collada_loader.cpp

	${ENGINE_SOURCE_DIR}/math/test_collision.cpp
	${ENGINE_SOURCE_DIR}/math/test_intersection.cpp
	${ENGINE_SOURCE_DIR}/math/test_matrix.cpp

	main.cpp
)

TARGET_LINK_LIBRARIES (tests
	math
	graphic
)

TARGET_LINK_LIBRARIES (tests debug
	xerces-c_3D
	cppunitd
)