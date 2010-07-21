
set(LUA_CORE_SRCS lapi.c lcode.c ldebug.c ldo.c ldump.c
     lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c
     lparser.c lstate.c lstring.c ltable.c ltm.c  lundump.c
     lvm.c lzio.c)

set(LUA_LIB_SRCS lauxlib.c lbaselib.c ldblib.c liolib.c
     lmathlib.c loslib.c ltablib.c lstrlib.c loadlib.c linit.c)

add_library(lua STATIC ${LUA_LIB_SRCS} ${LUA_CORE_SRCS})

add_executable(luai lua.c)
target_link_libraries(luai lua ${ENGINE_LIBM})
set_target_properties(luai PROPERTIES OUTPUT_NAME lua)

add_executable(luac luac.c print.c)
target_link_libraries(luac lua ${ENGINE_LIBM})
