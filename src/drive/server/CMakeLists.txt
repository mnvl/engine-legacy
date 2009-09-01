
FILE (GLOB DRIVE_SERVER_SOURCES *.h *.cpp)
ADD_EXECUTABLE (drive_server ${DRIVE_SERVER_SOURCES})
TARGET_LINK_LIBRARIES (drive_server util rpc drive_proto drive_model ${ENGINE_LIBRARIES})
