
INCLUDE_DIRECTORIES (${ENGINE_SOURCE_DIR})
LINK_DIRECTORIES (${ENGINE_SOURCE_DIR}/math)

ADD_EXECUTABLE (tests

	collision.cpp
	intersection.cpp
	test_matrix.cpp
	test_collada_loader.cpp

	main.cpp
)

TARGET_LINK_LIBRARIES (tests math engine xerces-c_3D cppunitd)
