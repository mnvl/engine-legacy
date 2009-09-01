
FILE (GLOB driveD_SOURCES *.h *.cpp)
ADD_EXECUTABLE (drived ${driveD_SOURCES})
TARGET_LINK_LIBRARIES (drived util rpc drive_proto drive_model ${ENGINE_LIBRARIES})
