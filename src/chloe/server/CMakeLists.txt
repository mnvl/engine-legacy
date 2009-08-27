
FILE (GLOB CHLOED_SOURCES *.h *.cpp)
ADD_EXECUTABLE (chloed ${CHLOED_SOURCES})
TARGET_LINK_LIBRARIES (chloed util rpc starmasters_proto starmasters_model ${ENGINE_LIBRARIES})
