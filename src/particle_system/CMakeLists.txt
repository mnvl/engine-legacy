
COLLECT_SOURCE_FILES(PARTICLE_SYSTEM_SOURCES PARTICLE_SYSTEM_TEST_SOURCES)

ADD_LIBRARY(particle_system STATIC ${PARTICLE_SYSTEM_SOURCES} ${DUMPED_FILES})

ADD_EXECUTABLE(test_particle_system ${PARTICLE_SYSTEM_TEST_SOURCES})
TARGET_LINK_LIBRARIES(test_particle_system ${ENGINE_LIBRARIES} particle_system graphic platform util math d3d9 d3dx9 dxerr)

ADD_TEST(particle_system ${EXECUTABLE_OUTPUT_PATH}/test_particle_system --log_level=test_suite)

