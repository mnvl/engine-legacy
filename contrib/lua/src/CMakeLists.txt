
set(LUA_CORE_SRCS lapi.c lcode.c ldebug.c ldo.c ldump.c 
     lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c
     lparser.c lstate.c lstring.c ltable.c ltm.c  lundump.c
     lvm.c lzio.c)

set(LUA_LIB_SRCS lauxlib.c lbaselib.c ldblib.c liolib.c
     lmathlib.c loslib.c ltablib.c lstrlib.c loadlib.c linit.c)

aux_source_directory(. LUA_SOURCES)
set_source_files_properties(${LUA_SOURCES) PROPERTIES LANGUAGE CXX)

add_library(lua ${LUA_LIB_SRCS} ${LUA_CORE_SRCS})

add_executable(luai lua.c)
target_link_libraries(luai lua)
set_target_properties(luai PROPERTIES OUTPUT_NAME lua)

add_executable(luac luac.c print.c)
target_link_libraries(luac lua)

set_target_properties(luai luac PROPERTIES LINKER_LANGUAGE CXX)

set(LUA_INCLUDE_DIR ${CMAKE_CURREN_SOURCE_DIR})
set(LUA_LIBRARIES lua)
