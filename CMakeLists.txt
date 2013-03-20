cmake_minimum_required(VERSION 2.6)
project(sf_common CXX)
# to install pkgconfig setup file.
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(INCLUDEDIR "\${prefix}/include")
SET(VERSION 1.0)

#set(CMAKE_INSTALL_PREFIX "$ENV{DATAFS}")
set(PROJECT_MAJOR_VERSION "0")
set(PROJECT_MINOR_VERSION "2")
set(PROJECT_RELEASE_VERSION "1")
set(CMAKE_VERBOSE_MAKEFILE OFF)

include(FindPkgConfig)
pkg_check_modules(rpkgs REQUIRED dlog)
add_definitions(${rpkgs_CFLAGS})

add_definitions(-DPREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DLOCALEDIR="$ENV{DATAFS}/share/locale")
add_definitions(-DFACTORYFS="$ENV{FACTORYFS}")
add_definitions(-DDATAFS="$ENV{DATAFS}")
#add_definitions(-DX1_PROF)
#add_definitions(-D_GETTEXT)
#add_definitions(-Wall -O3 -omit-frame-pointer) 
#add_definitions(-D_DEFAULT_FONT_NAME="Vera")
#add_definitions(-Wall -g -DUSE_FILE_DEBUG)
#add_definitions(-Wall -g -D_DEBUG)
#add_definitions(-Wl,--as-needed -Wl,-O1)
#add_definitions(-finstrument-functions)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} SHARED 
	src/csensor_module.cpp
	src/cfilter_module.cpp
	src/cprocessor_module.cpp
	src/clist.cpp
	src/cmodule.cpp
	src/cmutex.cpp
	src/cobject_type.cpp
	src/cpacket.cpp
	src/csock.cpp
	src/csync.cpp
	src/cworker.cpp
	src/cipc_worker.cpp
	src/common.cpp
)

target_link_libraries(${PROJECT_NAME} ${rpkgs_LDFLAGS} "-lrt -ldl")
configure_file(${PROJECT_NAME}.pc.in ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc @ONLY)

install(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${PROJECT_NAME}.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
install(FILES 
	include/sf_common.h
	include/csensor_module.h
	include/cfilter_module.h
	include/cprocessor_module.h
	include/clist.h
	include/cmodule.h
	include/cmutex.h
	include/cobject_type.h
	include/cpacket.h
	include/csock.h
	include/csync.h
	include/cworker.h
	include/cipc_worker.h
	include/common.h
	include/cutil.h
	DESTINATION include/${PROJECT_NAME}
)
