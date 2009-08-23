
FILE (GLOB STARMASTERSD_SOURCES *.h *.cpp)
ADD_EXECUTABLE (starmastersd ${STARMASTERSD_SOURCES})
TARGET_LINK_LIBRARIES (starmastersd util rpc starmasters_proto starmasters_model ${ENGINE_LIBRARIES})
