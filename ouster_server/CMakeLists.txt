file(GLOB_RECURSE ALL_TARGET_FILES *.cpp *.hpp *.h)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

add_executable(ouster_server ${ALL_TARGET_FILES})