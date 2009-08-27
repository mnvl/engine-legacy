
add_subdirectory(src)

if (ENGINE_ENABLE_CONTRIB_TESTS)
   add_subdirectory(test)
endif()