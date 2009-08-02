
SET (TESTS
  test_vector_of_object.cpp
  test_back_reference.cpp
  test_def_from_base.cpp
  test_object.cpp
  test_abstract_base.cpp
  test_attributes.cpp
  test_const.cpp
  test_construction.cpp
  test_exceptions.cpp
  test_free_functions.cpp
  test_held_type.cpp
  test_implicit_cast.cpp
  test_iterator.cpp
  test_lua_classes.cpp
  test_null_pointer.cpp
  test_operators.cpp
  test_policies.cpp
  test_scope.cpp
  test_separation.cpp
  test_simple_class.cpp
  test_yield.cpp
  test_private_destructors.cpp
  test_exception_handlers.cpp
  test_shadow.cpp
  test_tag_function.cpp
  test_adopt.cpp
  test_properties.cpp
  test_user_defined_converter.cpp
  test_adopt_wrapper.cpp
  test_builtin_converters.cpp
  test_class_info.cpp
)

FOREACH (TEST_SOURCE ${TESTS})
  GET_FILENAME_COMPONENT (TEST_NAME ${TEST_SOURCE} NAME_WE)
  ADD_EXECUTABLE (${TEST_NAME} ${TEST_SOURCE} main.cpp)
  TARGET_LINK_LIBRARIES (${TEST_NAME} lua luabind)
  ADD_TEST (${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME})
ENDFOREACH()
